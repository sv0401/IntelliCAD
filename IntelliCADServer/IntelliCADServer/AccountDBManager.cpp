#include "stdafx.h"
#include "AccountDBManager.h"
#include "Constant.h"
#include "Parser.hpp"

using namespace std;
using namespace tinyxml2;

const tstring &AccountDBManager::_getXMLSubPath() const
{
	return Constant::DB::ACCOUNT_SUBPATH;
}

shared_ptr<Account> AccountDBManager::getAccount(const tstring &id) const
{
	shared_ptr<Account> retVal = nullptr;

	const XMLNode * root 
		= __doc.FirstChildElement("account_list");

	const XMLNode * accountNode = root->FirstChild();

	while (accountNode != nullptr) {

		const tstring account_id 
			= Parser::string$tstring(
				(accountNode->FirstChildElement("id"))->GetText());

		if (id == account_id) {
			
			const tstring account_pass
				= Parser::string$tstring(
				(accountNode->FirstChildElement("password"))->GetText());
			const tstring account_name
				= Parser::string$tstring(
				(accountNode->FirstChildElement("name"))->GetText());

			retVal = make_shared<Account>(
				account_id, account_pass, account_name);

			break;
		}

		accountNode = accountNode->NextSibling();
	}

	return retVal;
}