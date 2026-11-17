#include "stdafx.h"
// Runs the portrayal of the dataset of a layer: the rules of its portrayal catalogue (Lua for
// S-101, XSLT for the others) turn the features into drawing instructions. No display code.
#include "S100Layer.h"
#include "S10XGML.h"
#include "ProcessS101.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include <atomic>
#include <chrono>
#include <filesystem>
#include <string>
#include <system_error>

namespace
{
	// The XSLT engine reads the dataset as a file and writes the display list as a file. Both
	// live in the temp directory of the system under a name of this run, and are removed after.
	class IntermediateFiles
	{
	public:
		IntermediateFiles()
		{
			static std::atomic<unsigned> counter{ 0 };
			const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
			const std::string stem = "opens100-portrayal-" + std::to_string(stamp) + "-" + std::to_string(counter++);
			std::error_code ec;
			auto directory = std::filesystem::temp_directory_path(ec);
			if (ec)
			{
				directory = std::filesystem::current_path(ec);
			}
			input = directory / (stem + "-input.xml");
			output = directory / (stem + "-output.xml");
		}

		~IntermediateFiles()
		{
			std::error_code ec;
			std::filesystem::remove(input, ec);
			std::filesystem::remove(output, ec);
		}

		std::filesystem::path input;
		std::filesystem::path output;
	};
}

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
		const IntermediateFiles files;
		gml->SaveToInputXML(files.input.string());
		ProcessS101::ProcessS100_XSLT(files.input.string(), pugi::as_utf8(mainRulePath), files.output.string());
		auto s100so = (S100SpatialObject*)m_spatialObject;
		s100so->OpenOutputXML(files.output.string());
	}

	return;
}
