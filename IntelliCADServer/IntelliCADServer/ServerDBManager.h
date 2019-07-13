#pragma once

#include "ConfigDBManager.h"
#include "AccountDBManager.h"
#include "FileDBManager.h"
#include "DepartmentDBManager.h"

class ServerDBManager : public DBManager
{
private:
	ConfigDBManager __configMgr;
	AccountDBManager __accountMgr;
	FileDBManager __fileMgr;
	DepartmentDBManager __deptMgr;
	
public:
	virtual bool load(const std::tstring &rootDir, const bool createIfNotExistent = true) override;
	bool load(const bool createIfNotExistent = true);

	ConfigDBManager &getConfigManager();
	AccountDBManager &getAccountManager();
	FileDBManager &getFileManager();
	DepartmentDBManager &getDepartmentManager();

	virtual bool store() override;
	virtual void close() override;
};
