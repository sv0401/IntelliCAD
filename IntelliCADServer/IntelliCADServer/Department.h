#pragma once

#include "tstring.h"
#include "Serializable.h"

class Department : public Serializable
{
private:
	mutable int __nameLength;
	mutable int __abbreviationLength;

protected:
	virtual std::vector<ElementMeta> _getStreamMeta() const override;

public:
	// 부서 인덱스
	int index;

	// 이름
	std::tstring name;

	// 약어
	std::tstring abbreviation;

	Department() = default;
	Department(const int index, const std::tstring &name, const std::tstring &abbreviation);

	explicit Department(ReadStream &stream);
};
