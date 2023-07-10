#include "stdafx.h"
#include "PortrayalCatalogue.h"
#include "S100_PointInstruction.h"
#include "S100_LineInstruction.h"
#include "S100_AreaInstruction.h"
#include "S100_TextInstruction.h"
#include "S100_ColorFill.h"
#include "S100_ColorProfiles.h"

#include "../S100Engine/S100PCManager.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include <vector>
#include <string>

PortrayalCatalogue::PortrayalCatalogue()
{

}

//PortrayalCatalogue::PortrayalCatalogue(std::wstring path) : PortrayalCatalogue()
//{
//	Open(path);
//}

PortrayalCatalogue::~PortrayalCatalogue()
{
	Delete();
}


bool PortrayalCatalogue::Open(std::wstring path)
{
	if (ReadPortrayalCatalogueByPugiXML(path)) {
		OpenSVG();
		OpenLineStyle();
		OpenAreaFill();
		OpenColorProfile();
		return true;
	}

	return false;
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

	//delete m_svgSymbolManager;
	//m_svgSymbolManager = nullptr;

	delete s100PCManager;
	s100PCManager = nullptr;
}

bool PortrayalCatalogue::ReadPortrayalCatalogueByPugiXML(std::wstring& path)
{
	CoInitialize(nullptr);

	if (!path.empty())
	{
		auto itor = path.find_last_of('\\');
		rootPath = path.substr(0, ++itor);
		catalogPath = path;

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(path.c_str()); /// read file
		pugi::xml_node displayList = doc.child("portrayalCatalog");

		auto productId = displayList.attribute("productId");
		if (productId)
		{
			product = pugi::as_wide(productId.value());
		}

		auto versionId = displayList.attribute("version");
		if (versionId)
		{
			//version = pugi::as_wide(versionId.value());
			version.setSource(versionId.value());
		}

		GetContents(displayList);
	}
	else
	{
		return false;
	}

	GetLineStylesByPugiXml();

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
			//구조없음
		}

		else if (!strcmp(instructionName, "colorProfiles")) // read color
		{
			colorProfiles.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "symbols"))
		{
			symbols.GetContents(instruction);
		}

		else if ((!strcmp(instructionName, "styleSheets"))) 
		{
			//문서에 없습니다
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
			//구조없음
		}

		else if (!strcmp(instructionName, "viewingGroups"))
		{
			viewingGroups.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "foundationMode")) 
		{
			foundationMode.GetContents(instruction,&viewingGroups);
		}

		else if (!strcmp(instructionName, "viewingGroupLayers"))
		{
			viewingGroupLayers.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "displayModes")) //진행중
		{
			displayModes.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "displayPlanes"))
		{
			displayPlanes.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "context"))
		{
			context.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "rules"))
		{
			rules.GetContents(instruction);
		}
		//else
		//{
		//	std::string value(instructionName);
		//	value + ": not Context\n";
		//}
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

void PortrayalCatalogue::SetProduct(std::wstring& value)
{
	product = value;
}
void PortrayalCatalogue::SetVersion(std::wstring& value)
{
	version.setSource(pugi::as_utf8(value));
	//version = value;
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

void PortrayalCatalogue::SetLineStyles(std::wstring& key, S100_LineStyleBase* value)
{
	s100_lineStyles.insert({ key, value });
}


void PortrayalCatalogue::SetS100PCManager(S100PCManager* value)
{
	s100PCManager = value;
}

//void PortrayalCatalogue::SetS100Render(S100Render* value)
//{
//	s100Render = *value;
//}

std::wstring PortrayalCatalogue::GetRootPath()
{
	return rootPath;
}

std::wstring PortrayalCatalogue::GetProduct() 
{
	return product;
}

std::string PortrayalCatalogue::getProduct() const
{
	return pugi::as_utf8(product);
}

std::wstring PortrayalCatalogue::GetVersion()
{
	return pugi::as_wide(version.getSource());
}

Version PortrayalCatalogue::getVersion() const
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

std::vector<S100_LineStyleBase*>* PortrayalCatalogue::GetLineStylesVector()
{
	return &s100_lineStyle_vec;
}

S100_LineStyles* PortrayalCatalogue::GetTreeLineStyles()
{
	return &lineStyles;
}

S100_Symbols* PortrayalCatalogue::GetSymbols()
{
	return &symbols;
}

S100_RuleFile* PortrayalCatalogue::GetMainRules(std::wstring& key)
{
	return mainRules[key];
}

std::unordered_map<std::wstring, S100_RuleFile*> PortrayalCatalogue::GetMainRules()
{
	return mainRules;
}

void PortrayalCatalogue::GetLineStylesByPugiXml()
{
	auto lineStyleFiles = lineStyles.GetLineStyleFiles();

	for (auto itor = lineStyleFiles.begin(); itor != lineStyleFiles.end(); itor++)
	{
		ExternalFile* lineStyleFile = itor->second;
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

					std::vector<S100_Description*>* vecDescription = lineStyleFile->GetDescription();
					for (int i = 0; i < vecDescription->size(); i++)
						s100_lineStyles[vecDescription->at(i)->Getname()] = lineStyle;
				}
				else if (!strcmp(instructionName, "compositeLineStyle"))
				{
					S100_CompositeLineStyle* cls = new S100_CompositeLineStyle();
					cls->GetContents(instruction);

					std::vector<S100_Description*>* vecDescription = lineStyleFile->GetDescription();
					for (int i = 0; i < vecDescription->size(); i++)
						s100_lineStyles[vecDescription->at(i)->Getname()] = cls;
				}
			}
		}
	}
}

S100_AreaFills* PortrayalCatalogue::GetAreaFills()
{
	return &areaFills;
}

std::wstring PortrayalCatalogue::GetMainRuleFilePath()
{
	auto itor = mainRules.begin();
	if (itor == mainRules.end())
		return L"";
	return GetRootPath() + L"Rules\\" + itor->first;
}

S100_RuleFile* PortrayalCatalogue::GetMainRuleFile()
{
	auto ruleFiles = rules.GetRuleFiles();
	for (auto itor = ruleFiles.begin(); itor != ruleFiles.end(); itor++) //Add main rule.
	{
		//load the "main.lua" file.
		S100_RuleFile* rf = itor->second;

		if (true == rf->IsTopLevelTemplate())
		{
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

bool PortrayalCatalogue::OpenSVG()
{
	auto symbol = symbols.GetSymbolsVector();
	for (auto i = symbol->begin(); i != symbol->end(); i++)
	{
		auto result = s100PCManager->AddS100Symbol(GetRootPath() + L"Symbols\\" + (*i)->GetFileName());
		if (result == false)
		{
			CString strErr;
			strErr.Format(L"Failed to open SVG(%s)\n", (*i)->GetFileName().c_str());
			return false;
		}
	}

	return true;
}

bool PortrayalCatalogue::HasSymbol(std::wstring fileName)
{
	if (s100PCManager->s100SymbolManager.GetSVG(fileName))
	{
		return true;
	}

	return false;
}

bool PortrayalCatalogue::OpenLineStyle()
{
	auto line = lineStyles.GetLineStyleFilesVector();
	for (auto i = line->begin(); i != line->end(); i++)
	{
		auto result = s100PCManager->AddS100LineStyle(GetRootPath() + L"LineStyles\\" + (*i)->GetFileName());
		if (result == false)
		{
			CString strErr;
			strErr.Format(L"Failed to open LineStyle(%s)\n", (*i)->GetFileName().c_str());
			return false;
		}
	}

	return true;
}

bool PortrayalCatalogue::OpenAreaFill()
{
	auto area = areaFills.GetAreaFillFilesVecter();
	for (auto i = area->begin(); i != area->end(); i++)
	{
		auto result = s100PCManager->AddS100AreaFill(GetRootPath() + L"AreaFills\\" + (*i)->GetFileName());
		if (result == false)
		{
			CString strErr;
			strErr.Format(L"Failed to open AreaFill(%s)\n", (*i)->GetFileName().c_str());
			return false;
		}
	}

	return true;
}

bool PortrayalCatalogue::OpenColorProfile()
{
	auto colorProfile = colorProfiles.GetColorProfilesVector();
	for (auto i = colorProfile->begin(); i != colorProfile->end(); i++)
	{
		auto result = s100PCManager->AddS100ColorProfile((*i)->GetId(), GetRootPath() + L"ColorProfiles\\" + (*i)->GetFileName());
		if (result == false)
		{
			CString strErr;
			strErr.Format(L"Failed to open AreaFill(%s)\n", (*i)->GetFileName().c_str());
			return false;
		}
	}

	return true;
}

Portrayal::FileFormat PortrayalCatalogue::GetRuleFileFormat()
{
	auto topLevelTemplate = GetMainRuleFile();
	if (topLevelTemplate)
	{
		return topLevelTemplate->GetFileFormat();
	}

	return Portrayal::FileFormat::none;
}

std::string PortrayalCatalogue::GetCataloguePathAsString()
{
	return LibMFCUtil::WStringToString(catalogPath);
}

void PortrayalCatalogue::ChangePalette(std::string paletteName, ID2D1Factory1* d2Factory, IWICImagingFactory* imageFactory, ID2D1StrokeStyle1* stroke)
{
	SetCurrentPaletteName(pugi::as_wide(paletteName));
	DeletePatternImage();
	CreatePatternImages(d2Factory, imageFactory, stroke);
	DeleteLineImages();
	CreateLineImages(d2Factory, imageFactory, stroke);
}