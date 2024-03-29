#pragma once

#include "Record.h"
#include "SGeometry.h"
#include "F_INAS.h"

class R_VectorRecord : public Record
{
public:
	R_VectorRecord();
	R_VectorRecord(const R_VectorRecord& other);
	virtual ~R_VectorRecord();

public:
	std::list<F_INAS*> m_inas;

public:
	virtual R_VectorRecord operator=(const R_VectorRecord& other);

public:
	std::vector<F_INAS*> GetAllInformationAssociations();
};