#include "stdafx.h"
#include "R_VectorRecord.h"

R_VectorRecord::R_VectorRecord()
{

}

R_VectorRecord::R_VectorRecord(const R_VectorRecord& other) :Record(other)
{
	for (const auto& iter : other.m_inas)
	{
		F_INAS* inas = new F_INAS(*iter);
		m_inas.push_back(inas);
	}
}

R_VectorRecord::~R_VectorRecord()
{

}

R_VectorRecord R_VectorRecord::operator=(const R_VectorRecord& other)
{
	for (auto& iter : m_inas)
	{
		if (iter)
		{
			delete iter;
			iter = nullptr;
		}
	}
	m_inas.clear();

	Record::operator=(other);

	for (const auto& iter : other.m_inas)
	{
		F_INAS* inas = new F_INAS(*iter);
		m_inas.push_back(inas);
	}

	return *this;
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