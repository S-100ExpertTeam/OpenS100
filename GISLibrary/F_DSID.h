#pragma once
#include <string>
#include <vector>
#include "BinaryWriter.h"
#include "Field.h"
#include "RecordName.h"

// Dataset Identification Field
class F_DSID : public Field
{
public:
	F_DSID();
	F_DSID(const F_DSID& other);
	virtual ~F_DSID();

public:
	RecordName m_name;
	std::wstring m_ensp; //encodingSpecification(have fixed value) //S-100 Part 10a
	std::wstring m_ened; //encodingSpecificationEdition(have fixed value) //1.1
	std::wstring m_prsp;	//productIdentifier(have fixed value) //INT.IHO.S-101.1.0
	std::wstring m_pred; //productEdition(have fixed value) //1.0
	std::wstring m_prof; //applicationProfile(have fixed value) //1
	std::wstring m_dsnm; //datasetFileIdentifier
	std::wstring m_dstl; //datasetTitle
	std::wstring m_dsrd; //datasetReferenceDate
	std::wstring m_dslg; //datasetLanguage(have fixed value) //EN
	std::wstring m_dsab; //datasetAbstract
	std::wstring m_dsed; //dataset Edition
	std::vector<int> m_dstc;

public:
	virtual F_DSID& operator=(const F_DSID& other);

public:
	void ReadField(BYTE *&buf);
	bool WriteField(s100::io::BinaryWriter* file);
	int GetFieldLength();
};