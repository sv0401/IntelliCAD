#pragma once

#include <memory>
#include "XMLBasedDBManager.h"
#include "Account.h"

class AccountDBManager : public XMLBasedDBManager
{
protected:
	virtual const std::tstring &_getXMLSubPath() const override;

public:
	std::shared_ptr<Account> getAccount(const std::tstring &id) const;
};
