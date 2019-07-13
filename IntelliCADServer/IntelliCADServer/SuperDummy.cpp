#include "SuperDummy.h"
#include "DummyClass.h"

SuperDummy SuperDummy::__instance = SuperDummy();

SuperDummy::~SuperDummy()
{
}

SuperDummy & SuperDummy::getInstance()
{
	return __instance;
}

void SuperDummy::onDummyReceived(std::shared_ptr<DummyClass> pObject)
{
	__dummys.push_back(pObject);
}
