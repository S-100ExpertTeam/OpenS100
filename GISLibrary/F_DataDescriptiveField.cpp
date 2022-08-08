#include "stdafx.h"
#include "F_DataDescriptiveField.h"

#include <algorithm>

F_DataDescriptiveField::F_DataDescriptiveField()
{

}

F_DataDescriptiveField::~F_DataDescriptiveField()
{

}

std::string F_DataDescriptiveField::Replace(std::string text)
{
	std::replace(text.begin(), text.end(), spaceOld, spaceNew);
	std::replace(text.begin(), text.end(), UTOld, UTNew);
	std::replace(text.begin(), text.end(), FTOld, FTNew);

}

bool F_DataDescriptiveField::Write_ATTR_DataDescriptiveField(CFile* file)
{
	std::string ddf = "2600;&%/GAttribute¡ã*NATC!ATIX!PAIX!ATIN!ATVL¡ã(3b12,b11,A)¡å";

	ddf = Replace(ddf);

	file->Write(ddf.data(), ddf.size());
}

bool F_DataDescriptiveField::Write_INAS_DataDescriptiveField(CFile* file)
{
	std::string ddf ="3600;&%/GInformation¡àAssociation¡ãRRNM!RRID!NIAC!NARC!IUIN\\*NATC!ATIX!PAIX!ATIN!ATVL¡ã(b11,b14,2b12,b11,{3b12,b11,A})¡å";

	ddf = Replace(ddf);

	file->Write(ddf.data(), ddf.size());
}