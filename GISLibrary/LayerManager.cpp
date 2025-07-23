#include "stdafx.h"
#include "LayerManager.h"
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
#include "S100Layer.h"
#include "SSurface.h"
#include "SMultiPoint.h"
#include "SCompositeCurve.h"
#include "SPoint.h"
#include "SCommonFuction.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include "../GeoMetryLibrary/GeometricFuc.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include "../LatLonUtility/LatLonUtility.h"

#include <ctime> 
#include <mmsystem.h> 
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <future>
#include <chrono>
#include "S100ExchangeCatalogue.h"

LayerManager::LayerManager(D2D1Resources* d2d1)
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

	D2 = d2d1;
}

LayerManager::LayerManager(Scaler* scaler, CatalogManager* catalogManager, D2D1Resources* d2d1) : LayerManager(d2d1)
{
	this->scaler = scaler;
	this->catalogManager = catalogManager;
}

LayerManager::~LayerManager()
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		delete* i;
	}
	layers.clear();

	featureOnOffMap.clear();
}

void LayerManager::MoveLayerFromList(int from, int to) {
	if (from == to) return;

	auto fromIt = std::next(layers.begin(), from);
	auto toIt = std::next(layers.begin(), to);

	layers.splice(toIt, layers, fromIt);
}

bool LayerManager::IsContainFilePathToLayer(CString _filepath)
{
	for (auto layer : layers) {
		if (layer->GetLayerPath().Compare(_filepath) == 0) {
			return true;
		}
	}
	return false;
}

bool LayerManager::AddBackgroundLayer(CString _filepath)
{
	CString file_extension = LibMFCUtil::GetExtension(_filepath);

	if (file_extension.CompareNoCase(_T("SHP")) == 0) 
	{
		if (backgroundLayer.Open(_filepath, D2) == false)
		{
			return false;
		}
	}

	double xmin = scaler->mxMinLimit;
	double xmax = scaler->mxMaxLimit;
	double ymin = scaler->myMinLimit;
	double ymax = scaler->myMaxLimit;

	MBR _mbr(xmin, ymin, xmax, ymax);
	if (m_isScreenFitEnabled)
		scaler->SetMap(_mbr);
	mbr.SetMBR(_mbr);

	return TRUE;
}

int LayerManager::AddLayer(Layer* _layer , CString FilePath, bool InsertBack)
{
	if (_layer == nullptr)
	{
		return -1;
	}

	_layer->SetID(CreateLayerID());

	if (LayerCount() == 0)
	{
		MBR newMBR = _layer->GetMBR();
		if (newMBR.IsEmpty())
		{
			newMBR.Extent(0.001);
		}

		mbr.SetMBR(newMBR);
		if (m_isScreenFitEnabled)
			scaler->SetMap(mbr);
	}
	else
	{
		mbr.CalcMBR(_layer->m_mbr);
	}

	if (FilePath == "")
		layers.push_back(_layer);
	else
	{
		for (auto it = layers.begin(); it != layers.end(); ++it) {
			if ((*it)->GetLayerPath() == FilePath)
			{
				if (InsertBack) {
					it++;
					layers.insert(it, _layer);
					break;
				}
				else
				{
					layers.insert(it, _layer);
					break;
				}

			}
		}
	}


	mapLayer.insert({ _layer->GetID(), _layer });

	return _layer->GetID();
}

int LayerManager::AddLayer(Layer* _layer)
{
	if (_layer == nullptr)
	{
		return -1;
	}

	_layer->SetID(CreateLayerID());

	if (LayerCount() == 0)
	{
		MBR newMBR = _layer->GetMBR();
		if (newMBR.IsEmpty())
		{
			newMBR.Extent(0.001);
		}

		mbr.SetMBR(newMBR);
		if (m_isScreenFitEnabled)
			scaler->SetMap(mbr);
	}
	else
	{
		mbr.CalcMBR(_layer->m_mbr);
	}
	
	layers.push_back(_layer);
	mapLayer.insert({ _layer->GetID(), _layer });

	return _layer->GetID();
}


int LayerManager::AddLayerFront(Layer* _layer)
{
	if (_layer == nullptr)
	{
		return -1;
	}

	_layer->SetID(CreateLayerID());

	if (LayerCount() == 0)
	{
		MBR newMBR = _layer->GetMBR();
		if (newMBR.IsEmpty())
		{
			newMBR.Extent(0.001);
		}

		mbr.SetMBR(newMBR);
		if (m_isScreenFitEnabled)
			scaler->SetMap(mbr);
	}
	else
	{
		mbr.CalcMBR(_layer->m_mbr);
	}

	layers.push_front(_layer);
	mapLayer.insert({ _layer->GetID(), _layer });

	return _layer->GetID();
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

int LayerManager::AddLayer(CString _filepath)
{
	Layer* layer = nullptr;
	size_t fitor = std::wstring::npos;

	S100_FileType fileType = CheckFileType(_filepath);

	if (fileType == S100_FileType::FILE_Shape ||
		fileType == S100_FileType::FILE_ETC)
	{
		layer = new Layer();
		if (layer->Open(_filepath, D2, this) == false)
		{
			delete layer;
			return -1;
		}
	}
	else if (fileType == S100_FileType::FILE_S_100_VECTOR ||
		fileType == S100_FileType::FILE_S_100_GRID_H5)
	{
		auto productNumber = pathToProductNumber(_filepath);
		auto fc = catalogManager->getFC(productNumber);
		auto pc = catalogManager->getPC(productNumber);

		if (fc)
		{
			//layer = new S100Layer(fc, pc);
			layer = new S100Layer(productNumber, fc, pc);
			if ((S100Layer*)layer->Open(_filepath, D2, this) == false)
			{
				delete layer;
				return -1;
			}

			BuildPortrayalCatalogue(layer);
		}
	}

	if (!layer ||
		!layer->m_spatialObject)
	{
		if (layer)
		{
			delete layer;
		}

		return -1;
	}

	auto tempTypePtr = dynamic_cast<S100ExchangeCatalogue*>(layer->m_spatialObject);
	if (tempTypePtr != nullptr)
	{
		CString firstlayerfilepath = tempTypePtr->GetFirstLayerFilePath();
		AddLayer(layer, firstlayerfilepath, false);
	}
	else
	{
		AddLayer(layer);
	}

	return layer->GetID();
}


int LayerManager::AddLayerFront(CString _filepath)
{
	Layer* layer = nullptr;
	size_t fitor = std::wstring::npos;

	S100_FileType fileType = CheckFileType(_filepath);

	if (fileType == S100_FileType::FILE_Shape ||
		fileType == S100_FileType::FILE_ETC)
	{
		layer = new Layer();
		if (layer->Open(_filepath, D2, this) == false)
		{
			delete layer;
			return -1;
		}
	}
	else if (fileType == S100_FileType::FILE_S_100_VECTOR ||
		fileType == S100_FileType::FILE_S_100_GRID_H5)
	{
		auto productNumber = pathToProductNumber(_filepath);
		auto fc = catalogManager->getFC(productNumber);
		auto pc = catalogManager->getPC(productNumber);

		if (fc)
		{
			layer = new S100Layer(fc, pc);
			if ((S100Layer*)layer->Open(_filepath, D2, this) == false)
			{
				delete layer;
				return -1;
			}

			BuildPortrayalCatalogue(layer);
		}
	}

	if (!layer ||
		!layer->m_spatialObject)
	{
		if (layer)
		{
			delete layer;
		}

		return -1;
	}


	AddLayerFront(layer);

	return layer->GetID();
}


void LayerManager::Draw(HDC& hdc, int offset)
{
	DrawBackground(hdc, offset);

	DrawS100Datasets(hdc, offset);

	DrawNonS100Datasets(hdc, offset);

	//gisLib->D2.Begin(hdc, rectView);
	//gisLib->DrawS100Symbol(101, L"NORTHAR1", 30, 50, 0);
	//gisLib->DrawScaleBar();
	//gisLib->D2.End();
}

void LayerManager::DrawInformationLayer(HDC& hDC, Layer* layer)
{
	if (nullptr == layer)
	{
		return;
	}

	if (false == MBR::CheckOverlap(scaler->GetMap(), layer->m_mbr))
	{
		return;
	}

	Gdiplus::Graphics graphics(hDC);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	Color internalColor(50, 255, 0, 0);
	Color lineColor(255, 255, 0, 0);

	SolidBrush internalBrush(internalColor);
	Pen linePen(lineColor);

	auto mbr = layer->GetMBR();
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
	layername.Format(_T("LayerName : %s \n"), layer->GetLayerName());
	CString strLayer;
	strLayer.Format(_T("LayerType : %s \n"), layer->GetLayerType());
	layername += strLayer;

	sf.SetAlignment(StringAlignmentNear);
	sf.SetLineAlignment(StringAlignmentNear);

	graphics.DrawRectangle(&linePen, position.X, position.Y, position.Width, position.Height);
	graphics.FillRectangle(&internalBrush, position.X, position.Y, position.Width, position.Height);
	graphics.DrawString(layername, -1, &F, position, &sf, &B);
}

void LayerManager::DrawInformationLayer(HDC& hDC, int nindex)
{
	auto selectedLayer = GetLayer(nindex);

	if (nullptr == selectedLayer)
	{
		return;
	}

	DrawInformationLayer(hDC, selectedLayer);
}

void LayerManager::DrawInformationLayerByKey(HDC& hDC, int key)
{
	auto selectedLayer = GetLayerByKey(key);

	if (nullptr == selectedLayer)
	{
		return;
	}

	DrawInformationLayer(hDC, selectedLayer);
}

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

	D2->pRT->SetTransform(scaler->GetMatrix());

	// Area
	for (auto i = area[drawingPrioriy].begin(); i != area[drawingPrioriy].end(); i++)
	{
		auto instruction = *i;

		D2->pBrush->SetOpacity(1.0f);
		instruction->DrawInstruction(
			D2,
			scaler,
			pc);
	}

	// Line
	for (auto i = line[drawingPrioriy].begin(); i != line[drawingPrioriy].end(); i++)
	{
		auto instruction = *i;
		instruction->DrawInstruction(
			D2,
			scaler,
			pc);
	}

	D2->pRT->SetTransform(D2D1::Matrix3x2F::Identity());

	// AugmentedRay
	for (auto i = augmentedRay[drawingPrioriy].begin(); i != augmentedRay[drawingPrioriy].end(); i++)
	{
		auto instruction = *i;
		instruction->DrawInstruction(D2, scaler, pc);
	}

	// AugmentedPath
	for (auto i = augmentedPath[drawingPrioriy].begin(); i != augmentedPath[drawingPrioriy].end(); i++)
	{
		auto instruction = *i;
		instruction->DrawInstruction(D2, scaler, pc);
	}

	// Point
	for (auto i = point[drawingPrioriy].begin(); i != point[drawingPrioriy].end(); i++)
	{
		auto instruction = (SENC_PointInstruction*)*i;

		points.clear();

		//if (ENCCommon::AREA_SYMBOL_DYNAMIC_POSITION_MODE)
		//{
		//	instruction->GetDrawPointsDynamic(scaler, points);
		//}
		//else
		//{
		//	instruction->GetDrawPoints(scaler, points);
		//}
		instruction->GetDrawPoints(scaler, points);

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
				rotation = (float)(instruction->symbol)->rotation;
			}

			float instructionScale = (instruction->symbol)->scaleFactor;

			if (instruction && instruction->symbol)
			{
				auto s100PCManager = pc->GetS100PCManager();
				if (s100PCManager)
				{
					auto pRenderTarget = D2->RenderTarget();

					D2D1::Matrix3x2F oldTransform;
					pRenderTarget->GetTransform(&oldTransform);
						
					s100PCManager->Draw(
						instruction->symbol->reference,
						pRenderTarget,
						D2->SolidColorBrush(),
						D2->SolidStrokeStyle(),
						D2D1::Point2F(pi->x, pi->y),
						rotation,
						5);

					pRenderTarget->SetTransform(oldTransform);
				}
			}
		}
	}

	// Text
	if (ENCCommon::TEXTOUT)
	{
		D2->pBrush->SetOpacity(1.0f);
		D2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
		if (text[drawingPrioriy].size() > 0)
		{
			HWND hWnd = ::GetActiveWindow();
			HDC hdc = ::GetDC(hWnd);

			int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);    // Monitor x axis dpi.
			int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);    // Monitor y axis dpi.
			::ReleaseDC(hWnd, hdc);
			D2D1_SIZE_F renderTargetSize = D2->pRT->GetSize();

			for (auto i = text[drawingPrioriy].begin(); i != text[drawingPrioriy].end(); i++)
			{
				auto instruction = (SENC_TextInstruction*)*i;
				points.clear();

				//if (ENCCommon::AREA_SYMBOL_DYNAMIC_POSITION_MODE)
				//{
				//	instruction->GetDrawPointsDynamic(scaler, points);
				//}
				//else
				//{
				//	instruction->GetDrawPoints(scaler, points);
				//}
				instruction->GetDrawPoints(scaler, points);

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
					{
						radian = -angle / 180. * M_PI;
					}

					int bodySize = (int)(element->bodySize * (float)1.358);

					IDWriteTextFormat* useWTF = nullptr;
					
					if (element->font.isUpright())
					{
						useWTF = D2->getWriteTextFormat(bodySize);
					}
					else
					{
						useWTF = D2->getSlantWriteTextFormat(bodySize);
					}

					useWTF->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

					if (!instruction->textMatrix)
					{
						IDWriteTextLayout* textLayout = nullptr;
						float caretX = renderTargetSize.width;
						float caretY = renderTargetSize.height;

						const wchar_t* vText = element->text.value.c_str();
						int vTextSize = (int)element->text.value.size();
						D2->pDWriteFactory->CreateTextLayout(
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

					//// INCH to mm
	/*				float offsetUnitX = (dpiX / (float)25.4);
					float offsetUnitY = (dpiY / (float)25.4);*/

					// Determine the size of the Offset
					//float XOFFS = ((float)textPoint->offset.x * offsetUnitX);
					//float YOFFS = -((float)textPoint->offset.y * offsetUnitY);
					float XOFFS = (float)(((float)textPoint->offset.x / 0.32) * 1.358);
					float YOFFS = (float)(-((float)textPoint->offset.y / 0.32) * 1.358);

					// HJUST 
					// CENTRE
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
						offset_y = -height;
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

					offset_x += XOFFS;
					offset_y += YOFFS;

					if (element->pColor)
					{
						D2->pBrush->SetColor(element->pColor);
						D2->pBrush->SetOpacity(1 - element->foreground.transparency);
					}

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

						D2->pRT->SetTransform(D2D1::Matrix3x2F::Identity());
						D2->pRT->DrawText(
							vText,
							vTextSize,
							useWTF,
							D2D1::RectF(x, y, x + width, y + height),
							D2->pBrush
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
		if (backgroundLayer.IsOn())
		{
			if (MBR::CheckOverlap(scaler->GetMap(), backgroundLayer.m_mbr))
			{
				backgroundLayer.Draw(hDC, scaler, offset - 360);
				backgroundLayer.Draw(hDC, scaler, offset);
				backgroundLayer.Draw(hDC, scaler, offset + 360);
			}
		}
	}
}

void LayerManager::DrawS100Datasets(HDC& hdc, int offset)
{
	if (onIC == false)
	{
		for (auto i = layers.begin(); i != layers.end(); i++)
		{
			auto layer = (*i);

			if (layer->IsOn())
			{
				if (layer->IsS100Layer())
				{
					if (layer->GetFileType() == S100_FileType::FILE_S_100_VECTOR)
					{
						DrawS100Layer(hdc, offset, (S100Layer*)layer);
					}
					else if (layer->GetFileType() == S100_FileType::FILE_S_100_GRID_H5)
					{
						DrawInformationLayer(hdc, layer);
					}
				}
			}
		}
	}
	else
	{
		for (auto i = layers.begin(); i != layers.end(); i++)
		{
			auto layer = (*i);

			if (layer->IsOn())
			{
				if (layer->IsS100Layer())
				{
					if (layer->GetFileType() == S100_FileType::FILE_S_100_VECTOR)
					{
						SetDrawingInstruction((S100Layer*)layer);
					}
				}
			}
		}

		for (auto i = layers.begin(); i != layers.end(); i++)
		{
			auto layer = (*i);

			if (layer->IsOn())
			{
				if (layer->IsS100Layer())
				{
					if (layer->GetFileType() == S100_FileType::FILE_S_100_VECTOR)
					{
						DrawS100Layer(hdc, offset, (S100Layer*)layer, 0, 3);
					}
				}
			}
		}

		for (auto i = layers.begin(); i != layers.end(); i++)
		{
			auto layer = (*i);

			if (layer->IsOn())
			{
				if (layer->IsS100Layer())
				{
					if (layer->GetFileType() == S100_FileType::FILE_S_100_GRID_H5)
					{
						layer->Draw(hdc, scaler);
					}
				}
			}
		}

		for (auto i = layers.begin(); i != layers.end(); i++)
		{
			auto layer = (*i);

			if (layer->IsOn())
			{
				if (layer->IsS100Layer())
				{
					if (layer->GetFileType() == S100_FileType::FILE_S_100_VECTOR)
					{
						DrawS100Layer(hdc, offset, (S100Layer*)layer, 4, 100);
					}
				}
			}
		}

		for (auto i = layers.begin(); i != layers.end(); i++)
		{
			auto layer = (*i);

			if (layer->IsOn())
			{
				if (layer->IsS100Layer())
				{
					if (layer->GetFileType() == S100_FileType::FILE_S_100_VECTOR)
					{
						auto s100Layer = (S100Layer*)layer;
						s100Layer->InitDraw();
					}
				}
			}
		}
	}
}

void LayerManager::DrawNonS100Datasets(HDC& hDC, int offset)
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = (*i);

		if (!layer->IsS100Layer())
		{
			layer->Draw(hDC, scaler, offset);
		}
	}
}

void LayerManager::DrawS100Layer(HDC& hDC, int offset, S100Layer* layer)
{
	auto fc = layer->GetFeatureCatalog();
	auto pc = layer->GetPC();
	layer->InitDraw();

	auto cell = (S101Cell*)layer->GetSpatialObject();
	if (nullptr == cell->pcManager ||
		nullptr == cell->pcManager->displayListSENC)
	{
		return;
	}

	if (false == MBR::CheckOverlap(scaler->GetMap(), layer->m_mbr))
	{
		return;
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
		cell->pcManager->displayListSENC->GetDrawingInstruction(i, 7, GetS100Scale(), scaler, itList);
		if (itList.size() > 0)
		{
			auto instructionList = layer->drawingSet.GetAugmentedRayList(i);
			for (const auto& iter : itList)
			{
				if (!IsFeatureOn(iter->code))
					continue;
				instructionList->push_back(iter);
				cnt++;
			}
		}

		// Augmented Path
		cell->pcManager->displayListSENC->GetDrawingInstruction(i, 8, GetS100Scale(), scaler, itList);
		if (itList.size() > 0)
		{
			auto instructionList = layer->drawingSet.GetAugmentedPathList(i);
			for (const auto& iter : itList)
			{
				if (!IsFeatureOn(iter->code))
					continue;
				instructionList->push_back(iter);
				cnt++;
			}
		}

		// Point
		cell->pcManager->displayListSENC->GetDrawingInstruction(i, 1, GetS100Scale(), scaler, itList);
		if (itList.size() > 0)
		{
			auto instructionList = layer->drawingSet.GetPointList(i);
			for (const auto& iter : itList)
			{
				if (!IsFeatureOn(iter->code))
					continue;
				instructionList->push_back(iter);
				cnt++;
			}
		}

		// Line
		cell->pcManager->displayListSENC->GetDrawingInstruction(i, 2, GetS100Scale(), scaler, itList);
		if (itList.size() > 0)
		{
			auto instructionList = layer->drawingSet.GetLineList(i);
			for (const auto& iter : itList)
			{
				if (!IsFeatureOn(iter->code))
					continue;
				instructionList->push_back(iter);
				cnt++;
			}
		}

		// Area
		cell->pcManager->displayListSENC->GetDrawingInstruction(i, 3, GetS100Scale(), scaler, itList);
		if (itList.size() > 0)
		{
			auto instructionList = layer->drawingSet.GetAreaList(i);
			for (const auto& iter : itList)
			{
				if (!IsFeatureOn(iter->code))
					continue;
				instructionList->push_back(iter);
				cnt++;
			}
		}

		// Text
		cell->pcManager->displayListSENC->GetDrawingInstruction(i, 5, GetS100Scale(), scaler, itList);
		if (itList.size() > 0)
		{
			auto instructionList = layer->drawingSet.GetTextList(i);
			for (const auto& iter : itList)
			{
				if (!IsFeatureOn(iter->code))
					continue;
				instructionList->push_back(iter);
				cnt++;
			}
		}

		if (cnt)
		{
			layer->drawingPriority.insert(i);
		}
	}

	itList.clear();

	// Line Suppression
	SuppressS101Lines(layer->drawingPriority, &layer->drawingSet);

	auto rt = D2->pRT;
	rt->BindDC(hDC, scaler->GetScreenRect());
	rt->BeginDraw();
	D2->pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	D2->pDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	pc->GetS100PCManager()->CreateBitmapBrush(D2->pRT);
	pc->GetS100PCManager()->InverseMatrixBitmapBrush(scaler->GetInverseMatrix());

	for (auto dp = layer->drawingPriority.begin(); dp != layer->drawingPriority.end(); dp++)
	{
		AddSymbolDrawing(*dp, hDC, offset,
			layer->drawingSet.GetAugmentedRayList(),
			layer->drawingSet.GetAugmentedPathList(),
			layer->drawingSet.GetPointList(),
			layer->drawingSet.GetLineList(),
			layer->drawingSet.GetAreaList(),
			layer->drawingSet.GetTextList(),
			pc);
	}

	pc->GetS100PCManager()->DeleteBitmapBrush();

	rt->EndDraw();
}

void LayerManager::SetDrawingInstruction(S100Layer* layer)
{
	auto fc = layer->GetFeatureCatalog();
	auto pc = layer->GetPC();

	auto cell = (S100SpatialObject*)layer->GetSpatialObject();
	if (nullptr == cell->pcManager ||
		nullptr == cell->pcManager->displayListSENC)
	{
		return;
	}

	if (false == MBR::CheckOverlap(scaler->GetMap(), layer->m_mbr))
	{
		return;
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
		cell->pcManager->displayListSENC->GetDrawingInstruction(i, 7, GetS100Scale(), scaler, itList);
		if (itList.size() > 0)
		{
			auto instructionList = layer->drawingSet.GetAugmentedRayList(i);
			for (const auto& iter : itList)
			{
				if (!IsFeatureOn(iter->code))
					continue;
				instructionList->push_back(iter);
				cnt++;
			}
		}

		// Augmented Path
		cell->pcManager->displayListSENC->GetDrawingInstruction(i, 8, GetS100Scale(), scaler, itList);
		if (itList.size() > 0)
		{
			auto instructionList = layer->drawingSet.GetAugmentedPathList(i);
			for (const auto& iter : itList)
			{
				if (!IsFeatureOn(iter->code))
					continue;
				instructionList->push_back(iter);
				cnt++;
			}
		}

		// Point
		cell->pcManager->displayListSENC->GetDrawingInstruction(i, 1, GetS100Scale(), scaler, itList);
		if (itList.size() > 0)
		{
			auto instructionList = layer->drawingSet.GetPointList(i);
			for (const auto& iter : itList)
			{
				if (!IsFeatureOn(iter->code))
					continue;
				instructionList->push_back(iter);
				cnt++;
			}
		}

		// Line
		cell->pcManager->displayListSENC->GetDrawingInstruction(i, 2, GetS100Scale(), scaler, itList);
		if (itList.size() > 0)
		{
			auto instructionList = layer->drawingSet.GetLineList(i);
			for (const auto& iter : itList)
			{
				if (!IsFeatureOn(iter->code))
					continue;
				instructionList->push_back(iter);
				cnt++;
			}
		}

		// Area
		cell->pcManager->displayListSENC->GetDrawingInstruction(i, 3, GetS100Scale(), scaler, itList);
		if (itList.size() > 0)
		{
			auto instructionList = layer->drawingSet.GetAreaList(i);
			for (const auto& iter : itList)
			{
				if (!IsFeatureOn(iter->code))
					continue;
				instructionList->push_back(iter);
				cnt++;
			}
		}

		// Text
		cell->pcManager->displayListSENC->GetDrawingInstruction(i, 5, GetS100Scale(), scaler, itList);
		if (itList.size() > 0)
		{
			auto instructionList = layer->drawingSet.GetTextList(i);
			for (const auto& iter : itList)
			{
				if (!IsFeatureOn(iter->code))
					continue;
				instructionList->push_back(iter);
				cnt++;
			}
		}

		if (cnt)
		{
			layer->drawingPriority.insert(i);
		}
	}

	itList.clear();

	// Line Suppression
	if (101 == layer->GetProductNumber()) {
		SuppressS101Lines(layer->drawingPriority, &layer->drawingSet);
	}
}

void LayerManager::DrawS100Layer(HDC& hDC, int offset, S100Layer* layer, int minPriority, int maxPriority)
{
	if (false == MBR::CheckOverlap(scaler->GetMap(), layer->m_mbr)) {
		return;
	}

	auto pc = layer->GetPC();
	if (nullptr == pc)
	{
		return;
	}

	auto rt = D2->pRT;
	rt->BindDC(hDC, scaler->GetScreenRect());
	rt->BeginDraw();
	D2->pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	D2->pDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	pc->GetS100PCManager()->CreateBitmapBrush(D2->pRT);
	pc->GetS100PCManager()->InverseMatrixBitmapBrush(scaler->GetInverseMatrix());

	for (auto dp = layer->drawingPriority.begin(); dp != layer->drawingPriority.end(); dp++)
	{
		if (*dp >= minPriority && *dp <= maxPriority)
		{
			AddSymbolDrawing(*dp, hDC, offset,
				layer->drawingSet.GetAugmentedRayList(),
				layer->drawingSet.GetAugmentedPathList(),
				layer->drawingSet.GetPointList(),
				layer->drawingSet.GetLineList(),
				layer->drawingSet.GetAreaList(),
				layer->drawingSet.GetTextList(),
				pc);
		}
	}

	pc->GetS100PCManager()->DeleteBitmapBrush();

	rt->EndDraw();
}

void LayerManager::S101RebuildPortrayal()
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = (*i);

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
	auto pc = ((S100Layer*)l)->GetPC();

	if (pc &&
		l->GetFileType() == S100_FileType::FILE_S_100_VECTOR) 
	{
		auto mainRuleFile = pc->GetMainRuleFile();
		auto fileName = mainRuleFile->GetFileName();
		auto rootPath = pc->GetRootPath();
		auto mainRulePath = rootPath + L"Rules\\" + fileName;

		if (pc->GetRuleFileFormat() == Portrayal::FileFormat::LUA) 
		{
			ProcessS101::ProcessS101_LUA(mainRulePath, (S100Layer*)l);
		}
		else if (pc->GetRuleFileFormat() == Portrayal::FileFormat::XSLT) 
		{
			auto gml = (S10XGML*)l->GetSpatialObject();
			gml->SaveToInputXML("..\\TEMP\\input.xml");
			ProcessS101::ProcessS100_XSLT("..\\TEMP\\input.xml", pugi::as_utf8(mainRulePath), "..\\TEMP\\output.xml");
			auto s100so = (S100SpatialObject*)l->GetSpatialObject();
			s100so->OpenOutputXML("..\\TEMP\\output.xml");
		}
	}
}

Layer* LayerManager::GetLayer(int index)
{
	if (index < 0 || index >= LayerCount())
	{
		return nullptr;
	}

	auto it = layers.begin();

	std::advance(it, index);

	return *it;
}

Layer* LayerManager::GetLayerByKey(int key)
{
	auto item = mapLayer.find(key);
	if (item == mapLayer.end())
	{
		return nullptr;
	}

	return item->second;
}

CString LayerManager::GetLayerName(int index)
{
	auto layer = GetLayer(index);

	if (layer)
	{
		layer->GetLayerName();
	}

	return L"";
}

CString LayerManager::GetLayerNameByKey(int key)
{
	auto layer = GetLayerByKey(key);

	if (layer)
	{
		layer->GetLayerName();
	}

	return L"";
}

bool LayerManager::IsOn(int index)
{
	auto layer = GetLayer(index);

	if (layer)
	{
		return layer->IsOn();
	}

	return false;
}

bool LayerManager::IsOnByKey(int key)
{
	auto layer = GetLayerByKey(key);

	if (layer)
	{
		return layer->IsOn();
	}

	return nullptr;
}

void LayerManager::DeleteLayer(int index)
{
	if (index < 0 || index >= LayerCount())
	{
		return;
	}

	auto it = layers.begin();

	std::advance(it, index);

	auto id = (*it)->GetID();

	delete (*it);

	layers.erase(it);
	mapLayer.erase(id);

	ReMBR();

	return;
}

void LayerManager::FindPathDeleteLayer(CString FindLayerPath)
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = *i;
		if(std::string::npos != layer->GetLayerPath().Find(FindLayerPath))
		{
			mapLayer.erase(layer->GetID());
			delete layer;
			layer = nullptr;
			layers.erase(i);
			ReMBR();
			return;
		}
	}
}

void LayerManager::DeleteLayer(CString filepath)
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = *i;
		if (layer->GetLayerPath().Compare(filepath) == 0)
		{
			mapLayer.erase(layer->GetID());
			delete layer;
			layer = nullptr;
			layers.erase(i);
			ReMBR();
			return;
		}
	}
}

void LayerManager::DeleteLayerByKey(int key)
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = (*i);
		if (layer->GetID() == key)
		{
			mapLayer.erase(layer->GetID());
			delete layer;
			layer = nullptr;
			layers.erase(i);
			ReMBR();
			return;
		}
	}
}

void LayerManager::ReMBR()
{
	if (LayerCount() == 0)
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
		mbr.InitMBR();

		for (auto i = layers.begin(); i != layers.end(); i++)
		{
			mbr.CalcMBR((*i)->GetMBR());
		}
	}
}

void LayerManager::SetViewMBR(RECT r)
{
	scaler->SetMap(r);
}

void LayerManager::ChangeS100ColorPalette(GeoMetryLibrary::ColorTable value)
{
	if (value == GeoMetryLibrary::ColorTable::Day)
	{
		ChangeS100ColorPalette(L"Day");
	}
	else if (value == GeoMetryLibrary::ColorTable::Dusk)
	{
		ChangeS100ColorPalette(L"Dusk");
	}
	else if (value == GeoMetryLibrary::ColorTable::Night)
	{
		ChangeS100ColorPalette(L"Night");
	}
}

void LayerManager::ChangeS100ColorPalette(std::wstring paletteName)
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = *i;
		if (layer && true == layer->IsS100Layer())
		{
			auto s100layer = (S100Layer*)layer;
			auto pc = s100layer->GetPC();
			if (nullptr != pc)
			{
				pc->SetCurrentPaletteName(paletteName);
				pc->DeletePatternImage();
				pc->CreatePatternImages(D2->pD2Factory, D2->pImagingFactory, D2->D2D1StrokeStyleGroup.at(0));
				pc->DeleteLineImages();
				pc->CreateLineImages(D2->pD2Factory, D2->pImagingFactory, D2->D2D1StrokeStyleGroup.at(0));
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

	catalogManager->ChangeColorPallete(paletteName, D2->pD2Factory, D2->pImagingFactory, D2->D2D1StrokeStyleGroup.at(0));
}

Scaler* LayerManager::GetScaler()
{
	return scaler;
}
/*
D2D1Resources* LayerManager::GetD2D1Resources()
{
	return D2;
}
*/
void LayerManager::SuppressS101Lines(std::set<int>& drawingPriority, DrawingSet* drawingSet)
{
	lineSuppressionMap.clear();

	// Drawing priority : High -> Low
	for (auto i = drawingPriority.rbegin(); i != drawingPriority.rend(); i++)
	{
		int drawingPriority = *i;

		auto lineInstructions = drawingSet->GetLineList()[drawingPriority];

		for (auto j = lineInstructions.begin(); j != lineInstructions.end(); j++)
		{
			auto lineInstruction = (SENC_LineInstruction*)*j;

			auto featureRecord = lineInstruction->fr;
			auto geom = featureRecord->GetGeometry();

			std::list<SCurve*> curListCurveLink;

			if (lineInstruction->spatialReference.size() > 0)
			{
				if (geom->GetType() == SGeometryType::Surface)
				{
					auto surface = (SSurface*)geom;

					for (auto iterLi = lineInstruction->spatialReference.begin(); iterLi != lineInstruction->spatialReference.end(); iterLi++)
					{
						SENC_SpatialReference* sred = *iterLi;
						int referencedID = sred->reference;

						auto curve = surface->GetCurve(referencedID);
						if (curve) {
							curListCurveLink.push_back(curve);
						}
					}
				}
				else if (geom->GetType() == SGeometryType::CompositeCurve)
				{
					auto compositeCurve = (SCompositeCurve*)geom;

					for (auto iterLi = lineInstruction->spatialReference.begin(); iterLi != lineInstruction->spatialReference.end(); iterLi++)
					{
						SENC_SpatialReference* sred = *iterLi;
						int referencedID = sred->reference;

						auto curve = compositeCurve->GetCurveByRCID(referencedID);
						if (curve) {
							curListCurveLink.push_back(curve);
						}
					}
				}
				else if (geom->GetType() == SGeometryType::Curve)
				{
					auto curveHasOrient = (SCurve*)geom;

					for (auto iterLi = lineInstruction->spatialReference.begin(); iterLi != lineInstruction->spatialReference.end(); iterLi++)
					{
						SENC_SpatialReference* sred = *iterLi;
						int referencedID = sred->reference;

						if (curveHasOrient->GetRCID() == referencedID)
						{
							curListCurveLink.push_back(curveHasOrient);
						}
					}
				}
			}
			else if (geom->GetType() == SGeometryType::Surface)
			{
				auto surface = (SSurface*)geom;
				surface->GetCurveList(curListCurveLink);
			}
			else if (geom->GetType() == SGeometryType::CompositeCurve)
			{
				auto compositeCurve = (SCompositeCurve*)geom;
				compositeCurve->GetCurveList(curListCurveLink);
			}
			else if (geom->GetType() == SGeometryType::Curve)
			{
				curListCurveLink.push_back(((SCurve*)geom));
			}

			for (auto m = curListCurveLink.begin(); m != curListCurveLink.end(); m++)
			{
				auto curve = *m;
				if (false == curve->GetMasking())
				{
					int id = curve->GetRCID();

					auto item = lineSuppressionMap.find(id);

					if (item == lineSuppressionMap.end())
					{
						lineSuppressionMap.insert(id);
					}
					else if (item != lineSuppressionMap.end())
					{
						curve->SetSuppress(true);
					}
				}
			}
		}
	}
}

int LayerManager::LayerCount()
{
	return (int)layers.size();
}

S100_FileType LayerManager::CheckFileType(CString path, int update)
{
	CFile file;
	S100_FileType ret = S100_FileType::FILE_NONE;

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
					ret = S100_FileType::FILE_S_57;
				}
				else if (tag == *((unsigned int*)"DSID"))
				{
					ret = S100_FileType::FILE_S_100_VECTOR;
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
				ret = S100_FileType::FILE_S_100_VECTOR;
			}
		}
	}
	else if (file_extension.CompareNoCase(_T("h5")) == 0)
	{
		ret = S100_FileType::FILE_S_100_GRID_H5;
	}

	return ret;
}

S100_FileType LayerManager::CheckFileType(CString path)
{
	CFile file;
	S100_FileType ret = S100_FileType::FILE_NONE;

	CString file_extension = LibMFCUtil::GetExtension(path);


	if ((file_extension.CompareNoCase(_T("000")) >= 0) &&
		(file_extension.CompareNoCase(_T("999")) <= 0))
	{
		if (file.Open(path, CFile::modeRead))
		{
			if (file.GetLength() > 1024)
			{
				BYTE* lpBuf = nullptr;
				BYTE* sBuf = nullptr;
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
					delete sBuf;
					return S100_FileType::FILE_S_57;
				}
				else if (tag == *((unsigned int*)"DSID"))
				{
					delete sBuf;
					return S100_FileType::FILE_S_100_VECTOR;
				}
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
				return S100_FileType::FILE_S_100_VECTOR;
			}
			else
			{
				return S100_FileType::FILE_ETC;
			}
		}
	}
	else if (file_extension.CompareNoCase(_T("h5")) == 0)
	{
		return S100_FileType::FILE_S_100_GRID_H5;
	}
	else if (file_extension.CompareNoCase(L"SHP") == 0)
	{
		return S100_FileType::FILE_Shape;
	}

	return ret;
}

int LayerManager::CreateLayerID()
{
	std::set<int> IDs;

	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = (*i);
		auto id = layer->GetID();
		IDs.insert(id);
	}


	if (IDs.size() > 0)
	{
		for (int i = 1; i <= INT_MAX && i >= 0; i++)
		{
			auto item = IDs.find(i);
			if (item == IDs.end())
			{
				return i;
			}
		}
	}

	return 1;
}

int LayerManager::pathToProductNumber(CString path)
{
	auto name = LibMFCUtil::GetFileName(path);
	if (name.GetLength() >= 3)
	{
		std::wstring strProductNumber = name.Left(3);

		try {
			return std::stoi(strProductNumber);
		}
		catch (const std::exception& e) {
		}
	}

	return 0;
}

void LayerManager::SetS100Scale(double value)
{
	s100Scale = value;
}

int LayerManager::GetS100Scale()
{
	if (s100Scale > 0)
	{
		return (int)s100Scale;
	}

	auto currentScale = scaler->GetCurrentScale();

	return currentScale;
}

void LayerManager::InitFeatureOnOffMap()
{
	auto fc = catalogManager->getFC("S-101");
	if (fc)
	{
		featureOnOffMap.clear();

		auto vector = fc->GetFeatureTypes()->GetVecFeatureType();
		for (auto i = vector.begin(); i != vector.end(); i++)
		{
			featureOnOffMap.insert({ (*i)->GetCodeAsWString(), true });
		}
	}
}

void LayerManager::SetFeatureOnOff(std::wstring code, bool on)
{
	auto itor = featureOnOffMap.find(code);
	if (itor != featureOnOffMap.end())
	{
		itor->second = on;
	}
	else
	{
		featureOnOffMap.insert({ code, on });
	}
}

bool LayerManager::IsFeatureOn(std::wstring& featureTypeCode)
{
	auto item = featureOnOffMap.find(featureTypeCode);
	if (item != featureOnOffMap.end())
	{
		return item->second;
	}

	return true;
}

