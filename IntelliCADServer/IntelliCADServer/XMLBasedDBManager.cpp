#include "stdafx.h"
#include "XMLBasedDBManager.h"
#include "Parser.hpp"
#include "Constant.h"

using namespace std;
using namespace tinyxml2;

bool XMLBasedDBManager::load(const tstring &rootDir, const bool createIfNotExistent)
{
	if (isLoaded())
		return true;

	const string path = Parser::tstring$string(rootDir + _getXMLSubPath());

	// XML Load logic
	XMLError error = __doc.LoadFile(path.c_str());
	_loaded = (error == XMLError::XML_SUCCESS);

	return _loaded;
}

bool XMLBasedDBManager::store()
{
	if (!isLoaded())
		return false;

	const string path
		= Parser::tstring$string(Constant::DB::ROOT_DIR + _getXMLSubPath());

	XMLError error = __doc.SaveFile(path.c_str());

	return error == XMLError::XML_SUCCESS;
}

void XMLBasedDBManager::close()
{

}