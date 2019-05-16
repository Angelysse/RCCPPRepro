#pragma once

#include <iostream>

#include "RuntimeObjectSystem/RuntimeInclude.h"
#include "RuntimeObjectSystem/RuntimeSourceDependency.h"

RUNTIME_MODIFIABLE_INCLUDE;
RUNTIME_COMPILER_SOURCEDEPENDENCY_FILE("Src/UserTest1", ".cpp");

class UserTest1
{
	public:
		void funcTestUser();
};