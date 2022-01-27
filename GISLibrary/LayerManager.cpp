#include "StdAfx.h"
#include "LayerManager.h"
#include "S101Layer.h"
#include "BAGLayer.h"
#include "F_ATTR.h"
#include "R_FeatureRecord.h"
#include "ATTR.h"
#include "CodeWithNumericCode.h"
#include "DrawingSet.h"
#include "ProcessS101.h"
#include "PCOutputSchemaManager.h"
#include "SENC_SpatialReference.h"
#include "SENC_DisplayList.h"
#include "SENC_Instruction.h"
#include "SENC_LineInstruction.h"
#include "SENC_PointInstruction.h"
#include "SENC_TextInstruction.h"
#include "SENC_AlertReference.h"
#include "SENC_Define.h"
#include "DRReader.h"
#include "S100Utilities.h"
#include "GISLibrary.h"
#include "F_FASC.h"
#include "F_INAS.h"
#include "R_InformationRecord.h"

#include "..\\S100Geometry\\SSurface.h"
#include "..\\S100Geometry\\SMultiPoint.h"
#include "..\\S100Geometry\\SCompositeCurve.h"
#include "..\\S100Geometry\\SCurve.h"
#include "..\\S100Geometry\\SPoint.h"
#include "..\\S100Geometry\\SCommonFuction.h"
#include "..\\FeatureCatalog\\FeatureCatalogue.h"
#include "..\\LibMFCUtil\\LibMFCUtil.h"
#include "..\\S100_SVG_D2D1_DLL\\S100_SVG_D2D1_DLL.h"
#include "..\\S100_SVG_D2D1_DLL\\RealSymbol.h"
#include "..\\GeoMetryLibrary\\ENCGeometry.h"
#include "..\\GeoMetryLibrary\\GeometricFuc.h"
#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"

#include <ctime> 
#include <mmsystem.h> 
#include <string>
#include <vector>
#include <map>
#pragma comment(lib, "winmm")

// PC list 
std::vector<PortrayalCatalogue*>* LayerManager::pPortrayalCatalogues = nullptr;
std::unordered_map<std::wstring, PortrayalCatalogue*>* LayerManager::hash_PC = nullptr;

std::unordered_map<FeatureCatalogue*, PortrayalCatalogue*> LayerManager::CatalogueList;
std::unordered_map<std::wstring, FeatureCatalogue*>* LayerManager::hash_FC = nullptr;
std::vector<FeatureCatalogue*>* LayerManager::pS100Catalogs = nullptr;

// calc Performance
UINT wTimerRes;
CRITICAL_SECTION g_CsAddLayer;

LayerManager::LayerManager(void)
{
	CString strFolderPath;
	::GetModuleFileName(NULL, strFolderPath.GetBuffer(MAX_PATH), MAX_PATH);
	strFolderPath.ReleaseBuffer();
	if (strFolderPath.Find('\\') != -1)
	{
		for (int i = strFolderPath.GetLength() - 1; i >= 0; i--)
		{
			TCHAR ch = strFolderPath[i];
			strFolderPath.Delete(i);
			if (ch == '\\') break;
		}
	}

	::InitializeCriticalSection(&g_CsAddLayer);
}

LayerManager::LayerManager(Scaler* scaler) : LayerManager()
{
	this->scaler = scaler;
}

LayerManager::~LayerManager()
{
	POSITION pos = m_listBackgroundLayer.GetHeadPosition();

	while (pos)
	{
		delete m_listBackgroundLayer.GetNext(pos);
	}

	for (auto itor = m_listLayer.begin(); itor != m_listLayer.end(); itor++)
	{
		delete* itor;
		*itor = nullptr;

	}
	m_listLayer.clear();

	if (pPortrayalCatalogues)
	{
		for (auto itor = pPortrayalCatalogues->begin(); itor != pPortrayalCatalogues->end(); itor++)
		{
			delete* itor;
		}
		delete pPortrayalCatalogues;
		pPortrayalCatalogues = NULL;
	}

	if (pS100Catalogs)
	{
		for (auto itor = pS100Catalogs->begin(); itor != pS100Catalogs->end(); itor++)
		{
			delete* itor;
		}
		delete pS100Catalogs;
		pS100Catalogs = NULL;
	}

	if (hash_FC)
	{
		delete hash_FC;
		hash_FC = NULL;
	}
	if (hash_PC)
	{
		delete hash_PC;
		hash_PC = NULL;
	}

	// calc perfomance
	timeEndPeriod(wTimerRes);

	if (m_routeDetection_DangerHighlight_Area)
	{
		SafeRelease(&m_routeDetection_DangerHighlight_Area);
		m_routeDetection_DangerHighlight_Area = nullptr;
	}
	for (auto i = m_routeDetection_DangerHighlight_Lines.begin(); i != m_routeDetection_DangerHighlight_Lines.end(); i++)
	{
		SafeRelease(&(*i));
	}
	m_routeDetection_DangerHighlight_Lines.clear();
	m_routeDetection_DangerHighlight_Points.clear();

	if (m_routeDetection_InformationHighlight_Area)
	{
		SafeRelease(&m_routeDetection_InformationHighlight_Area);
		m_routeDetection_InformationHighlight_Area = nullptr;
	}
	for (auto i = m_routeDetection_InformationHighlight_Lines.begin(); i != m_routeDetection_InformationHighlight_Lines.end(); i++)
	{
		SafeRelease(&(*i));
	}
	m_routeDetection_InformationHighlight_Lines.clear();
	m_routeDetection_InformationHighlight_Points.clear();
}

void LayerManager::LoadFeatureCatalogs()
{
	CString strFolderPath;
	::GetModuleFileName(NULL, strFolderPath.GetBuffer(MAX_PATH), MAX_PATH);
	strFolderPath.ReleaseBuffer();
	if (strFolderPath.Find('\\') != -1)
	{
		for (int i = strFolderPath.GetLength() - 1; i >= 0; i--)
		{
			TCHAR ch = strFolderPath[i];
			strFolderPath.Delete(i);
			if (ch == '\\') break;
		}
	}
}

bool LayerManager::AddBackgroundLayer(CString _filepath)
{
	CString file_extension = _T("");

	file_extension.AppendChar(_filepath.GetAt(_filepath.GetLength() - 3));
	file_extension.AppendChar(_filepath.GetAt(_filepath.GetLength() - 2));
	file_extension.AppendChar(_filepath.GetAt(_filepath.GetLength() - 1));

	Layer* layer;

	if (file_extension.CompareNoCase(_T("SHP")) == 0) {
		layer = new Layer();
		if (layer->Open(_filepath) == FALSE)
		{
			return FALSE;
		}
	}
	if (layer->m_spatialObject == NULL)
		return FALSE;

	m_listBackgroundLayer.AddTail(layer);

	double xmin, ymin, xmax, ymax;

	if (m_listBackgroundLayer.GetCount() == 1)
	{
		xmin = scaler->mxMinLimit;
		xmax = scaler->mxMaxLimit;
		ymin = scaler->myMinLimit;
		ymax = scaler->myMaxLimit;

	}
	else
	{
		xmin = layer->m_mbr.xmin;
		ymin = layer->m_mbr.ymin;
		xmax = layer->m_mbr.xmax;
		ymax = layer->m_mbr.ymax;
	}

	MBR _mbr(xmin, ymin, xmax, ymax);
	scaler->SetMap(_mbr);

	if (m_listBackgroundLayer.GetCount() <= 2)
	{
		mbr.SetMBR(_mbr);
	}
	else
	{
		mbr.ReMBR(_mbr);
	}

	return TRUE;
}

S101Cell* LayerManager::FindBaseLayer(S101Cell* cell)
{

	CString dsed = cell->m_dsgir.m_dsid.m_dsed;
	int BaseNum = _ttoi(dsed.Left(1));
	int updateNum = _ttoi(dsed.Right(1));

	S101Cell* Basecell = nullptr;
	for (Layer* BaseLayer : m_listLayer)
	{
		auto BaseLayerNum = ((S101Cell*)BaseLayer->GetSpatialObject())->m_dsgir.m_dsid.m_dsed;
		int B = _ttoi(BaseLayerNum.Left(1));


		if (BaseNum == B)
		{
			Basecell = ((S101Cell*)BaseLayer->GetSpatialObject());
			if (Basecell->updates.size() == 0 && (updateNum - 1 == 0))
			{
				return Basecell;
			}

			S101Cell* update = Basecell->updates.back();
			int U = _ttoi(update->m_dsgir.m_dsid.m_dsed.Right(1));

			if (updateNum == (U + 1))
			{
				return Basecell;
			}
			else
			{
				return nullptr;
			}
		}
	}

	return nullptr;
}

bool LayerManager::AddLayer(Layer* layer)
{
	if (layer == nullptr)
	{
		return false;
	}

	if (m_listLayer.size() <= 1)
	{
		mbr.SetMBR(layer->m_mbr);
	}
	else
	{
		mbr.ReMBR(layer->m_mbr);
	}

	m_listLayer.push_back(layer);

	return true;
}

bool LayerManager::AddUpdateLayer(Layer* Base, Layer* Update)
{

	return false;
}

int LayerManager::isUpdate(CString filePath)
{
	CString file_extension = LibMFCUtil::GetExtension(filePath);

	try
	{
		int value = _ttoi(file_extension);
		return value;
	}
	catch (std::exception e)
	{
		return 0;
	}

	return 0;
}

bool LayerManager::AddLayer(CString _filepath)
{
	CString file_extension = LibMFCUtil::GetExtension(_filepath);

	Layer* layer = nullptr;
	size_t fitor = std::wstring::npos;

	if (file_extension.CompareNoCase(_T("SHP")) == 0)
	{
		layer = new Layer();
		if (layer->Open(_filepath) == false)
		{
			delete layer;
			return false;
		}
	}
	else if (file_extension.CompareNoCase(_T("000")) == 0)
	{
		FeatureCatalogue* fc = nullptr;
		PortrayalCatalogue* pc = nullptr;
		std::wstring fcName = L"";

		int type = CheckFileType(_filepath);
		// S-101 8211 or GML or H5
		if (type == 2)
		{
			// Product Number (S-XXX)
			CString strProductNumber = LibMFCUtil::GetFileName(_filepath).Left(3);
			CString s = _T("S-");
			strProductNumber = s + strProductNumber;

			fc = GetCatalogFromName(strProductNumber);

			if (fc != nullptr)
			{
				pc = GetPortrayalCatalogueFromName(strProductNumber);
				fcName = fc->GetName();
				fitor = fcName.find(L"S-");
			}

			if (type == 2)
			{
				layer = new S101Layer(fc, pc);
				if ((S101Layer*)layer->Open(_filepath) == false)
				{
					delete layer;
					return false;
				}
			}
		}
	}

	if (!layer ||
		!layer->m_spatialObject)
	{
		return false;
	}

	std::wstring layerName(LibMFCUtil::GetFileName(layer->GetLayerName()));

	std::wstring inputPath = L"..\\ProgramData\\S100_PC_IO_XML\\INPUT\\";
	std::wstring outputPath = L"..\\ProgramData\\S100_PC_IO_XML\\OUTPUT\\";

	inputPath.append(layerName);
	inputPath.append(L".xml");

	outputPath.append(layerName);
	outputPath.append(L".xml");

	//	 ENC, Lua
	if (layer->m_spatialObject->m_FileType == S100_FileType::FILE_S_100_VECTOR &&
		((S101Layer*)layer)->GetPC()->GetPortrayalRuleType() == PortrayalRuleType::LUA)
	{
		BuildPortrayalCatalogue(layer);
	}

	AddLayer(layer);

	return true;
}

bool LayerManager::AddOverlayLayer(CString _filepath)
{
	CString file_extension = _T("");

	file_extension.AppendChar(_filepath.GetAt(_filepath.GetLength() - 3));
	file_extension.AppendChar(_filepath.GetAt(_filepath.GetLength() - 2));
	file_extension.AppendChar(_filepath.GetAt(_filepath.GetLength() - 1));

	Layer* layer;

	if (file_extension.CompareNoCase(_T("SHP")) == 0) {
		layer = new Layer();
		if (layer->Open(_filepath) == FALSE) {
			return FALSE;
		}
	}

	if (layer->m_spatialObject == NULL)
		return FALSE;


	double xmin, ymin, xmax, ymax;

	xmin = layer->m_mbr.xmin;
	ymin = layer->m_mbr.ymin;
	xmax = layer->m_mbr.xmax;
	ymax = layer->m_mbr.ymax;

	MBR _mbr(xmin, ymin, xmax, ymax);

	if (m_listLayer.size() <= 1)
	{
		mbr.SetMBR(_mbr);
	}
	else
	{
		mbr.ReMBR(_mbr);
	}

	return TRUE;
}

void LayerManager::Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Factory* pDXFactory, double offsetX, double offsetY)
{
	POSITION pos = m_listBackgroundLayer.GetHeadPosition();

	while (pos)
	{
		if (m_listBackgroundLayer.GetAt(pos)->IsOn())
		{
			Layer* layer = m_listBackgroundLayer.GetNext(pos);

			if (MBR::CheckOverlap(scaler->GetMapCalcMBR(), layer->m_mbr))
			{
				layer->Draw(pRenderTarget, pDXFactory, scaler, offsetX - 360);
				layer->Draw(pRenderTarget, pDXFactory, scaler, offsetX);
				layer->Draw(pRenderTarget, pDXFactory, scaler, offsetX + 360);
			}
		}
		else
		{
			m_listBackgroundLayer.GetNext(pos);
		}
	}

	std::list<SENC_Instruction*> pointList[100];
	std::list<SENC_Instruction*> lineList[100];
	std::list<SENC_Instruction*> areaList[100];
	std::list<SENC_Instruction*> textList[100];

	for (auto itor = m_listLayer.begin(); itor != m_listLayer.end(); itor++)
	{
		if ((*itor)->IsOn()) {
			Layer* layer = (*itor);
			if (MBR::CheckOverlap(scaler->GetMapCalcMBR(), layer->m_mbr))
			{
				if (layer->m_spatialObject->m_FileType != S100_FileType::FILE_S_100_VECTOR)
					continue;
				S101Cell* cell = (S101Cell*)layer->m_spatialObject;

				if (!cell->pcManager)
				{
					continue;
				}
				if (!cell->pcManager->displayListSENC)
				{
					continue;
				}
				std::list<SENC_Instruction*> itList;
				for (int i = 0; i < 100; i++)
				{
					/*
					* Type Of Instruction
					* 0 : Null Instruction
					* 1 : Point Instruction
					* 2 : Line Instruction
					* 3 : Area Instruction
					* 4 : Text Instruction
					*/
					cell->pcManager->displayListSENC->GetDrawingInstruction(i, 1, scaler, itList);
					pointList[i].insert(pointList[i].begin(), itList.begin(), itList.end());

					cell->pcManager->displayListSENC->GetDrawingInstruction(i, 2, scaler, itList);
					lineList[i].insert(lineList[i].begin(), itList.begin(), itList.end());

					cell->pcManager->displayListSENC->GetDrawingInstruction(i, 3, scaler, itList);
					areaList[i].insert(areaList[i].begin(), itList.begin(), itList.end());

					cell->pcManager->displayListSENC->GetDrawingInstruction(i, 5, scaler, itList);
					textList[i].insert(textList[i].begin(), itList.begin(), itList.end());

					cell->pcManager->displayListSENC->GetDrawingInstruction(i, 0, scaler, itList);
				}

				itList.clear();
			}
		}
	}

	SENC_Instruction* it = NULL;
	for (int i = 0; i < 100; i++)
	{
		for (auto itor = areaList[i].begin(); itor != areaList[i].end(); itor++)
		{
			it = *itor;
			it->DrawInstruction(pRenderTarget, pDXFactory, scaler);
		}
		for (auto itor = lineList[i].begin(); itor != lineList[i].end(); itor++)
		{
			it = *itor;
		}
		for (auto itor = pointList[i].begin(); itor != pointList[i].end(); itor++)
		{
			it = *itor;
		}
		for (auto itor = textList[i].begin(); itor != textList[i].end(); itor++)
		{
			it = *itor;
		}
	}

}

void LayerManager::Draw(HDC& hdc, int offset)
{
	CDC* pDC = CDC::FromHandle(hdc);
	CRect rectView = scaler->GetScreenRect();

	DrawBackground(hdc, offset);

	/////////////////////////////////
	//Exchange Set Drawing Select..

	DrawS100Datasets(hdc, offset); //Draw

	gisLib->D2.Begin(hdc);

	gisLib->DrawS100Symbol(101, L"NORTHAR1", 30, 50, 0);
	gisLib->DrawScaleBar();
	gisLib->D2.End();
}

void LayerManager::DrawInformationLayer(HDC& hDC, int nindex)
{
	Gdiplus::Graphics graphics(hDC);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	Color internalColor(50, 255, 0, 0);
	Color lineColor(255, 255, 0, 0);

	SolidBrush internalBrush(internalColor);
	Pen linePen(lineColor);

	auto selectedLayer = GetLayer(nindex);

	auto mbr = selectedLayer->GetMBR();
	long sxmin = 0;
	long symax = 0;

	// Internal coordinate system -> Screen coordinate system.
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

	CString layername;
	layername.Format(_T("LayerName : %s \n"), selectedLayer->GetLayerName());
	CString layer;
	layer.Format(_T("LayerType : %s \n"), selectedLayer->GetLayerType());
	layername += layer;

	sf.SetAlignment(StringAlignmentNear);
	sf.SetLineAlignment(StringAlignmentNear);

	graphics.DrawRectangle(&linePen, position.X, position.Y, position.Width, position.Height);
	graphics.FillRectangle(&internalBrush, position.X, position.Y, position.Width, position.Height);
	graphics.DrawString(layername, -1, &F, position, &sf, &B);

}



void LayerManager::ClearInformationLayer(int nindex)
{
	for (auto i = m_SelectedLayer.begin(); i != m_SelectedLayer.end(); i++)
	{
		auto SelectedLayer = m_listLayer[nindex];
		if ((*i) == SelectedLayer)
		{
			auto it = std::remove(m_SelectedLayer.begin(), m_SelectedLayer.end(), SelectedLayer);
			m_SelectedLayer.resize(std::distance(m_SelectedLayer.begin(), it));
			return;

		}
	}
	return;
}

void LayerManager::ClearInformationLayer(CString filepath)
{
	for (auto i = m_SelectedLayer.begin(); i != m_SelectedLayer.end(); i++)
	{
		Layer* layer = (*i);

		if (layer->GetLayerPath().Compare(filepath) == 0)
		{
			auto it = std::remove(m_SelectedLayer.begin(), m_SelectedLayer.end(), layer);
			m_SelectedLayer.resize(std::distance(m_SelectedLayer.begin(), it));
			return;

		}
	}
}

void LayerManager::ClearAllInformationLayer()
{
	m_SelectedLayer.erase(m_SelectedLayer.begin(), m_SelectedLayer.end());
}


void LayerManager::DrawDataCoverageOverscale(HDC& hdc)
{

}



ID2D1PathGeometry* combine_multiple_path_geometries(ID2D1Factory1*& srcfactory, int geo_count, std::vector<ID2D1PathGeometry*> geos) {
	ID2D1PathGeometry* path_geo_1 = NULL;
	ID2D1PathGeometry* path_geo_2 = NULL;

	srcfactory->CreatePathGeometry(&path_geo_1);
	srcfactory->CreatePathGeometry(&path_geo_2);

	for (short i = 0; i < geo_count; i++) {

		ID2D1GeometrySink* cmpl_s1 = NULL;
		ID2D1GeometrySink* cmpl_s2 = NULL;

		if (i % 2 == 0) {
			path_geo_1->Open(&cmpl_s1);

			if (i == 0)
				geos[i]->CombineWithGeometry(geos[i], D2D1_COMBINE_MODE_UNION, NULL, cmpl_s1);
			else
				geos[i]->CombineWithGeometry(path_geo_2, D2D1_COMBINE_MODE_UNION, NULL, NULL, cmpl_s1);

			cmpl_s1->Close();
			cmpl_s1->Release();
			if (i != 0) {
				path_geo_2->Release();
				srcfactory->CreatePathGeometry(&path_geo_2);
			}
		}
		else {
			path_geo_2->Open(&cmpl_s2);

			geos[i]->CombineWithGeometry(path_geo_1, D2D1_COMBINE_MODE_UNION, NULL, cmpl_s2);

			cmpl_s2->Close();
			cmpl_s2->Release();
			path_geo_1->Release();
			srcfactory->CreatePathGeometry(&path_geo_1);
		}
	}

	if (geo_count % 2 == 0) {
		if (path_geo_1)
			path_geo_1->Release();
		return path_geo_2;
	}
	else {
		if (path_geo_2)
			path_geo_2->Release();
		return path_geo_1;
	}
}

void LayerManager::DrawLayerList(HDC &hdc, int offset)
{
	std::set<int> drawingPriority;

	unsigned numeric_number_of_text_placement = 0;

	auto rt = gisLib->D2.pRT;
	rt->BindDC(hdc, scaler->GetScreenRect());
	rt->BeginDraw();
	for (auto itor = m_listLayer.begin(); itor != m_listLayer.end(); itor++)
	{
		std::unordered_map<PortrayalCatalogue*, DrawingSet> drawingSetByPC;
		if ((*itor)->IsOn())
		{
			Layer* layer = *itor;

			if (MBR::CheckOverlap(scaler->GetMapCalcMBR(), layer->m_mbr)) //in range
			{
				if (layer->m_spatialObject->m_FileType == S100_FileType::FILE_S_100_VECTOR)
				{
					auto s100Layer = (S100Layer*)layer;
					auto cell = (S101Cell*)layer->m_spatialObject;
					auto fc = s100Layer->GetFeatureCatalog();
					auto pc = s100Layer->GetPC();
					numeric_number_of_text_placement = 0;

					DrawingSet* pCurDrawingSet = NULL;

					// Initialization work is carried out to organize the instructions according to the PC and Priority used.
					auto ipc = drawingSetByPC.find(pc);
					if (ipc == drawingSetByPC.end())
					{
						DrawingSet curDrawingSet;
						drawingSetByPC.insert(std::make_pair(pc, curDrawingSet));

						pCurDrawingSet = &drawingSetByPC.find(pc)->second;
					}
					else
					{
						pCurDrawingSet = &ipc->second;
					}

					auto ii = cell->m_dsgir.m_ftcs->m_arrFindForCode.find(L"TextPlacement");
					if (ii != cell->m_dsgir.m_ftcs->m_arrFindForCode.end())
					{
						numeric_number_of_text_placement = ii->second->m_nmcd;
					}

					if (nullptr == cell->pcManager || nullptr == cell->pcManager->displayListSENC)
					{
						continue;
					}

					std::list<SENC_Instruction*> itList;
					for (int i = 0; i < 100; i++)
					{
						/*
						* Type Of Instruction
						* 0 : Null Instruction
						* 1 : Point Instruction
						* 2 : Line Instruction
						* 3 : Area Instruction
						* 4 : Text Instruction
						*/
						int cnt = 0;

						// AugmentedRay
						cell->pcManager->displayListSENC->GetDrawingInstruction(i, 7, scaler, itList);
						if (itList.size() > 0)
						{
							std::list<SENC_Instruction*>* instructionList = pCurDrawingSet->GetAugmentedRayList(i);
							instructionList->insert(instructionList->begin(), itList.begin(), itList.end());
							cnt += (int)itList.size();
						}

						// AugmentedPath
						cell->pcManager->displayListSENC->GetDrawingInstruction(i, 8, scaler, itList);
						if (itList.size() > 0)
						{
							std::list<SENC_Instruction*>* instructionList = pCurDrawingSet->GetAugmentedPathList(i);
							instructionList->insert(instructionList->begin(), itList.begin(), itList.end());
							cnt += (int)itList.size();
						}

						// Point
						cell->pcManager->displayListSENC->GetDrawingInstruction(i, 1, scaler, itList);
						if (itList.size() > 0)
						{
							std::list<SENC_Instruction*>* instructionList = pCurDrawingSet->GetPointList(i);
							instructionList->insert(instructionList->begin(), itList.begin(), itList.end());
							cnt += (int)itList.size();
						}

						// Line
						cell->pcManager->displayListSENC->GetDrawingInstruction(i, 2, scaler, itList);
						if (itList.size() > 0)
						{
							std::list<SENC_Instruction*>* instructionList = pCurDrawingSet->GetLineList(i);
							instructionList->insert(instructionList->begin(), itList.begin(), itList.end());
							cnt += (int)itList.size();
						}

						// Area
						cell->pcManager->displayListSENC->GetDrawingInstruction(i, 3, scaler, itList);
						if (itList.size() > 0)
						{
							std::list<SENC_Instruction*>* instructionList = pCurDrawingSet->GetAreaList(i);
							instructionList->insert(instructionList->begin(), itList.begin(), itList.end());
							cnt += (int)itList.size();
						}


						cell->pcManager->displayListSENC->GetDrawingInstructionByCondition(i, 5, scaler, itList, numeric_number_of_text_placement);


						if (itList.size() > 0)
						{
							std::list<SENC_Instruction*>* instructionList = pCurDrawingSet->GetTextList(i);
							instructionList->insert(instructionList->begin(), itList.begin(), itList.end());
							cnt += (int)itList.size();
						}

						cell->pcManager->displayListSENC->GetDrawingInstruction(i, 0, scaler, itList);

						if (cnt)
						{
							drawingPriority.insert(i);
						}
					}


					itList.clear();
				}

				lineSuppressionMap.clear();

				for (auto fi = drawingSetByPC.begin(); fi != drawingSetByPC.end(); fi++)
				{
					DrawingSet* pCurDrawingSet = &fi->second;

					for (auto ii = drawingPriority.rbegin(); ii != drawingPriority.rend(); ii++)
					{
						int i = *ii;
						auto lineInstructions = pCurDrawingSet->GetLineList()[i];

						for (auto j = lineInstructions.begin(); j != lineInstructions.end(); j++)
						{
							auto lineInstruction = (SENC_LineInstruction*)*j;
							std::list<SCurveHasOrient>* curListCurveLink = NULL;
							R_FeatureRecord* fr = lineInstruction->fr;

							if (lineInstruction->spatialReference.size() > 0)
							{
								if (lineInstruction->m_listCurveLink.size() == 0)
								{
									if (fr->m_geometry->type == 3)
									{
										auto surface = (SSurface*)fr->m_geometry;

										curListCurveLink = &surface->m_listCurveLink;
									}
									else if (fr->m_geometry->type == 2)
									{
										auto compositeCurve = (SCompositeCurve*)fr->m_geometry;

										curListCurveLink = &compositeCurve->m_listCurveLink;
									}

									for (auto iterLi = lineInstruction->spatialReference.begin(); iterLi != lineInstruction->spatialReference.end(); iterLi++)
									{
										SENC_SpatialReference* sred = *iterLi;
										int referencedID = sred->reference;

										for (auto iterCurLcl = curListCurveLink->begin(); iterCurLcl != curListCurveLink->end(); iterCurLcl++)
										{
											auto curve = *iterCurLcl;

											int id = curve.GetCurve()->m_id & 0x0000FFFF;

											if (id == referencedID)
											{
												lineInstruction->m_listCurveLink.push_back(curve);
												break;
											}
										}
									}
								}
								curListCurveLink = &lineInstruction->m_listCurveLink;
							}
							else if (fr->m_geometry->type == 3)
							{
								auto surface = (SSurface*)fr->m_geometry;

								curListCurveLink = &surface->m_listCurveLink;
							}
							else if (fr->m_geometry->type == 2)
							{
								auto compositeCurve = (SCompositeCurve*)fr->m_geometry;

								curListCurveLink = &compositeCurve->m_listCurveLink;
							}

							for (auto k = curListCurveLink->begin(); k != curListCurveLink->end(); k++)
							{
								SCurveHasOrient* cw = &(*k);
								auto curve = cw->GetCurve();
								int id = curve->m_id & 0x0000FFFF;

								auto iterExist = lineSuppressionMap.find(id);

								if (iterExist == lineSuppressionMap.end() && !(*k).GetMasking())
								{
									lineSuppressionMap.insert(id);
									cw->SetIsDuplicated(true);
								}
								else if ((*k).GetMasking())
								{
									cw->SetIsDuplicated(true);
								}
								else
								{
									cw->SetIsDuplicated(true);
								}
							}
						}
					}
				}

				gisLib->D2.pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				gisLib->D2.pDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

				drawingPriority.insert(2);
				drawingPriority.insert(4);

				for (auto ii = drawingPriority.begin(); ii != drawingPriority.end(); ii++)
				{
					// i : drawing priority
					int i = *ii;

					for (auto fi = drawingSetByPC.begin(); fi != drawingSetByPC.end(); fi++)
					{
						auto pc = fi->first;
						pc->GetS100PCManager()->CreateBitmapBrush(rt);
						pc->GetS100PCManager()->InverseMatrixBitmapBrush(scaler->GetInverseMatrix());

						DrawingSet* pCurDrawingSet = &fi->second;

						AddSymbolDrawing(i, hdc, offset,
							pCurDrawingSet->GetAugmentedRayList(),
							pCurDrawingSet->GetAugmentedPathList(),
							pCurDrawingSet->GetPointList(),
							pCurDrawingSet->GetLineList(),
							pCurDrawingSet->GetAreaList(),
							pCurDrawingSet->GetTextList(),
							pc);

						pc->GetS100PCManager()->DeleteBitmapBrush();
					}

					for (auto itor = m_listLayer.begin(); itor != m_listLayer.end(); itor++)
					{
						if ((*itor)->IsOn())
						{
							Layer* layer = *itor;

							S100_FileType filetype = layer->m_spatialObject->GetFileType();

							if (filetype != FILE_S_100_GRID_H5)
								continue;

							if (MBR::CheckOverlap(scaler->GetMapCalcMBR(), layer->m_mbr)) //in range
							{
								auto l = (S101Layer*)layer;
								auto c = (S100SpatialObject*)l->m_spatialObject;
							}
						}
					}
				}
			}
		}
	}

	rt->EndDraw();
}

// i : drawing priority
void LayerManager::AddSymbolDrawing(
	int drawingPrioriy,
	HDC& hdc,
	int offset,
	std::list<SENC_Instruction*>* augmentedRay,
	std::list<SENC_Instruction*>* augmentedPath,
	std::list<SENC_Instruction*>* point,
	std::list<SENC_Instruction*>* line,
	std::list<SENC_Instruction*>* area,
	std::list<SENC_Instruction*>* text,
	PortrayalCatalogue* pc)
{
	std::list<D2D1_POINT_2F> points;

	gisLib->D2.pRT->SetTransform(scaler->GetMatrix());

	// Area
	for (auto i = area[drawingPrioriy].begin(); i != area[drawingPrioriy].end(); i++)
	{
		auto instruction = *i;

		gisLib->D2.pBrush->SetOpacity(1.0f);
		instruction->DrawInstruction(
			gisLib->D2.pRT,
			gisLib->D2.pD2Factory,
			gisLib->D2.pBrush,
			&gisLib->D2.D2D1StrokeStyleGroup,
			scaler,
			pc);
	}

	// Line
	for (auto i = line[drawingPrioriy].begin(); i != line[drawingPrioriy].end(); i++)
	{
		auto instruction = *i;
		instruction->DrawInstruction(
			gisLib->D2.pRT,
			gisLib->D2.pD2Factory,
			gisLib->D2.pBrush,
			&gisLib->D2.D2D1StrokeStyleGroup,
			scaler,
			pc);
	}

	gisLib->D2.pRT->SetTransform(D2D1::Matrix3x2F::Identity());

	// AugmentedRay
	for (auto i = augmentedRay[drawingPrioriy].begin(); i != augmentedRay[drawingPrioriy].end(); i++)
	{
		auto instruction = *i;
		instruction->DrawInstruction(gisLib->D2.pRT, gisLib->D2.pD2Factory, gisLib->D2.pBrush, &gisLib->D2.D2D1StrokeStyleGroup, scaler, pc);
	}

	// AugmentedPath
	for (auto i = augmentedPath[drawingPrioriy].begin(); i != augmentedPath[drawingPrioriy].end(); i++)
	{
		auto instruction = *i;
		instruction->DrawInstruction(gisLib->D2.pRT, gisLib->D2.pD2Factory, gisLib->D2.pBrush, &gisLib->D2.D2D1StrokeStyleGroup, scaler, pc);
	}

	// Point
	for (auto i = point[drawingPrioriy].begin(); i != point[drawingPrioriy].end(); i++)
	{
		auto instruction = (SENC_PointInstruction*)*i;

		points.clear();

		if (ENCCommon::AREA_SYMBOL_DYNAMIC_POSITION_MODE)
		{
			instruction->GetDrawPointsDynamic(scaler, points);
		}
		else
		{
			instruction->GetDrawPoints(scaler, points);
		}

		S100_SVG_D2D1_DLL::RealSymbol rs;
		for (auto pi = points.begin(); pi != points.end(); pi++)
		{
			// rotate Point
			if (scaler->GetRotationDegree())
			{
				double radian = (180 - scaler->GetRotationDegree()) * DEG2RAD;

				FLOAT tempX = (float)(scaler->soy - pi->y) * (float)sin(radian) + (float)(scaler->sox - pi->x) * (float)cos(radian);
				FLOAT tempY = (float)(scaler->soy - pi->y) * (float)cos(radian) - (float)(scaler->sox - pi->x) * (float)sin(radian);

				pi->x = tempX + (float)scaler->sox;
				pi->y = tempY + (float)scaler->soy;
			}

			float rotation = 0;

			if ((instruction->symbol)->rotation)
			{
				rotation = (float)(instruction->symbol)->rotation->value;
			}

			float instructionScale = (instruction->symbol)->scaleFactor;

			if (instruction &&
				instruction->symbol &&
				instruction->symbol->pSvg)
			{
				rs.symName = instruction->symbol->pSvg->m_svgName;
				rs.rotation = rotation;
				rs.x = pi->x;
				rs.y = pi->y;
				rs.scale = instructionScale;
				pc->GetSVGManager()->DrawRealSymbol(rs, pc);
			}
		}
	}

	// Text
	if (ENCCommon::TEXTOUT)
	{
		if (text[drawingPrioriy].size() > 0)
		{
			HWND hWnd = ::GetActiveWindow();
			HDC hdc = ::GetDC(hWnd);

			int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);    // Monitor x axis dpi.
			int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);    // Monitor y axis dpi.
			::ReleaseDC(hWnd, hdc);
			D2D1_SIZE_F renderTargetSize = gisLib->D2.pRT->GetSize();

			for (auto i = text[drawingPrioriy].begin(); i != text[drawingPrioriy].end(); i++)
			{
				auto instruction = (SENC_TextInstruction*)*i;
				points.clear();

				if (ENCCommon::AREA_SYMBOL_DYNAMIC_POSITION_MODE)
				{
					instruction->GetDrawPointsDynamic(scaler, points);
				}
				else
				{
					instruction->GetDrawPoints(scaler, points);
				}

				SENC_TextPoint* textPoint = instruction->textPoint;
				auto itorTp = textPoint->elements.begin();

				if (itorTp != textPoint->elements.end())
				{
					unsigned textSize = 1;
					SENC_Element* element = *itorTp;

					std::wstring viewString = element->text.value;

					int angle = element->font.slant;
					double radian = 0;
					if (angle)
						radian = -angle / 180. * M_PI;

					COLORREF color = 0x00000000;
					if (element->pColor == NULL)
					{
					}
					else
					{
						color = *element->pColor;
					}

					int bodySize = element->bodySize;

					IDWriteTextFormat* useWTF = NULL;
					if (bodySize != 15)
					{
						auto sizedFontIter = gisLib->D2.writeTextFormatListByFontSize.find(bodySize);

						if (sizedFontIter == gisLib->D2.writeTextFormatListByFontSize.end())
						{
							IDWriteTextFormat* newWriteTextFormat = NULL;
							HRESULT hr = gisLib->D2.pDWriteFactory->CreateTextFormat(
								ENCCommon::DISPLAY_FONT_NAME.c_str(),
								NULL,
								DWRITE_FONT_WEIGHT_NORMAL,
								DWRITE_FONT_STYLE_NORMAL,
								DWRITE_FONT_STRETCH_NORMAL,
								(float)bodySize * (float)1.358,
								L"", //locale
								&newWriteTextFormat
							);

							gisLib->D2.writeTextFormatListByFontSize.insert(std::make_pair(bodySize, newWriteTextFormat));

							useWTF = newWriteTextFormat;

						}
						else
						{
							useWTF = sizedFontIter->second;
						}
					}
					else
					{
						useWTF = gisLib->D2.pDWriteTextFormat;
					}

					useWTF->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

					if (!instruction->textMatrix)
					{
						IDWriteTextLayout* textLayout = nullptr;
						float caretX = renderTargetSize.width;
						float caretY = renderTargetSize.height;

						const wchar_t* vText = element->text.value.c_str();
						int vTextSize = (int)element->text.value.size();
						gisLib->D2.pDWriteFactory->CreateTextLayout(
							vText,
							vTextSize,
							useWTF,
							caretX,
							caretY,
							&textLayout
						);

						instruction->textMatrix = new DWRITE_TEXT_METRICS();
						textLayout->GetMetrics(instruction->textMatrix);

						SafeRelease(&textLayout);
					}

					float width = instruction->textMatrix->width;
					float height = instruction->textMatrix->height;

					float offset_x = 0;
					float offset_y = 0;

					// INCH to mm
					float offsetUnitX = (dpiX / (float)25.4);
					float offsetUnitY = (dpiY / (float)25.4);

					// Determine the size of the Offset
					float XOFFS = ((float)textPoint->offset.x * offsetUnitX);
					float YOFFS = -((float)textPoint->offset.y * offsetUnitY);

					// HJUST 
					// CENTRE
					//[ Text Placement ]
					if (instruction->fr->m_textBearing)
					{
						if (*instruction->fr->m_textBearing > 90 && *instruction->fr->m_textBearing <= 270)
						{
							offset_x = -width;
							XOFFS = -((float)textPoint->offset.x * offsetUnitX);
						}
						else
						{
							offset_x = 0;
						}
						offset_y = -height / (float)2.;
					}
					else {
						if (textPoint->horizontalAlignment == CENTER)
						{
							offset_x = -width / (float)2.;
						}
						// RIGHT
						else if (textPoint->horizontalAlignment == END)
						{
							offset_x = -width;
						}
						// LEFT
						else if (textPoint->horizontalAlignment == START)
						{
							offset_x = 0;
						}
						else
						{
							offset_x = 0;
						}

						// VJUST
						// BOTTOM
						if (textPoint->verticalAlignment == BOTTOM)
						{
							offset_y = 0;
						}
						// CENTRE
						else if (textPoint->verticalAlignment == CENTER)
						{
							offset_y = -height / (float)2.;
						}
						// TOP
						else if (textPoint->verticalAlignment == TOP)
						{
							offset_y = -height;
						}
						else
						{
							offset_y = -height / (float)2.;
						}

					}

					offset_x += XOFFS;
					offset_y += YOFFS;

					int r = (color >> 16) & 0xff;
					int g = ((color >> 8) & 0xff);
					int b = ((color >> 0) & 0xff);
					gisLib->D2.pBrush->SetColor(D2D1::ColorF((FLOAT)(GetRValue(color)) / (float)255.0, (GetGValue(color)) / (float)255.0, (GetBValue(color) / (float)255.0)));

					for (auto itor = points.begin(); itor != points.end(); itor++)
					{
						D2D1_POINT_2F* p = &(*itor);

						float x = p->x + offset_x;
						float y = p->y + offset_y;
						float width = instruction->textMatrix->width;
						float height = instruction->textMatrix->height;

						const wchar_t* vText = element->text.value.c_str();

						int vTextSize = (int)element->text.value.size();


						// rotate Point
						if (scaler->GetRotationDegree())
						{
							double radian = (180 - scaler->GetRotationDegree()) * DEG2RAD;

							FLOAT tempX = (float)(scaler->soy - y) * (float)sin(radian) + (float)(scaler->sox - x) * (float)cos(radian);
							FLOAT tempY = (float)(scaler->soy - y) * (float)cos(radian) - (float)(scaler->sox - x) * (float)sin(radian);

							x = tempX + (float)scaler->sox;
							y = tempY + (float)scaler->soy;
						}

						gisLib->D2.pRT->SetTransform(D2D1::Matrix3x2F::Identity());
						gisLib->D2.pRT->DrawText(
							vText,
							vTextSize,
							useWTF,
							D2D1::RectF(x, y, x + width, y + height),
							gisLib->D2.pBrush
						);
					}
				}
			}
		}
	}
}


void LayerManager::DrawBackground(HDC &hDC, int offset)
{
	if (m_baseMapOn)
	{
		POSITION pos = m_listBackgroundLayer.GetHeadPosition();

		while (pos)
		{
			if (m_listBackgroundLayer.GetAt(pos)->IsOn())
			{
				Layer* layer = m_listBackgroundLayer.GetNext(pos);

				if (MBR::CheckOverlap(scaler->GetMapCalcMBR(), layer->m_mbr))
				{
					layer->Draw(hDC, scaler, offset - 360);
					layer->Draw(hDC, scaler, offset);
					layer->Draw(hDC, scaler, offset + 360);
				}
			}
			else
			{
				m_listBackgroundLayer.GetNext(pos);
			}
		}
	}
}

void LayerManager::DrawS100Datasets(HDC& hdc, int offset)
{

	{
		std::set<int> drawingPriority;

		unsigned numeric_number_of_text_placement = 0;

		std::unordered_map<PortrayalCatalogue*, DrawingSet*> drawingSetByPC;

		// Create drawing sets for each PC.
		for (auto itor = m_listLayer.begin(); itor != m_listLayer.end(); itor++)
		{
			auto layer = *itor;

			if (true == IsOn(layer))
			{
				if (layer->m_spatialObject->m_FileType != S100_FileType::FILE_S_100_VECTOR)
				{
					continue;
				}

				if (MBR::CheckOverlap(scaler->GetMapCalcMBR(), layer->m_mbr))
				{
					auto s100Layer = (S100Layer*)layer;
					auto cell = (S101Cell*)layer->GetSpatialObject();
					auto fc = s100Layer->GetFeatureCatalog();
					auto pc = s100Layer->GetPC();

					numeric_number_of_text_placement = 0;
					auto ii = cell->m_dsgir.m_ftcs->m_arrFindForCode.find(L"TextPlacement");
					if (ii != cell->m_dsgir.m_ftcs->m_arrFindForCode.end())
					{
						numeric_number_of_text_placement = ii->second->m_nmcd;
					}

					DrawingSet* pCurDrawingSet = nullptr;
					auto ipc = drawingSetByPC.find(pc);
					if (ipc == drawingSetByPC.end())
					{
						auto curDrawingSet = new DrawingSet();
						drawingSetByPC.insert({ pc, curDrawingSet });
						pCurDrawingSet = curDrawingSet;
					}
					else
					{
						pCurDrawingSet = ipc->second;
					}

					if (cell->m_FileType == S100_FileType::FILE_S_100_VECTOR)
					{
						if (nullptr == cell->pcManager ||
							nullptr == cell->pcManager->displayListSENC)
						{
							continue;
						}

						std::list<SENC_Instruction*> itList;
						for (int i = 0; i < 100; i++)
						{
							/*
							* Type Of Instruction
							* 0 : Null Instruction
							* 1 : Point Instruction
							* 2 : Line Instruction
							* 3 : Area Instruction
							* 4 : Text Instruction
							*/
							int cnt = 0;

							// Augmented Ray
							cell->pcManager->displayListSENC->GetDrawingInstruction(i, 7, scaler, itList);

							if (itList.size() > 0)
							{
								auto instructionList = pCurDrawingSet->GetAugmentedRayList(i);
								instructionList->insert(instructionList->begin(), itList.begin(), itList.end());
								cnt += (int)itList.size();
							}

							// Augmented Path
							cell->pcManager->displayListSENC->GetDrawingInstruction(i, 8, scaler, itList);

							if (itList.size() > 0)
							{
								auto instructionList = pCurDrawingSet->GetAugmentedPathList(i);
								instructionList->insert(instructionList->begin(), itList.begin(), itList.end());
								cnt += (int)itList.size();
							}

							// Point
							cell->pcManager->displayListSENC->GetDrawingInstruction(i, 1, scaler, itList);

							if (itList.size() > 0)
							{
								auto instructionList = pCurDrawingSet->GetPointList(i);
								instructionList->insert(instructionList->begin(), itList.begin(), itList.end());
								cnt += (int)itList.size();
							}

							// Line
							cell->pcManager->displayListSENC->GetDrawingInstruction(i, 2, scaler, itList);

							if (itList.size() > 0)
							{
								auto instructionList = pCurDrawingSet->GetLineList(i);
								instructionList->insert(instructionList->begin(), itList.begin(), itList.end());
								cnt += (int)itList.size();
							}

							// Area
							cell->pcManager->displayListSENC->GetDrawingInstruction(i, 3, scaler, itList);

							if (itList.size() > 0)
							{
								auto instructionList = pCurDrawingSet->GetAreaList(i);
								instructionList->insert(instructionList->begin(), itList.begin(), itList.end());
								cnt += (int)itList.size();
							}

							// Text

							cell->pcManager->displayListSENC->GetDrawingInstructionByCondition(i, 5, scaler, itList, numeric_number_of_text_placement);



							if (itList.size() > 0)
							{
								auto instructionList = pCurDrawingSet->GetTextList(i);
								instructionList->insert(instructionList->begin(), itList.begin(), itList.end());
								cnt += (int)itList.size();
							}

							cnt += (int)itList.size();

							cell->pcManager->displayListSENC->GetDrawingInstruction(i, 0, scaler, itList);

							if (cnt)
							{
								drawingPriority.insert(i);
							}
						}

						itList.clear();
					}
				}
			}
		}

		// Line Suppression
		SuppressS101Lines(drawingPriority, drawingSetByPC);

		std::list<D2D1_POINT_2F> points;

		auto rt = gisLib->D2.pRT;
		rt->BindDC(hdc, scaler->GetScreenRect());
		rt->BeginDraw();
		gisLib->D2.pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		gisLib->D2.pDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		// IC Level 0 - S-10X Vector drawing
		for (auto i = m_listLayer.begin(); i != m_listLayer.end(); i++)
		{
			auto layer = (S100Layer*)*i;
			auto cell = layer->GetS100SpatialObject();

			if (layer->IsOn() == true &&
				MBR::CheckOverlap(scaler->GetMapCalcMBR(), layer->m_mbr))
			{
				if (cell->m_FileType == S100_FileType::FILE_S_100_VECTOR)
				{
					auto s101Cell = (S101Cell*)cell;
					auto pc = layer->GetPC();

					numeric_number_of_text_placement = 0;
					auto ii = s101Cell->m_dsgir.m_ftcs->m_arrFindForCode.find(L"TextPlacement");
					if (ii != s101Cell->m_dsgir.m_ftcs->m_arrFindForCode.end())
					{
						numeric_number_of_text_placement = ii->second->m_nmcd;
					}

					if (nullptr == s101Cell->pcManager ||
						nullptr == s101Cell->pcManager->displayListSENC)
					{
						continue;
					}

					for (auto dp = drawingPriority.begin(); dp != drawingPriority.end(); dp++)
					{
						int drawingPriority = *dp;

						for (auto j = drawingSetByPC.begin(); j != drawingSetByPC.end(); j++)
						{
							auto pc = j->first;
							auto pCurDrawingSet = j->second;

							if (pc == layer->GetPC())
							{
								pc->GetS100PCManager()->CreateBitmapBrush(gisLib->D2.pRT);
								pc->GetS100PCManager()->InverseMatrixBitmapBrush(scaler->GetInverseMatrix());

								AddSymbolDrawing(drawingPriority, hdc, offset,
									pCurDrawingSet->GetAugmentedRayList(),
									pCurDrawingSet->GetAugmentedPathList(),
									pCurDrawingSet->GetPointList(),
									pCurDrawingSet->GetLineList(),
									pCurDrawingSet->GetAreaList(),
									pCurDrawingSet->GetTextList(),
									pc);

								pc->GetS100PCManager()->DeleteBitmapBrush();
							}
						}
					}
				}
				else if (cell->m_FileType == S100_FileType::FILE_S_100_GRID_H5)
				{
					rt->EndDraw();
					layer->Draw(hdc, scaler, offset);
					rt->BindDC(hdc, scaler->GetScreenRect());
					rt->BeginDraw();
				}
			}
		}

		rt->EndDraw();

		for (auto i = drawingSetByPC.begin(); i != drawingSetByPC.end(); i++)
		{
			delete i->second;
		}

		drawingSetByPC.clear();
	}
}

void LayerManager::S101RebuildPortrayal(/*PORTRAYAL_BUILD_TYPE type*/)
{
	for (auto itor = m_listLayer.begin(); itor != m_listLayer.end(); itor++)
	{
		Layer* layer = *itor;
		if (layer->m_spatialObject->m_FileType == S100_FileType::FILE_S_100_VECTOR)
		{
			BuildPortrayalCatalogue(layer);
		}
		else if (layer->m_spatialObject->m_FileType == S100_FileType::FILE_S_100_GRID_H5)
		{
			BuildPortrayalCatalogue(layer);
		}
		else if (layer->m_spatialObject->m_FileType == S100_FileType::FILE_S_100_GRID_BAG)
		{
			BuildPortrayalCatalogue(layer);
		}
	}
}

void LayerManager::BuildPortrayalCatalogue(Layer* l)
{
	std::wstring layerName(l->GetLayerName());
	auto ci = layerName.find_last_of(L"\\");
	layerName = layerName.substr(++ci);

	ci = layerName.find_last_of(L".");
	layerName = layerName.substr(0, ci);

	std::wstring inputPath = L"..\\ProgramData\\S100_PC_IO_XML\\INPUT\\";
	std::wstring outputPath = L"..\\ProgramData\\S100_PC_IO_XML\\OUTPUT\\";

	inputPath.append(layerName);
	inputPath.append(L".xml");

	outputPath.append(layerName);
	outputPath.append(L".xml");

	CString excuteFolderPath;
	::GetModuleFileName(NULL, excuteFolderPath.GetBuffer(MAX_PATH), MAX_PATH);
	excuteFolderPath.ReleaseBuffer();
	if (excuteFolderPath.Find('\\') != -1)
	{
		for (int i = excuteFolderPath.GetLength() - 1; i >= 0; i--)
		{
			TCHAR ch = excuteFolderPath[i];
			excuteFolderPath.Delete(i);
			if (ch == '\\') break;

		}
	}

	if (l->m_spatialObject->m_FileType == S100_FileType::FILE_S_100_VECTOR && ((S101Layer*)l)->GetPC()->GetPortrayalRuleType() == PortrayalRuleType::LUA)
	{
		DeleteFile(inputPath.c_str());

		ProcessS101::ProcessS101_LUA(L"..\\ProgramData\\S101_Portrayal\\Rules\\main.lua", (S101Layer*)l); //memory leak
	}
}

void LayerManager::DrawValidationLayers(HDC& hdc, int offset)
{
	POSITION pos = m_listBackgroundLayer.GetHeadPosition();

	while (pos)
	{
		if (m_listBackgroundLayer.GetAt(pos)->IsOn())
		{
			Layer* layer = m_listBackgroundLayer.GetNext(pos);

			if (MBR::CheckOverlap(scaler->GetMapCalcMBR(), layer->m_mbr))
			{
				layer->Draw(hdc, scaler, offset - 360);
				layer->Draw(hdc, scaler, offset);
				layer->Draw(hdc, scaler, offset + 360);
			}
		}
		else
		{
			m_listBackgroundLayer.GetNext(pos);
		}
	}
}

void LayerManager::DrawOverlay(HDC& hdc, int type, int offset)
{
	switch (type)
	{
	case 1:
		break;
	}
}

int LayerManager::GetLayerCount()
{
	return (int)m_listLayer.size();
}

void LayerManager::GetLayer(int index, Layer* _layer)
{
	if ((index >= 0) && (index < GetLayerCount()))
	{
		auto iter = m_listLayer.begin();
		std::advance(iter, index);
		_layer = *iter;
	}

	_layer = nullptr;
	return;
}

Layer* LayerManager::GetLayer(int index)
{
	if (index < 0 || index >= GetLayerCount())
	{
		return NULL;
	}

	Layer* retLayer = NULL;
	if (index >= GetLayerCount())
	{
		return NULL;
	}


	if (index < GetLayerCount())
	{
		auto iter = m_listLayer.begin();
		std::advance(iter, index);
		retLayer = *iter;
	}

	return retLayer;
}

int LayerManager::GetLayerIndex(CString _pathName)
{
	for (unsigned i = 0; i < m_listLayer.size(); i++)
	{
		if (m_listLayer.at(i)->m_spatialObject->GetFileName() == _pathName)
		{
			return i;
		}
	}

	return -1;
}

CString LayerManager::GetLayerName(int index)
{
	if ((index >= 0) && (index < GetLayerCount()))
	{
		auto iter = m_listLayer.begin();
		std::advance(iter, index);

		return (*iter)->GetLayerName();
	}

	return L"";
}

BOOL LayerManager::IsOn(int index)
{
	if ((index >= 0) && (index < GetLayerCount()))
	{
		auto iter = m_listLayer.begin();
		std::advance(iter, index);

		return (*iter)->IsOn();
	}

	return FALSE;
}

void LayerManager::DeleteLayer(int index)
{
	if ((index >= 0) && (index < GetLayerCount()))
	{
		auto i = m_listLayer.begin();
		std::advance(i, index);
		auto layer = *i;

		m_listLayer.erase(i);

		delete layer;
		layer = nullptr;

		ReMBR();
	}

	return;
}

void LayerManager::DeleteLayer(CString filepath)
{
	for (auto i = m_listLayer.begin(); i != m_listLayer.end(); i++)
	{
		Layer* layer = *i;

		if (layer->GetLayerPath().Compare(filepath) == 0)
		{
			m_listLayer.erase(i);

			delete layer;
			layer = nullptr;

			ReMBR();
			return;
		}

	}
}

void LayerManager::DeleteAllLayer()
{
	m_listLayer.erase(m_listLayer.begin(), m_listLayer.end());
}

void LayerManager::ReMBR()
{
	if (m_listLayer.size() == 0)
	{
		double xmin = -170.0;
		double ymin = -30.0;
		double xmax = 170.0;
		double ymax = 30.0;

		projection(xmin, ymax);
		projection(xmax, ymin);

		mbr.xmin = xmin;
		mbr.ymin = ymin;
		mbr.xmax = xmax;
		mbr.ymax = ymax;
	}
	else
	{
		MBR _mbr;
		for (auto itor = m_listLayer.begin(); itor != m_listLayer.end(); itor++)
		{
			if (itor == m_listLayer.begin())
			{
				_mbr = (*itor)->m_mbr;
			}
			else
			{
				_mbr.ReMBR((*itor)->m_mbr);
			}

		}
		mbr.SetMBR(_mbr);
	}
}

void LayerManager::SetViewMBR(RECT r)
{
	scaler->SetMap(r);
}

void LayerManager::ShowTextPlacement(BOOL bShow)
{
	ENCCommon::SHOW_TEXT_PLACEMENT = bShow;
}

FeatureCatalogue* LayerManager::AddS100FC(std::wstring path)
{
	auto fc = new FeatureCatalogue(path);

	pS100Catalogs->push_back(fc);
	hash_FC->insert({ fc->GetName(), fc });

	return fc;
}

PortrayalCatalogue* LayerManager::AddS100PC(FeatureCatalogue* fc, std::wstring path)
{
	auto pc = new PortrayalCatalogue(path);

	pPortrayalCatalogues->push_back(pc);
	hash_PC->insert({ pc->GetProduct(), pc });

	// connect FC and PC
	CatalogueList[fc] = pc;

	return pc;
}

PortrayalCatalogue* LayerManager::AddS100PC(FeatureCatalogue* fc, std::wstring path, std::wstring currentName)
{
	auto pc = new PortrayalCatalogue(path);
	pc->SetCurrentPaletteName(currentName);

	pPortrayalCatalogues->push_back(pc);

	(*hash_PC)[pc->GetProduct()] = pc;

	// connect FC and PC
	CatalogueList[fc] = pc;

	return pc;
}

FeatureCatalogue* LayerManager::GetCatalog(int index)
{
	try
	{
		return pS100Catalogs->at(index);
	}
	catch (std::out_of_range e)
	{
		return nullptr;
	}

	return nullptr;
}

FeatureCatalogue* LayerManager::GetCatalogFromName(CString value)
{
	try
	{
		return hash_FC->at(std::wstring(value));
	}
	catch (std::out_of_range e)
	{
		return nullptr;
	}

	return nullptr;
}



PortrayalCatalogue* LayerManager::GetPortrayalCatalogue(int index)
{
	try
	{
		return pPortrayalCatalogues->at(index);
	}
	catch (std::out_of_range e)
	{
		return nullptr;
	}

	return nullptr;
}

PortrayalCatalogue* LayerManager::GetPortrayalCatalogueFromName(CString value)
{
	try
	{
		return hash_PC->at(std::wstring(value));
	}
	catch (std::out_of_range e)
	{
		return nullptr;
	}

	return nullptr;
}

void LayerManager::ChangeS100ColorPalette(std::wstring paletteName)
{
	for (auto i = m_listLayer.begin(); i != m_listLayer.end(); i++)
	{
		auto layer = *i;
		if (true == layer->IsS100Layer())
		{
			auto s100layer = (S100Layer*)layer;
			auto pc = s100layer->GetPC();
			if (nullptr != pc)
			{
				pc->SetCurrentPaletteName(paletteName);
				pc->DeletePatternImage();
				pc->CreatePatternImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));
				pc->DeleteLineImages();
				pc->CreateLineImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));
			}

			auto s100so = (S100SpatialObject*)layer->GetSpatialObject();
			if (nullptr != s100so)
			{
				auto pcOutputManager = s100so->GetPCOutputManager();
				if (nullptr != pcOutputManager)
				{
					pcOutputManager->ChangePallete(pc);
				}
			}
		}
	}

	for (auto i = pPortrayalCatalogues->begin(); i != pPortrayalCatalogues->end(); i++)
	{
		auto pc = *i;
		pc->SetCurrentPaletteName(paletteName);
	}
}

Scaler* LayerManager::GetScaler()
{
	return scaler;
}

// [Text Placement ]
float LayerManager::GetTextPlaceBearingValue(S101Cell* c, R_FeatureRecord* fr)
{
	int numeric_number_of_bearing = 0;
	auto ii = c->m_dsgir.m_atcs->m_arrFindForCode.find(L"flipBearing");
	if (ii != c->m_dsgir.m_atcs->m_arrFindForCode.end())
	{
		numeric_number_of_bearing = ii->second->m_nmcd;
	}

	for (auto itorParent = fr->m_attr.begin(); itorParent != fr->m_attr.end(); itorParent++)
	{
		F_ATTR* attr = *itorParent;
		for (auto aitor = attr->m_arr.begin(); aitor != attr->m_arr.end(); aitor++)
		{
			ATTR* attr = *aitor;

			if (attr->m_natc == numeric_number_of_bearing)
			{
				return (float)_wtof(attr->m_atvl);
			}
		}
	}

	return 0;
}

// [Text Placement ]
void LayerManager::DrawSemiCircle(float x, float y, float radius, float startAngle, float endAngle)
{
	// rotate Point
	if (scaler->GetRotationDegree())
	{
		double radian = (180 - scaler->GetRotationDegree()) * DEG2RAD;

		FLOAT tempX = (float)(scaler->soy - y) * (float)sin(radian) + (float)(scaler->sox - x) * (float)cos(radian);
		FLOAT tempY = (float)(scaler->soy - y) * (float)cos(radian) - (float)(scaler->sox - x) * (float)sin(radian);

		x = tempX + (float)scaler->sox;
		y = tempY + (float)scaler->soy;
	}

	ID2D1PathGeometry* pGeometryArc = nullptr;
	ID2D1PathGeometry* pGeometryLeftLine = nullptr;
	ID2D1PathGeometry* pGeometryRightLine = nullptr;

	HRESULT hr = gisLib->D2.pD2Factory->CreatePathGeometry(&pGeometryArc);
	ID2D1PathGeometry* pathGeo = (ID2D1PathGeometry*)pGeometryArc;
	ID2D1GeometrySink* pSink = nullptr;
	hr = pathGeo->Open(&pSink);

	FLOAT displayRadius = 150;

	D2D1_POINT_2F centerPoint = { 0.0F, 0.0F };
	D2D1_POINT_2F startPoint = { 0.0F, displayRadius };
	D2D1_POINT_2F endPoint = { 0.0F, displayRadius };

	D2D1::Matrix3x2F matrix1 = D2D1::Matrix3x2F::Identity();
	matrix1 = matrix1.Rotation(startAngle);
	startPoint = matrix1.TransformPoint(startPoint);

	D2D1::Matrix3x2F matrix2 = D2D1::Matrix3x2F::Identity();
	matrix2 = matrix2.Rotation(endAngle);
	endPoint = matrix2.TransformPoint(endPoint);


	pSink->BeginFigure(
		startPoint,
		D2D1_FIGURE_BEGIN_HOLLOW);

	pSink->AddArc(
		D2D1::ArcSegment(
			endPoint,
			D2D1::SizeF(displayRadius, displayRadius),
			0,
			D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
			(endAngle - startAngle) > 180 ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL
		)
	);
	pSink->EndFigure(D2D1_FIGURE_END_OPEN);
	hr = pSink->Close();
	SafeRelease(&pSink);

	hr = gisLib->D2.pD2Factory->CreatePathGeometry(&pGeometryLeftLine);
	pathGeo = (ID2D1PathGeometry*)pGeometryLeftLine;
	pSink = nullptr;
	hr = pathGeo->Open(&pSink);

	pSink->BeginFigure(
		centerPoint,
		D2D1_FIGURE_BEGIN_FILLED);
	pSink->AddLine(startPoint);
	pSink->EndFigure(D2D1_FIGURE_END_OPEN);
	hr = pSink->Close();
	SafeRelease(&pSink);


	hr = gisLib->D2.pD2Factory->CreatePathGeometry(&pGeometryRightLine);
	pathGeo = (ID2D1PathGeometry*)pGeometryRightLine;
	pSink = nullptr;
	hr = pathGeo->Open(&pSink);

	pSink->BeginFigure(
		centerPoint,
		D2D1_FIGURE_BEGIN_FILLED);
	pSink->AddLine(endPoint);
	pSink->EndFigure(D2D1_FIGURE_END_OPEN);
	hr = pSink->Close();
	SafeRelease(&pSink);

	D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Translation(x, y);

	gisLib->D2.pRT->SetTransform(trans);
	gisLib->D2.pBrush->SetColor(D2D1::ColorF((FLOAT)(255 / 255.0), (FLOAT)(222 / 255.0), (FLOAT)(100 / 255.0)));
	gisLib->D2.pRT->DrawGeometry(pGeometryArc, gisLib->D2.pBrush, 2, gisLib->D2.D2D1StrokeStyleGroup[0]);
	gisLib->D2.pRT->DrawGeometry(pGeometryRightLine, gisLib->D2.pBrush, 2, gisLib->D2.D2D1StrokeStyleGroup[0]);
	gisLib->D2.pRT->DrawGeometry(pGeometryLeftLine, gisLib->D2.pBrush, 2, gisLib->D2.D2D1StrokeStyleGroup[0]);

	SafeRelease(&pGeometryArc);
	SafeRelease(&pGeometryRightLine);
	SafeRelease(&pGeometryLeftLine);
}

FeatureCatalogue* LayerManager::GetFC(int productNumber)
{
	auto fcName = L"S-" + std::to_wstring(productNumber);
	auto fc = GetFC(fcName);
	return fc;
}

PortrayalCatalogue* LayerManager::GetPC(int productNumber)
{
	auto pcName = L"S-" + std::to_wstring(productNumber);
	auto pc = GetPC(pcName);
	return pc;
}

FeatureCatalogue* LayerManager::GetFC(std::wstring fcName)
{
	if (nullptr == hash_FC)
	{
		return nullptr;
	}

	auto item = hash_FC->find(fcName);
	if (item == hash_FC->end())
	{
		return nullptr;
	}

	return item->second;
}


PortrayalCatalogue* LayerManager::GetPC(std::wstring pcName)
{
	if (nullptr == hash_PC)
	{
		return nullptr;
	}

	auto item = hash_PC->find(pcName);
	if (item == hash_PC->end())
	{
		return nullptr;
	}

	return item->second;
}

void LayerManager::SuppressS101Lines(
	std::set<int>& drawingPriority,
	std::unordered_map<PortrayalCatalogue*, DrawingSet*>& drawingSetByPC)
{
	// Supression
	lineSuppressionMap.clear();
	for (auto i = drawingSetByPC.begin(); i != drawingSetByPC.end(); i++)
	{
		auto pCurDrawingSet = i->second;

		for (auto j = drawingPriority.rbegin(); j != drawingPriority.rend(); j++)
		{
			int drawingPriority = *j;
			auto lineInstructions = pCurDrawingSet->GetLineList()[drawingPriority];

			for (auto k = lineInstructions.begin(); k != lineInstructions.end(); k++)
			{
				auto lineInstruction = (SENC_LineInstruction*)*k;
				std::list<SCurveHasOrient>* curListCurveLink = nullptr;
				R_FeatureRecord* fr = lineInstruction->fr;

				if (lineInstruction->spatialReference.size() > 0)
				{
					if (lineInstruction->m_listCurveLink.size() == 0)
					{
						if (fr->m_geometry->type == 3)
						{
							auto surface = (SSurface*)fr->m_geometry;

							curListCurveLink = &surface->m_listCurveLink;
						}
						else if (fr->m_geometry->type == 2)
						{
							auto compositeCurve = (SCompositeCurve*)fr->m_geometry;

							curListCurveLink = &compositeCurve->m_listCurveLink;
						}

						for (auto iterLi = lineInstruction->spatialReference.begin(); iterLi != lineInstruction->spatialReference.end(); iterLi++)
						{
							SENC_SpatialReference* sred = *iterLi;
							int referencedID = sred->reference;

							for (auto iterCurLcl = curListCurveLink->begin(); iterCurLcl != curListCurveLink->end(); iterCurLcl++)
							{
								auto curve = *iterCurLcl;

								int id = curve.GetCurve()->m_id & 0x0000FFFF;

								if (id == referencedID)
								{
									lineInstruction->m_listCurveLink.push_back(curve);
									break;
								}
							}
						}
					}
					curListCurveLink = &lineInstruction->m_listCurveLink;
				}
				else if (fr->m_geometry->type == 3)
				{
					auto surface = (SSurface*)fr->m_geometry;

					curListCurveLink = &surface->m_listCurveLink;
				}
				else if (fr->m_geometry->type == 2)
				{
					auto compositeCurve = (SCompositeCurve*)fr->m_geometry;

					curListCurveLink = &compositeCurve->m_listCurveLink;
				}

				for (auto m = curListCurveLink->begin(); m != curListCurveLink->end(); m++)
				{
					auto cw = m;
					auto curve = cw->GetCurve();
					int id = curve->m_id & 0x0000FFFF;

					auto iterExist = lineSuppressionMap.find(id);

					if (iterExist == lineSuppressionMap.end() && !cw->GetMasking())
					{
						lineSuppressionMap.insert(id);
						cw->SetIsDuplicated(false);
					}
					else if (cw->GetMasking())
					{
						cw->SetIsDuplicated(true);
					}
					else
					{
						cw->SetIsDuplicated(true);
					}
				}
			}
		}
	}
}

int LayerManager::CheckFileType(CString path, int update)
{
	CFile file;
	int ret = 0;

	CString file_extension = LibMFCUtil::GetExtension(path);
	auto isUpdatFile = update > 0 ? true : false;

	if (file_extension.CompareNoCase(_T("000")) == 0 || isUpdatFile)
	{
		if (file.Open(path, CFile::modeRead))
		{
			if (file.GetLength() > 1024)
			{
				BYTE *lpBuf, *sBuf;
				lpBuf = new BYTE[1024];
				file.Read(lpBuf, 1024);
				sBuf = lpBuf;
				DRReader drReader;
				drReader.ReadReader(lpBuf);
				lpBuf += drReader.m_fieldPosition;
				lpBuf += drReader.m_fieldLength;
				lpBuf += 4;
				int tag = *((unsigned int*)lpBuf);

				if (tag == *((unsigned int*)"0001"))
				{
					ret = 1; // 57
				}
				else if (tag == *((unsigned int*)"DSID"))
				{
					ret = 2; // 101
				}
				delete sBuf;
			}

			file.Close();
		}
	}
	else if (file_extension.CompareNoCase(_T("XML")) == 0 || file_extension.CompareNoCase(_T("GML")) == 0)
	{
		if (!path.IsEmpty())
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(path);
			pugi::xml_node nodeList = doc.first_child();

			std::wstring nodeName = pugi::as_wide(nodeList.name());
			if ((nodeName.find(L"DataSet") != std::wstring::npos) ||
				(nodeName.find(L"Dataset") != std::wstring::npos))
			{
				ret = 3;
			}
		}
	}
	else if (file_extension.CompareNoCase(_T("h5")) == 0)
	{
		ret = 4;
	}

	return ret;
}

int LayerManager::CheckFileType(CString path)
{
	CFile file;
	int ret = 0;

	CString file_extension = LibMFCUtil::GetExtension(path);


	if (file_extension.CompareNoCase(_T("000")) == 0)
	{
		if (file.Open(path, CFile::modeRead))
		{
			if (file.GetLength() > 1024)
			{
				BYTE *lpBuf, *sBuf;
				lpBuf = new BYTE[1024];
				file.Read(lpBuf, 1024);
				sBuf = lpBuf;
				DRReader drReader;
				drReader.ReadReader(lpBuf);
				lpBuf += drReader.m_fieldPosition;
				lpBuf += drReader.m_fieldLength;
				lpBuf += 4;
				int tag = *((unsigned int*)lpBuf);

				if (tag == *((unsigned int*)"0001"))
				{
					ret = 1; // 57
				}
				else if (tag == *((unsigned int*)"DSID"))
				{
					ret = 2; // 101
				}
				delete sBuf;
			}

			file.Close();
		}
	}
	else if (file_extension.CompareNoCase(_T("XML")) == 0 || file_extension.CompareNoCase(_T("GML")) == 0)
	{
		if (!path.IsEmpty())
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(path);
			pugi::xml_node nodeList = doc.first_child();

			std::wstring nodeName = pugi::as_wide(nodeList.name());
			if ((nodeName.find(L"DataSet") != std::wstring::npos) ||
				(nodeName.find(L"Dataset") != std::wstring::npos))
			{
				ret = 3;
			}
		}
	}
	else if (file_extension.CompareNoCase(_T("h5")) == 0)
	{
		ret = 4;
	}

	return ret;
}

bool LayerManager::IsOn(Layer* layer)
{
	return layer->IsOn();
}

struct ROUTE_FOR_PROCESSING
{
	int LENGTH;
	int SPLIT_NUMBER;
};

std::vector<S100Layer*> LayerManager::GetS100Layers(int productNumber)
{
	std::vector<S100Layer*> result;

	for (
		auto i = m_listLayer.begin();
		i != m_listLayer.end();
		i++)
	{
		auto type = (*i)->GetLayerType();
		if (type.Find(L"S_100", 0) >= 0)
		{
			auto layer = (S100Layer*)(*i);
			if (productNumber == 101)
			{
				result.push_back(layer);
			}
		}
	}

	return result;
}

std::wstring LayerManager::GetObjectAttributeString(S101Cell* cell, F_ATTR* f_attr)
{
	FeatureCatalogue* fc = ((S101Layer*)cell->m_pLayer)->GetFeatureCatalog();

	std::wstring ret = L"";
	for (auto aitor = f_attr->m_arr.begin(); aitor != f_attr->m_arr.end(); aitor++)
	{
		ATTR* attr = *aitor;
		ret.append(L"[");
		auto atcsitor = cell->m_dsgir.m_atcs->m_arr.find(attr->m_natc);

		if (atcsitor != cell->m_dsgir.m_atcs->m_arr.end())
		{
			CodeWithNumericCode* nc = atcsitor->second;
			std::wstring ts(nc->m_code);
//			nc->m_code.ReleaseBuffer();
			auto ai = fc->GetSimpleAttribute(ts);
			if (ai != nullptr)
			{
				SimpleAttribute* sa = ai;
				ret.append(sa->GetName());
				ret.append(L"]\n");

				FCD::S100_CD_AttributeValueType avt = sa->GetValueType();
				if (avt == FCD::S100_CD_AttributeValueType::enumeration)
				{
					auto ei = sa->GetListedValuesPointer().begin()->GetListedValuePointer().find(_wtoi(attr->m_atvl));
					if (ei == sa->GetListedValuesPointer().begin()->GetListedValuePointer().end())
					{
						ret.append(L" : ");
						ret.append(attr->m_atvl);
						ret.append(L"\n");
					}
					else
					{
						ListedValue* lv = &ei->second;
						ret.append(L" : ");
						ret.append(lv->GetLabel().c_str());
						ret.append(L"\n");
					}
				}
				else if (attr->m_atvl.Compare(L"") != 0)
				{
					ret.append(L" : ");
					ret.append(attr->m_atvl);
					ret.append(L"\n");
				}
			}
			else
			{

			}
		}
	}

	return ret;
}