#include "stdafx.h"
#include "R_DDR.h"

#include <algorithm>

R_DDR::R_DDR()
{

}

R_DDR::~R_DDR()
{

}

bool R_DDR::WriteRecord(CFile* file)
{
	directory.clear();

	// Set directory
	int fieldOffset = 0;
	int fieldLength = f_FieldControlField.GetFieldLength();
	Directory dirFCF("0000", fieldLength, fieldOffset);
	directory.push_back(dirFCF);
	fieldOffset += fieldLength;

	for (auto i = f_DataDescriptiveField.begin(); i != f_DataDescriptiveField.end(); i++)
	{
		auto DDF = *i;
		fieldLength = DDF.GetFieldLength();
		Directory dir((char*)DDF.GetTagName().c_str(), fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	int totalFieldSize = fieldOffset;

	// Set leader
	SetLeader(totalFieldSize);
	leader.SetAsDDR();
	leader.WriteLeader(file);

	// Write directory
	WriteDirectory(file);

	// Write field area
	f_FieldControlField.WriteField(file);

	for (auto i = f_DataDescriptiveField.begin(); i != f_DataDescriptiveField.end(); i++)
	{
		auto DDF = *i;
		DDF.WriteField(file);
	}

	return true;
}

void R_DDR::AddDDF(F_DataDescriptiveField ddf)
{
	for (auto i = f_DataDescriptiveField.begin(); i != f_DataDescriptiveField.end(); i++)
	{
		if (*i == ddf)
		{
			return;
		}
	}

	f_DataDescriptiveField.push_back(ddf);
}

R_DDR* R_DDR::Clone() const
{
	R_DDR* r_ddr = new R_DDR();
	r_ddr->f_FieldControlField = f_FieldControlField;
	for (const auto& iter : f_DataDescriptiveField)
	{
		F_DataDescriptiveField ddf = iter;
		r_ddr->f_DataDescriptiveField.push_back(ddf);
	}

	return r_ddr;
}

