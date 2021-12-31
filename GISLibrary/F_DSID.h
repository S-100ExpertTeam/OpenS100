#pragma once
#include "Field.h"
#include "RecordName.h"

// Dataset Identification Field
class F_DSID : Field
{
public:
	F_DSID();
	virtual ~F_DSID();

public:
	RecordName m_name;
	CString m_ensp; //encodingSpecification(have fixed value) //S-100 Part 10a
	CString m_ened; //encodingSpecificationEdition(have fixed value) //1.1
	CString m_prsp;	//productIdentifier(have fixed value) //INT.IHO.S-101.1.0
	CString m_pred; //productEdition(have fixed value) //1.0
	CString m_prof; //applicationProfile(have fixed value) //1
	CString m_dsnm; //datasetFileIdentifier
	CString m_dstl; //datasetTitle
	CString m_dsrd; //datasetReferenceDate
	CString m_dslg; //datasetLanguage(have fixed value) //EN
	CString m_dsab; //datasetAbstract
	CString m_dsed; //dataset Edition
	CArray<int> m_dstc;

public:
	void ReadField(BYTE *&buf);
	BOOL Save(CFile *file);
	int GetFieldLength();
};