#include "stdafx.h"
#include "DBManager.h"

using namespace std;

bool DBManager::isLoaded() const
{
	return _loaded;
}