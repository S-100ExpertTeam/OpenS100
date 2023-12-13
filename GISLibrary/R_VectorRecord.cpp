#include "stdafx.h"
#include "R_VectorRecord.h"

R_VectorRecord::R_VectorRecord()
{

}

R_VectorRecord::R_VectorRecord(const R_VectorRecord& other)
{
	for (const auto& iter : other.m_inas)
	{
		F_INAS* inas = iter;
		m_inas.push_back(inas);
	}
}

R_VectorRecord::~R_VectorRecord()
{

}

std::vector<F_INAS*> R_VectorRecord::GetAllInformationAssociations()
{
	std::vector<F_INAS*> result;
	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		result.push_back(*i);
	}

	return result;
}