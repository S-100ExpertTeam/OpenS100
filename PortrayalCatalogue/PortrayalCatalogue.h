#pragma once
#include "S100_Rules.h"
#include "S100_LineStyles.h"
#include "S100_AreaFills.h"
#include "S100_Transform.h"
#include "S100_SymbolFill.h"
#include "S100_ColorProfile.h"
#include "S100_ColorProfiles.h"
#include "Enum_PortrayalRuleType.h"
#include "ViewingGroups.h"
#include "ViewingGroupLayers.h"
#include "DisplayModes.h"
#include "FoundationMode.h"
#include "DisplayPlanes.h"
#include "Context.h"
#include "S100_Symbols.h"
#include "ViewingGroupLayer.h"
#include "DisplayMode.h"
#include "DisplayPlane.h"
#include "ContextParameter.h"
#include "S100_LineStyleFile.h"


#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"
#include "..\\S100_SVG_D2D1_DLL\\SVGManager.h"
#include "..\\S100Engine\\S100Render.h"

#include <unordered_map>
#include <string>

using namespace Portrayal;

class S100PCManager;

class PortrayalCatalogue
{
public:
	PortrayalCatalogue();
	PortrayalCatalogue(std::wstring path);
	PortrayalCatalogue(std::wstring path, bool startVisualTool);
	virtual ~PortrayalCatalogue();

private:
	// include PC folder (ex: ..\ProgramData\\S101_Portrayal\\)
	std::wstring rootPath;

	PortrayalRuleType portrayalRuleType = XSLT;

	// ex : S-101
	std::wstring product;

	// ex : 1.1.1
	std::wstring version;

	// Rules 
	S100_Rules rules;

	// ViewingGroups 
	ViewingGroups viewingGroups;

	// FoundationMode 
	FoundationMode foundationMode;
	
	// ViewingGroupLayers 
	ViewingGroupLayers viewingGroupLayers;

	// DisplayModes
	DisplayModes displayModes;

	// Displayplane
	DisplayPlanes displayPlanes;

	// Context 
	Context context;

	// S100_Symbols
	S100_Symbols symbols; ////////

	// ColorProfiles 
	S100_ColorProfiles colorProfiles; //read color

	// lineStyle
	S100_LineStyles lineStyles;///////
	
	// AreaFills
	S100_AreaFills areaFills;////

	std::unordered_map<std::wstring, S100_LineStyleBase*> s100_lineStyles; //save linestyle value
	std::vector< S100_LineStyleBase*> s100_lineStyle_vec;

	// FOR SENC
	std::unordered_map<std::wstring, S100_RuleFile*> mainRules;

	S100_SVG_D2D1_DLL::SVGManager *m_svgSymbolManager = nullptr;
	
	std::wstring currentPalette = L"Day";
	S100PCManager *s100PCManager = new S100PCManager();
	S100Render s100Render;

	bool bVisualToolOn = false;

public:
	void Open(std::wstring& path);
	void Init();
	void Delete();
	bool ReadPortrayalCatalogueByPugiXML(std::wstring& path);	//change pugixml
	void GetContents(pugi::xml_node& node);
	void CreateSVGD2Geometry(ID2D1Factory1* factory);

	//set
	void SetRootPath(std::wstring& value);
	void SetPortrayalRuleType(PortrayalRuleType value);
	void SetProduct(std::wstring& value);
	void SetVersion(std::wstring& value);
	void SetRules(S100_Rules* value);
	void SetVewingGroups(ViewingGroups* value);
	void SetFoundationMode(FoundationMode* value);
	void SetViewingGroupLayers(ViewingGroupLayers* value);
	void SetDisplaModes(DisplayModes* value);
	void SetDisplayPlanes(DisplayPlanes* value);
	void SetContext(Context* value);
	void SetColorProfiles(S100_ColorProfiles* value);
	void SetAreaFills(S100_AreaFills* value);
	void SetLineStyles(std::wstring& key, S100_LineStyleBase* value);
	void SetCurrentPaletteName(std::wstring& paletteName);
	void SetS100PCManager(S100PCManager* value);
	void SetS100Render(S100Render* value);
	void SetSVGManager(S100_SVG_D2D1_DLL::SVGManager* value);
	void SetRuleType(PortrayalRuleType value);

	//get
	std::wstring GetRootPath();
	PortrayalRuleType GetPortrayalRuleType();
	std::wstring GetProduct();
	std::wstring GetVersion();
	S100_Rules* GetRules();
	ViewingGroups* GetViewingGroups();
	FoundationMode* GetFoundationMode();
	ViewingGroupLayers* GetViewingGroupLayers();
	DisplayModes* GetDisplaModes();
	DisplayPlanes* GetDisplayPlanes();
	Context* GetContext();
	S100_ColorProfiles* GetColorProfiles();
	S100_AreaFills* GetAreaFills();
	S100_ColorProfile* GetColorProfile();
	S100_ColorProfile* GetColorProfile(std::wstring& id);
	S100_LineStyleBase* GetLineStyles(std::wstring& key);
	S100_Symbols* GetSymbols();
	std::unordered_map<std::wstring, S100_LineStyleBase*> GetLineStyles();
	std::vector<S100_LineStyleBase*>* PortrayalCatalogue::GetLineStylesVector();

	S100_RuleFile* GetMainRules(std::wstring& key);
	std::unordered_map<std::wstring, S100_RuleFile*> GetMainRules();

	S100Render GetS100Render();
	S100PCManager* GetS100PCManager();
	S100_SVG_D2D1_DLL::SVGManager* GetSVGManager();

	S100_LineStyles* GetTreeLineStyles();

	void GetLineStylesByPugiXml();

	S100_RuleFile* GetMainRuleFile();
	std::wstring GetMainRuleFilePath();
	
	std::wstring GetCurrentPaletteName();

	PortrayalRuleType GetRuleType();

	void CreatePatternImages(ID2D1Factory1* d2Factory, IWICImagingFactory* imageFactory, ID2D1StrokeStyle1* stroke);
	void DeletePatternImage();

	void CreateLineImages(ID2D1Factory1* d2Factory, IWICImagingFactory* imageFactory, ID2D1StrokeStyle1* stroke);
	void DeleteLineImages();

};