#pragma once

struct MASK
{
public:
	RecordName m_name;
	/*
	* Mask Indicator
	* Format		: b11
	* {1} - Truncated by the dataset limit
	* {2} - Suppress portrayal
	*/
	int m_mind;
	/*
	* Mask Update Instruction
	* Format		: b11
	* {1} - Insert
	* {2} - Delete
	*/
	int m_muin;
	
public:
	static int GetSize();
};