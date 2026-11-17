#include "stdafx.h"
// Runs the portrayal of the dataset of a layer: the rules of its portrayal catalogue (Lua for
// S-101, XSLT for the others) turn the features into drawing instructions. No display code.
#include "S100Layer.h"
#include "S10XGML.h"
#include "ProcessS101.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include <filesystem>

void S100Layer::BuildPortrayalCatalogue()
{
	if ((portrayalCatalogue == nullptr) ||
		(m_spatialObject == nullptr))
		return;

	if (m_spatialObject->m_FileType != S100_FileType::FILE_S_100_VECTOR)
		return;

	auto mainRuleFile = portrayalCatalogue->GetMainRuleFile();
	auto RulefileFormat = portrayalCatalogue->GetRuleFileFormat();
	auto fileName = mainRuleFile->GetFileNameW();
	auto rootPath = portrayalCatalogue->GetRootPathW();
	// The catalogue root ends with a separator.
	const std::wstring mainRulePath = (std::filesystem::path(rootPath + L"Rules") / fileName).wstring();
	
	if (RulefileFormat == Portrayal::FileFormat::LUA)
		ProcessS101::ProcessS101_LUA(mainRulePath, this);
	else if (RulefileFormat == Portrayal::FileFormat::XSLT)
	{
		auto gml = (S10XGML*)m_spatialObject;
		gml->SaveToInputXML("..\\TEMP\\input.xml");
		ProcessS101::ProcessS100_XSLT("..\\TEMP\\input.xml", pugi::as_utf8(mainRulePath), "..\\TEMP\\output.xml");
		auto s100so = (S100SpatialObject*)m_spatialObject;
		s100so->OpenOutputXML("..\\TEMP\\output.xml");
	}

	return;
}
