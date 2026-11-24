#pragma once
#include "DeviceTypes.h"
#include <string>

#include "S101Cell.h"
#include "Layer.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../GeoMetryLibrary/ENCCommon.h"

#include <vector>
#include <list>
#include <functional>
#include <memory>
#include <unordered_map>

using namespace GISLibrary;

class Scaler;
class ENCCell;
class PortrayalCatalogue;
class SENC_Instruction;
class DrawingSet;
class CatalogManager;

class LayerManager
{
public:
	LayerManager();
	LayerManager(Scaler* scaler, CatalogManager* catalogManager);
	virtual ~LayerManager();

public:
	Scaler* scaler = nullptr;
	CatalogManager* catalogManager = nullptr;

	// Background layer
	Layer backgroundLayer;

	std::list<Layer*> layers;

	// Key : ID
	std::unordered_map<int, Layer*> mapLayer;

	MBR mbr;

	bool m_baseMapOn = true;
	bool onIC = true;
	// With onIC off: consecutive vector layers in the same group are drawn as one chart, every
	// layer's drawing priority before the next priority, so a cell's area fills do not cover the
	// symbols and text a neighbouring cell draws across their common edge. Layers without a group
	// are drawn one at a time, each over the one before.
	std::unordered_map<const Layer*, int> drawGroups;
	bool m_isScreenFitEnabled = true;

private:
	// S-101 Filter 
	// Key : FeatureType code
	// Value : On/Off
	std::unordered_map<std::wstring, bool> featureOnOffMap;

	double s100Scale = -1;

public:
	void MoveLayerFromList(int from, int to);
	bool IsContainFilePathToLayer(const std::wstring& _filepath);


	void SetViewMBR(RECT r);

	bool AddBackgroundLayer(const std::wstring& _filepath);

	// Opens the file as a layer without adding it; nullptr when it cannot be opened. S-100
	// datasets take the newest catalogues of the product the file name implies.
	std::unique_ptr<Layer> CreateLayer(const std::wstring& _filepath);
	// Opens an S-100 dataset with the given catalogues (as resolved for it) and runs its
	// portrayal; nullptr without a feature catalogue or when the file cannot be opened.
	std::unique_ptr<Layer> CreateS100Layer(const std::wstring& _filepath, int productNumber, FeatureCatalogue* fc, PortrayalCatalogue* pc);

	// Success : id (> 0)
	// Fail : -1
	int AddLayer(const std::wstring& _filepath);
	int AddLayer(Layer* layer);
	int AddLayer(Layer* _layer, const std::wstring& FilePath, bool InsertBack = true);

	int AddLayerFront(const std::wstring& _filepath);
	int AddLayerFront(Layer* _layer);

	int isUpdate(const std::wstring& filePath);
	bool AddUpdateLayer(Layer* Base, Layer* Update);

	void BuildPortrayalCatalogue(Layer* l);
	void S101RebuildPortrayal();
	void Draw(HDC &hDC, int offset = 0);
	void DrawBackground(HDC &hDC, int offset = 0);

	void DrawS100Datasets(HDC &hDC, int offset = 0);
	void DrawECDISMode(HDC& hDC, int offset = 0);
	void DrawLayerContainers(HDC& hDC, int offset = 0);
	void DrawNonS100Datasets(HDC &hDC, int offset = 0);
	void DrawS100Layer(HDC& hDC, int offset, S100Layer* layer);
	void SetDrawingInstruction(S100Layer* layer);
	void DrawS100Layer(HDC& hDC, int offset, S100Layer* layer, int minPriority, int maxPriority);
	// Draws the layers priority by priority across them (see drawGroups).
	void DrawS100Layers(HDC& hDC, int offset, const std::vector<S100Layer*>& group);
	void AddSymbolDrawing(int drawingPrioriy, DrawingSet& drawingSet, PortrayalCatalogue* pc);
	void CollectInstructions(S100Layer* layer);

	void DrawInformationLayer(HDC& hDC, Layer* layer);

	Layer* GetLayer(int index);
	Layer* GetLayerByKey(int key);

	std::wstring GetLayerName(int index);
	std::wstring GetLayerNameByKey(int key);

	bool IsOn(int index);
	bool IsOnByKey(int key);

	// Takes the layer out without deleting it; nullptr when it is not one of this manager's.
	std::unique_ptr<Layer> ReleaseLayer(Layer* layer);

	void DeleteLayer(int index);
	void DeleteLayer(const std::wstring& filepath);
	void DeleteLayerByKey(int key);
	void FindPathDeleteLayer(const std::wstring& FindLayerPath);

	void ReMBR();

	// Reads every layer again from its file, and the background: after the chart projection
	// changed (SetMapProjection), since layers hold world coordinates. Layers keep whether they
	// are shown; those that cannot be read again are dropped.
	void ReloadLayers();

	void ChangeS100ColorPalette(GeoMetryLibrary::ColorTable value);
	void ChangeS100ColorPalette(std::wstring paletteName);
	// The palette of every S-100 layer's drawing instructions, including the layers of layer
	// containers; the catalogues' own palettes are left to ChangeS100ColorPalette.
	void ChangeLayerPalettes(const std::wstring& paletteName);

	Scaler* GetScaler();

	int LayerCount();

	// Return type : enum S100_FileType
	static S100_FileType CheckFileType(const std::wstring& path, int update);
	static S100_FileType CheckFileType(const std::wstring& path);

	int CreateLayerID();

	int pathToProductNumber(const std::wstring& path);

	void SetS100Scale(double value);
	int GetS100Scale();

	void InitFeatureOnOffMap();
	void SetFeatureOnOff(std::wstring code, bool on);
	bool IsFeatureOn(const std::wstring& featureTypeCode);
};