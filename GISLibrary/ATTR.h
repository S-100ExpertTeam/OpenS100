#pragma once

// Attribute Field
struct ATTR
{
public:
	ATTR();
	ATTR(const ATTR& other);
	ATTR(int NATC, int ATIX, int PAIX, int ATIN, CString& ATVL);
	virtual ~ATTR();

public:
	/*
	* Numeric attribute code
	* Format		: b12
	* Description	: A valid attribute code as defined in the ATCS field of the Dataset General Information Record
	*/
	int m_natc = 0;

	/*
	* Attribute index
	* Format		: b12
	* Description	: Index (position) of the attribute in the sequence of attributes with the same code and the same parent (starting with 1).
	*/
	int m_atix = 0;

	/*
	* Parent index
	* Format		: b12
	* Description	: Index (position) of the parent complex attribute within this ATTR field (starting with 1).
	  If the attribute has no parent (top level attribute) the value is 0.
	*/
	int m_paix = 0;

	/*
	* Attribute instruction
	* Format		: b11
	* {1} - Insert
	* {2} - Delete
	* {3} - Modify
	*/
	int m_atin = 0;

	/*
	* Attribute value
	* Format		: A()
	* A string containing a valid value for the domain of the attribute specified by the subfields above.
	*/
	CString m_atvl = L"";

public:
	static int GetOffsetToATVL();
	
	std::string getValueAsString();


};