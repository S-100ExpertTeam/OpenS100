#pragma once

#include "../GeoMetryLibrary/Scaler.h"

#include <list>
#include <string>
#include <optional>

class R_FeatureRecord;
class SENC_TextInstruction;
class SSurface;
class Symbol;
class SENC_SymbolFill;
class SENC_LineStyle;
class SENC_ArcByRadius;
class SENC_SpatialReference;
class S100_Instruction;
class S100SpatialObject;

class SENC_Instruction
{
public:
	SENC_Instruction();
	virtual ~SENC_Instruction();

public:
	/*
	*  Type Of Instruction
	*  0 : Null Instruction
	*  1 : Point Instruction
	*  2 : Line Instruction
	*  3 : Area Instruction
	** 4 : Coverage Instruction
	*  5 : Text Instruction
	** 6 : Augmented Point
	** 7 : Augmented Ray
	** 8 : Augmented Path
	** 9 : Augmented Area
	*/
	unsigned type = 0;

	//////////////////////////////////////////////////////////
	// Instruction Information
	std::optional<std::string> id;

	std::optional<std::string> parentId;

	std::optional<bool> hover;

	// Feature ID
	std::string featureReference;

	// FeatureType code (for performance)
	std::wstring code;
	
	// Vector ID
	std::list<SENC_SpatialReference*> spatialReference;
	
	/*
	* display group
	* 10001 ~ 20000   :		display base 
	* 20001 ~ 30000   :		standard
	* 30001 ~ 40000   :		others(all)
	*/
	int viewingGroup;
	/*
	* display plane
	* 0 : UNDERRADAR  
	* 1 : Overradar
	*/
	int displayPlane;
	/*
	* display priority
	* 10 ~ 90 (term unit is 10 in ENC Object)
	*/
	int drawingPriority;
	//int drawingPriority_initValue;

	// Scale Minimum
	unsigned scaleMinimum;
	// Scale Maximum
	unsigned scaleMaximum;

	// For Drawing
	GF::FeatureType* fr = nullptr;

public:
	void setID(std::string id);
	std::optional<std::string> getID();

	void setParentID(std::string id);
	std::optional<std::string> getParentID();

	void setHover(bool hover);
	std::optional<bool> getHover();

	std::string FeatureReference();
	int DrawingPriority();

	// Ensure that Instruction is referring to the Space Reference separately (if referring separately, it should be drawn in the geometry of the information in the corresponding Space Reference).
	bool HasSpatialReference();

	virtual void GetDrawPoints(Scaler *scaler, std::list<D2D1_POINT_2F> &points){};
	virtual void GetDrawPointsDynamic(Scaler *scaler, std::list<D2D1_POINT_2F> &points){};
	
	virtual void DrawInstruction(
		D2D1Resources* d2,
		Scaler *scaler, 
		PortrayalCatalogue* pc = nullptr) {};

	virtual void ChangePallete(PortrayalCatalogue *pc) {};

	virtual void FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output) {};

	static SENC_Instruction* S1002SENC(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output, S100SpatialObject* so);
};