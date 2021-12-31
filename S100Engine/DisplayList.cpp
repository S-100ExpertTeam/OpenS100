#include "stdafx.h"
#include "DisplayList.h"
#include "DrawingInstruction.h"


DisplayList::DisplayList()
{

}

DisplayList::~DisplayList()
{
	Release();
}

void DisplayList::Release()
{
	for (auto i = m_vecInstruction.begin(); i != m_vecInstruction.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}

	m_vecInstruction.clear();
}

void DisplayList::Sort()
{
	if (m_vecInstruction.size() > 0)
	{
		std::qsort(&m_vecInstruction[0], m_vecInstruction.size(), sizeof(DrawingInstruction*), [](const void* a, const void* b)
		{
			DrawingInstruction** di1 = (DrawingInstruction**)a;
			DrawingInstruction** di2 = (DrawingInstruction**)b;

			if ((*di1)->m_nDrawingPriority < (*di2)->m_nDrawingPriority) return -1;
			if ((*di1)->m_nDrawingPriority > (*di2)->m_nDrawingPriority) return 1;
			if ((*di1)->m_nDrawingPriority == (*di2)->m_nDrawingPriority)
			{
				if ((*di1)->m_nRefGeometryType > (*di2)->m_nRefGeometryType) return -1;
				if ((*di1)->m_nRefGeometryType < (*di2)->m_nRefGeometryType) return 1;
				return 0;
			}
			return 0;
		});
	}
}