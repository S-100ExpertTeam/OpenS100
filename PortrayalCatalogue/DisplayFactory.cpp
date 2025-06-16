#include "stdafx.h"
#include "DisplayFactory.h"

S100_DisplayList* CDisplayFactory::createDisplayList()
{
	S100_DisplayList* displayList = new S100_DisplayList();

	return displayList;
}

bool CDisplayFactory::createPointInstruction(S100_DisplayList* displayList,
	std::string& featureID,
	std::string& drawingPriority,
	std::string& displayPlane,
	std::string& viewingGroup,
	std::string& scaleMinimum,
	std::string& reference,
	std::string& rotation,
	std::string& augmentedPoint,
	std::list<std::string> liSpatialReference)
{
	if (displayList == nullptr)
		return false;

	S100_PointInstruction* instruction = new S100_PointInstruction();

	instruction->SetFeatureReference(string2wstring(featureID));
	instruction->SetDrawingPriority(string2wstring(drawingPriority));
	instruction->SetDisplayPlane(string2wstring(displayPlane));
	instruction->SetViewingGroup(string2wstring(viewingGroup));
	instruction->SetScaleMinimum(string2wstring(scaleMinimum));

	std::vector<std::string> vecRotation = splitString(rotation, ',');
	instruction->SetSymbol(string2wstring(reference), ((int)vecRotation.size() == 2) ? std::stod(vecRotation[1]) : 0.0);
	vecRotation.clear();

	std::vector<std::string> vecAugmentedPoint = splitString(augmentedPoint, ',');
	if (vecAugmentedPoint.size() == 3)
		instruction->SetVectorPoint(string2wstring(vecAugmentedPoint[1]), string2wstring(vecAugmentedPoint[2]));
	vecAugmentedPoint.clear();

	for (const auto& iter : liSpatialReference)
	{
		std::vector<std::string> vecSpatailRef = splitString(iter, '|');
		if (vecSpatailRef.size() == 2)
			instruction->SetSpatialReference(vecSpatailRef[0], vecSpatailRef[1]);
		vecSpatailRef.clear();
	}

	displayList->SetDisplayInstruction((S100_Instruction*)instruction);

	return true;
}

bool CDisplayFactory::createLineInstruction(S100_DisplayList* displayList,
	std::string& featureID,
	std::string& drawingPriority,
	std::string& displayPlane,
	std::string& viewingGroup,
	std::string& scaleMinimum,
	std::string& reference,
	std::string& lineStyle,
	std::string& dash,
	std::list<std::string> liSpatialReference)
{
	if (displayList == nullptr)
		return false;

	S100_LineInstruction* instruction = new S100_LineInstruction();

	instruction->SetFeatureReference(string2wstring(featureID));
	instruction->SetDrawingPriority(string2wstring(drawingPriority));
	instruction->SetDisplayPlane(string2wstring(displayPlane));
	instruction->SetViewingGroup(string2wstring(viewingGroup));
	instruction->SetScaleMinimum(string2wstring(scaleMinimum));

	instruction->SetLineStyleReference(string2wstring(reference));
	instruction->SetLineStyle(lineStyle, dash);
	for (const auto& iter : liSpatialReference)
	{
		std::vector<std::string> vecSpatailRef = splitString(iter, '|');
		if (vecSpatailRef.size() == 2)
			instruction->SetSpatialReference(vecSpatailRef[0], vecSpatailRef[1]);
		vecSpatailRef.clear();
	}

	displayList->SetDisplayInstruction((S100_Instruction*)instruction);

	return true;
}

bool CDisplayFactory::createAreaInstructionA(S100_DisplayList* displayList,
	std::string& featureID,
	std::string& drawingPriority,
	std::string& displayPlane,
	std::string& viewingGroup,
	std::string& scaleMinimum,
	std::string& colorFill)
{
	if (displayList == nullptr)
		return false;

	S100_AreaInstruction* instruction = new S100_AreaInstruction();

	instruction->SetFeatureReference(string2wstring(featureID));
	instruction->SetDrawingPriority(string2wstring(drawingPriority));
	instruction->SetDisplayPlane(string2wstring(displayPlane));
	instruction->SetViewingGroup(string2wstring(viewingGroup));
	instruction->SetScaleMinimum(string2wstring(scaleMinimum));

	std::vector<std::string> vecColorFill = splitString(colorFill, ',');
	std::wstring name = (vecColorFill.size() > 0) ? string2wstring(vecColorFill[0]) : _T("");
	std::wstring transparency = (vecColorFill.size() > 1) ? string2wstring(vecColorFill[1]) : _T("");
	instruction->SetAreaFill(name, name, transparency);
	vecColorFill.clear();

	displayList->SetDisplayInstruction((S100_Instruction*)instruction);

	return true;
}

bool CDisplayFactory::createAreaInstructionB(S100_DisplayList* displayList,
	std::string& featureID,
	std::string& drawingPriority,
	std::string& displayPlane,
	std::string& viewingGroup,
	std::string& scaleMinimum,
	std::string& areaFillReference)
{
	if (displayList == nullptr)
		return false;

	S100_AreaInstruction* instruction = new S100_AreaInstruction();

	instruction->SetFeatureReference(string2wstring(featureID));
	instruction->SetDrawingPriority(string2wstring(drawingPriority));
	instruction->SetDisplayPlane(string2wstring(displayPlane));
	instruction->SetViewingGroup(string2wstring(viewingGroup));
	instruction->SetScaleMinimum(string2wstring(scaleMinimum));

	instruction->SetAreaFill(string2wstring(areaFillReference));

	displayList->SetDisplayInstruction((S100_Instruction*)instruction);

	return true;
}

bool CDisplayFactory::createTextInstruction(S100_DisplayList* displayList,
	std::string& featureID,
	std::string& drawingPriority,
	std::string& displayPlane,
	std::string& viewingGroup,
	std::string& scaleMinimum,
	std::string& textAlignVertical,
	std::string& textAlignHorizontal,
	std::string& localOffset,
	std::string& textInstruction,
	std::string& fontSize,
	std::string& fontSlant,
	std::string& fontColor)
{
	if (displayList == nullptr)
		return false;

	S100_TextInstruction* instruction = new S100_TextInstruction();

	instruction->SetFeatureReference(string2wstring(featureID));
	instruction->SetDrawingPriority(string2wstring(drawingPriority));
	instruction->SetDisplayPlane(string2wstring(displayPlane));
	instruction->SetViewingGroup(string2wstring(viewingGroup));
	instruction->SetScaleMinimum(string2wstring(scaleMinimum));

	S100_TextPoint* pTextPoint = new S100_TextPoint();
	pTextPoint->SetVerticalAlignment(string2wstring(textAlignVertical));
	pTextPoint->SetHorizontalAlignment(string2wstring(textAlignHorizontal));
	std::vector<std::string> vecLocalOffset = splitString(localOffset, ',');
	if (vecLocalOffset.size() == 2)
		pTextPoint->SetOffset(string2wstring(vecLocalOffset[0]), string2wstring(vecLocalOffset[1]));
	vecLocalOffset.clear();

	std::vector<std::string> vecText = splitString(textInstruction, ',');
	if (!vecText.empty())
		pTextPoint->SetElement(string2wstring(vecText[0]), string2wstring(fontSize), fontColor, string2wstring(fontSlant));
	vecText.clear();

	instruction->SetTextPoint(pTextPoint);

	displayList->SetDisplayInstruction((S100_Instruction*)instruction);

	return true;
}

bool CDisplayFactory::createAugmentedRay(S100_DisplayList* displayList,
	std::string& featureID,
	std::string& drawingPriority,
	std::string& displayPlane,
	std::string& viewingGroup,
	std::string& scaleMinimum,
	std::string& lineStyle,
	std::string& dash,
	std::string& augmentedRay)
{
	if (displayList == nullptr)
		return false;

	S100_AugmentedRay* instruction = new S100_AugmentedRay();

	instruction->SetFeatureReference(string2wstring(featureID));
	instruction->SetDrawingPriority(string2wstring(drawingPriority));
	instruction->SetDisplayPlane(string2wstring(displayPlane));
	instruction->SetViewingGroup(string2wstring(viewingGroup));
	instruction->SetScaleMinimum(string2wstring(scaleMinimum));

	instruction->SetLineStyle(lineStyle, dash);

	std::vector<std::string> vecAugmentedRay = splitString(augmentedRay, ',');
	if (vecAugmentedRay.size() == 4)
	{
		instruction->SetDirection(string2wstring(vecAugmentedRay[1]));
		instruction->SetLength(string2wstring(vecAugmentedRay[3]));
	}
	vecAugmentedRay.clear();

	displayList->SetDisplayInstruction((S100_Instruction*)instruction);

	return true;
}

bool CDisplayFactory::createAugmentedPath(S100_DisplayList* displayList,
	std::string& featureID,
	std::string& drawingPriority,
	std::string& displayPlane,
	std::string& viewingGroup,
	std::string& scaleMinimum,
	std::string& lineStyle,
	std::string& dash,
	std::string& arcByRadius)
{
	if (displayList == nullptr)
		return false;

	S100_AugmentedPath* instruction = new S100_AugmentedPath();

	instruction->SetFeatureReference(string2wstring(featureID));
	instruction->SetDrawingPriority(string2wstring(drawingPriority));
	instruction->SetDisplayPlane(string2wstring(displayPlane));
	instruction->SetViewingGroup(string2wstring(viewingGroup));
	instruction->SetScaleMinimum(string2wstring(scaleMinimum));

	instruction->SetLineStyle(lineStyle, dash);

	std::vector<std::string> vecArcByRadius = splitString(arcByRadius, ',');
	if (vecArcByRadius.size() == 5)
		instruction->SetPath(string2wstring(vecArcByRadius[0]),
			string2wstring(vecArcByRadius[1]),
			string2wstring(vecArcByRadius[2]),
			string2wstring(vecArcByRadius[3]),
			string2wstring(vecArcByRadius[4]));
	vecArcByRadius.clear();

	displayList->SetDisplayInstruction((S100_Instruction*)instruction);

	return true;
}

void CDisplayFactory::destroyDisplayList(S100_DisplayList* displayList)
{
	if (displayList)
		delete displayList, displayList = nullptr;

	return;
}

inline std::wstring CDisplayFactory::string2wstring(const std::string& str)
{
	if (str.empty())
		return _T("");

	USES_CONVERSION_EX;
	LPCWSTR wideStr = A2CW_EX(str.c_str(), str.length());
	if (wideStr == nullptr)
		return _T("");

	return std::wstring(wideStr);
}

inline std::vector<std::string> CDisplayFactory::splitString(const std::string& str, char delimiter)
{
	std::vector<std::string> tokens;
	size_t start = 0, end;

	while ((end = str.find(delimiter, start)) != std::string::npos) {
		if (end > start) { // Ignore empty substrings
			tokens.push_back(str.substr(start, end - start));
		}
		start = end + 1;
	}

	// Add the last token if it's not empty
	if (start < str.size()) {
		tokens.push_back(str.substr(start));
	}

	return tokens;
}


