#pragma once

#include "S101Cell.h"

#include "..\\FeatureCatalog\\FeatureCatalogue.h"

#include "..\\S100_SVG_D2D1_DLL\\SVGManager.h"

#include <vector>
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
	Scaler					*scaler = nullptr;

	// Background layer.
	CList <Layer*, Layer* > m_listBackgroundLayer;

	/*
	** Cell Layer
	*/
	std::vector<Layer*>		m_listLayer;
	std::vector<Layer*>		m_SelectedLayer;

	// Overlay Layer
	MBR						mbr;

	std::set<int> lineSuppressionMap;
	bool m_baseMapOn = true;

public:
	void SetViewMBR(RECT r);

	bool AddBackgroundLayer(CString _filepath);
	bool AddLayer(CString _filepath);
	int isUpdate(CString filePath);
	bool AddLayer(Layer* layer);
	bool AddUpdateLayer(Layer* Base, Layer* Update);
	S101Cell* FindBaseLayer(S101Cell* cell);

	

	bool AddOverlayLayer(CString _filepath);;
	// Drawing temporary S-111 files.
	unsigned m_surfaceCurrentIndex = 0;

	void BuildPortrayalCatalogue(Layer* l);
	void S101RebuildPortrayal(/*PORTRAYAL_BUILD_TYPE type = PORTRAYAL_BUILD_TYPE::ALL*/);
	void Draw(HDC &hDC, int offset = 0);
	void DrawLayerList(HDC &hDC, int offset = 0);
	void DrawBackground(HDC &hDC, int offset = 0);

	void DrawS100Datasets(HDC &hDC, int offset = 0);
	void DrawS100Layer(HDC& hDC, int offset, S100Layer* layer);
	float GetTextPlaceBearingValue(S101Cell* c, R_FeatureRecord* fr);
	void DrawSemiCircle(float x, float y, float radius, float startAngle, float endAngle);
	void AddSymbolDrawing(
		int ,
		HDC &hdc,
		int offset,
		std::list<SENC_Instruction*>*augmentedRay,
		std::list<SENC_Instruction*>*augmentedPath,
		std::list<SENC_Instruction*>* point,
		std::list<SENC_Instruction*>* line,
		std::list<SENC_Instruction*>* area,
		std::list<SENC_Instruction*>* text,
		PortrayalCatalogue *pc = nullptr);


	void DrawInformationLayer(HDC &hDC, int nIndex);
	void ClearInformationLayer(int nindex);
	void ClearInformationLayer(CString filepath);
	void ClearAllInformationLayer();

	void DrawDataCoverageOverscale(HDC &hdc);

	void DrawValidationLayers(HDC &hDC, int offset = 0);
	void DrawOverlay(HDC &hDC, int type, int offset = 0);
	void GetLayer(int index, Layer *_layer);
	Layer* GetLayer(int index);

	int GetLayerIndex(CString _pathName);
	CString GetLayerName(int index);
	BOOL IsOn(int index);
	void DeleteLayer(int index);
	void DeleteLayer(CString filepath);
	void DeleteAllLayer();
	void ReMBR();

	int GetLayerCount();

	void ChangeS100ColorPalette(std::wstring paletteName);

	Scaler* GetScaler();

	void SuppressS101Lines(std::set<int>& drawingPriority, DrawingSet* drawingSet);

	// Result
	// 1 : S-57
	// 2 : S-101
	// 3 : S-100 GML
	// 4 : S-100 H5
	static int CheckFileType(CString path, int update);
	static int CheckFileType(CString path);
};