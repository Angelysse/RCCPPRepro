#include <iostream>

#include "RuntimeObjectSystem/ObjectInterfacePerModule.h"
#include "RuntimeObjectSystem/IObject.h"

#include "UserTest1.h"

class Dummy2 : public IObject
{};

REGISTERCLASS(Dummy2);