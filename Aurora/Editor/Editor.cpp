#include <iostream>

#include <RuntimeObjectSystem/IObjectFactorySystem.h>
#include <RuntimeObjectSystem/ObjectInterface.h>
#include <RuntimeObjectSystem/ObjectFactorySystem/ObjectFactorySystem.h>
#include <RuntimeObjectSystem/RuntimeObjectSystem.h>

#include <RuntimeCompiler/BuildTool.h>
#include <RuntimeCompiler/ICompilerLogger.h>
#include <RuntimeCompiler/FileSystemUtils.h>
#include <RuntimeCompiler/FileChangeNotifier.h>

#include <filesystem>
#include <thread>
#include <chrono>
#include <cassert>

#include "Logger.h"

using FileSystemUtils::Path;
using namespace std::filesystem;
using namespace std::chrono_literals;

class Listener : public IObjectFactoryListener
{
	public:
		virtual void OnConstructorsAdded() override
		{
			std::cout << "Constructor added" << std::endl;
		}
};

IRuntimeObjectSystem*	m_ros		= nullptr;
Logger*					m_logger	= nullptr;
Listener*				m_listener	= nullptr;

bool init()
{
	//Initialise the RuntimeObjectSystem
	m_ros		= new RuntimeObjectSystem();
	m_logger	= new Logger();
	m_listener	= new Listener();

	path pathToInternalInclude	= std::filesystem::current_path();								//Executable code
	path pathToUserDir			= std::filesystem::current_path().parent_path() / "UserCode";	//User code directory
	path pathToLibInclude		= std::filesystem::current_path().parent_path();				//RCCPP includes

	path pathToUserInclude		= pathToUserDir / "Include";
	path pathToUserSource		= pathToUserDir / "Src";

	//Make sure user files exist
	assert(exists(pathToUserInclude));
	assert(exists(pathToUserSource));
	assert(exists(pathToUserInclude / "UserTest1.h"));
	assert(exists(pathToUserInclude / "Dummy2.cpp"));
	assert(exists(pathToUserSource / "UserTest1.cpp"));

	m_ros->SetAutoCompile(true);

	//Setup Directories we look files into
	m_ros->AddIncludeDir(pathToInternalInclude.string().c_str(), 0);		//Not need but just to make sure
	m_ros->AddIncludeDir(pathToLibInclude.string().c_str());
	m_ros->AddIncludeDir(pathToUserInclude.string().c_str(), 0);

	m_ros->AddPathToSourceSearch(pathToInternalInclude.string().c_str());	//Not need but just to make sure
	m_ros->AddPathToSourceSearch(pathToUserSource.string().c_str());

	m_ros->SetAdditionalCompileOptions("/FC /std:c++17", 0);

	if (!m_ros->Initialise(m_logger, nullptr))
    {
		delete m_ros;
        m_ros = nullptr;

		std::cout << "Failed to init" << std::endl;

        return false;
    }

	m_ros->GetObjectFactorySystem()->AddListener(m_listener);

	std::cout << "Init successfully" << std::endl;

	return true;
}

void cleanup()
{
    // clean temp object files
	if (m_ros != nullptr)
        m_ros->CleanObjectFiles();

    if ( m_ros && m_ros->GetObjectFactorySystem())
		m_ros->GetObjectFactorySystem()->RemoveListener(m_listener);

	if (m_ros != nullptr)
		delete m_ros;
	
	if (m_logger != nullptr)
		delete m_logger;
}

int main()
{
   if (init())
	{
		std::cout << "Start loop" << std::endl;

		while (true)
		{
			m_ros->GetFileChangeNotifier()->Update(1.0f);

			if (m_ros->GetIsCompiledComplete())
				m_ros->LoadCompiledModule();

			std::this_thread::sleep_for(1s);
		}
	}

	cleanup();

	return EXIT_SUCCESS;
}