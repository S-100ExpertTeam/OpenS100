#pragma once
#include "AugmentedLineOrArea.h"

class AreaFill;
class AugmentedArea :
	public AugmentedLineOrArea
{ 
public:
	AugmentedArea();
	virtual ~AugmentedArea();

public:
	AreaFill *pAreaFill;
};