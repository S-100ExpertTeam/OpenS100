#include "stdafx.h"
#include "S100PCManager.h"
#include "SVGReader.h"
#include "S100PCLineStylesReader.h"
#include "LineStylesPackage.h"

#include "../GeoMetryLibrary/ENCCommon.h"


S100PCManager::S100PCManager()
{

}

S100PCManager::~S100PCManager()
{

}

bool S100PCManager::OpenS100ColorProfile(std::wstring _path)
{
	char* path = ConvertWCtoC((wchar_t*)_path.c_str());
	bool ret = s100ColorProfile.OpenByPugi(path);
	delete[] path;
	return ret;
}


bool S100PCManager::OpenS100LineStyles(std::wstring _path)
{
	S100XMLReader::S100PCLineStylesReader::OpenByPugi(_path, &lineStyles);
	return true;
}


bool S100PCManager::OpenS100AreaFills(std::wstring _path)
{
	CFileFind  finder; 
	BOOL bWorking = finder.FindFile(_path.c_str());
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		std::wstring path(finder.GetFilePath());
		if (path.find(L".xml") != -1)
		{
			AreaFill areaFill;
			areaFill.ReadByPugi(path);

			auto item = GetAreaFill(areaFill._name);
			if (nullptr == item)
			{
				areaFills.push_back(areaFill);
			}
			else
			{
				OutputDebugString(_T("Already exist in areaFills\n"));
			}
		}
	}

	return true;
}

bool S100PCManager::OpenS100Symbol(std::wstring _path)
{
	return s100SymbolManager.Open(_path);
}

void S100PCManager::DrawLineStyle(std::wstring _name, ID2D1RenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, ID2D1StrokeStyle1* pStrokeStyle, std::wstring paletteName)
{
	auto i = lineStyles.mapLineStyle.find(_name.c_str());

	if (i != lineStyles.mapLineStyle.end())
	{
		int cntLineStyles = i->second->GetLineStyleCount();

		if (cntLineStyles > 1)
		{
			for (int j = 0; j < cntLineStyles; j++)
			{
				LineStylesPackage::LineStyle *pLineStyle = i->second->GetLineStyle(j);

				D2D1_POINT_2F p1, p2;
				p1.x = 0;
				p2.x = 100;
				p1.y = (FLOAT)(pLineStyle->offset / 0.32);
				p2.y = (FLOAT)(pLineStyle->offset / 0.32);
				FLOAT width = 1;
				D2D1_COLOR_F color;
				if (pLineStyle->pen)
				{
					width = (FLOAT)(pLineStyle->pen->width / 0.32);
					color = s100ColorProfile.GetColor(pLineStyle->pen->color.GetToken());
					pBrush->SetColor(color);
					pBrush->SetOpacity(1);
				}
				pRenderTarget->DrawLine(p1, p2, pBrush, width, pStrokeStyle);
			}
		}
		else if (cntLineStyles == 1)
		{
			LineStylesPackage::LineStyle *pLineStyle = i->second->GetLineStyle(0);

			if (pLineStyle)
			{
				pBrush->SetColor(s100ColorProfile.GetColor(pLineStyle->pen->color.GetToken()));
				D2D1_RECT_F rect = GetLineStyleRect(_name.c_str(), 2);

				for (auto j = pLineStyle->dash.begin(); j != pLineStyle->dash.end(); j++)
				{
					D2D1_POINT_2F p1, p2;

					p1.x = (FLOAT)((*j)->start / 0.32)*2;
					p2.x = (FLOAT)((*j)->length / 0.32)*2 + p1.x;

					p1.y = (rect.bottom - rect.top) / 2;
					p2.y = (rect.bottom - rect.top) / 2;

					pRenderTarget->DrawLine(p1, p2, pBrush, 1, pStrokeStyle);
				}

				for (auto k = pLineStyle->symbol.begin(); k != pLineStyle->symbol.end(); k++)
				{
					D2D1_POINT_2F pSymbol;
					pSymbol.x = (FLOAT)((*k)->position / 0.32)*2;
					pSymbol.y = (rect.bottom - rect.top) / 2;

					Draw((*k)->reference, pRenderTarget, pBrush, pStrokeStyle, pSymbol, 0, ENCCommon::DISPLAY_SYMBOL_SCALE, paletteName);
				}
			}
		}
		else
		{
			OutputDebugString(_T("invalied linestyle count\n"));
		}
	}
}

void S100PCManager::DrawAreaFill(std::wstring _symbolName, ID2D1RenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, ID2D1StrokeStyle1* pStrokeStyle, std::wstring paletteName)
{
	AreaFill* pAreaFill = GetAreaFill(_symbolName);

	if (!pAreaFill)
	{
		return;
	}

	SVGReader* pSVG = s100SymbolManager.GetSVG(pAreaFill->_symbolReference);

	if (!pSVG) return;

	float width = pAreaFill->v1_x;
	float height = pAreaFill->v2_y;

	if (pAreaFill->v1_y < 0.0001 && pAreaFill->v2_x < 0.0001)
	{
		D2D1_POINT_2F point1 = {
			0,
			0
		};

		D2D1_POINT_2F point2 = {
			pAreaFill->v1_x,
			0
		};

		D2D1_POINT_2F point3 = {
			pAreaFill->v1_x,
			pAreaFill->v2_y
		};

		D2D1_POINT_2F point4 = {
			0,
			pAreaFill->v2_y
		};

		point1.x *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point1.y *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point2.x *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point2.y *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point3.x *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point3.y *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point4.x *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point4.y *= ENCCommon::DISPLAY_SYMBOL_SCALE;

		Draw(pAreaFill->_symbolReference, pRenderTarget, pBrush, pStrokeStyle, point1, 0, ENCCommon::DISPLAY_SYMBOL_SCALE, paletteName);
		Draw(pAreaFill->_symbolReference, pRenderTarget, pBrush, pStrokeStyle, point2, 0, ENCCommon::DISPLAY_SYMBOL_SCALE, paletteName);
		Draw(pAreaFill->_symbolReference, pRenderTarget, pBrush, pStrokeStyle, point3, 0, ENCCommon::DISPLAY_SYMBOL_SCALE, paletteName);
		Draw(pAreaFill->_symbolReference, pRenderTarget, pBrush, pStrokeStyle, point4, 0, ENCCommon::DISPLAY_SYMBOL_SCALE, paletteName);
	}
	else
	{
		D2D1_POINT_2F point1 = { 
			0,
			0
		};

		D2D1_POINT_2F point2 = { 
			pAreaFill->v1_x,
			0
		};

		D2D1_POINT_2F point3 = {
			pAreaFill->v1_x - width / 2,
			height
		};

		D2D1_POINT_2F point4 = {
			0, 
			height * 2
		};

		D2D1_POINT_2F point5 = { 
			pAreaFill->v1_x,
			height * 2
		};

		point1.x *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point1.y *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point2.x *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point2.y *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point3.x *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point3.y *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point4.x *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point4.y *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point5.x *= ENCCommon::DISPLAY_SYMBOL_SCALE;
		point5.y *= ENCCommon::DISPLAY_SYMBOL_SCALE;

		Draw(pAreaFill->_symbolReference, pRenderTarget, pBrush, pStrokeStyle, point1, 0, ENCCommon::DISPLAY_SYMBOL_SCALE, paletteName);
		Draw(pAreaFill->_symbolReference, pRenderTarget, pBrush, pStrokeStyle, point2, 0, ENCCommon::DISPLAY_SYMBOL_SCALE, paletteName);
		Draw(pAreaFill->_symbolReference, pRenderTarget, pBrush, pStrokeStyle, point3, 0, ENCCommon::DISPLAY_SYMBOL_SCALE, paletteName);
		Draw(pAreaFill->_symbolReference, pRenderTarget, pBrush, pStrokeStyle, point4, 0, ENCCommon::DISPLAY_SYMBOL_SCALE, paletteName);
		Draw(pAreaFill->_symbolReference, pRenderTarget, pBrush, pStrokeStyle, point5, 0, ENCCommon::DISPLAY_SYMBOL_SCALE, paletteName);
	}
}

void S100PCManager::Draw(
	std::wstring& _symbolName,
	ID2D1RenderTarget* pRenderTarget,
	ID2D1SolidColorBrush* pBrush,
	ID2D1StrokeStyle1* pStrokeStyle,
	D2D1_POINT_2F point,
	FLOAT rotation,
	FLOAT scale,
	std::wstring paletteName)
{
	SVGReader* pSVG = s100SymbolManager.GetSVG(_symbolName);

	//inform01
	if (!wcscmp(_symbolName.c_str(),L"INFORM01"))
	{
		if (ENCCommon::Show_INFORM01==false)
		{
			return;
		}
	}

	if (!pSVG) return;

	D2D1_POINT_2F yReversePoint = { point.x, point.y };

	D2D1::Matrix3x2F rot = D2D1::Matrix3x2F::Rotation(rotation);
	D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Translation(point.x, point.y);
	D2D1::Matrix3x2F scale11 = D2D1::Matrix3x2F::Scale(D2D1::SizeF(scale, scale));

	pRenderTarget->SetTransform(scale11 * rot * trans);

	for (auto i = pSVG->geometry.begin(); i != pSVG->geometry.end(); i++)
	{
		if (i->pGeometry)
		{
			if (i->bFill)
			{
				pBrush->SetColor(s100ColorProfile.GetColor(i->fillColorName));
				pBrush->SetOpacity(i->alpha);
				pRenderTarget->FillGeometry(i->pGeometry, pBrush);
			}

			if (i->bStoke)
			{
				pBrush->SetColor(s100ColorProfile.GetColor(i->strokeColorName));
				pBrush->SetOpacity(i->alpha);
				pRenderTarget->DrawGeometry(i->pGeometry, pBrush, i->width * 0.03937f * 96, pStrokeStyle);
			}
		}
		else
		{
			if (i->bFill)
			{
				pBrush->SetColor(s100ColorProfile.GetColor(i->fillColorName));
				pBrush->SetOpacity(i->alpha);
				pRenderTarget->FillEllipse(i->ellipse, pBrush);
			}

			if (i->bStoke)
			{
				pBrush->SetColor(s100ColorProfile.GetColor(i->strokeColorName));
				pBrush->SetOpacity(i->alpha);
				pRenderTarget->DrawEllipse(i->ellipse, pBrush, i->width * 0.03937f * 96, pStrokeStyle);
			}
		}
	}
}

char* S100PCManager::ConvertWCtoC(wchar_t* str)
{
	//Declaration of char* variable to return
	char* pStr = nullptr;

	//Find the length of the input wchar_t variable.
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

	if (strSize <= 0)
	{
		OutputDebugString(_T("Failed to WideCharToMultiByte()\n"));
		return nullptr;
	}

	//Char* Memory allocation.
	pStr = new char[strSize];

	//Change your mind.
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
	return pStr;
}


void S100PCManager::CreateSVGGeometry(ID2D1Factory1* m_pDirect2dFactory)
{
	s100SymbolManager.CreateSVGGeometry(m_pDirect2dFactory);
}


AreaFill* S100PCManager::GetAreaFill(std::wstring _name)
{
	for (auto i = areaFills.begin(); i != areaFills.end(); i++)
	{
		if (!i->_name.compare(_name))
		{
			return &(*i);
		}
	}

	return nullptr;
}


D2D1_RECT_F S100PCManager::GetLineStyleRect(CString _name, FLOAT scale)
{
	D2D1_RECT_F rect = { 0, 0, 100, 100 };

	auto keyValue = lineStyles.mapLineStyle.find(std::wstring(_name));
	if (keyValue != lineStyles.mapLineStyle.end())
	{
		if (keyValue->second->subClassType == LineStylesPackage::eLineStyle)
		{
			LineStylesPackage::LineStyle *pLineStyle = (LineStylesPackage::LineStyle *)keyValue->second;
			rect.left = 0;
			rect.right = (FLOAT)(pLineStyle->intervalLength / 0.32 * scale);

			for (auto i = pLineStyle->symbol.begin(); i != pLineStyle->symbol.end(); i++)
			{
				SVGReader *pSVGSymbol = s100SymbolManager.GetSVG((*i)->reference);
				if (pSVGSymbol)
				{
					if (i == pLineStyle->symbol.begin())
					{
						rect.top = (pSVGSymbol->bounding_y / 0.32f - 10);
						rect.bottom = (pSVGSymbol->bounding_height / 0.32f + rect.top + 10);
					}
					else
					{
						if (rect.top > pSVGSymbol->bounding_y / 0.32)
						{
							rect.top = pSVGSymbol->bounding_y / 0.32f - 10;
						}

						if (rect.bottom < (pSVGSymbol->bounding_height / 0.32 + pSVGSymbol->bounding_y / 0.32))
						{
							rect.bottom = pSVGSymbol->bounding_height / 0.32f + pSVGSymbol->bounding_y / 0.32f + 10;
						}
					}

					rect.top *= scale;
					rect.bottom *= scale;
				}
				else
				{
					OutputDebugString(_T("Null symbol\n"));
				}
			}

			if (pLineStyle->symbol.size() == 0)
			{
				rect.top = 0;
				rect.bottom = 100;
			}
		}
		else if (keyValue->second->subClassType == LineStylesPackage::eLineStyleReference)
		{
			// TDB
		}
		else if (keyValue->second->subClassType == LineStylesPackage::eCompositeLineStyle)
		{
			rect.left = 0;
			rect.top = 0;
			rect.right = 100;
			rect.bottom = 100;
		}
		else
		{
			OutputDebugString(_T("Invalied line style\n"));
		}
	}

	return rect;
}


void S100PCManager::CreateLineImage(ID2D1Factory1* pDirect2dFactory, IWICImagingFactory* pImagingFactory, ID2D1StrokeStyle1* pStrokeStyleS101Solid, std::wstring paletteName)
{
	for (
		auto i = lineStyles.mapLineStyle.begin();
		i != lineStyles.mapLineStyle.end();
		i++
		)
	{
		IWICBitmap *pBitmap = nullptr;
		D2D1_RECT_F lineStyleRect = GetLineStyleRect(i->second->name.c_str(), 2);
		HRESULT hr = pImagingFactory->CreateBitmap((UINT)(lineStyleRect.right - lineStyleRect.left), (UINT)(lineStyleRect.bottom - lineStyleRect.top), GUID_WICPixelFormat32bppPRGBA, WICBitmapCacheOnDemand, &pBitmap);

		if (!SUCCEEDED(hr))
		{
			OutputDebugString(_T("FAILED to create bitmap\n"));
		}

		ID2D1RenderTarget* pCurrentRenderTarget = nullptr;
		hr = pDirect2dFactory->CreateWicBitmapRenderTarget(pBitmap, D2D1::RenderTargetProperties(), &pCurrentRenderTarget);

		if (SUCCEEDED(hr))
		{
			ID2D1SolidColorBrush* pCurrentBrush = nullptr;
			pCurrentRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Crimson),
				&pCurrentBrush
			);

			pCurrentRenderTarget->BeginDraw();
			DrawLineStyle(i->second->name, pCurrentRenderTarget, pCurrentBrush, pStrokeStyleS101Solid, paletteName);
			pCurrentRenderTarget->EndDraw();
		}

		lineImageMap.SetAt(i->second->name.c_str(), pBitmap);
	}
}


void S100PCManager::CreateBitmapBrush(ID2D1RenderTarget* pRenderTarget)
{
	for (auto i = areaFillInfo.patternMap.begin(); i != areaFillInfo.patternMap.end(); i++)
	{
		ID2D1Bitmap* pCurrentBitmap = nullptr;
		HRESULT hr = pRenderTarget->CreateBitmapFromWicBitmap(i->second->pIWICBitmap, nullptr, &pCurrentBitmap);

		if (SUCCEEDED(hr))
		{
			ID2D1BitmapBrush* pCurrentBitmapBrush = nullptr;
			hr = pRenderTarget->CreateBitmapBrush(pCurrentBitmap, &pCurrentBitmapBrush);

			if (SUCCEEDED(hr))
			{
				pCurrentBitmapBrush->SetExtendModeX(D2D1_EXTEND_MODE_WRAP);
				pCurrentBitmapBrush->SetExtendModeY(D2D1_EXTEND_MODE_WRAP);
				pCurrentBitmapBrush->SetInterpolationMode(D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
				i->second->pBitmapBrush = pCurrentBitmapBrush;
			}

			SafeRelease(&pCurrentBitmap);
		}
	}
}


void S100PCManager::CreateBitmapImage(ID2D1Factory1* pDirect2dFactory, IWICImagingFactory* pImagingFactory, ID2D1StrokeStyle1* pStrokeStyleS101Solid, std::wstring paletteName)
{
	for (auto i = areaFills.begin(); i != areaFills.end(); i++)
	{
		AreaFill* pAreaFill = GetAreaFill(i->_name);
		if (pAreaFill)
		{
			D2D1_RECT_F rect = GetAreaFillRect(pAreaFill, ENCCommon::DISPLAY_SYMBOL_SCALE);

			AreaPatternBitmap* areaPatternBitmap = new AreaPatternBitmap();
			HRESULT hr = pImagingFactory->CreateBitmap((UINT)(rect.right - rect.left), (UINT)(rect.bottom - rect.top), GUID_WICPixelFormat32bppPRGBA, WICBitmapCacheOnDemand, &areaPatternBitmap->pIWICBitmap);

			if (SUCCEEDED(hr))
			{
				ID2D1RenderTarget* pCurrentRenderTarget = nullptr;
				D2D1_RENDER_TARGET_PROPERTIES prop = D2D1::RenderTargetProperties();
				hr = pDirect2dFactory->CreateWicBitmapRenderTarget(areaPatternBitmap->pIWICBitmap, prop, &pCurrentRenderTarget);

				areaFillInfo.patternMap.insert({ pAreaFill->_name.c_str(), areaPatternBitmap });

				if (SUCCEEDED(hr))
				{
					ID2D1SolidColorBrush* pCurrentBrush = nullptr;
					pCurrentRenderTarget->CreateSolidColorBrush(
						D2D1::ColorF(D2D1::ColorF::Crimson),
						&pCurrentBrush
					);

					pCurrentRenderTarget->BeginDraw();
					DrawAreaFill(pAreaFill->_name, pCurrentRenderTarget, pCurrentBrush, pStrokeStyleS101Solid, paletteName);
					pCurrentRenderTarget->EndDraw();
				}
				else
				{
					OutputDebugString(L"Failed to create a iwic bitmap in AreaFill\n");
					delete areaPatternBitmap;
				}
			}
			else
			{
				OutputDebugString(L"Failed to create a bitmap in AreaFill\n");
				delete areaPatternBitmap;
			}
		}
	}
}


D2D1_RECT_F S100PCManager::GetAreaFillRect(AreaFill* pAreaFill, FLOAT scale)
{
	D2D1_RECT_F rect = { 0, 0, 0, 0 };

	SVGReader* pSVG = s100SymbolManager.GetSVG(pAreaFill->_symbolReference);

	if (pSVG)
	{
		if (pAreaFill->v1_y < 0.0001 && pAreaFill->v2_x < 0.0001)
		{
			rect.left = 0;
			rect.top = 0;
			rect.right = pAreaFill->v1_x;
			rect.bottom = pAreaFill->v2_y;
		}
		else
		{
			rect.left = 0;
			rect.top = 0;
			
			rect.right = pAreaFill->v1_x;
			rect.bottom = pAreaFill->v2_y * 2;
		}

		rect.left *= scale;
		rect.top *= scale;
		rect.right = rect.right * scale;
		rect.bottom = rect.bottom * scale;
	}

	return rect;
}


void S100PCManager::DeleteBitmapBrush()
{
	for (auto i = areaFillInfo.patternMap.begin(); i != areaFillInfo.patternMap.end(); i++)
	{
		SafeRelease(&i->second->pBitmapBrush);
	}
}


void S100PCManager::DeleteBitmapImage()
{
	for (auto i = areaFillInfo.patternMap.begin(); i != areaFillInfo.patternMap.end(); i++)
	{
		delete i->second;
	}

	areaFillInfo.patternMap.clear();
}


void S100PCManager::DeleteLineImage()
{
	auto pos = lineImageMap.GetStartPosition();
	while (pos)
	{
		CString key;
		IWICBitmap* bitmap = nullptr;
		lineImageMap.GetNextAssoc(pos, key, bitmap);

		SafeRelease(&bitmap);
	}

	lineImageMap.RemoveAll();
}


void S100PCManager::InverseMatrixBitmapBrush(D2D1::Matrix3x2F matrix)
{
	for (auto i = areaFillInfo.patternMap.begin(); i != areaFillInfo.patternMap.end(); i++)
	{
		i->second->pBitmapBrush->SetTransform(matrix);
	}
}


IWICBitmap* S100PCManager::GetLineImage(std::wstring& key)
{
	IWICBitmap* result;

	// If you can't find it,
	if (0 == lineImageMap.Lookup(key.c_str(), result))
	{
		return nullptr;
	}
	return result;
}

S100ColorProfile* S100PCManager::GetS100ColorProfile()
{
	return &s100ColorProfile;
}