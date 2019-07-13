#pragma once

#include "XMLBasedDBManager.h"
#include "Department.h"

class DepartmentDBManager : public XMLBasedDBManager
{
protected:
	virtual const std::tstring &_getXMLSubPath() const override;

public:
	std::shared_ptr<Department> getDepartment(const int index) const;
	std::shared_ptr<Department> getDepartment(const std::tstring &name) const;
	std::vector<Department> getDepartmentList() const;
};