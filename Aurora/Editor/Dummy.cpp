#include <iostream>

#include "RuntimeObjectSystem/ObjectInterfacePerModule.h"
#include "RuntimeObjectSystem/IObject.h"

#include "Test1.h"	// << Need to include all dependencies so that they are runtime compiled
#include "Test2.h"	// << Need to include all dependencies so that they are runtime compiled

#include "../UserCode/Include/Dummy2.cpp"	// < To start including user code

class Dummy : public IObject
{};

REGISTERCLASS(Dummy);