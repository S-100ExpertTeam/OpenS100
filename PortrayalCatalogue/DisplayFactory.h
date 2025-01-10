#pragma once

#include "S100_DisplayList.h"
#include "S100_PointInstruction.h"
#include "S100_LineInstruction.h"
#include "S100_AreaInstruction.h"
#include "S100_TextInstruction.h"
#include "S100_ColorFill.h"
#include "S100_AugmentedRay.h"
#include "S100_AugmentedPath.h"
#include "S100_AreaFillReference.h"

class CDisplayFactory
{
public:
	static S100_DisplayList* createDisplayList();

	static bool createPointInstruction(S100_DisplayList*,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::list<std::string>);
	static bool createLineInstruction(S100_DisplayList*,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::list<std::string>);
	static bool createAreaInstructionA(S100_DisplayList*,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&);
	static bool createAreaInstructionB(S100_DisplayList*,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&);
	static bool createTextInstruction(S100_DisplayList*,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&);
	static bool createAugmentedRay(S100_DisplayList*,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&);
	static bool createAugmentedPath(S100_DisplayList*,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&,
		std::string&);
	static void destroyDisplayList(S100_DisplayList*);

private:
	static inline std::wstring string2wstring(const std::string& str);
	static inline std::vector<std::string> splitString(const std::string& str, char delimiter);
};

