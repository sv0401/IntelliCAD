#include "stdafx.h"
#include "ConfigDBManager.h"
#include "Constant.h"
#include "Parser.hpp"

using namespace std;
using namespace tinyxml2;

const tstring &ConfigDBManager::_getXMLSubPath() const
{
	return Constant::DB::CONFIG_SUBPATH;
}

tstring ConfigDBManager::getAttribute(
	const ConfigDBSectionType type, const tstring &attributeName) const
{
	// 첫 자식이 "<?xml version="1.0" encoding="utf-8"?>"으로 읽히는듯. 따라서 아래와 같이 변경
	   // const tinyxml2::XMLNode * node = doc.FirstChild();
	const XMLNode * node = __doc.FirstChildElement("config");

	const XMLElement * elem = nullptr, *attr = nullptr;

	switch (type) {
	case ConfigDBSectionType::SERVER_NETWORK:
		elem = node->FirstChildElement("server_network");
		break;
	}

	const string path = Parser::tstring$string(attributeName);
	attr = elem->FirstChildElement(path.c_str());

	const char* value = attr->GetText();

	return Parser::string$tstring(value);
}