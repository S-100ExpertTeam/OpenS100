#include "stdafx.h"
#include "S100Layer.h"
#include "GISLibrary.h"
#include "S10XGML.h"
#include "S100H5.h"
#include "S102H5.h"
#include "ProcessS101.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include <sstream>

S100Layer::S100Layer(FeatureCatalogue* fc, PortrayalCatalogue *pc) : Layer()
{	
	SetFeatureCatalog(fc);
	SetPC(pc);
	SetProductNumber(fc->GetProductId());
}

S100Layer::~S100Layer()
{	
}

bool S100Layer::Open(CString _filepath, D2D1Resources* d2d1, LayerManager* lm)
{
	auto extension = LibMFCUtil::GetExtension(_filepath);

	if ((extension.CompareNoCase(L"000") >= 0) &&
		(extension.CompareNoCase(L"999") <= 0))
	{
		m_spatialObject = new S101Cell(d2d1);
		m_spatialObject->SetLayer(this);

		auto enc = (S101Cell*)m_spatialObject;

		if (!m_spatialObject->Open(_filepath))
		{
			delete enc;
			return false;
		}

		//enc->SetAllNumericCode(GetFC());
		return true;
	}
	else if (!extension.CompareNoCase(L"gml"))
	{
		if (GetFC()->getProductId().compare("S-101") == 0)
		{
			m_spatialObject = new S101Cell(d2d1);
		}
		else
		{
			m_spatialObject = new S10XGML(d2d1);
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
	else if (!extension.CompareNoCase(L"h5"))
	{
		if (GetFC()->getProductId().compare("S-102") == 0)
		{
			m_spatialObject = new S102H5(GetPC(), d2d1);
		}
		else
		{
			m_spatialObject = new S100H5(d2d1);
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

void S100Layer::Draw(HDC& hDC, Scaler* scaler, double offset)
{
	if (GetFileType() == S100_FileType::FILE_S_100_GRID_H5) {
		m_spatialObject->Draw(hDC, scaler, offset);
	}
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

bool S100Layer::OpenFC(CString path)
{
	FeatureCatalogue* fc = new FeatureCatalogue();
	fc->Read(std::wstring(path));
	
	if (fc) {
		SetIndividualFC(true);
		SetFeatureCatalog(fc);
		return true;
	}
		
	SetIndividualFC(false);
	delete fc;
	return false;
}

bool S100Layer::OpenPC(CString path, D2D1Resources* d2d1)
{
	PortrayalCatalogue* pc = new PortrayalCatalogue();

	if (pc) {
		pc->CreateSVGD2Geometry(d2d1->pD2Factory);
		pc->CreatePatternImages(d2d1->pD2Factory, d2d1->pImagingFactory, d2d1->D2D1StrokeStyleGroup.at(0));
		pc->CreateLineImages(d2d1->pD2Factory, d2d1->pImagingFactory, d2d1->D2D1StrokeStyleGroup.at(0));

		SetIndividualPC(true);
		SetPC(pc);
		return true;
	}

	SetIndividualPC(false);
	delete pc;
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

		CString chatName;
		chatName.Format(_T("Chart Name : %s \n"), m_spatialObject->GetFileName());

		sf.SetAlignment(StringAlignmentNear);
		sf.SetLineAlignment(StringAlignmentNear);

		graphics.DrawRectangle(&linePen, position.X, position.Y, position.Width, position.Height);
		graphics.FillRectangle(&internalBrush, position.X, position.Y, position.Width, position.Height);
		graphics.DrawString(chatName, -1, &F, position, &sf, &B); 
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
		productNumber = std::stoi(tokens.at(1));
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
	auto fileName = mainRuleFile->GetFileName();
	auto rootPath = portrayalCatalogue->GetRootPath();
	auto mainRulePath = rootPath + L"Rules\\" + fileName;
	
	if (RulefileFormat == Portrayal::FileFormat::LUA)
		ProcessS101::ProcessS101_LUA(mainRulePath, this);
	else if (RulefileFormat == Portrayal::FileFormat::XSLT)
	{
		auto gml = (S10XGML*)m_spatialObject;
		gml->SaveToInputXML("..\\TEMP\\input.xml");
		ProcessS101::ProcessS100_XSLT("..\\TEMP\\input.xml", pugi::as_utf8(mainRulePath), "..\\TEMP\\output.xml", this);
		auto s100so = (S100SpatialObject*)m_spatialObject;
		s100so->OpenOutputXML("..\\TEMP\\output.xml");
	}

	return;
}
