#include "stdafx.h"
#include "PortrayalCatalogue.h"
#include "S100_PointInstruction.h"
#include "S100_LineInstruction.h"
#include "S100_AreaInstruction.h"
#include "S100_TextInstruction.h"
#include "S100_ColorFill.h"
#include "S100_ColorProfiles.h"

#include "..\\S100Engine\\S100PCManager.h"

#include <vector>
#include <string>

PortrayalCatalogue::PortrayalCatalogue()
{

}

PortrayalCatalogue::PortrayalCatalogue(std::wstring path) : PortrayalCatalogue()
{
	Open(path);
}


PortrayalCatalogue::~PortrayalCatalogue()
{
	Delete();
}


void PortrayalCatalogue::Open(std::wstring& path)
{
	ReadPortrayalCatalogueByPugiXML(path);

	m_svgSymbolManager = new S100_SVG_D2D1_DLL::SVGManager(path, GetCurrentPaletteName(), colorProfiles.GetColorProfile());

	s100PCManager->OpenS100Symbol(path + L"\\Symbols\\*.svg");
	s100PCManager->OpenS100ColorProfile(path + L"ColorProfiles\\" + GetColorProfile()->GetfileName());
	s100PCManager->OpenS100AreaFills(path + L"AreaFills\\*.xml");
	s100PCManager->OpenS100LineStyles(path + L"LineStyles");
}


void PortrayalCatalogue::Init()
{
	s100PCManager = new S100PCManager();
}


void PortrayalCatalogue::Delete()
{
	for (auto i = s100_lineStyles.begin(); i != s100_lineStyles.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	s100_lineStyles.clear();

	for (auto i = s100_symbolFill.begin(); i != s100_symbolFill.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	s100_symbolFill.clear();

	for (auto i = s100_transform.begin(); i != s100_transform.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	s100_transform.clear();

	delete m_svgSymbolManager;
	m_svgSymbolManager = nullptr;

	delete s100PCManager;
	s100PCManager = nullptr;
}

bool PortrayalCatalogue::ReadPortrayalCatalogueByPugiXML(std::wstring& path)
{
	CoInitialize(nullptr);

	std::wstring PCPath = path + _T("portrayal_catalogue.xml");

	if (!PCPath.empty())
	{
		auto itor = PCPath.find_last_of('\\');
		rootPath = PCPath.substr(0, ++itor);

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(PCPath.c_str()); /// read file
		pugi::xml_node displayList = doc.child("portrayalCatalog");

		auto productId = displayList.attribute("productId");
		if (productId)
		{
			product = pugi::as_wide(productId.value());
		}

		auto versionId = displayList.attribute("version");
		if (versionId)
		{
			version = pugi::as_wide(versionId.value());
		}

		GetContents(displayList);
	}
	else
	{
		return false;
	}

	GetLineStylesByPugiXml();
	GetMainRuleFile();

	if (colorProfiles.ReadColorProfiles(path) == false)
	{
	}

	return true;
}

void PortrayalCatalogue::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "pixmaps"))
		{
		}
		else if (!strcmp(instructionName, "colorProfiles")) // read color
		{
			colorProfiles.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "symbols"))
		{
			// not read a symbol
		}

		else if (!strcmp(instructionName, "lineStyles"))
		{
			lineStyles.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "areaFills"))
		{
			areaFills.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "fonts"))
		{
		}

		else if (!strcmp(instructionName, "viewingGroups"))
		{
			// not read a contents
			viewingGroups.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "foundationMode"))
		{
			foundationMode.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "viewingGroupLayers"))
		{
			viewingGroupLayers.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "displayModes"))
		{
		}

		else if (!strcmp(instructionName, "displayPlane"))
		{
		}

		else if (!strcmp(instructionName, "context"))
		{
		}

		else if (!strcmp(instructionName, "rules"))
		{
			rules.GetContents(instruction);
		}
		else
		{
			std::string value(instructionName);
			value + ": not Context\n";

		}
	}
}

void PortrayalCatalogue::CreateSVGD2Geometry(ID2D1Factory1* factory)
{
	s100PCManager->CreateSVGGeometry(factory);
}

void PortrayalCatalogue::SetRootPath(std::wstring& value)
{
	rootPath = value;
}

void PortrayalCatalogue::SetPortrayalRuleType(PortrayalRuleType value)
{
	portrayalRuleType = value;
}
void PortrayalCatalogue::SetProduct(std::wstring& value)
{
	product = value;
}
void PortrayalCatalogue::SetVersion(std::wstring& value)
{
	version = value;
}
void PortrayalCatalogue::SetRules(S100_Rules* value)
{
	rules = *value;
}
void PortrayalCatalogue::SetFoundationMode(FoundationMode* value)
{
	foundationMode = *value;
}

void PortrayalCatalogue::SetViewingGroupLayers(ViewingGroupLayers* value)
{
	viewingGroupLayers = *value;
}

void PortrayalCatalogue::SetVewingGroups(ViewingGroups* value)
{
	viewingGroups = *value;
}

void PortrayalCatalogue::SetDisplaModes(DisplayModes* value)
{
	displayModes = *value;
}

void PortrayalCatalogue::SetDisplayPlanes(DisplayPlanes* value)
{
	displayPlanes = *value;
}

void PortrayalCatalogue::SetContext(Context* value)
{
	context = *value;
}

void PortrayalCatalogue::SetColorProfiles(S100_ColorProfiles* value)
{
	colorProfiles = *value;
}

void PortrayalCatalogue::SetAreaFills(S100_AreaFills* value)
{
	areaFills = *value;
}

void PortrayalCatalogue::SetColorProfile(S100_ColorProfile* value)
{
	s100_colorProfile = *value;
}

void PortrayalCatalogue::SetLineStyles(std::wstring& key, S100_LineStyleBase* value)
{
	s100_lineStyles.insert({ key, value });
}

void PortrayalCatalogue::SetSymbolFill(std::wstring& key, S100_SymbolFill* value)
{
	s100_symbolFill.insert({ key,value });
}

void PortrayalCatalogue::SetTransform(S100_Transform* value)
{
	s100_transform.push_back(value);
}

void PortrayalCatalogue::SetS100PCManager(S100PCManager* value)
{
	s100PCManager = value;
}

void PortrayalCatalogue::SetS100Render(S100Render* value)
{
	s100Render = *value;
}

std::wstring PortrayalCatalogue::GetRootPath()
{
	return rootPath;
}

PortrayalRuleType PortrayalCatalogue::GetPortrayalRuleType() 
{
	return portrayalRuleType;
}

std::wstring PortrayalCatalogue::GetProduct() 
{
	return product;
}

std::wstring PortrayalCatalogue::GetVersion()
{
	return version;
}

S100_Rules* PortrayalCatalogue::GetRules() 
{
	return &rules;
}

ViewingGroups* PortrayalCatalogue::GetViewingGroups()
{
	return &viewingGroups;
}

FoundationMode* PortrayalCatalogue::GetFoundationMode()
{
	return &foundationMode;
}

ViewingGroupLayers* PortrayalCatalogue::GetViewingGroupLayers()
{
	return &viewingGroupLayers;
}

DisplayModes* PortrayalCatalogue::GetDisplaModes() 
{
	return &displayModes;
}

DisplayPlanes* PortrayalCatalogue::GetDisplayPlanes() 
{
	return &displayPlanes;
}

Context* PortrayalCatalogue::GetContext() 
{
	return &context;
}

S100_ColorProfiles* PortrayalCatalogue::GetColorProfiles() 
{
	return &colorProfiles;
}

S100_LineStyleBase* PortrayalCatalogue::GetLineStyles(std::wstring& key) 
{
	return s100_lineStyles[key];
}

std::unordered_map<std::wstring, S100_LineStyleBase*> PortrayalCatalogue::GetLineStyles()
{
	return s100_lineStyles;
}

S100_SymbolFill* PortrayalCatalogue::GetSymbolFill(std::wstring& key)
{
	return s100_symbolFill[key];
}

std::unordered_map<std::wstring, S100_SymbolFill*> PortrayalCatalogue::GetSymbolFill()
{
	return s100_symbolFill;
}

S100_Transform* PortrayalCatalogue::GetTransform(int index)
{
	return s100_transform.at(index);
}

std::vector<S100_Transform*> PortrayalCatalogue::GetTransform()
{
	return s100_transform;
}

S100_RuleFile* PortrayalCatalogue::GetMainRules(std::wstring& key)
{
	return mainRules[key];
}

std::unordered_map<std::wstring, S100_RuleFile*> PortrayalCatalogue::GetMainRules()
{
	return mainRules;
}

S100Render PortrayalCatalogue::GetS100Render()
{
	return s100Render;
}

S100_ColorProfile* PortrayalCatalogue::GetColorProfile()
{
	auto colorpro = colorProfiles.GetColorProfiles();
	if (colorpro.begin() == colorpro.end())
	{
		return nullptr;
	}
	auto result = colorpro.begin()->second;
	return result;
}

S100_ColorProfile* PortrayalCatalogue::GetColorProfile(std::wstring& id)
{
	auto colorpro = colorProfiles.GetColorProfiles();
	if (colorpro.find(id) == colorpro.end())
	{
		return nullptr;
	}

	auto result = colorpro[id];
	return result;
}

void PortrayalCatalogue::GetLineStylesByPugiXml()
{
	auto lineStyleFiles = lineStyles.GetLineStyleFiles();

	for (auto itor = lineStyleFiles.begin(); itor != lineStyleFiles.end(); itor++)
	{
		S100_LineStyleFile *lineStyleFile = itor->second;
		std::wstring path = lineStyleFile->GetFileName();

		std::wstring head = rootPath + L"LineStyles\\";
		path.insert(path.begin(), head.begin(), head.end());

		if (!path.empty())
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(path.c_str()); /// read file

			if (!result)
			{
				continue;
			}

			for (pugi::xml_node instruction = doc.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "lineStyle"))
				{
					S100_LineStyle* lineStyle = new S100_LineStyle();
					lineStyle->GetContents(instruction);
					s100_lineStyles[lineStyleFile->GetDescription()->Getname()] = lineStyle;
				}
				else if (!strcmp(instructionName, "compositeLineStyle"))
				{
					S100_CompositeLineStyle* cls = new S100_CompositeLineStyle();
					cls->GetContents(instruction);
					s100_lineStyles[lineStyleFile->GetDescription()->Getname()] = cls;
				}
				else
				{
					std::string unValue(instructionName);
					unValue + "is unValue Context";
				}
			}
		}
	}
}

void PortrayalCatalogue::GetAreaFills()
{
	auto areaFillFiles = areaFills.GetAreaFillFiles();
	for (auto itor = areaFillFiles.begin(); itor != areaFillFiles.end(); itor++)
	{
		S100_AreaFillFile *areaFillFile = itor->second;

		std::wstring path = areaFillFile->GetFileName();

		S100_SymbolFill* areaFill = new S100_SymbolFill();

		if (!areaFill->ReadFileByPugiXml(rootPath + L"AreaFills\\" + path))
		{
			delete areaFill;
			continue;
		}

		s100_symbolFill.insert({ areaFillFile->GetDescription()->Getname(), areaFill });
	}
}

S100_RuleFile* PortrayalCatalogue::GetMainRuleFile()
{
	auto ruleFiles = rules.GetRuleFiles();
	for (auto itor = ruleFiles.begin(); itor != ruleFiles.end(); itor++) //Add main rule.
	{
		//load the "main.lua" file.
		S100_RuleFile* rf = itor->second;

		if (true == rf->IsTypeLevelTemplate())
		{
			mainRules[rf->GetFileName()] = rf;
			if (true == rf->IsLua())
			{
				SetRuleType(LUA);
			}

			return rf;
		}
	}

	return nullptr;
}

std::wstring PortrayalCatalogue::GetCurrentPaletteName()
{
	return currentPalette;
}

void PortrayalCatalogue::SetCurrentPaletteName(std::wstring& paletteName)
{
	currentPalette = paletteName;

	if (nullptr != s100PCManager)
	{
		s100PCManager->GetS100ColorProfile()->ChangePalette(paletteName);
	}
}

S100PCManager* PortrayalCatalogue::GetS100PCManager()
{
	return s100PCManager;
}

void PortrayalCatalogue::SetRuleType(PortrayalRuleType value)
{
	portrayalRuleType = value;
}

PortrayalRuleType PortrayalCatalogue::GetRuleType()
{
	return portrayalRuleType;
}

void PortrayalCatalogue::SetSVGManager(S100_SVG_D2D1_DLL::SVGManager* value)
{
	m_svgSymbolManager = value;
}

S100_SVG_D2D1_DLL::SVGManager* PortrayalCatalogue::GetSVGManager()
{
	return m_svgSymbolManager;
}

void PortrayalCatalogue::CreatePatternImages(ID2D1Factory1* d2Factory, IWICImagingFactory* imageFactory, ID2D1StrokeStyle1* stroke)
{
	if (nullptr != s100PCManager)
	{
		s100PCManager->CreateBitmapImage(d2Factory, imageFactory, stroke, GetCurrentPaletteName());
	}
}

void PortrayalCatalogue::DeletePatternImage()
{
	if (nullptr != s100PCManager)
	{
		s100PCManager->DeleteBitmapImage();
	}
}

void PortrayalCatalogue::CreateLineImages(ID2D1Factory1* d2Factory, IWICImagingFactory* imageFactory, ID2D1StrokeStyle1* stroke)
{
	if (nullptr != s100PCManager)
	{
		s100PCManager->CreateLineImage(d2Factory, imageFactory, stroke, GetCurrentPaletteName());
	}
}

void PortrayalCatalogue::DeleteLineImages()
{
	if (nullptr != s100PCManager)
	{
		s100PCManager->DeleteLineImage();
	}
}