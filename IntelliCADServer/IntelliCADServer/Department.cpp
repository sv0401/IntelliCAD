#include "stdafx.h"
#include  "Department.h"

using namespace std;

Department::Department(const int index, const tstring &name, const tstring &abbreviation) :
	index(index), name(name), abbreviation(abbreviation)
{}

Department::Department(ReadStream &stream)
{
	stream.getAs(index);
	stream.getAs(__nameLength);
	stream.getAs(__abbreviationLength);

	name.resize(__nameLength);
	stream.get(name.data(), __nameLength * static_cast<int>(sizeof(int)));

	abbreviation.resize(__abbreviationLength);
	stream.get(abbreviation.data(), __abbreviationLength * static_cast<int>(sizeof(int)));
}

vector<ElementMeta> Department::_getStreamMeta() const
{
	__nameLength = static_cast<int>(name.size());
	__abbreviationLength = static_cast<int>(abbreviation.size());

	return
	{
		{ sizeof(index), &index },
		{ sizeof(__nameLength), &__nameLength },
		{ sizeof(__abbreviationLength), &__abbreviationLength },

		{ __nameLength * static_cast<int>(sizeof(int)), name.data() },
		{ __abbreviationLength * static_cast<int>(sizeof(int)), abbreviation.data() }
	};
}