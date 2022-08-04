#include "stdafx.h"
#include "ObjectName.h"

ObjectName::ObjectName(void)
{
	m_agen = 0;
	m_fidn = 0;
	m_fids = 0;
}

ObjectName::~ObjectName(void)
{

}

__int64 ObjectName::GetName()
{
	return ((__int64)m_agen) << 48 | ((__int64)m_fidn) << 16 | m_fids;
}

int ObjectName::GetLength()
{
	return 8;
}