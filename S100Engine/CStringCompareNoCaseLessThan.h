#pragma once
#include <atlstr.h>

struct CStringCompareNoCaseLessThan
{
	bool operator()(const CString& lhs, const CString& rhs) const
	{
		return lhs.CompareNoCase(rhs) < 0;
	}
};