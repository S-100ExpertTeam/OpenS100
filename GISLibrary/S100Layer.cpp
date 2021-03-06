#include "stdafx.h"
#include "S100Layer.h"
#include "GISLibrary.h"

#include "..\\LibMFCUtil\\LibMFCUtil.h"
#include "..\\FeatureCatalog\\FeatureCatalogue.h"
#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"

S100Layer::S100Layer() : Layer()
{
	hasIndividualFC = false;
	hasIndividualPC = false;
}

S100Layer::S100Layer(FeatureCatalogue*fc, PortrayalCatalogue *pc) : Layer()
{
	SetFeatureCatalog(fc);
	SetPC(pc);
}

S100Layer::~S100Layer()
{
	DeleteCatalog();
}

void S100Layer::SetFeatureCatalog(FeatureCatalogue* value)
{
	featureCatalogue = value;
}

FeatureCatalogue* S100Layer::GetFeatureCatalog()
{
	if (nullptr == featureCatalogue)
	{
		return nullptr;
	}

	return featureCatalogue;
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
	try
	{
		FeatureCatalogue* fc = new FeatureCatalogue(std::wstring(path));
		SetIndividualFC(true);
		SetFeatureCatalog(fc);
		return true;
	}
	catch (int /*e*/)
	{
		SetIndividualFC(false);
		return false;
	}
	return false;
}

bool S100Layer::OpenPC(CString path)
{
	try
	{
		auto filePath = LibMFCUtil::GetFolderPathFromFilePath(path);
		PortrayalCatalogue* pc = new PortrayalCatalogue(std::wstring(filePath));

		pc->CreateSVGD2Geometry(gisLib->D2.pD2Factory);
		pc->CreatePatternImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));
		pc->CreateLineImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));

		SetIndividualPC(true);
		SetPC(pc);
		return true;
	}
	catch (int /*e*/)
	{
		SetIndividualPC(false);
		return false;
	}
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
		chatName.Format(_T("Chart Name : %s \n"), GetChartName().c_str());
		CString EditionName;
		EditionName.Format(_T("Edition Number : %s \n"), GetEditionNumberAsWstring().c_str());
		CString IssueDate;
		IssueDate.Format(_T("ISSUED DATE : %s \n"), GetIssueDateAsWstring().c_str());

		chatName += EditionName;
		chatName += IssueDate;

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