#include "stdafx.h"
#include "FileDBManager.h"
#include "Constant.h"
#include <fstream>
#include "Parser.hpp"

using namespace std;

bool FileDBManager::load(const tstring &rootDir, const bool createIfNotExistent)
{
	// load 기능 없음. 파일명 반환 기능만 제공하므로.
	_loaded = true;
	return _loaded;
}

tstring FileDBManager::getPath(const ServerFileDBSectionType sectionType, const tstring &fileName) const
{
	const tstring FILEDB_DIR = (Constant::DB::ROOT_DIR + Constant::DB::FILEDB_SUBDIR);
	tstring path = _T("");

	if (sectionType == ServerFileDBSectionType::CT_IMAGE)
		path = FILEDB_DIR + Constant::DB::CT_IMAGE_SUBDIR + fileName;

	// 파일 존재 파악
	ifstream read;
	read.open(Parser::tstring$string(path));
	if (read.fail()) // 파일 없음
		return _T("");
	
	return path;
}

bool FileDBManager::store()
{
	// store 기능 없음.
	return true;
}

void FileDBManager::close()
{

}