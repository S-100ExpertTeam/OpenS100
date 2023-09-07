#pragma once

#include "../GISLibrary/Layer.h"

class TestGISLibrary
{
public:
	static int layerKey;

public:
	static void TestCreateNewCode();
	static void TestSave();
	static void CreateNewLayer();
	static void CreateMemoryLayer();
	static void CopySelectedFeatureToNewLayer();
	static void OffFeature();
	static void OpenUpdate();
};