#pragma once
#include "AugmentedLineOrArea.h"
#include "GraphicBasePackage.h"

class Path;
class AugmentedPath :
	public AugmentedLineOrArea
{
public:
	AugmentedPath();
	virtual ~AugmentedPath();

public:
	Path *pPath = nullptr;
	GraphicBasePackage::Path _path;
};