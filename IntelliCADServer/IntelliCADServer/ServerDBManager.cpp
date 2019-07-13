#include "stdafx.h"
#include "ServerDBManager.h"
#include "Constant.h"
#include "Parser.hpp"

using namespace std;

bool ServerDBManager::load(const tstring &rootDir, const bool createIfNotExistent)
{
	if (isLoaded())
		return true;

	const bool CONFIG_SUCCESS = __configMgr.load(rootDir, createIfNotExistent);
	const bool ACCOUNT_SUCCESS = __accountMgr.load(rootDir, createIfNotExistent);
	const bool FILE_SUCCESS = __fileMgr.load(rootDir, createIfNotExistent);
	const bool DEPT_SUCCESS = __deptMgr.load(rootDir, createIfNotExistent);

	// 4개 Mgr 모두 성공 시에만 true
	if (CONFIG_SUCCESS
		&& ACCOUNT_SUCCESS
		&& FILE_SUCCESS
		&& DEPT_SUCCESS)
		return _loaded = true;
	else
		return false;
}

bool ServerDBManager::load(const bool createIfNotExistent)
{
	return load(Constant::DB::ROOT_DIR, createIfNotExistent);
}

ConfigDBManager &ServerDBManager::getConfigManager()
{
	return __configMgr;
}

AccountDBManager &ServerDBManager::getAccountManager()
{
	return __accountMgr;
}

FileDBManager &ServerDBManager::getFileManager()
{
	return __fileMgr;
}

DepartmentDBManager &ServerDBManager::getDepartmentManager()
{
	return __deptMgr;
}

bool ServerDBManager::store()
{
	const bool CONFIG_SUCCESS = __configMgr.store();
	const bool ACCOUNT_SUCCESS = __accountMgr.store();
	const bool FILE_SUCCESS = __fileMgr.store();
	const bool DEPT_SUCCESS = __deptMgr.store();

	// 4개 Mgr 모두 성공 시에만 true
	if (CONFIG_SUCCESS
		&& ACCOUNT_SUCCESS
		&& FILE_SUCCESS
		&& DEPT_SUCCESS)
		return _loaded = true;
	else
		return false;
}

void ServerDBManager::close()
{

}