#pragma once

#include <vector>

class DrawingInstruction;
class DisplayList
{
public:
	DisplayList();
	virtual ~DisplayList();

public:
	std::vector<DrawingInstruction*> m_vecInstruction;

public:
	void Sort();
	void Release();
};