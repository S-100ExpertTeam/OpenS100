#pragma once
class S100_IC_DrawingInstruction
{
	//@4
	std::string Identifier;
	std::string FeatureCode;
	//Product
	//GeometryType
	std::vector<std::string> AttributeCombination;
	int DrawingPriority;
	int ViewingGroup;
	std::string* SubstituteSymbolization;


};

