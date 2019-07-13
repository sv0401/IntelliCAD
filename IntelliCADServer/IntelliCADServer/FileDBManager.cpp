#include "stdafx.h"
#include "FileDBManager.h"
#include "Constant.h"
#include <fstream>
#include "Parser.hpp"

using namespace std;

bool FileDBManager::load(const tstring &rootDir, const bool createIfNotExistent)
{
	// load ��� ����. ���ϸ� ��ȯ ��ɸ� �����ϹǷ�.
	_loaded = true;
	return _loaded;
}

tstring FileDBManager::getPath(const ServerFileDBSectionType sectionType, const tstring &fileName) const
{
	const tstring FILEDB_DIR = (Constant::DB::ROOT_DIR + Constant::DB::FILEDB_SUBDIR);
	tstring path = _T("");

	if (sectionType == ServerFileDBSectionType::CT_IMAGE)
		path = FILEDB_DIR + Constant::DB::CT_IMAGE_SUBDIR + fileName;

	// ���� ���� �ľ�
	ifstream read;
	read.open(Parser::tstring$string(path));
	if (read.fail()) // ���� ����
		return _T("");
	
	return path;
}

bool FileDBManager::store()
{
	// store ��� ����.
	return true;
}

void FileDBManager::close()
{

}