#pragma once

#include "TransferFunctionType.h"

class TransferFunctionUpdateListener
{
public:
	virtual void onTransferFunctionUpdate(
		const TransferFunctionType funcType, const double *const pNewTable) = 0;
};