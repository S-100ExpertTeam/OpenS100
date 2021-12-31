#include "StdAfx.h"
#include "ENC_ExchangeSet.h"
#include "ATTF.h"
#include "ISO8211Fuc.h"
#include "DRReader.h"
#include "DRDirectoryInfo.h"

#include "..\\GeoMetryLibrary\\GeometricFuc.h"

ENC_ExchangeSet::ENC_ExchangeSet(void)
{
	//overlayIndex = 0;
}

ENC_ExchangeSet::~ENC_ExchangeSet(void)
{
	CatalogueList::iterator itor;
	CatalogueMap::iterator mitor;

	for (mitor = m_catList.begin(); mitor != m_catList.end(); mitor++)
	{
		delete mitor->second;
	}

	//for( itor = m_overlayLayers.begin(); itor != m_overlayLayers.end(); itor++)
	//{
	//	delete (*itor);
	//}
}

void ENC_ExchangeSet::SetPLLibrary(
	CGeoObjectMap *object,
	CAttributeMap *attribute,
	CColorMap *color,
	CSymbolMap *&symbol)
{

	this->p_object = object;
	this->p_attribute = attribute;
	this->p_color = color;
	this->p_symbol = symbol;
}

bool ENC_ExchangeSet::Open(CString fileName)
{
#ifdef _DEBUG
	USES_CONVERSION;
	LARGE_INTEGER liCounter1, liCounter2, liFrequency;
	QueryPerformanceFrequency(&liFrequency);  // retrieves the frequency of the high-resolution performance counter   
	QueryPerformanceCounter(&liCounter1);         // Start
#endif

	ifstream ifs;
	ifs.open(fileName, std::ios::in | std::ios::ate | ios::binary);
	ifs.seekg(0, std::ios::end);
	long size = (long)ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	BYTE* buf = new BYTE[size];
	BYTE* buf_head = buf;
	ifs.read((char*)buf, size);
	ifs.close();

	ReadCatalogue(buf, (unsigned)size);

	buf = NULL;
	delete[]buf_head;

#ifdef _DEBUG
	QueryPerformanceCounter(&liCounter2);  // End
	TRACE(W2A(TEXT("Catalogue 로딩 시간 : %f sec\n")), (double)(liCounter2.QuadPart - liCounter1.QuadPart) / (double)liFrequency.QuadPart);
#endif

	return true;
}

CatalogueMap* ENC_ExchangeSet::GetCatalogueMap()
{
	return &m_catList;
}

bool ENC_ExchangeSet::ReadCatalogue(BYTE*& buf, unsigned size)
{
	USES_CONVERSION;

	BYTE* endOfBuf = buf + size - 1;

	ReadDDR(buf);
	DRDirectoryInfo drDir;

	int cellIndex = 0;

	while (buf < endOfBuf)
	{
		DRReader drReader;
		int subFieldCount = 0;
		drReader.ReadReader(buf);
		subFieldCount = (drReader.m_fieldAreaLoc - DR_LENGTH - 1) / (4 + drReader.m_fieldLength + drReader.m_fieldPosition);

		if (subFieldCount < 1)
		{
			TRACE(W2A(TEXT("SubFieldCount Error")));
			continue;
		}

		drDir.ReAllocateDirectory(subFieldCount);
		drDir.ReadDir(drReader, buf);

		if (*buf != 0x1E)//{}
			TRACE(W2A(TEXT("terminator error")));
		buf++;

		if (drDir.GetDirectory(0)->tag == *((unsigned int*)"0001")) {
			buf += drDir.GetDirectory(0)->length - 1;
			if (*(buf++) != 0x1E)//{}
				TRACE(W2A(TEXT("terminator error\n")));
		}
		else {
			TRACE(W2A(TEXT("Abnormal record data\n")));
		}

		if (drDir.GetDirectory(1)->tag == *((unsigned int*)"CATD")) {
			ENC_Catalogue *ca = new ENC_Catalogue();
			ca->ReadRecord(&drDir, buf);

			wstring wtstr = ca->m_catd.FILE.substr(9, 3);
			if (wtstr.compare(TEXT("000")) == 0)
			{
				wchar_t t = ca->m_catd.FILE.c_str()[2];
				int i = asNumeric(t);
				//m_catList.push_back(ca);
				ca->m_index = cellIndex;
				m_catList.insert(CatalogueMap::value_type(cellIndex, ca));
				m_catListByLevel[i - 1].push_back(ca);

				cellIndex++;
			}
			else if (wtstr.compare(TEXT("SNC")) == 0)
			{
				wchar_t t = ca->m_catd.FILE.c_str()[2];
				int i = asNumeric(t);
				//m_catList.push_back(ca);
				ca->m_index = cellIndex;
				m_catList.insert(CatalogueMap::value_type(cellIndex, ca));
				m_catListByLevel[i - 1].push_back(ca);

				cellIndex++;
			}
			else
			{
				delete ca;
			}
		}
	}
	return true;
}

bool ENC_ExchangeSet::ReadDDR(BYTE*& buf)
{
	int ddfsSize = atoi(buf, 5);
	buf += ddfsSize - 5;
	//ifs->read(buf, ddfsSize);	

	return true;
}

void ENC_ExchangeSet::CheckExchangeSetLayer(Scaler *scaler)
{
//#ifndef _DEBUG // & ROUTE_CHECK
	/*
	enum PurposeScale {
	Berthing = 4000,
	Harbour = 25000,
	Approach = 90000,
	Coastal = 350000,
	General = 1500000,
	Overview = 3000000,
	};
	*/

	int viewIndex = 0;
	if (scaler->currentScale < Berthing)
	{
		viewIndex = 6;
	}
	else if (scaler->currentScale < Harbour)
	{
		viewIndex = 5;
	}
	else if (scaler->currentScale < Approach)
	{
		viewIndex = 4;
	}
	else if (scaler->currentScale < Coastal)
	{
		viewIndex = 3;
	}
	else if (scaler->currentScale < General)
	{
		viewIndex = 2;
	}
	else if (scaler->currentScale < Overview)
	{
		viewIndex = 1;
	}

	CatalogueList::iterator clItor;
	int i = 0;

	bool bCovered[16] = { false };
	m_showLayer.clear();

	for (i = viewIndex + 1; i < 6; i++)
	{
		for (clItor = m_loadedListCat[i].begin(); clItor != m_loadedListCat[i].end(); clItor++)
		{
			delete (*clItor)->m_layer;
			(*clItor)->m_layer = NULL;
			(*clItor)->m_bLoaded = false;
			(*clItor)->m_bShow = false;
		}
		m_loadedListCat[i].clear();
	}

	//try{
	for (i = 0; i < viewIndex + 1; i++)
	{
		if (i < 0 || i >= 6)
			continue;

		for (clItor = m_loadedListCat[i].begin(); clItor != m_loadedListCat[i].end();)
		{
			MBR marginMBR = scaler->GetMap();
			marginMBR.Scale(0.8);
			
			//double marginX = (LayerManager::ViewMBR.xmax - LayerManager::ViewMBR.xmin) * 0.8;
			//double marginY = (LayerManager::ViewMBR.ymax - LayerManager::ViewMBR.ymin) * 0.8;
			//marginMBR.xmax = LayerManager::ViewMBR.xmax + marginX;
			//marginMBR.xmin = LayerManager::ViewMBR.xmin - marginX;
			//marginMBR.ymax = LayerManager::ViewMBR.ymax + marginY;
			//marginMBR.ymin = LayerManager::ViewMBR.ymin - marginY;

			if (!MBR::CheckOverlap(marginMBR, (*clItor)->m_mbr))
			{
				(*clItor)->m_bShow = false;
				delete (*clItor)->m_layer;
				(*clItor)->m_layer = NULL;
				(*clItor)->m_bLoaded = false;

				clItor = m_loadedListCat[i].erase(clItor);
			}
			else
			{
				clItor++;
			}
		}
	}

	//for(i = 0; i <  viewIndex; i++)
	for (i = viewIndex - 1; i >= 0; i--)
	{
		if (i < 0 || i > 6)
			continue;


		bool bCoveredAll = true;
		for (int k = 0; k < 16; k++)
		{
			if (!bCovered[k])
			{
				bCoveredAll = false;
			}
		}

		if (bCoveredAll)
		{
			for (clItor = m_catListByLevel[i].begin(); clItor != m_catListByLevel[i].end(); clItor++)
			{
				(*clItor)->m_bShow = false;
				continue;
			}
		}

		for (clItor = m_catListByLevel[i].begin(); clItor != m_catListByLevel[i].end(); clItor++)
		{
			if (MBR::CheckOverlap(scaler->GetMap(), (*clItor)->m_mbr))
			{
				ENCLayer *layer = NULL;
				if ((*clItor)->m_bLoaded == false)
				{
					layer = new ENCLayer(p_object, p_attribute, p_color, p_symbol); //

					// 파일 열기에 실패했을 때
					CString filePath = (*clItor)->m_filePath;
					if (layer->Open((*clItor)->m_filePath) == FALSE) {
						//AfxMessageBox(L"Failure to open enc cell on Exchange set ");
						(*clItor)->m_bShow = false;
						delete layer;
						continue;
					}

					(*clItor)->m_layer = layer;

					// Layer List의 Tail에 추가
					m_loadedListCat[i].push_back(*clItor);

					(*clItor)->m_bLoaded = true;
				}
				else
				{
					layer = (*clItor)->m_layer;
				}

				if (layer)
				{
					m_showLayer.push_back(layer);

					ENCCell* pc = (ENCCell*)layer->m_spatialObject;
					list<ENC_Feature*>::iterator fitor;

					for (fitor = pc->m_M_COVR.begin(); fitor != pc->m_M_COVR.end(); fitor++)
					{
						list<ATTF*>::iterator attfitor;

						bool isCoverageAvailable = true;
						for (attfitor = (*fitor)->m_attfList.begin();
							attfitor != (*fitor)->m_attfList.end();
							attfitor++)
						{
							if ((*attfitor)->ATTL == 18)
							{
								if (_wtoi((*attfitor)->ATVL.c_str()) == 1)
								{
									isCoverageAvailable = true;
								}
								else {
									isCoverageAvailable = false;
								}
							}
						}

						if (!isCoverageAvailable)
							continue;

						if ((*fitor)->m_frid.PRIM != 3)
							continue;

						auto mapMBR = scaler->GetMap();
						double xa[6] = { mapMBR.GetXMin(),
							mapMBR.GetXMin() + mapMBR.GetWidth() / 5,
							mapMBR.GetXMin() + mapMBR.GetWidth() / 5 * 2,
							mapMBR.GetXMin() + mapMBR.GetWidth() / 5 * 3,
							mapMBR.GetXMin() + mapMBR.GetWidth() / 5 * 4,
							mapMBR.GetXMax()
						};

						double ya[6] = { mapMBR.GetYMin(),
							mapMBR.GetYMin() + mapMBR.GetYMax() - mapMBR.GetYMin() / 5,
							mapMBR.GetYMin() + mapMBR.GetYMax() - mapMBR.GetYMin() / 5 * 2,
							mapMBR.GetYMin() + mapMBR.GetYMax() - mapMBR.GetYMin() / 5 * 3,
							mapMBR.GetYMin() + mapMBR.GetYMax() - mapMBR.GetYMin() / 5 * 4,
							mapMBR.GetYMax()
						};

						int ti = i;
						int k = 0, l = 0;
						for (k = 0; k < 6; k++)
						{
							for (l = 0; l < 6; l++)
							{
								if (!bCovered[k*l + l])
									bCovered[k*l + l] = GeometricFuc::inside(xa[k], ya[l], (ENCArea*)(*fitor)->m_geometry) > 0 ? true : false;
							}
						}

						int a111 = 0;
						i = ti;
					}
				}
				(*clItor)->m_bShow = true;
			}
			else
			{
				(*clItor)->m_bShow = false;
			}
		}
	}
	//}
	//catch(...)
	//{
	//	AfxMessageBox(L"Failure to delete Layer");
	//}
//#endif
}

void ENC_ExchangeSet::Draw(HDC &hDC, Scaler *scaler, double offset)
{
	int viewIndex = 0;
	if (scaler->currentScale < Berthing)
	{
		viewIndex = 6;
	}
	else if (scaler->currentScale < Harbour)
	{
		viewIndex = 5;
	}
	else if (scaler->currentScale < Approach)
	{
		viewIndex = 4;
	}
	else if (scaler->currentScale < Coastal)
	{
		viewIndex = 3;
	}
	else if (scaler->currentScale < General)
	{
		viewIndex = 2;
	}
	else if (scaler->currentScale < Overview)
	{
		viewIndex = 1;
	}

	int i = 0, j = 0;
	CatalogueList::iterator clItor;
	ENCLayer* enclayer;

	//for(i = viewIndex - 1; i <  viewIndex; i++)
	for (i = 0; i < viewIndex; i++)
	{
		ENCGeometry::InitTextRect();
		ENCGeometry::InitSoundingTextRect();
		ENCGeometry::InitIsobathTextRect();

		if (i < 0 || i > 6)
			continue;
		for (j = 0; j < 10; j++)
		{
			for (clItor = m_catListByLevel[i].begin(); clItor != m_catListByLevel[i].end(); clItor++)
			{
				if (!(*clItor)->m_bShow)
					continue;

				//try{
				enclayer = (*clItor)->m_layer;
				enclayer->Draw(hDC, scaler, j, 3, offset);

				//}
				//catch (...)
				//{
				//	AfxMessageBox(_T("Caused exception by drawing"));
				//}
			}
			for (clItor = m_catListByLevel[i].begin(); clItor != m_catListByLevel[i].end(); clItor++)
			{
				if (!(*clItor)->m_bShow)
					continue;

				//try{
				enclayer = (*clItor)->m_layer;
				enclayer->Draw(hDC, scaler, j, 2, offset);

				//}
				//catch (...)
				//{
				//	AfxMessageBox(_T("Caused exception by drawing"));
				//}
			}
			for (clItor = m_catListByLevel[i].begin(); clItor != m_catListByLevel[i].end(); clItor++)
			{
				if (!(*clItor)->m_bShow)
					continue;

				//try{
				enclayer = (*clItor)->m_layer;
				enclayer->Draw(hDC, scaler, j, 1, offset);

				//}
				//catch (...)
				//{
				//	AfxMessageBox(_T("Caused exception by drawing"));
				//}
			}
		}

		for (clItor = m_catListByLevel[i].begin(); clItor != m_catListByLevel[i].end(); clItor++)
		{
			if (!(*clItor)->m_bShow)
				continue;

			//try{
			enclayer = (*clItor)->m_layer;

			if (MBR::CheckOverlap(scaler->GetMap(), enclayer->m_mbr))
			{
				enclayer->Draw(hDC, scaler, j, 4, offset);
			}
			//}
			//catch(...)
			//{
			//	AfxMessageBox(_T("Caused exception by drawing"));
			//}
		}
	}
}

void ENC_ExchangeSet::UpdateCS_SafetyDepth()
{
	CatalogueList::iterator itor;

	for (int i = 0; i < 6; i++)
	{
		for (itor = m_loadedListCat[i].begin(); itor != m_loadedListCat[i].end(); itor++)
		{
			Layer* layer = (*itor)->m_layer;
			((ENCLayer *)layer)->UpdateCS_SafetyDepth();
		}
	}
	//for(itor = m_overlayLayers.begin(); itor != m_overlayLayers.end(); itor++)
	//{
	//	Layer* layer = (*itor)->m_layer;
	//	((ENCLayer *)layer)->UpdateCS_SafetyDepth();
	//}
}


void ENC_ExchangeSet::UpdateCS_SafetyContour()
{
	CatalogueList::iterator itor;

	for (int i = 0; i < 6; i++)
	{
		for (itor = m_loadedListCat[i].begin(); itor != m_loadedListCat[i].end(); itor++)
		{
			Layer* layer = (*itor)->m_layer;
			((ENCLayer *)layer)->UpdateCS_SafetyContour();
		}
	}
	//for(itor = m_overlayLayers.begin(); itor != m_overlayLayers.end(); itor++)
	//{
	//	Layer* layer = (*itor)->m_layer;
	//	((ENCLayer *)layer)->UpdateCS_SafetyContour();
	//}
}

void ENC_ExchangeSet::UpdateCS_BoundarySymbol()
{
	CatalogueList::iterator itor;

	for (int i = 0; i < 6; i++)
	{
		for (itor = m_loadedListCat[i].begin(); itor != m_loadedListCat[i].end(); itor++)
		{
			Layer* layer = (*itor)->m_layer;
			((ENCLayer *)layer)->UpdateCS_BoundarySymbol();
		}
	}
	//for(itor = m_overlayLayers.begin(); itor != m_overlayLayers.end(); itor++)
	//{
	//	Layer* layer = (*itor)->m_layer;
	//	((ENCLayer *)layer)->UpdateCS_BoundarySymbol();
	//}
}

void ENC_ExchangeSet::UpdateCS_ShadeType()
{
	CatalogueList::iterator itor;

	for (int i = 0; i < 6; i++)
	{
		for (itor = m_loadedListCat[i].begin(); itor != m_loadedListCat[i].end(); itor++)
		{
			Layer* layer = (*itor)->m_layer;
			((ENCLayer *)layer)->UpdateCS_ShadeType();
		}
	}
	//for(itor = m_overlayLayers.begin(); itor != m_overlayLayers.end(); itor++)
	//{
	//	Layer* layer = (*itor)->m_layer;
	//	((ENCLayer *)layer)->UpdateCS_ShadeType();
	//}
}

void ENC_ExchangeSet::ChangeDisplayMode()
{
	CatalogueList::iterator itor;

	for (int i = 0; i < 6; i++)
	{
		for (itor = m_loadedListCat[i].begin(); itor != m_loadedListCat[i].end(); itor++)
		{
			Layer* layer = (*itor)->m_layer;
			((ENCLayer *)layer)->ChangeDisplayMode();
		}
	}
	//for(itor = m_overlayLayers.begin(); itor != m_overlayLayers.end(); itor++)
	//{
	//	Layer* layer = (*itor)->m_layer;
	//	((ENCLayer *)layer)->ChangeDisplayMode();
	//}
}
//
//int ENC_ExchangeSet::AddOverlayLayer(ENCCell* cell)
//{
//	ENC_Catalogue* catalogue = new ENC_Catalogue();
//
//	catalogue->m_index = overlayIndex;
//	catalogue->m_layer = (ENCLayer*) cell->m_pLayer;
//
//	m_overlayLayers.push_back(catalogue);
//
//	overlayIndex++;
//	return 0;
//}