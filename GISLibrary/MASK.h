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
	int m_mind = 0;
	/*
	* Mask Update Instruction
	* Format		: b11
	* {1} - Insert
	* {2} - Delete
	*/
	int m_muin = 0;
	
public:
	static int GetSize();
};