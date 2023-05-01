#include "pch.h"
#include "MainFrm.h"
#include "DockablePaneEditWindow.h"
#include "resource.h"
#include "OpenS100.h"
#include "OpenS100View.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../GISLibrary/S100Layer.h"
#include "../GISLibrary/F_CUCO.h"
#include "../GISLibrary/F_PTAS.h"
#include "../GISLibrary/F_SPAS.h"
#include "../GISLibrary/F_C3IL.h"
#include "../GISLibrary/F_C2IL.h"
#include "../GISLibrary/F_C2IT.h"
#include "../GISLibrary/F_ATTR.h"
#include "../GISLibrary/F_RIAS.h"
#include "../GISLibrary/R_MultiPointRecord.h"
#include "../GISLibrary/R_CurveRecord.h"
#include "../GISLibrary/R_CompositeRecord.h"
#include "../GISLibrary/R_SurfaceRecord.h"
#include "../GISLibrary/R_FeatureRecord.h"
#include "../GISLibrary/R_PointRecord.h"
#include "../GISLibrary/ATTR.h"
#include "../GISLibrary/C3IL.h"
#include "../GISLibrary/IC2D.h"
#include "../GISLibrary/CodeWithNumericCode.h"
#include "../GISLibrary/CUCO.h"
#include "../GISLibrary/SPAS.h"
#include "../GISLibrary/PTAS.h"
#include "../GISLibrary/RIAS.h"
#include "../GISLibrary/F_INAS.h"
#include "../GISLibrary/SpatialObject.h"
#include "../GISLibrary/GISLibrary.h"
#include "../GISLibrary/S100SpatialObject.h"
#include "../GISLibrary/S100Layer.h"
#include "../GISLibrary/GeometryType.h"
#include "../GISLibrary/GM_SurfacePatch.h"
#include "../GISLibrary/GM_Polygon.h"

#include <vector>
#include <sstream>
#include <string>

const UINT WM_FILTER_LIST = ::RegisterWindowMessage(_T("WM_FILTER_LIST"));
const CString tabNameAttribute = L"Attributes";
const CString tabNameVector = L"Vector";

// CFeatureView
//
CPropertyGridVectorModify	CDockablePaneEditWindow::m_wndListAttribute;



IMPLEMENT_DYNAMIC(CDockablePaneEditWindow, CDockablePane)

CDockablePaneEditWindow::CDockablePaneEditWindow()
{
	
}

CDockablePaneEditWindow::~CDockablePaneEditWindow()
{
	DeleteAllItems();
}

BEGIN_MESSAGE_MAP(CDockablePaneEditWindow, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int CDockablePaneEditWindow::OnCreate(LPCREATESTRUCT lp)
{
	if (CDockablePane::OnCreate(lp) == -1)
	{
		return -1;
	}

	CRect rect;
	GetClientRect(&rect);

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}
	m_wndTabs.HideSingleTab(TRUE);


	// create attribute tab
	if (!m_wndListAttribute.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL
		| LVS_REPORT, rect, &m_wndTabs, 1)
		)
	{
		TRACE0("Failed to create Nodes List\n");
		return -1;
	}
	m_wndListAttribute.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_wndListAttribute.EnableHeaderCtrl(FALSE);
	m_wndListAttribute.EnableDescriptionArea();
	m_wndListAttribute.SetVSDotNetLook();
	m_wndListAttribute.MarkModifiedProperties();

	m_wndTabs.AddTab(&m_wndListAttribute, tabNameAttribute, (UINT)0, 0);

	::ShowScrollBar(m_wndListAttribute.GetSafeHwnd(), SB_VERT, FALSE);

	if (!m_wndListVector.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL
		| LVS_REPORT, rect, &m_wndTabs, 1)
		)
	{
		TRACE0("Failed to create Nodes List\n");
		return -1;
	}
	m_wndListVector.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_wndListVector.EnableHeaderCtrl(FALSE);
	m_wndListVector.EnableDescriptionArea();
	m_wndListVector.SetVSDotNetLook();
	m_wndListVector.MarkModifiedProperties();

	m_wndTabs.AddTab(&m_wndListVector, tabNameVector, (UINT)0, 0);

	::ShowScrollBar(m_wndListVector.GetSafeHwnd(), SB_VERT, FALSE);

	//	viewer mode
	return 0;

	if (!m_wndDrawingCommands.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL
		| LVS_REPORT, rect, &m_wndTabs, 1)
		)
	{
		TRACE0("Failed to create Nodes List\n");
		return -1;
	}
	m_wndDrawingCommands.EnableWindow(false); //change readonly

	m_wndTabs.AddTab(&m_wndDrawingCommands, L"Drawing Commands", (UINT)0, 0);

	::ShowScrollBar(m_wndDrawingCommands.GetSafeHwnd(), SB_VERT, FALSE);

	return 0;
}

void CDockablePaneEditWindow::SettingEnumType(std::vector<ListedValue*> list, CMFCPropertyGridProperty* pSuperProperty)
{
	for (auto i = list.begin(); i != list.end(); i++)
	{
		ListedValue* lv = *i;
		CString label(lv->GetLabel().c_str());
		CString namestr;
		namestr.Format(_T("%d.%s"), i, label);
		LPCTSTR name = namestr;
		pSuperProperty->AddOption(name);
	}
}

void CDockablePaneEditWindow::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	CRect rect;
	GetClientRect(&rect);
	m_wndTabs.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

//we need to erase backround becuase our edit ctrl have margin
BOOL CDockablePaneEditWindow::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	::FillRect(pDC->m_hDC, &rect, GetSysColorBrush(COLOR_3DFACE));
	return TRUE;
}

void CDockablePaneEditWindow::SetFeatureType(std::wstring featureID)
{
	selectedFeatureID = featureID;
	selectedInformationID = L"";

	m_selectedObjectType = L"Feature";

	SetAttributes();
	SetVectors();
}

void CDockablePaneEditWindow::SetInformationType(std::wstring informationID)
{
	selectedFeatureID = L"";
	selectedInformationID = informationID;
	
	m_selectedObjectType = L"Information";

	SetAttributes();
	ClearVector(); 
}


void CDockablePaneEditWindow::SetAttributes() //After the point click, it goes over here.
{
	auto selectedID = pugi::as_utf8(selectedFeatureID);

	DeleteAttributeItems();
	std::vector<CMFCPropertyGridProperty*> pAttrItemList;

	if (m_cell == nullptr)
	{
		return;
	}

	auto s100Layer = (S100Layer*)m_cell->GetLayer();
	auto fc = s100Layer->GetFeatureCatalog();
	if (nullptr == fc)
	{
		return;
	}

	std::string id = "";
	int type = 0;
	if (m_selectedObjectType == L"Feature")
	{
		id = pugi::as_utf8(selectedFeatureID);
		type = 1;
	}
	else if (m_selectedObjectType == L"Information")
	{
		id = pugi::as_utf8(selectedInformationID);
		type = 2;
	}

	auto object = m_cell->GetObjectType(type, id);

	if (object)
	{
		int attrCnt = object->GetAttributeCount();
		for (int i = 0; i < attrCnt; i++)
		{
			auto value = object->GetAttributeValue(i);
			auto code = m_cell->GetObjectAttributeCode(type, id, i);
			CString strValue;
			CMFCPropertyGridProperty* pAttribute = nullptr;

			Attribute* attribute = fc->GetSimpleAttribute(code);
			bool isSimple = true;
			if (!attribute)
			{
				attribute = fc->GetComplexAttribute(code);
				isSimple = false;
				if (!attribute)
				{
					continue;
				}
			}

			if (isSimple)
			{
				auto sa = (SimpleAttribute*)attribute;
						
				if (sa->GetValueType() == FCD::S100_CD_AttributeValueType::enumeration)
				{
					auto iValue = atoi(value.c_str());
					auto listedValue = sa->GetListedValue(iValue);
					if (listedValue)
					{
						strValue.Format(L"%d. %s", listedValue->GetCode(), listedValue->GetLabel().c_str());
					}
				}
				else
				{
					strValue = LibMFCUtil::StringToWString(value).c_str();
				}

				pAttribute = new CMFCPropertyGridProperty(sa->GetName().c_str(), strValue);
			}
			else
			{
				auto ca = (ComplexAttribute*)attribute;
				if (ca)
				{
					pAttribute = new CMFCPropertyGridProperty(ca->GetName().c_str());
				}
			}
					
			pAttrItemList.push_back(pAttribute);
			pAttribute->SetDescription(attribute->GetDefinition().c_str());

			auto parentIndex = object->GetParentAttributeIndex(i);

			if (parentIndex != 0)
			{
				pAttrItemList[parentIndex - 1]->AddSubItem(pAttribute);
			}
			else
			{
				m_wndListAttribute.AddProperty(pAttribute);
			}
		}
	}

	m_wndListAttribute.ExpandAll();
}

void CDockablePaneEditWindow::SetVectors()
{
	if (nullptr == m_cell)
	{
		return;
	}

	auto s100Layer = (S100Layer*)m_cell->GetLayer();
	//if (101 == s100Layer->GetProductNumber())
	{
		auto cell = (S101Cell*)m_cell;
		auto m_pFeature = cell->GetFeatureType(pugi::as_utf8(selectedFeatureID));

		m_wndListVector.ShowWindow(FALSE);
		DeleteVectorItems();

		std::wstring str;
		//R_PointRecord* pr = NULL;
		//R_MultiPointRecord* mpr = NULL;
		//R_CurveRecord* cr = NULL;
		//R_CompositeRecord* ccr = NULL;
		//R_SurfaceRecord* sr = NULL;

		if (cell == nullptr || m_pFeature == nullptr)
		{
			m_wndListVector.ShowWindow(TRUE);
			return;
		}

		//if (m_cell->m_FileType == FILE_S_100_VECTOR)
		{
			//S101Cell* cell = (S101Cell*)m_cell;

			auto geom = m_pFeature->GetGeometry();
			if (geom)
			{
				
			}


					//switch (spas->m_name.RCNM)
					//{
					//case 110:
					//	pr = cell->GetPointRecord(spas->m_name.GetName());
					//	SetVector(pr);
					//	break;
					//case 115:
					//	mpr = cell->GetMultiPointRecord(spas->m_name.GetName());
					//	SetVector(mpr);
					//	break;
					//case 120:
					//	cr = cell->GetCurveRecord(spas->m_name.GetName());
					//	SetVector(cr);
					//	break;
					//case 125:
					//	ccr = cell->GetCompositeCurveRecord(spas->m_name.GetName());
					//	SetVector(ccr);
					//	break;
					//case 130:
					//	sr = cell->GetSurfaceRecord(spas->m_name.GetName());
					//	SetVector(sr);
					//	break;
		}

		m_wndListVector.ExpandAll(FALSE);
		m_wndListVector.ShowWindow(TRUE);
	}
}

void CDockablePaneEditWindow::SetVector(int RCNM, R_VectorRecord* r, CMFCPropertyGridProperty *pSuperProperty)
{
	//switch (RCNM)
	//{
	//case 110:
	//	SetVector((R_PointRecord*)r, pSuperProperty);
	//	break;
	//case 115:
	//	SetVector((R_MultiPointRecord*)r, pSuperProperty);
	//	break;
	//case 120:
	//	SetVector((R_CurveRecord*)r, pSuperProperty);
	//	break;
	//case 125:
	//	SetVector((R_CompositeRecord*)r, pSuperProperty);
	//	break;
	//case 130:
	//	SetVector((R_SurfaceRecord*)r, pSuperProperty);
	//	break;
	//}
}

void CDockablePaneEditWindow::SetVector(S100Geometry::Point* r, CMFCPropertyGridProperty* pSuperProperty)
{
	auto infoCount = r->GetInformationRelationCount();

	CString strInfoCount;
	strInfoCount.Format(L"%d", infoCount);

	std::wstring pointName = L"Point (ID : " + r->GetIDAsWString() + L", Relation: " + std::wstring(strInfoCount) + L")";

	CMFCPropertyGridProperty* pProperty = new CMFCPropertyGridProperty(pointName.c_str());
	if (pSuperProperty)
	{
		pSuperProperty->AddSubItem(pProperty);
	}
	else
	{
		m_wndListVector.AddProperty(pProperty);
	}

	if (infoCount > 0) // If there's even one number,
	{
		CMFCPropertyGridProperty* pInasParent = new CMFCPropertyGridProperty(L"INAS");

		pProperty->AddSubItem(pInasParent);

		for (int i = 0; i < infoCount; i++)
		{
			auto infoID = r->GetAssociatedInformationID(i);
			auto info = m_cell->GetInformationType(infoID);
			if (info)
			{
				std::wstring inasName = L"Information id";
				CMFCPropertyGridProperty* pInas = new CMFCPropertyGridProperty(inasName.c_str(), info->GetIDAsWString().c_str());
				pInasParent->AddSubItem(pInas);
				pInas->SetName(inasName.c_str());
				pInas->AllowEdit(FALSE);
			}
		}
	}

	int precision_x = 0;
	int cmf = m_cell->CoordinateMultiplicationFactorForX();
	while (cmf >= 10)
	{
		cmf /= 10;
		precision_x++;
	}

	int precision_y = 0;
	cmf = m_cell->CoordinateMultiplicationFactorForY();
	while (cmf >= 10)
	{
		cmf /= 10;
		precision_y++;
	}

	std::wstringstream wss;
	wss.setf(std::ios::fixed, std::ios::floatfield);

	double x = r->position.GetX();
	wss.precision(precision_x);
	
	double y = r->position.GetY();
	wss.precision(precision_y);

	wss << y << L"," << x;

	CMFCPropertyGridProperty* pChildProperty = new CMFCPropertyGridProperty(L"Position", wss.str().c_str());
	pProperty->AddSubItem(pChildProperty);

	pChildProperty->SetName(L"Point");
}

void CDockablePaneEditWindow::SetVector(SPoint* geom, CMFCPropertyGridProperty* pSuperProperty)
{
	//auto infoCount = r->GetInformationRelationCount();

	//CString strInfoCount;
	//strInfoCount.Format(L"%d", infoCount);

	//std::wstring pointName = L"Point (ID : " + r->GetIDAsWString() + L", Relation: " + std::wstring(strInfoCount) + L")";

	//CMFCPropertyGridProperty* pProperty = new CMFCPropertyGridProperty(pointName.c_str());
	//if (pSuperProperty)
	//{
	//	pSuperProperty->AddSubItem(pProperty);
	//}
	//else
	//{
	//	m_wndListVector.AddProperty(pProperty);
	//}

	//if (infoCount > 0) // If there's even one number,
	//{
	//	CMFCPropertyGridProperty* pInasParent = new CMFCPropertyGridProperty(L"INAS");

	//	pProperty->AddSubItem(pInasParent);

	//	for (int i = 0; i < infoCount; i++)
	//	{
	//		auto infoID = r->GetAssociatedInformationID(i);
	//		auto info = m_cell->GetInformationType(infoID);
	//		if (info)
	//		{
	//			std::wstring inasName = L"Information id";
	//			CMFCPropertyGridProperty* pInas = new CMFCPropertyGridProperty(inasName.c_str(), info->GetIDAsWString().c_str());
	//			pInasParent->AddSubItem(pInas);
	//			pInas->SetName(inasName.c_str());
	//			pInas->AllowEdit(FALSE);
	//		}
	//	}
	//}

	//int precision_x = 0;
	//int cmf = m_cell->CoordinateMultiplicationFactorForX();
	//while (cmf >= 10)
	//{
	//	cmf /= 10;
	//	precision_x++;
	//}

	//int precision_y = 0;
	//cmf = m_cell->CoordinateMultiplicationFactorForY();
	//while (cmf >= 10)
	//{
	//	cmf /= 10;
	//	precision_y++;
	//}

	//std::wstringstream wss;
	//wss.setf(std::ios::fixed, std::ios::floatfield);

	//double x = r->position.GetX();
	//wss.precision(precision_x);

	//double y = r->position.GetY();
	//wss.precision(precision_y);

	//wss << y << L"," << x;

	//CMFCPropertyGridProperty* pChildProperty = new CMFCPropertyGridProperty(L"Position", wss.str().c_str());
	//pProperty->AddSubItem(pChildProperty);

	//pChildProperty->SetName(L"Point");
}

//void CDockablePaneEditWindow::SetVector(R_PointRecord* r, CMFCPropertyGridProperty *pSuperProperty)
//{
//	int inasCount = (int)r->m_inas.size();
//	CString count;
//	count.Format(L"%d", inasCount);
//
//	std::wstring pointName = L"Point (RCID : " + r->GetRCIDasWstring() + L", Relation: " + std::wstring(count) + L")";
//	CMFCPropertyGridProperty *pProperty = new CMFCPropertyGridProperty(pointName.c_str());
//	if (pSuperProperty)
//	{
//		pSuperProperty->AddSubItem(pProperty);
//	}
//	else
//	{
//		m_wndListVector.AddProperty(pProperty);
//	}
//
//	if (inasCount > 0) // If there's even one number,
//	{
//		CMFCPropertyGridProperty *pInasParent = new CMFCPropertyGridProperty(L"INAS");
//
//		pProperty->AddSubItem(pInasParent);
//		for (F_INAS* inas : r->m_inas)
//		{
//			std::wstring inasName = L"Information id";
//			CMFCPropertyGridProperty *pInas = new CMFCPropertyGridProperty(inasName.c_str(), inas->m_name.GetRCIDasWstring().c_str());
//			pInasParent->AddSubItem(pInas);
//
//			MultiData *multiData = InsertPropertyMultiData(200, pInasParent, (DWORD_PTR)inas);
//
//			pInas->SetName(inasName.c_str());
//			pInas->SetData((DWORD_PTR)multiData);
//			pInas->AllowEdit(FALSE);
//		}
//	}
//
//	S101Cell* cell = (S101Cell*)m_cell;
//	int precision_x = 0;
//	int cmf = cell->m_dsgir.m_dssi.m_cmfx;
//	while (cmf >= 10)
//	{
//		cmf /= 10;
//		precision_x++;
//	}
//
//	int precision_y = 0;
//	cmf = cell->m_dsgir.m_dssi.m_cmfy;
//	while (cmf >= 10)
//	{
//		cmf /= 10;
//		precision_y++;
//	}
//
//	if (r->m_c2it)
//	{
//		std::wstringstream wss;
//		wss.setf(std::ios::fixed, std::ios::floatfield);
//		double x = r->m_c2it->m_xcoo / (double)cell->m_dsgir.m_dssi.m_cmfx;
//		wss.precision(precision_x);
//		double y = r->m_c2it->m_ycoo / (double)cell->m_dsgir.m_dssi.m_cmfy;
//		wss.precision(precision_y);
//
//		wss << y << L"," << x;
//
//		CMFCPropertyGridProperty *pChildProperty = new CMFCPropertyGridProperty(L"Position", wss.str().c_str());
//		pProperty->AddSubItem(pChildProperty);
//
//		pChildProperty->SetName(L"C2IT");
//		MultiData *multiData = InsertPropertyMultiData(101, pChildProperty, (DWORD_PTR)r, (DWORD_PTR)r->m_c2it);
//		pChildProperty->SetData((DWORD_PTR)multiData);
//	}
//}

void CDockablePaneEditWindow::SetVector(S100Geometry::MultiPoint* r, CMFCPropertyGridProperty* pSuperProperty)
{
	int infoCount = r->GetInformationRelationCount();
	CString count;
	count.Format(L"%d", infoCount);

	std::wstring multpointName = L"MultiPoint (RCID : " + r->GetIDAsWString() + L", Relation: " + std::wstring(count) + L")";
	CMFCPropertyGridProperty* pProperty = new CMFCPropertyGridProperty(multpointName.c_str());
	if (pSuperProperty)
	{
		pSuperProperty->AddSubItem(pProperty);
	}
	else
	{
		m_wndListVector.AddProperty(pProperty);
	}

	if (infoCount > 0) // If there's even one number,
	{
		CMFCPropertyGridProperty* pInasParent = new CMFCPropertyGridProperty(L"INAS");

		pProperty->AddSubItem(pInasParent);

		for (int i = 0; i < infoCount; i++)
		{
			auto infoID = r->GetAssociatedInformationID(i);
			auto info = m_cell->GetInformationType(infoID);
			if (info)
			{
				std::wstring inasName = L"Information id";
				CMFCPropertyGridProperty* pInas = new CMFCPropertyGridProperty(inasName.c_str(), info->GetIDAsWString().c_str());
				pInasParent->AddSubItem(pInas);

				pInas->SetName(inasName.c_str());
				pInas->AllowEdit(FALSE);
			}
		}
	}

	int precision_x = 0;
	int cmf = m_cell->CoordinateMultiplicationFactorForX();
	while (cmf >= 10)
	{
		cmf /= 10;
		precision_x++;
	}

	int precision_y = 0;
	cmf = m_cell->CoordinateMultiplicationFactorForY();
	while (cmf >= 10)
	{
		cmf /= 10;
		precision_y++;
	}

	int precision_z = 0;
	cmf = m_cell->CoordinateMultiplicationFactorForZ();
	while (cmf >= 10)
	{
		cmf /= 10;
		precision_z++;
	}

	for (auto i = 0; i < r->position.size(); i++)
	{
		std::wstringstream wss;
		wss.setf(std::ios::fixed, std::ios::floatfield);
		double x = r->position.at(i).GetX();
		wss.precision(precision_x);
		wss << x << L",";
		double y = r->position.at(i).GetY();
		wss.precision(precision_y);
		wss << y << L",";
		double z = r->position.at(i).GetZ();
		wss.precision(precision_z);
		wss << z;

		CMFCPropertyGridProperty* pChildProperty = new CMFCPropertyGridProperty(L"Position", wss.str().c_str());
		pProperty->AddSubItem(pChildProperty);

		pChildProperty->SetName(L"C3IL");
	}
}

//void CDockablePaneEditWindow::SetVector(R_MultiPointRecord* r, CMFCPropertyGridProperty *pSuperProperty)
//{
//
//	int inasCount = (int)r->m_inas.size();
//	CString count;
//	count.Format(L"%d", inasCount);
//
//	std::wstring multpointName = L"MultiPoint (RCID : " + r->GetRCIDasWstring() + L", Relation: " + std::wstring(count) + L")";
//	CMFCPropertyGridProperty *pProperty = new CMFCPropertyGridProperty(multpointName.c_str());
//	if (pSuperProperty)
//	{
//		pSuperProperty->AddSubItem(pProperty);
//	}
//	else
//	{
//		m_wndListVector.AddProperty(pProperty);
//	}
//
//	if (inasCount > 0) // If there's even one number,
//	{
//		CMFCPropertyGridProperty *pInasParent = new CMFCPropertyGridProperty(L"INAS");
//
//		pProperty->AddSubItem(pInasParent);
//		for (F_INAS* inas : r->m_inas)
//		{
//			std::wstring inasName = L"Information id";
//			CMFCPropertyGridProperty *pInas = new CMFCPropertyGridProperty(inasName.c_str(), inas->m_name.GetRCIDasWstring().c_str());
//			pInasParent->AddSubItem(pInas);
//
//			MultiData *multiData = InsertPropertyMultiData(200, pInasParent, (DWORD_PTR)inas);
//
//			pInas->SetName(inasName.c_str());
//			pInas->SetData((DWORD_PTR)multiData);
//			pInas->AllowEdit(FALSE);
//
//		}
//	}
//
//	S101Cell* cell = (S101Cell*)m_cell;
//	int precision_x = 0;
//	int cmf = cell->m_dsgir.m_dssi.m_cmfx;
//	while (cmf >= 10)
//	{
//		cmf /= 10;
//		precision_x++;
//	}
//
//	int precision_y = 0;
//	cmf = cell->m_dsgir.m_dssi.m_cmfy;
//	while (cmf >= 10)
//	{
//		cmf /= 10;
//		precision_y++;
//	}
//
//	int precision_z = 0;
//	cmf = cell->m_dsgir.m_dssi.m_cmfz;
//	while (cmf >= 10)
//	{
//		cmf /= 10;
//		precision_z++;
//	}
//
//	for (auto itor = r->m_c3il.begin(); itor != r->m_c3il.end(); itor++)
//	{
//		for (auto itor2 = (*itor)->m_arr.begin(); itor2 != (*itor)->m_arr.end(); itor2++)
//		{
//			C3IL* c3 = *itor2;
//			std::wstringstream wss;
//			wss.setf(std::ios::fixed, std::ios::floatfield);
//			double x = c3->m_xcoo / (double)cell->m_dsgir.m_dssi.m_cmfx;
//			wss.precision(precision_x);
//			wss << x << L",";
//			double y = c3->m_ycoo / (double)cell->m_dsgir.m_dssi.m_cmfy;
//			wss.precision(precision_y);
//			wss << y << L",";
//			double z = c3->m_zcoo / (double)cell->m_dsgir.m_dssi.m_cmfz;
//			wss.precision(precision_z);
//			wss << z;
//
//			CMFCPropertyGridProperty *pChildProperty = new CMFCPropertyGridProperty(L"Position", wss.str().c_str());
//			pProperty->AddSubItem(pChildProperty);
//
//			pChildProperty->SetName(L"C3IL");
//			MultiData *multiData = InsertPropertyMultiData(102, pChildProperty, (DWORD_PTR)r, (DWORD_PTR)c3, (DWORD_PTR)*itor, m_vIndex++);
//			pChildProperty->SetData((DWORD_PTR)multiData);
//
//		}
//	}
//}

void CDockablePaneEditWindow::SetVector(S100Geometry::Curve* r, CMFCPropertyGridProperty* pSuperProperty)
{
	int infoCount = r->GetInformationRelationCount();
	CString count;
	count.Format(L"%d", infoCount);

	std::wstring curveName = L"Curve (RCID : " + r->GetIDAsWString() + L", Relation: " + std::wstring(count) + L")";

	CMFCPropertyGridProperty* pProperty = new CMFCPropertyGridProperty(curveName.c_str());

	if (pSuperProperty)
	{
		pSuperProperty->AddSubItem(pProperty);
	}
	else
	{
		m_wndListVector.AddProperty(pProperty);
	}

	if (infoCount > 0) // If there's even one number,
	{
		CMFCPropertyGridProperty* pInasParent = new CMFCPropertyGridProperty(L"INAS");
		pProperty->AddSubItem(pInasParent);

		for (int i = 0; i < infoCount; i++)
		{
			auto infoID = r->GetAssociatedInformationID(i);
			auto info = m_cell->GetInformationType(infoID);
			if (info)
			{
				std::wstring inasName = L"Information id";
				CMFCPropertyGridProperty* pInas = new CMFCPropertyGridProperty(inasName.c_str(), info->GetIDAsWString().c_str());
				pInasParent->AddSubItem(pInas);

				pInas->SetName(inasName.c_str());
				pInas->AllowEdit(FALSE);
			}
		}
	}

	SetVector(&r->boundary.startPoint, pProperty);

	int precision_x = 0;
	int cmf = m_cell->CoordinateMultiplicationFactorForX();
	while (cmf >= 10)
	{
		cmf /= 10;
		precision_x++;
	}

	int precision_y = 0;
	cmf = m_cell->CoordinateMultiplicationFactorForY();
	while (cmf >= 10)
	{
		cmf /= 10;
		precision_y++;
	}

	for (int i = 0; i < r->segment.size(); i++)
	{
		for (int j = 0; j < r->segment.at(i).controlPoints.size(); j++)
		{
			std::wstringstream wss;
			wss.setf(std::ios::fixed, std::ios::floatfield);

			double x = r->segment.at(i).controlPoints.at(j).GetX();
			wss.precision(precision_x);

			double y = r->segment.at(i).controlPoints.at(j).GetY();
			wss.precision(precision_y);

			wss << y << L"," << x;

			CMFCPropertyGridProperty* pChildProperty = new CMFCPropertyGridProperty(L"Position", wss.str().c_str());
			pProperty->AddSubItem(pChildProperty);
			pChildProperty->SetName(L"C2IL");
		}
	}

	SetVector(&r->boundary.endPoint, pProperty);
}

//void CDockablePaneEditWindow::SetVector(R_CurveRecord* r, CMFCPropertyGridProperty *pSuperProperty)
//{
//	int inasCount = (int)r->m_inas.size();
//	CString count;
//	count.Format(L"%d", inasCount);
//
//	S101Cell* cell = (S101Cell*)m_cell;
//
//	std::wstring curveName = L"Curve (RCID : " + r->GetRCIDasWstring() + L", Relation: " + std::wstring(count) + L")";
//
//	CMFCPropertyGridProperty *pProperty = new CMFCPropertyGridProperty(curveName.c_str());
//
//	if (pSuperProperty)
//	{
//		pSuperProperty->AddSubItem(pProperty);
//	}
//	else
//	{
//		m_wndListVector.AddProperty(pProperty);
//	}
//
//	if (inasCount > 0) // If there's even one number,
//	{
//		CMFCPropertyGridProperty *pInasParent = new CMFCPropertyGridProperty(L"INAS");
//		pProperty->AddSubItem(pInasParent);
//
//		for (F_INAS* inas : r->m_inas)
//		{
//			std::wstring inasName = L"Information id";
//			CMFCPropertyGridProperty *pInas = new CMFCPropertyGridProperty(inasName.c_str(), inas->m_name.GetRCIDasWstring().c_str());
//			pInasParent->AddSubItem(pInas);
//
//			MultiData *multiData = InsertPropertyMultiData(200, pInasParent, (DWORD_PTR)inas);
//
//			pInas->SetName(inasName.c_str());
//			pInas->SetData((DWORD_PTR)multiData);
//			pInas->AllowEdit(FALSE);
//
//		}
//	}
//
//	POSITION ptasPos = NULL;
//	PTAS *ptas = NULL;
//	R_PointRecord *spr = NULL, *epr = NULL;
//	__int64 iKey;
//
//	for (auto i = r->m_ptas->m_arr.begin(); i != r->m_ptas->m_arr.end(); i++)
//	{
//		auto ptas = *i;
//
//		iKey = ((__int64)ptas->m_name.RCNM) << 32 | ptas->m_name.RCID;
//		if (ptas->m_topi == 1 // Beginning node 
//			)
//		{
//			spr = cell->GetPointRecord(iKey);
//		}
//		else if (ptas->m_topi == 2 // End node
//			)
//		{
//			epr = cell->GetPointRecord(iKey);
//		}
//		else if (ptas->m_topi == 3)
//		{
//			spr = cell->GetPointRecord(iKey);
//			epr = spr;
//		}
//	}
//
//	SetVector(spr, pProperty);
//
//	int precision_x = 0;
//	int cmf = cell->m_dsgir.m_dssi.m_cmfx;
//	while (cmf >= 10)
//	{
//		cmf /= 10;
//		precision_x++;
//	}
//
//	int precision_y = 0;
//	cmf = cell->m_dsgir.m_dssi.m_cmfy;
//	while (cmf >= 10)
//	{
//		cmf /= 10;
//		precision_y++;
//	}
//
//	for (auto itorParent = r->m_c2il.begin(); itorParent != r->m_c2il.end(); itorParent++)
//	{
//		int index = 0;
//		for (auto itor = (*itorParent)->m_arr.begin(); itor != (*itorParent)->m_arr.end(); itor++)
//		{
//			IC2D* ic = *itor;
//
//			std::wstringstream wss;
//			wss.setf(std::ios::fixed, std::ios::floatfield);
//
//			double x = ic->m_xcoo / (double)cell->m_dsgir.m_dssi.m_cmfx;
//			wss.precision(precision_x);
//
//			double y = ic->m_ycoo / (double)cell->m_dsgir.m_dssi.m_cmfy;
//			wss.precision(precision_y);
//
//			wss << y << L"," << x;
//
//			CMFCPropertyGridProperty *pChildProperty = new CMFCPropertyGridProperty(L"Position", wss.str().c_str());
//			pProperty->AddSubItem(pChildProperty);
//			pChildProperty->SetName(L"C2IL");
//			MultiData *multiData = InsertPropertyMultiData(100, pChildProperty, (DWORD_PTR)r, (DWORD_PTR)ic, (DWORD_PTR)*itorParent, m_vIndex++);
//			pChildProperty->SetData((DWORD_PTR)multiData);
//		}
//	}
//
//	SetVector(epr, pProperty);
//}

void CDockablePaneEditWindow::SetVector(S100Geometry::CompositeCurve* r, CMFCPropertyGridProperty* pSuperProperty)
{
	int infoCount = (int)r->GetInformationRelationCount();
	CString count;
	count.Format(L"%d", infoCount);

	S101Cell* cell = (S101Cell*)m_cell;
	std::wstring comName = L"Composite Curve (RCID : " + r->GetIDAsWString() + L", Relation: " + std::wstring(count) + L")";
	CMFCPropertyGridProperty* pProperty = new CMFCPropertyGridProperty(comName.c_str());
	if (pSuperProperty)
	{
		pSuperProperty->AddSubItem(pProperty);
	}
	else
	{
		m_wndListVector.AddProperty(pProperty);
	}

	if (infoCount > 0) // If there's even one number,
	{
		CMFCPropertyGridProperty* pInasParent = new CMFCPropertyGridProperty(L"INAS");

		pProperty->AddSubItem(pInasParent);

		for (int i = 0; i < infoCount; i++)
		{
			auto infoID = r->GetAssociatedInformationID(i);
			auto info = m_cell->GetInformationType(infoID);
			if (info)
			{
				std::wstring inasName = L"Information id";
				CMFCPropertyGridProperty* pInas = new CMFCPropertyGridProperty(inasName.c_str(), info->GetIDAsWString().c_str());
				pInasParent->AddSubItem(pInas);

				pInas->SetName(inasName.c_str());
				pInas->AllowEdit(FALSE);
			}
		}
	}

	for (int i = 0; i < r->component.size(); i++)
	{
		if (r->component.at(i).GetType() == S100Geometry::GeometryType::Curve)
		{
			SetVector((S100Geometry::Curve*)&r->component.at(i), pProperty);
		}
		else if (r->component.at(i).GetType() == S100Geometry::GeometryType::CompositeCurve)
		{
			SetVector((S100Geometry::CompositeCurve*)&r->component.at(i), pProperty);
		}
	}
}

//void CDockablePaneEditWindow::SetVector(R_CompositeRecord* r, CMFCPropertyGridProperty *pSuperProperty)
//{
//	int inasCount = (int)r->m_inas.size();
//	CString count;
//	count.Format(L"%d", inasCount);
//
//	S101Cell* cell = (S101Cell*)m_cell;
//	std::wstring comName = L"Composite Curve (RCID : " + r->GetRCIDasWstring() + L", Relation: " + std::wstring(count) + L")";
//	CMFCPropertyGridProperty *pProperty = new CMFCPropertyGridProperty(comName.c_str());
//	if (pSuperProperty)
//	{
//		pSuperProperty->AddSubItem(pProperty);
//	}
//	else
//	{
//		m_wndListVector.AddProperty(pProperty);
//	}
//
//	if (inasCount > 0) // If there's even one number,
//	{
//		CMFCPropertyGridProperty *pInasParent = new CMFCPropertyGridProperty(L"INAS");
//
//		pProperty->AddSubItem(pInasParent);
//		for (F_INAS* inas : r->m_inas)
//		{
//			std::wstring inasName = L"Information id";
//			CMFCPropertyGridProperty *pInas = new CMFCPropertyGridProperty(inasName.c_str(), inas->m_name.GetRCIDasWstring().c_str());
//			pInasParent->AddSubItem(pInas);
//
//			MultiData *multiData = InsertPropertyMultiData(200, pInasParent, (DWORD_PTR)inas);
//
//			pInas->SetName(inasName.c_str());
//			pInas->SetData((DWORD_PTR)multiData);
//			pInas->AllowEdit(FALSE);
//
//		}
//	}
//
//	for (auto itorParent = r->m_cuco.begin(); itorParent != r->m_cuco.end(); itorParent++)
//	{
//		F_CUCO* cucoParent = *itorParent;
//
//		for (auto itor = cucoParent->m_arr.begin(); itor != cucoParent->m_arr.end(); itor++)
//		{
//			CUCO* cuco = *itor;
//			R_VectorRecord* vectorRecord = nullptr;
//
//			if (120 == cuco->m_name.RCNM)
//			{
//				vectorRecord = cell->GetCurveRecord(cuco->m_name.GetName());
//			}
//			else if (125 == cuco->m_name.RCNM)
//			{
//				vectorRecord = cell->GetCurveRecord(cuco->m_name.GetName());	
//			}
//
//			if (vectorRecord)
//			{
//				SetVector(cuco->m_name.RCNM, vectorRecord, pProperty);
//			}
//		}
//	}
//}

void CDockablePaneEditWindow::SetVector(S100Geometry::Surface* r, CMFCPropertyGridProperty* pSuperProperty)
{
	int infoCount = (int)r->GetInformationRelationCount();
	CString count;
	count.Format(L"%d", infoCount);

	std::wstring surName = L"Surface (RCID: " + r->GetIDAsWString() + L", Relation: " + std::wstring(count) + L")";

	CMFCPropertyGridProperty* pProperty = new CMFCPropertyGridProperty(surName.c_str());

	if (pSuperProperty)
	{
		pSuperProperty->AddSubItem(pProperty);
	}
	else
	{
		m_wndListVector.AddProperty(pProperty);
	}

	if (infoCount > 0) // If there's even one number,
	{
		for (int i = 0; i < infoCount; i++)
		{
			auto infoID = r->GetAssociatedInformationID(i);
			auto info = m_cell->GetInformationType(infoID);

			if (info)
			{
				CMFCPropertyGridProperty* pInasParent = new CMFCPropertyGridProperty(L"INAS");

				pProperty->AddSubItem(pInasParent);

				for (int i = 0; i < infoCount; i++)
				{
					std::wstring inasName = L"Information id";
					CMFCPropertyGridProperty* pInas = new CMFCPropertyGridProperty(inasName.c_str(), info->GetIDAsWString().c_str());
					pInasParent->AddSubItem(pInas);

					pInas->SetName(inasName.c_str());
					pInas->AllowEdit(FALSE);
				}
			}
		}
	}

	auto exteriorCompositeCurve = (S100Geometry::CompositeCurve*)&r->GetPolygon()->boundary.exterior;
	SetVector(exteriorCompositeCurve, pProperty);

	for (int i = 0; i < r->GetPolygon()->boundary.interior.size(); i++)
	{
		auto interiorCompositeCurve = (S100Geometry::CompositeCurve*)&r->GetPolygon()->boundary.interior.at(i);
		SetVector(interiorCompositeCurve, pProperty);
	}
}

//void CDockablePaneEditWindow::SetVector(R_SurfaceRecord* r, CMFCPropertyGridProperty *pSuperProperty)
//{
//	int inasCount = (int)r->m_inas.size();
//	CString count;
//	count.Format(L"%d", inasCount);
//
//	S101Cell* cell = (S101Cell*)m_cell;
//	std::wstring surName = L"Surface (RCID: " + r->GetRCIDasWstring() + L", Relation: " + std::wstring(count) + L")";
//
//	CMFCPropertyGridProperty *pProperty = new CMFCPropertyGridProperty(surName.c_str());
//
//	if (pSuperProperty)
//	{
//		pSuperProperty->AddSubItem(pProperty);
//	}
//	else
//	{
//		m_wndListVector.AddProperty(pProperty);
//	}
//
//	if (inasCount > 0) // If there's even one number,
//	{
//		CMFCPropertyGridProperty *pInasParent = new CMFCPropertyGridProperty(L"INAS");
//
//		pProperty->AddSubItem(pInasParent);
//		for (F_INAS* inas : r->m_inas)
//		{
//			std::wstring inasName = L"Information id";
//			CMFCPropertyGridProperty *pInas = new CMFCPropertyGridProperty(inasName.c_str(), inas->m_name.GetRCIDasWstring().c_str());
//			pInasParent->AddSubItem(pInas);
//
//			MultiData *multiData = InsertPropertyMultiData(200, pInasParent, (DWORD_PTR)inas);
//
//			pInas->SetName(inasName.c_str());
//			pInas->SetData((DWORD_PTR)multiData);
//			pInas->AllowEdit(FALSE);
//
//		}
//	}
//
//	for (auto itorParent = r->m_rias.begin(); itorParent != r->m_rias.end(); itorParent++)
//	{
//		F_RIAS* riasParent = *itorParent;
//
//		for (auto itor = riasParent->m_arr.begin(); itor != riasParent->m_arr.end(); itor++)
//		{
//			RIAS* rias = *itor;
//
//			R_VectorRecord* vectorRecord = nullptr;
//
//			if (120 == rias->m_name.RCNM)
//			{
//				vectorRecord = cell->GetCurveRecord(rias->m_name.GetName());
//			}
//			else if (125 == rias->m_name.RCNM)
//			{
//				vectorRecord = cell->GetCompositeCurveRecord(rias->m_name.GetName());
//			}
//
//			if (vectorRecord)
//			{
//				SetVector(rias->m_name.RCNM, vectorRecord, pProperty);
//			}
//		}
//	}
//}

void CDockablePaneEditWindow::SetFeatureList()
{
	DeleteAllItems();

	CMainFrame *pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
}

void CDockablePaneEditWindow::DeleteAllItems()
{
	DeleteAttributeItems();
	DeleteVectorItems();
}

void CDockablePaneEditWindow::DeleteAttributeItems()
{
	while (m_wndListAttribute.GetPropertyCount() > 0)
	{
		CMFCPropertyGridProperty* prop = m_wndListAttribute.GetProperty(0);
		m_wndListAttribute.DeleteProperty(prop);
	}

	for (auto itor = m_propertyAttributeMultiData.begin();
		itor != m_propertyAttributeMultiData.end();
		itor++)
	{
		delete itor->second;
	}
	m_propertyAttributeMultiData.clear();
}
void CDockablePaneEditWindow::DeleteVectorItems()
{
	while (m_wndListVector.GetPropertyCount() > 0)
	{
		CMFCPropertyGridProperty* prop = m_wndListVector.GetProperty(0);
		m_wndListVector.DeleteProperty(prop);
	}

	if (m_wndListVector)
		m_wndListVector.RemoveAll();

	for (auto itor = m_propertyVectorMultiData.begin();
		itor != m_propertyVectorMultiData.end();
		itor++)
	{
		delete itor->second;
	}
	m_propertyVectorMultiData.clear();
}

BOOL CDockablePaneEditWindow::PreTranslateMessage(MSG* pMsg)
{
	//HWND thisHandle = this->m_hWnd;
	//HWND attributeHandle = m_wndListAttribute.m_hWnd;

	//int index = 0;
	//R_VectorRecord* vr = NULL;
	//IC2D* ic = NULL;
	//F_C2IL* fic = NULL;
	//F_C2IT* it = NULL;

	return CDockablePane::PreTranslateMessage(pMsg);
}

MultiData* CDockablePaneEditWindow::InsertPropertyMultiData(
	int multidataType,
	CMFCPropertyGridProperty* pGP,
	DWORD_PTR pointer_1,
	DWORD_PTR pointer_2,
	DWORD_PTR pointer_3,
	DWORD_PTR pointer_4)
{
	MultiData* multiData = new MultiData();

	if (multidataType % 100 > 10)
	{
		m_propertyAttributeMultiData.insert
		(
			std::unordered_map<CMFCPropertyGridProperty*, MultiData*>::value_type(pGP, multiData)
		);
	}
	else
	{
		m_propertyVectorMultiData.insert
		(
			std::unordered_map<CMFCPropertyGridProperty*, MultiData*>::value_type(pGP, multiData)
		);
	}

	multiData->type = multidataType;
	multiData->data.push_back((DWORD_PTR)m_cell);
	multiData->data.push_back((DWORD_PTR)pointer_1);
	multiData->data.push_back((DWORD_PTR)pointer_2);
	multiData->data.push_back((DWORD_PTR)pointer_3);
	multiData->data.push_back((DWORD_PTR)pointer_4);

	return multiData;
}

void CDockablePaneEditWindow::SetSpatialObject(S100SpatialObject *object)
{
	m_cell = object;
	//m_wndListAttribute.SetSpatialObject(m_cell);
}

//SpatialObject *CDockablePaneEditWindow::GetSpatialObject()
//{
//	return m_cell;
//}

//S101Cell* CDockablePaneEditWindow::GetS101Cell()
//{
//	return (S101Cell*)m_cell;
//}

//CString CDockablePaneEditWindow::GetActiveTabName()
//{
//	int tabNum = m_wndTabs.GetActiveTab();
//
//	CString tabName;
//
//	m_wndTabs.GetTabLabel(tabNum, tabName);
//
//	return tabName;
//}

//bool CDockablePaneEditWindow::IsAttributeTab()
//{
//	int tabNum = m_wndTabs.GetActiveTab();
//
//	CString tabName;
//
//	m_wndTabs.GetTabLabel(tabNum, tabName);
//
//	if (0 == tabName.Compare(tabNameAttribute))
//	{
//		return true;
//	}
//
//	return false;
//}

void CDockablePaneEditWindow::ClearVector()
{
	m_wndListVector.ShowWindow(FALSE);
	DeleteVectorItems();
	m_wndListVector.ShowWindow(TRUE);
}