#pragma once

#include "S101Cell.h"
#include "Layer.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../GeoMetryLibrary/ENCCommon.h"

#include <vector>
#include <list>
#include <functional>
#include <unordered_map>

class Scaler;
class ENCCell;
class PortrayalCatalogue;
class SENC_Instruction;
class DrawingSet;

class LayerManager
{
public:
	LayerManager();
	LayerManager(Scaler* scaler);
	virtual ~LayerManager();

public:
	Scaler* scaler = nullptr;

	// Background layer
	Layer backgroundLayer;

	std::list<Layer*> layers;

	// Key : ID
	std::unordered_map<int, Layer*> mapLayer;

	MBR mbr;

	// RCID of already rendered curveRecords
	std::set<int> lineSuppressionMap;
	bool m_baseMapOn = true;
	bool onIC = true;

public:
	void SetViewMBR(RECT r);

	bool AddBackgroundLayer(CString _filepath);

	// Success : id (> 0)
	// Fail : -1
	int AddLayer(CString _filepath);
	int AddLayer(Layer* layer);

	int isUpdate(CString filePath);
	bool AddUpdateLayer(Layer* Base, Layer* Update);

	void BuildPortrayalCatalogue(Layer* l);
	void S101RebuildPortrayal(/*PORTRAYAL_BUILD_TYPE type = PORTRAYAL_BUILD_TYPE::ALL*/);
	void Draw(HDC &hDC, int offset = 0);
	void DrawBackground(HDC &hDC, int offset = 0);

	void DrawS100Datasets(HDC &hDC, int offset = 0);
	void DrawNonS100Datasets(HDC &hDC, int offset = 0);
	void DrawS100Layer(HDC& hDC, int offset, S100Layer* layer);
	void SetDrawingInstruction(S100Layer* layer);
	void DrawS100Layer(HDC& hDC, int offset, S100Layer* layer, int minPriority, int maxPriority);
	void AddSymbolDrawing(
		int ,
		HDC &hdc,
		int offset,
		std::list<SENC_Instruction*>* augmentedRay,
		std::list<SENC_Instruction*>* augmentedPath,
		std::list<SENC_Instruction*>* point,
		std::list<SENC_Instruction*>* line,
		std::list<SENC_Instruction*>* area,
		std::list<SENC_Instruction*>* text,
		PortrayalCatalogue *pc = nullptr);

	void DrawInformationLayer(HDC& hDC, Layer* layer);
	void DrawInformationLayer(HDC& hDC, int nIndex);
	void DrawInformationLayerByKey(HDC& hDC, int key);

	Layer* GetLayer(int index);
	Layer* GetLayerByKey(int key);

	CString GetLayerName(int index);
	CString GetLayerNameByKey(int key);

	bool IsOn(int index);
	bool IsOnByKey(int key);

	void DeleteLayer(int index);
	void DeleteLayer(CString filepath);
	void DeleteLayerByKey(int key);

	void ReMBR();

	void ChangeS100ColorPalette(GeoMetryLibrary::ColorTable value);
	void ChangeS100ColorPalette(std::wstring paletteName);

	Scaler* GetScaler();

	void SuppressS101Lines(std::set<int>& drawingPriority, DrawingSet* drawingSet);

	int LayerCount();

	// Return type : enum S100_FileType
	static S100_FileType CheckFileType(CString path, int update);
	static S100_FileType CheckFileType(CString path);

	int CreateLayerID();

	int pathToProductNumber(CString path);
};