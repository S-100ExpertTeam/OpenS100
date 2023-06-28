#pragma once

#include "S100H5.h"

class S102H5 : 
	public S100H5
{
public:
	S102H5();
	virtual ~S102H5();

public:
	bool Open(CString _filepath) override;
};

