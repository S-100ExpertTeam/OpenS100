#pragma once
#include <atlstr.h>

struct FASC {
	/*
	* Numeric Attribute Code
	* Format		: b12
	* Description	: A valid attribute code as defined in the ATCS field of the Dataset General Information Record
	*/
	int m_natc;
	/*
	* Attribute index
	* Format		: b12
	* Description	: Index (position) of the attribute in the sequence of attributes with the same code and the same parent (starting with 1).
	*/
	int m_atix;
	/*
	* Parent index
	* Format		: b12
	* Description	: Index (position) of the parent complex attribute within this FASC field (starting with 1).
	* If the attribute has no parent (top level attribute) the value is 0.
	*/
	int m_paix;
	/*
	* Attribute Instruction
	* Format		: b11
	* Description	:	{1} - Insert
	*					{2} - Delete
	*					{3} - Modify
	*/
	int m_atin;
	/*
	* Attribute value
	* Format		: A()
	* Description	: A string containing a valid value for the domain of the attribute specified by the subfields above.
	*/
	CString m_atvl;
	static int GetSize();
};