#pragma once

#include "indeterminatePosition.h"

class IndeterminateDate
{
public:
	IndeterminateDate();
	virtual ~IndeterminateDate();

public:
	indeterminatePosition *indeterminatePosition = nullptr;
	time_t dateValue = 0;
};

