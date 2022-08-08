#pragma once

#include "Field.h"

class F_DataDescriptiveField :
	public Field
{
public:
	F_DataDescriptiveField();
	virtual ~F_DataDescriptiveField();

private:
	char spaceOld = 'бр';
	char spaceNew = 0x20;

	char UTOld = 'бу';
	char UTNew = 0x1F;

	char FTOld = 'бх';
	char FTNew = 0x1E;

public:
	std::string Replace(std::string text);

	bool Write_ATTR_DataDescriptiveField(CFile* file);
	bool Write_INAS_DataDescriptiveField(CFile* file);
	bool Write_DSID_DataDescriptiveField(CFile* file);
};

