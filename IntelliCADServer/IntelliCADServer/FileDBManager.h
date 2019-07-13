#pragma once

#include "DBManager.h"
#include "ServerFileDBSectionType.h"

class FileDBManager : public DBManager
{
public:
	virtual bool load(const std::tstring &rootDir, const bool createIfNotExistent) override;

	std::tstring getPath(const ServerFileDBSectionType sectionType, const std::tstring &fileName) const;

	virtual bool store() override;
	virtual void close() override;
};
