#pragma once

#include "F_FRID.h"

class F_IRID : public F_FRID
{

public:
	F_IRID();
	F_IRID(RecordName recordName, int NFTC, int RVER = 1, int RUIN = 1);
	virtual ~F_IRID();

	int NITC();
	void NITC(int value);
};