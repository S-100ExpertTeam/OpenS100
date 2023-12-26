#pragma once

#include "Field.h"

enum class DDFType
{
	ATTR = 0,
	INAS,
	DSID,
	DSSI,
	ATCS,
	ITCS,
	FTCS,
	IACS,
	FACS,
	ARCS,
	CSID,
	CRSH,
	CSAX,
	PROJ,
	GDAT,
	VDAT,
	IRID,
	COCC,
	C2IT,
	C3IT,
	C2FT,
	C3FT,
	C2IL,
	C3IL,
	C2FL,
	C3FL,
	KNOT,
	DRVF,
	DRVI,
	PRID,
	MRID,
	CRID,
	PTAS,
	SECC,
	SEGH,
	CIPM,
	ARPM,
	SPLI,
	PSPL,
	CCID,
	CCOC,
	CUCO,
	SRID,
	RIAS,
	FRID,
	FOID,
	SPAS,
	FASC,
	THAS,
	MASK
};

class F_DataDescriptiveField :
	public Field
{
public:
	F_DataDescriptiveField();
	F_DataDescriptiveField(const F_DataDescriptiveField& other);
	F_DataDescriptiveField(DDFType type);
	virtual ~F_DataDescriptiveField();

public:
	std::string spaceOld = "бр";
	std::string spaceNew = { 0x20 };

	std::string UTOld = "бу";
	std::string UTNew = { 0x1F };

	std::string FTOld = "бх";
	std::string FTNew = { 0x1E };

	int type = 0;

public:
	std::string Replace(std::string text);
	void ReadField(BYTE*& buf) {};
	bool WriteField(CFile* file);
	int GetFieldLength();
	std::string GetTagName();

	virtual F_DataDescriptiveField* Clone() const;
public:
	bool operator==(const F_DataDescriptiveField& other) const;
	//bool operator<(const F_DataDescriptiveField& other) const;
};

