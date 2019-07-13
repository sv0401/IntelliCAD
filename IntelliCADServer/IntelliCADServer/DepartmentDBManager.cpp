#include "stdafx.h"
#include "DepartmentDBManager.h"
#include "Constant.h"
#include "Parser.hpp"

using namespace std;
using namespace tinyxml2;

const tstring &DepartmentDBManager::_getXMLSubPath() const
{
	return Constant::DB::DEPT_SUBPATH;
}

shared_ptr<Department> DepartmentDBManager::getDepartment(const int index) const
{
	shared_ptr<Department> retVal = nullptr;

	const XMLNode * root
		= __doc.FirstChildElement("dept_list");

	const XMLNode * deptNode = root->FirstChild();

	while (deptNode != nullptr) {

		int dept_index;
		(deptNode->FirstChildElement("index"))->QueryIntText(&dept_index);

		if (index == dept_index) {

			const tstring dept_name
				= Parser::string$tstring(
				(deptNode->FirstChildElement("name"))->GetText());
			const tstring dept_abbreviation
				= Parser::string$tstring(
				(deptNode->FirstChildElement("abbreviation"))->GetText());

			retVal = make_shared<Department>(
				dept_index, dept_name, dept_abbreviation);

			break;
		}

		deptNode = deptNode->NextSibling();
	}

	return retVal;
}

shared_ptr<Department> DepartmentDBManager::getDepartment(const tstring &name) const
{
	shared_ptr<Department> retVal = nullptr;

	const XMLNode * root
		= __doc.FirstChildElement("dept_list");

	const XMLNode * deptNode = root->FirstChild();

	while (deptNode != nullptr) {

		const tstring dept_name
			= Parser::string$tstring(
			(deptNode->FirstChildElement("name"))->GetText());

		if (name == dept_name) {

			int dept_index;
			(deptNode->FirstChildElement("index"))->QueryIntText(&dept_index);
			
			const tstring dept_abbreviation
				= Parser::string$tstring(
				(deptNode->FirstChildElement("abbreviation"))->GetText());

			retVal = make_shared<Department>(
				dept_index, dept_name, dept_abbreviation);

			break;
		}

		deptNode = deptNode->NextSibling();
	}

	return retVal;
}

vector<Department> DepartmentDBManager::getDepartmentList() const
{
	vector<Department> dv;

	const XMLNode * root
		= __doc.FirstChildElement("dept_list");

	const XMLNode * deptNode = root->FirstChild();

	while (deptNode != nullptr) {

		int dept_index;
		(deptNode->FirstChildElement("index"))->QueryIntText(&dept_index);

		const tstring dept_name
			= Parser::string$tstring(
			(deptNode->FirstChildElement("name"))->GetText());

		const tstring dept_abbreviation
			= Parser::string$tstring(
			(deptNode->FirstChildElement("abbreviation"))->GetText());

		dv.emplace_back(Department(
				dept_index, dept_name, dept_abbreviation));

		deptNode = deptNode->NextSibling();
	}

	return dv;
}