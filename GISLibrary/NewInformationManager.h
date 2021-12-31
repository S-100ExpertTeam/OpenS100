#pragma once
#include <list>

class SpatialObject;
class NewInformationManager
{
public :
	NewInformationManager();
	virtual ~NewInformationManager();

public:
	SpatialObject* m_cell;
	int m_informationType;
};