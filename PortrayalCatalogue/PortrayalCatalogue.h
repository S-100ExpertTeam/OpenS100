#pragma once
#include "S100_Rules.h"
#include "S100_LineStyles.h"
#include "S100_AreaFills.h"
#include "S100_Transform.h"
#include "S100_SymbolFill.h"
#include "S100_ColorProfile.h"
#include "S100_ColorProfiles.h"
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
#include "PortrayalResourceStore.h"

#include <pugixml.hpp>

#include "../FeatureCatalog/Version.h"

#include <unordered_map>
#include <string>
#include <memory>

using namespace Portrayal;

class PortrayalCatalogue
{
public:
	PortrayalCatalogue();
	// A catalogue that loads its symbols, line styles, area fills and colour profiles into the store.
	explicit PortrayalCatalogue(std::unique_ptr<Portrayal::ResourceStore> _resources);
	virtual ~PortrayalCatalogue();

protected:
	// include PC folder (ex: ..\\ProgramData\\S101_Portrayal\\)
	std::string rootPath;

	// path of portrayal_catalogue.xml (ex: ..\\ProgramData\\S101_Portrayal\\portrayal_catalogue.xml)
	std::string catalogPath;

	// ex : S-101
	std::string product;

	// ex : 1.1.1
	Version version;

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
	S100_Symbols symbols;

	// ColorProfiles
	S100_ColorProfiles colorProfiles;

	// lineStyle
	S100_LineStyles lineStyles;

	// AreaFills
	S100_AreaFills areaFills;

	std::unordered_map<std::string, S100_LineStyleBase*> s100_lineStyles;
	std::vector<S100_LineStyleBase*> s100_lineStyle_vec;

	// FOR SENC
	std::unordered_map<std::string, S100_RuleFile*> mainRules;

	std::string currentPalette = "Day";
	// Symbol/line-style/area-fill/colour-profile resources of this catalogue, in the form the
	// renderer draws them from. Owned here, supplied by whoever opens the catalogue; null for
	// a catalogue that is used as data only.
	std::unique_ptr<Portrayal::ResourceStore> resources;

	// Lives as long as what the catalogue holds does (see Lifetime).
	std::shared_ptr<const int> life = std::make_shared<const int>(0);

public:
	// Expires when the catalogue is deleted or emptied (Delete). Whoever keeps something made
	// from a catalogue keeps this with it, and not the address alone: another catalogue may be
	// made at the address of one that was deleted.
	std::weak_ptr<const void> Lifetime() const;

	bool Read(std::wstring path);
	bool Read(const std::string& path);
	bool Open(std::wstring path);
	bool Open(const std::string& path);
	void Delete();
	bool ReadPortrayalCatalogueByPugiXML(std::wstring& path);
	void GetContents(pugi::xml_node& node);

	//set
	void SetRootPath(const std::string& value);
	void SetRootPath(const std::wstring& value);
	void SetProduct(const std::string& value);
	void SetProduct(const std::wstring& value);
	void SetVersion(const std::string& value);
	void SetVersion(const std::wstring& value);
	void SetRules(S100_Rules* value);
	void SetVewingGroups(ViewingGroups* value);
	void SetFoundationMode(FoundationMode* value);
	void SetViewingGroupLayers(ViewingGroupLayers* value);
	void SetDisplaModes(DisplayModes* value);
	void SetDisplayPlanes(DisplayPlanes* value);
	void SetContext(Context* value);
	void SetColorProfiles(S100_ColorProfiles* value);
	void SetAreaFills(S100_AreaFills* value);
	void SetLineStyles(const std::string& key, S100_LineStyleBase* value);
	void SetLineStyles(const std::wstring& key, S100_LineStyleBase* value);
	void SetCurrentPaletteName(const std::string& paletteName);
	void SetCurrentPaletteName(const std::wstring& paletteName);

	//get
	std::string  GetRootPath();
	std::wstring GetRootPathW();
	std::string  GetProduct();
	std::wstring GetProductW();
	std::string  getProduct() const;
	std::string  GetVersion();
	std::wstring GetVersionW();
	Version getVersion() const;
	S100_Rules* GetRules();
	ViewingGroups* GetViewingGroups();
	FoundationMode* GetFoundationMode();
	ViewingGroupLayers* GetViewingGroupLayers();
	DisplayModes* GetDisplaModes();
	DisplayPlanes* GetDisplayPlanes();
	Context* GetContext();
	S100_ColorProfiles* GetColorProfiles();
	S100_AreaFills* GetAreaFills();
	S100_LineStyleBase* GetLineStyles(const std::string& key);
	S100_LineStyleBase* GetLineStyles(const std::wstring& key);
	S100_Symbols* GetSymbols();
	std::unordered_map<std::string, S100_LineStyleBase*> GetLineStyles();
	std::vector<S100_LineStyleBase*>* GetLineStylesVector();

	S100_RuleFile* GetMainRules(const std::string& key);
	S100_RuleFile* GetMainRules(const std::wstring& key);
	std::unordered_map<std::string, S100_RuleFile*> GetMainRules();

	Portrayal::ResourceStore* GetResources();

	S100_LineStyles* GetTreeLineStyles();

	void GetLineStylesByPugiXml();

	S100_RuleFile* GetMainRuleFile();
	std::string  GetMainRuleFilePath();
	std::wstring GetMainRuleFilePathW();

	std::string  GetCurrentPaletteName();
	std::wstring GetCurrentPaletteNameW();

	// Model-side palette switch (name + colour profile). Rebuilding pattern/line
	// bitmaps for the new palette is the renderer's job (PortrayalRender::ChangePalette).
	void ChangePalette(const std::string& paletteName);

	bool OpenSVG();

	// Without extension
	bool HasSymbol(std::wstring fileName);

	bool OpenLineStyle();
	bool OpenAreaFill();
	bool OpenColorProfile();

	Portrayal::FileFormat GetRuleFileFormat();

	std::string GetCataloguePathAsString();
};
