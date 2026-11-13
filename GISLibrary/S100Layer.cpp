#include "stdafx.h"
#include "PathUtil.h"
#include "TextCompare.h"
#include "S100Layer.h"
#include "GISLibrary.h"
#include "S10XGML.h"
#include "S100H5.h"
#include "CoverageDataset.h"
#include "PortrayalRenderCache.h"
#include "ProcessS101.h"

#include "..\\LatLonUtility\\cpp_util.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include <sstream>

S100Layer::S100Layer() : Layer()
{

}

S100Layer::S100Layer(int productNumber) : Layer()
{
	SetProductNumber(productNumber);
}

S100Layer::S100Layer(int productNumber, FeatureCatalogue* fc, PortrayalCatalogue* pc) : Layer()
{
	SetFeatureCatalog(fc);

	if (pc) {
		SetPC(pc);
	}

	SetProductNumber(productNumber);
}

S100Layer::S100Layer(FeatureCatalogue* fc, PortrayalCatalogue *pc) : Layer()
{	
	SetFeatureCatalog(fc);
	SetPC(pc);
	SetProductNumber(fc->GetProductIdW());
}

S100Layer::~S100Layer()
{
	DeleteCatalog();
}

bool S100Layer::Open(const std::wstring& _filepath, LayerManager* lm)
{
	const auto extension = s100::path::Extension(_filepath);

	if ((s100::text::CompareNoCase(extension, L"000") >= 0) &&
		(s100::text::CompareNoCase(extension, L"999") <= 0))
	{
		m_spatialObject = new S101Cell();
		m_spatialObject->SetLayer(this);

		auto enc = (S101Cell*)m_spatialObject;

		FeatureCatalogue* fc = nullptr;
		PortrayalCatalogue* pc = nullptr;

		if (enc->OpenMetadata(_filepath))
		{
			auto version = enc->GetVersion();
			fc = lm->catalogManager->getFC(GetProductNumber(), enc->GetVersion());
			pc = lm->catalogManager->getPC(GetProductNumber(), enc->GetVersion());

			if (nullptr == fc) {
				fc = lm->catalogManager->getFC(GetProductNumber());
			}

			if (nullptr == fc) {
				pc = lm->catalogManager->getPC(GetProductNumber());
			}

			SetFeatureCatalog(fc);
			SetPC(pc);
		}

		enc->RemoveAll();

		if (fc && pc && !m_spatialObject->Open(_filepath))
		{
			enc->GetVersion();
			delete m_spatialObject;
			m_spatialObject = nullptr;
			return false;
		}

		//enc->SetAllNumericCode(GetFC());
		return true;
	}
	else if (!s100::text::CompareNoCase(extension, L"gml"))
	{
		SetFeatureCatalog(lm->catalogManager->getFC(GetProductNumber()));
		SetPC(lm->catalogManager->getPC(GetProductNumber()));
		if (GetFC()->getProductId().compare("S-101") == 0)
		{
			m_spatialObject = new S101Cell();
		}
		else
		{
			m_spatialObject = new S10XGML();
		}

		m_spatialObject->SetLayer(this);

		if (!m_spatialObject->Open(_filepath))
		{
			delete m_spatialObject;
			m_spatialObject = nullptr;
			return false;
		}

		if (GetFC()->getProductId().compare("S-101") != 0)
		{
			auto gml = (S10XGML*)m_spatialObject;
			gml->SetGeometry();
			gml->CalcMBR();
		}
		
		return true;
	}
	else if (!s100::text::CompareNoCase(extension, L"h5"))
	{
		if (GetFC()->getProductId().compare("S-102") == 0 || GetFC()->getProductId().compare("S-104") == 0
			|| GetFC()->getProductId().compare("S-111") == 0)
		{
			// S-102: portrayed by its portrayal catalogue (CoverageFill), drawn as one image. S-104: the
			// water levels S-102 is adjusted by (S-98 D-2), not drawn itself. S-111: the arrows of its
			// portrayal catalogue at the grid points, thinned as the chart is zoomed out.
			auto coverage = new GISLibrary::CoverageDataset();
			m_spatialObject = coverage;
			coverage->SetLayer(this);
			if (!coverage->Open(_filepath))
			{
				delete m_spatialObject;
				m_spatialObject = nullptr;
				return false;
			}
			coverage->Portray(GetFC(), GetPC());
			return true;
		}
		else
		{
			m_spatialObject = new S100H5();
		}

		m_spatialObject->SetLayer(this);
		if (!m_spatialObject->Open(_filepath))
		{
			delete m_spatialObject;
			m_spatialObject = nullptr;
			return false;
		}

		return true;
	}
	
	return false;
}

bool S100Layer::DrawGrid(D2D1Resources* d2, HDC& hDC, Scaler* scaler)
{
	if (GetFileType() != S100_FileType::FILE_S_100_GRID_H5 || !m_spatialObject)
	{
		return false;
	}

	if (auto coverage = dynamic_cast<GISLibrary::CoverageDataset*>(m_spatialObject))
	{
		coverage->DrawCoverage(d2, hDC, scaler);
		return true;
	}

	return false;
}

void S100Layer::SetFeatureCatalog(FeatureCatalogue* value)
{
	featureCatalogue = value;
}

FeatureCatalogue* S100Layer::GetFeatureCatalog()
{
	return featureCatalogue;
}

FeatureCatalogue* S100Layer::GetFC()
{
	return GetFeatureCatalog();
}

void S100Layer::SetPC(PortrayalCatalogue *value)
{
	portrayalCatalogue = value;
}

PortrayalCatalogue* S100Layer::GetPC()
{
	return portrayalCatalogue;
}

bool S100Layer::OpenFC(const std::wstring& path)
{
	FeatureCatalogue* fc = new FeatureCatalogue();
	fc->Read(path);
	
	if (fc) {
		SetIndividualFC(true);
		SetFeatureCatalog(fc);
		return true;
	}
		
	SetIndividualFC(false);
	delete fc;
	return false;
}

void S100Layer::SetIndividualFC(bool value) 
{
	hasIndividualFC = value;
}

bool S100Layer::GetIndividualFC() 
{
	return hasIndividualFC;
}

void S100Layer::SetIndividualPC(bool value) 
{
	hasIndividualPC = value;
}
bool S100Layer::GetIndividualPC() 
{
	return hasIndividualPC;
}

void S100Layer::DeleteCatalog()
{
	if (GetIndividualFC())
	{
		delete	featureCatalogue;
		featureCatalogue = nullptr;
	}

	if (GetIndividualPC())
	{
		delete portrayalCatalogue;
		portrayalCatalogue = nullptr;
	}
}

void S100Layer::DrawInfo(HDC &hDC, Scaler *scaler)
{
	if (Info == true)
	{
		Gdiplus::Graphics graphics(hDC);
		graphics.SetSmoothingMode(SmoothingModeHighQuality);

		Color internalColor(50, 255, 0, 0);
		Color lineColor(255, 255, 0, 0);

		SolidBrush internalBrush(internalColor); 
		Pen linePen(lineColor); 

		// MBR : Minimum bounding rectangle 
		auto mbr = GetMBR();
		long sxmin = 0;
		long symax = 0;

		scaler->WorldToDevice(mbr.xmin, mbr.ymin, &sxmin, &symax);

		long sxmax = 0;
		long symin = 0;
		scaler->WorldToDevice(mbr.xmax, mbr.ymax, &sxmax, &symin);

		RectF position = {
			(float)sxmin,
			(float)symin,
			(float)sxmax - sxmin,
			(float)symax - symin
		};

		Gdiplus::Font F(L"Arial", 20, FontStyleRegular, UnitPixel);
		SolidBrush B(Color(0, 0, 255));
		StringFormat sf;

		const std::wstring chatName = L"Chart Name : " + m_spatialObject->GetFileName() + L" \n";

		sf.SetAlignment(StringAlignmentNear);
		sf.SetLineAlignment(StringAlignmentNear);

		graphics.DrawRectangle(&linePen, position.X, position.Y, position.Width, position.Height);
		graphics.FillRectangle(&internalBrush, position.X, position.Y, position.Width, position.Height);
		graphics.DrawString(chatName.c_str(), -1, &F, position, &sf, &B); 
	}
}

S100SpatialObject* S100Layer::GetS100SpatialObject()
{
	return (S100SpatialObject*)m_spatialObject;
}

int S100Layer::GetProductNumber()
{
	return productNumber;
}

void S100Layer::SetProductNumber(int value)
{
	productNumber = value;
}

void S100Layer::SetProductNumber(std::wstring value)
{
	// tokenize value by "-"
	std::vector<std::wstring> tokens;

	std::wstringstream ss(value);
	std::wstring token;
	while (std::getline(ss, token, L'-'))
	{
		tokens.push_back(token);
	}

	if (tokens.size() == 2)
	{
		productNumber = cpp_util::stoi(tokens.at(1));
	}
}

void S100Layer::InitDraw()
{
	drawingPriority.clear();
	drawingSet.Init();
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
	auto mainRulePath = rootPath + L"Rules\\" + fileName;
	
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
