#pragma once

#include "Record.h"
#include "SGeometry.h"

class R_VectorRecord : public Record
{
public:
	R_VectorRecord();
	virtual ~R_VectorRecord();

public:
	std::list<F_INAS*> m_inas;

public:
	std::vector<F_INAS*> GetAllInformationAssociations();
};