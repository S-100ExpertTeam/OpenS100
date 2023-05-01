#include "pch.h"
#include "MainFrm.h"
#include "DockablePaneEditWindow.h"
#include "resource.h"
#include "OpenS100.h"
#include "OpenS100View.h"

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
//#include "../GISLibrary/R_InformationRecord.h"
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
	//ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)

END_MESSAGE_MAP()

// CFeatureView message handlers
//LRESULT CDockablePaneEditWindow::OnPropertyChanged(WPARAM wparam, LPARAM lparam)
//{
//	if (m_cell->m_FileType == FILE_S_100_VECTOR)
//	{
//		S101Cell* cell = (S101Cell*)m_cell;
//
//		UINT nID = (UINT)wparam;
//		CMFCPropertyGridProperty *pProp = (CMFCPropertyGridProperty*)lparam;
//
//		if (!pProp)
//			return 0;
//
//		if (nID == IDC_PROPERTY_GRID_VECTOR)
//		{
//			std::wstring name = pProp->GetName();
//			CString value = pProp->GetValue().bstrVal;
//
//			CString xValue, yValue;
//
//			AfxExtractSubString(xValue, value, 0, ',');
//			AfxExtractSubString(yValue, value, 1, ',');
//
//			int x = (int)(_wtof(xValue) * cell->m_dsgir.m_dssi.m_cmfx);
//			int y = (int)(_wtof(yValue) * cell->m_dsgir.m_dssi.m_cmfy);
//
//
//		}
//		else if (nID == IDC_PROPERTY_GRID_ATTRIBUTE)
//		{
//			MultiData* md = (MultiData*)pProp->GetData();
//			ATTR* attr = (ATTR*)md->data[2];
//
//			if (!attr)
//			{
//				AfxMessageBox(L"ERROR");
//				return 0;
//			}
//
//			auto fc = ((S100Layer*)cell->m_pLayer)->GetFeatureCatalog();
//			if (nullptr == fc)
//			{
//				return 0;
//			}
//
//			auto aitor = cell->m_dsgir.m_atcs->m_arr.find(attr->m_natc);
//
//			SimpleAttribute* sa = fc->GetSimpleAttribute(aitor->second->m_code.GetBuffer());
//
//			if (sa)
//			{
//				if (sa->GetValueType() == FCD::S100_CD_AttributeValueType::enumeration)
//				{
//					std::wstring propValue = pProp->GetValue().bstrVal;
//
//					int numindex = (int)(propValue.find('.')) + 1;
//					if (numindex > 0)
//					{
//						propValue.erase(propValue.begin(), propValue.begin() + numindex);
//					}
//
//					auto listedValue = sa->GetListedValue(propValue);
//					if (listedValue)
//					{
//						attr->m_atvl = propValue.c_str();
//					}
//				}
//				else
//				{
//					attr->m_atvl = pProp->GetValue().bstrVal;
//				}
//			}
//
//			theApp.gisLib->BuildPortrayalCatalogue(cell->m_pLayer);
//			theApp.pView->MapRefresh();
//		}
//	}
//	return 0;
//}

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

	if (m_cell->GetProductNumber() == 101)
	{
		S101Cell* cell = (S101Cell*)m_cell;

		auto s100Layer = (S100Layer*)m_cell->GetLayer();
		auto fc = s100Layer->GetFeatureCatalog();
		if (nullptr == fc)
		{
			return;
		}

		if (m_selectedObjectType == L"Feature")
		{
			auto feature = m_cell->GetFeatureType(selectedID);
			if (nullptr == feature)
			{
				return;
			}

			auto m_pFeature = cell->GetFeatureRecord(selectedFeatureID);
			if (m_pFeature->m_attr.size() == 0)
			{
				return;
			}

			int numAttribute = feature->GetAttributeCount();
			for (int i = 0; i < numAttribute; i++)
			{
				auto value = feature->GetAttributeValue(i);
				OutputDebugString(pugi::as_wide(value).c_str());
				OutputDebugString(L"\n");
			}

			for (auto itorParent = m_pFeature->m_attr.begin(); itorParent != m_pFeature->m_attr.end(); itorParent++)
			{
				F_ATTR* attrParent = *itorParent;
				for (auto itor = attrParent->m_arr.begin(); itor != attrParent->m_arr.end(); itor++)
				{
					ATTR* attr = *itor;

					auto aitor = cell->m_dsgir.m_atcs->m_arr.find(attr->m_natc);
					SimpleAttribute* sa = fc->GetSimpleAttribute(std::wstring(aitor->second->m_code));

					if (sa)
					{
						std::wstring value;
						if (sa->GetValueType() == FCD::S100_CD_AttributeValueType::enumeration)
						{
							auto attributeValue = _wtoi(attr->m_atvl);

							auto listedValue = sa->GetListedValue(attributeValue);
							if (listedValue)
							{
								auto result = attr->m_atvl;
								result.Append(L".");
								result.Append(listedValue->GetLabel().c_str());
								value = result;
							}
						}
						else
						{
							value = attr->m_atvl;
						}

						CMFCPropertyGridProperty *pAttribute = new CMFCPropertyGridProperty(sa->GetName().c_str(), value.c_str());
						
						pAttrItemList.push_back(pAttribute);
						pAttribute->SetDescription(sa->GetDefinition().c_str());
						MultiData *multiData = InsertPropertyMultiData(111, pAttribute, (DWORD_PTR)m_pFeature, (DWORD_PTR)attr, 0);
						pAttribute->SetData((DWORD_PTR)multiData);

						if (attr->m_paix != 0)
						{
							if (pAttrItemList.size() < 1)
							{
								CString msg;
								msg.Format(L"[%s] parent[index : %d] attribute not found - EDIT WINDOW", sa->GetName().c_str(), attr->m_paix);
								continue;
							}
							else
							{

								pAttrItemList[attr->m_paix - 1]->AddSubItem(pAttribute);
							}
						}
						else
						{
							pAttribute->AllowEdit(FALSE);
							m_wndListAttribute.AddProperty(pAttribute);
						}

						if (sa->GetValueType() == FCD::S100_CD_AttributeValueType::enumeration)
						{
							SettingEnumType(sa->GetListedValuePointer(), pAttribute);
						}
					}
					else  //Add a complex.
					{
						ComplexAttribute* ca = fc->GetComplexAttribute(aitor->second->m_code.GetBuffer());
						if (ca)
						{
							CMFCPropertyGridProperty *pAttribute = new CMFCPropertyGridProperty(ca->GetName().c_str());
							
							pAttrItemList.push_back(pAttribute);
							pAttribute->SetDescription(ca->GetDefinition().c_str());

							MultiData *multiData = InsertPropertyMultiData(111, pAttribute, (DWORD_PTR)m_pFeature, (DWORD_PTR)attr, 0);
							pAttribute->SetData((DWORD_PTR)multiData);

							if (attr->m_paix != 0)
							{
								if (pAttrItemList[attr->m_paix - 1]->IsGroup()) //Modifications: Add as Attribute
								{
									pAttrItemList[attr->m_paix - 1]->AddSubItem(pAttribute);
								}
								else
								{
									pAttrItemList[attr->m_paix]->AddSubItem(pAttribute);
								}
							}
							else
							{
								//Simply add attributes.
								pAttribute->AllowEdit(FALSE);
								m_wndListAttribute.AddProperty(pAttribute);
							}
						}
						else
						{
							CString msg;
							msg.Format(L"[%s] attribute not exist in feature catalog - EDIT WINDOW", aitor->second->m_code);

							CMFCPropertyGridProperty *pAttribute = new CMFCPropertyGridProperty(aitor->second->m_code);
							
							pAttrItemList.push_back(pAttribute);

							MultiData *multiData = InsertPropertyMultiData(111, pAttribute, (DWORD_PTR)m_pFeature, (DWORD_PTR)attr, 0);
							pAttribute->SetData((DWORD_PTR)multiData);

							if (attr->m_paix != 0)
							{
								pAttrItemList[attr->m_paix]->AddSubItem(pAttribute);
							}
							else
							{
								pAttribute->AllowEdit(FALSE);
								m_wndListAttribute.AddProperty(pAttribute);
							}
						}
					}
				}
			}
		}
		else if (m_selectedObjectType = L"Information")
		{
			auto id = pugi::as_utf8(selectedInformationID);

			auto info = cell->GetInformationType(id);

			if (info)
			{
				int attrCnt = info->GetAttributeCount();
				for (int i = 0; i < attrCnt; i++)
				{
					auto value = info->GetAttributeValue(i);
					auto code = cell->GetInformationAttributeCode(id, i);

					auto sa = fc->GetSimpleAttribute(code);
					if (sa)
					{
						if (sa->GetValueType() == FCD::S100_CD_AttributeValueType::enumeration)
						{
							sa->GetListedValue()
						}
						else
						{

						}
					}
					else
					{
						auto ca = fc->GetComplexAttribute(code);
						if (ca)
						{
						}
					}
				}
			}



			//auto m_pInformation = cell->GetInformationRecord(selectedInformationID);

			// If the size is 0, pass.
			//if (m_pInformation == nullptr ||
			//	m_pInformation->m_attr.size() == 0)
			//{
			//	return;
			//}

			//for (auto itorParent = m_pInformation->m_attr.begin(); itorParent != m_pInformation->m_attr.end(); itorParent++)
			//{
			//	F_ATTR* attrParent = *itorParent;

			//	for (auto itor = attrParent->m_arr.begin(); itor != attrParent->m_arr.end(); itor++)
			//	{
					ATTR* attr = *itor;

					CString value = L"";
					{
						value = attr->m_atvl;
					}

					auto aitor = cell->m_dsgir.m_atcs->m_arr.find(attr->m_natc);

					SimpleAttribute* sa = fc->GetSimpleAttribute(aitor->second->m_code.GetBuffer());

					if (sa) //simple attribute type
					{
						std::wstring value;
						if (sa->GetValueType() == FCD::S100_CD_AttributeValueType::enumeration)
						{
							auto listedValue = sa->GetListedValue(std::wstring(attr->m_atvl));

							if (listedValue)
							{
								value = listedValue->GetLabel().c_str();
							}
						}
						else
						{
							value = attr->m_atvl;
						}

						CMFCPropertyGridProperty *pAttribute = new CMFCPropertyGridProperty(sa->GetName().c_str(), value.c_str());
						
						pAttrItemList.push_back(pAttribute);
						pAttribute->SetDescription(sa->GetDefinition().c_str());

						MultiData *multiData = InsertPropertyMultiData(111, pAttribute, (DWORD_PTR)m_pInformation, (DWORD_PTR)attr, 1);
						pAttribute->SetData((DWORD_PTR)multiData);

						if (attr->m_paix != 0)
						{
							pAttrItemList[attr->m_paix - 1]->AddSubItem(pAttribute);
						}
						else
						{
							pAttribute->AllowEdit(FALSE);
							m_wndListAttribute.AddProperty(pAttribute);
						}

						//if (sa->GetValueType() == FCD::S100_CD_AttributeValueType::enumeration)
						//{
						//	for (auto itor = sa->GetListedValuePointer().begin(); itor != sa->GetListedValuePointer().end(); itor++)
						//	{
						//		ListedValue* lv = *itor;

						//		pAttribute->AddOption(lv->GetLabel().c_str());			
						//	}
						//}
					}
					else
					{
						ComplexAttribute* ca = fc->GetComplexAttribute(std::wstring(aitor->second->m_code));
						//ComplexAttribute type
						if (ca)
						{
							CMFCPropertyGridProperty *pAttribute = new CMFCPropertyGridProperty(ca->GetName().c_str());
							
							pAttrItemList.push_back(pAttribute);
							pAttribute->SetDescription(ca->GetDefinition().c_str());
							MultiData *multiData = InsertPropertyMultiData(111, pAttribute, (DWORD_PTR)m_pInformation, (DWORD_PTR)attr, 1);
							pAttribute->SetData((DWORD_PTR)multiData);

							if (attr->m_paix != 0)
							{
								pAttrItemList[attr->m_paix]->AddSubItem(pAttribute);
							}
							else
							{
								pAttribute->AllowEdit(FALSE);
								m_wndListAttribute.AddProperty(pAttribute);
							}
						}
					}
			//	}
			//}

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
	if (101 == s100Layer->GetProductNumber())
	{
		auto cell = (S101Cell*)m_cell;
		auto m_pFeature = cell->GetFeatureRecord(selectedFeatureID);

		m_wndListVector.ShowWindow(FALSE);
		DeleteVectorItems();

		std::wstring str;
		R_PointRecord* pr = NULL;
		R_MultiPointRecord* mpr = NULL;
		R_CurveRecord* cr = NULL;
		R_CompositeRecord* ccr = NULL;
		R_SurfaceRecord* sr = NULL;

		if (cell == nullptr || m_pFeature == nullptr)
		{
			m_wndListVector.ShowWindow(TRUE);
			return;
		}
		if (m_cell->m_FileType == FILE_S_100_VECTOR)
		{
			S101Cell* cell = (S101Cell*)m_cell;

			for (auto itorParent = m_pFeature->m_spas.begin(); itorParent != m_pFeature->m_spas.end(); itorParent++)
			{
				F_SPAS* spasParent = *itorParent;

				for (auto itor = spasParent->m_arr.begin(); itor != spasParent->m_arr.end(); itor++)
				{
					SPAS* spas = *itor;

					switch (spas->m_name.RCNM)
					{
					case 110:
						pr = cell->GetPointRecord(spas->m_name.GetName());
						SetVector(pr);
						break;
					case 115:
						mpr = cell->GetMultiPointRecord(spas->m_name.GetName());
						SetVector(mpr);
						break;
					case 120:
						cr = cell->GetCurveRecord(spas->m_name.GetName());
						SetVector(cr);
						break;
					case 125:
						ccr = cell->GetCompositeCurveRecord(spas->m_name.GetName());
						SetVector(ccr);
						break;
					case 130:
						sr = cell->GetSurfaceRecord(spas->m_name.GetName());
						SetVector(sr);
						break;
					}
				}
			}
		}
		m_wndListVector.ExpandAll(FALSE);
		m_wndListVector.ShowWindow(TRUE);
	}
}

void CDockablePaneEditWindow::SetVector(int RCNM, R_VectorRecord* r, CMFCPropertyGridProperty *pSuperProperty)
{
	switch (RCNM)
	{
	case 110:
		SetVector((R_PointRecord*)r, pSuperProperty);
		break;
	case 115:
		SetVector((R_MultiPointRecord*)r, pSuperProperty);
		break;
	case 120:
		SetVector((R_CurveRecord*)r, pSuperProperty);
		break;
	case 125:
		SetVector((R_CompositeRecord*)r, pSuperProperty);
		break;
	case 130:
		SetVector((R_SurfaceRecord*)r, pSuperProperty);
		break;
	}
}

void CDockablePaneEditWindow::SetVector(R_PointRecord* r, CMFCPropertyGridProperty *pSuperProperty)
{
	int inasCount = (int)r->m_inas.size();
	CString count;
	count.Format(L"%d", inasCount);

	std::wstring pointName = L"Point (RCID : " + r->GetRCIDasWstring() + L", Relation: " + std::wstring(count) + L")";
	CMFCPropertyGridProperty *pProperty = new CMFCPropertyGridProperty(pointName.c_str());
	if (pSuperProperty)
	{
		pSuperProperty->AddSubItem(pProperty);
	}
	else
	{
		m_wndListVector.AddProperty(pProperty);
	}

	if (inasCount > 0) // If there's even one number,
	{
		CMFCPropertyGridProperty *pInasParent = new CMFCPropertyGridProperty(L"INAS");

		pProperty->AddSubItem(pInasParent);
		for (F_INAS* inas : r->m_inas)
		{
			std::wstring inasName = L"Information id";
			CMFCPropertyGridProperty *pInas = new CMFCPropertyGridProperty(inasName.c_str(), inas->m_name.GetRCIDasWstring().c_str());
			pInasParent->AddSubItem(pInas);

			MultiData *multiData = InsertPropertyMultiData(200, pInasParent, (DWORD_PTR)inas);

			pInas->SetName(inasName.c_str());
			pInas->SetData((DWORD_PTR)multiData);
			pInas->AllowEdit(FALSE);
		}
	}

	S101Cell* cell = (S101Cell*)m_cell;
	int precision_x = 0;
	int cmf = cell->m_dsgir.m_dssi.m_cmfx;
	while (cmf >= 10)
	{
		cmf /= 10;
		precision_x++;
	}

	int precision_y = 0;
	cmf = cell->m_dsgir.m_dssi.m_cmfy;
	while (cmf >= 10)
	{
		cmf /= 10;
		precision_y++;
	}

	if (r->m_c2it)
	{
		std::wstringstream wss;
		wss.setf(std::ios::fixed, std::ios::floatfield);
		double x = r->m_c2it->m_xcoo / (double)cell->m_dsgir.m_dssi.m_cmfx;
		wss.precision(precision_x);
		double y = r->m_c2it->m_ycoo / (double)cell->m_dsgir.m_dssi.m_cmfy;
		wss.precision(precision_y);

		wss << y << L"," << x;

		CMFCPropertyGridProperty *pChildProperty = new CMFCPropertyGridProperty(L"Position", wss.str().c_str());
		pProperty->AddSubItem(pChildProperty);

		pChildProperty->SetName(L"C2IT");
		MultiData *multiData = InsertPropertyMultiData(101, pChildProperty, (DWORD_PTR)r, (DWORD_PTR)r->m_c2it);
		pChildProperty->SetData((DWORD_PTR)multiData);
	}
}

void CDockablePaneEditWindow::SetVector(R_MultiPointRecord* r, CMFCPropertyGridProperty *pSuperProperty)
{

	int inasCount = (int)r->m_inas.size();
	CString count;
	count.Format(L"%d", inasCount);

	std::wstring multpointName = L"MultiPoint (RCID : " + r->GetRCIDasWstring() + L", Relation: " + std::wstring(count) + L")";
	CMFCPropertyGridProperty *pProperty = new CMFCPropertyGridProperty(multpointName.c_str());
	if (pSuperProperty)
	{
		pSuperProperty->AddSubItem(pProperty);
	}
	else
	{
		m_wndListVector.AddProperty(pProperty);
	}

	if (inasCount > 0) // If there's even one number,
	{
		CMFCPropertyGridProperty *pInasParent = new CMFCPropertyGridProperty(L"INAS");

		pProperty->AddSubItem(pInasParent);
		for (F_INAS* inas : r->m_inas)
		{
			std::wstring inasName = L"Information id";
			CMFCPropertyGridProperty *pInas = new CMFCPropertyGridProperty(inasName.c_str(), inas->m_name.GetRCIDasWstring().c_str());
			pInasParent->AddSubItem(pInas);

			MultiData *multiData = InsertPropertyMultiData(200, pInasParent, (DWORD_PTR)inas);

			pInas->SetName(inasName.c_str());
			pInas->SetData((DWORD_PTR)multiData);
			pInas->AllowEdit(FALSE);

		}
	}

	S101Cell* cell = (S101Cell*)m_cell;
	int precision_x = 0;
	int cmf = cell->m_dsgir.m_dssi.m_cmfx;
	while (cmf >= 10)
	{
		cmf /= 10;
		precision_x++;
	}

	int precision_y = 0;
	cmf = cell->m_dsgir.m_dssi.m_cmfy;
	while (cmf >= 10)
	{
		cmf /= 10;
		precision_y++;
	}

	int precision_z = 0;
	cmf = cell->m_dsgir.m_dssi.m_cmfz;
	while (cmf >= 10)
	{
		cmf /= 10;
		precision_z++;
	}

	for (auto itor = r->m_c3il.begin(); itor != r->m_c3il.end(); itor++)
	{
		for (auto itor2 = (*itor)->m_arr.begin(); itor2 != (*itor)->m_arr.end(); itor2++)
		{
			C3IL* c3 = *itor2;
			std::wstringstream wss;
			wss.setf(std::ios::fixed, std::ios::floatfield);
			double x = c3->m_xcoo / (double)cell->m_dsgir.m_dssi.m_cmfx;
			wss.precision(precision_x);
			wss << x << L",";
			double y = c3->m_ycoo / (double)cell->m_dsgir.m_dssi.m_cmfy;
			wss.precision(precision_y);
			wss << y << L",";
			double z = c3->m_zcoo / (double)cell->m_dsgir.m_dssi.m_cmfz;
			wss.precision(precision_z);
			wss << z;

			CMFCPropertyGridProperty *pChildProperty = new CMFCPropertyGridProperty(L"Position", wss.str().c_str());
			pProperty->AddSubItem(pChildProperty);

			pChildProperty->SetName(L"C3IL");
			MultiData *multiData = InsertPropertyMultiData(102, pChildProperty, (DWORD_PTR)r, (DWORD_PTR)c3, (DWORD_PTR)*itor, m_vIndex++);
			pChildProperty->SetData((DWORD_PTR)multiData);

		}
	}
}

void CDockablePaneEditWindow::SetVector(R_CurveRecord* r, CMFCPropertyGridProperty *pSuperProperty)
{
	int inasCount = (int)r->m_inas.size();
	CString count;
	count.Format(L"%d", inasCount);

	S101Cell* cell = (S101Cell*)m_cell;

	std::wstring curveName = L"Curve (RCID : " + r->GetRCIDasWstring() + L", Relation: " + std::wstring(count) + L")";

	CMFCPropertyGridProperty *pProperty = new CMFCPropertyGridProperty(curveName.c_str());

	if (pSuperProperty)
	{
		pSuperProperty->AddSubItem(pProperty);
	}
	else
	{
		m_wndListVector.AddProperty(pProperty);
	}

	if (inasCount > 0) // If there's even one number,
	{
		CMFCPropertyGridProperty *pInasParent = new CMFCPropertyGridProperty(L"INAS");
		pProperty->AddSubItem(pInasParent);

		for (F_INAS* inas : r->m_inas)
		{
			std::wstring inasName = L"Information id";
			CMFCPropertyGridProperty *pInas = new CMFCPropertyGridProperty(inasName.c_str(), inas->m_name.GetRCIDasWstring().c_str());
			pInasParent->AddSubItem(pInas);

			MultiData *multiData = InsertPropertyMultiData(200, pInasParent, (DWORD_PTR)inas);

			pInas->SetName(inasName.c_str());
			pInas->SetData((DWORD_PTR)multiData);
			pInas->AllowEdit(FALSE);

		}
	}

	POSITION ptasPos = NULL;
	PTAS *ptas = NULL;
	R_PointRecord *spr = NULL, *epr = NULL;
	__int64 iKey;

	for (auto i = r->m_ptas->m_arr.begin(); i != r->m_ptas->m_arr.end(); i++)
	{
		auto ptas = *i;

		iKey = ((__int64)ptas->m_name.RCNM) << 32 | ptas->m_name.RCID;
		if (ptas->m_topi == 1 // Beginning node 
			)
		{
			spr = cell->GetPointRecord(iKey);
		}
		else if (ptas->m_topi == 2 // End node
			)
		{
			epr = cell->GetPointRecord(iKey);
		}
		else if (ptas->m_topi == 3)
		{
			spr = cell->GetPointRecord(iKey);
			epr = spr;
		}
	}

	SetVector(spr, pProperty);

	int precision_x = 0;
	int cmf = cell->m_dsgir.m_dssi.m_cmfx;
	while (cmf >= 10)
	{
		cmf /= 10;
		precision_x++;
	}

	int precision_y = 0;
	cmf = cell->m_dsgir.m_dssi.m_cmfy;
	while (cmf >= 10)
	{
		cmf /= 10;
		precision_y++;
	}

	for (auto itorParent = r->m_c2il.begin(); itorParent != r->m_c2il.end(); itorParent++)
	{
		int index = 0;
		for (auto itor = (*itorParent)->m_arr.begin(); itor != (*itorParent)->m_arr.end(); itor++)
		{
			IC2D* ic = *itor;

			std::wstringstream wss;
			wss.setf(std::ios::fixed, std::ios::floatfield);

			double x = ic->m_xcoo / (double)cell->m_dsgir.m_dssi.m_cmfx;
			wss.precision(precision_x);

			double y = ic->m_ycoo / (double)cell->m_dsgir.m_dssi.m_cmfy;
			wss.precision(precision_y);

			wss << y << L"," << x;

			CMFCPropertyGridProperty *pChildProperty = new CMFCPropertyGridProperty(L"Position", wss.str().c_str());
			pProperty->AddSubItem(pChildProperty);
			pChildProperty->SetName(L"C2IL");
			MultiData *multiData = InsertPropertyMultiData(100, pChildProperty, (DWORD_PTR)r, (DWORD_PTR)ic, (DWORD_PTR)*itorParent, m_vIndex++);
			pChildProperty->SetData((DWORD_PTR)multiData);
		}
	}

	SetVector(epr, pProperty);
}

void CDockablePaneEditWindow::SetVector(R_CompositeRecord* r, CMFCPropertyGridProperty *pSuperProperty)
{
	int inasCount = (int)r->m_inas.size();
	CString count;
	count.Format(L"%d", inasCount);

	S101Cell* cell = (S101Cell*)m_cell;
	std::wstring comName = L"Composite Curve (RCID : " + r->GetRCIDasWstring() + L", Relation: " + std::wstring(count) + L")";
	CMFCPropertyGridProperty *pProperty = new CMFCPropertyGridProperty(comName.c_str());
	if (pSuperProperty)
	{
		pSuperProperty->AddSubItem(pProperty);
	}
	else
	{
		m_wndListVector.AddProperty(pProperty);
	}

	if (inasCount > 0) // If there's even one number,
	{
		CMFCPropertyGridProperty *pInasParent = new CMFCPropertyGridProperty(L"INAS");

		pProperty->AddSubItem(pInasParent);
		for (F_INAS* inas : r->m_inas)
		{
			std::wstring inasName = L"Information id";
			CMFCPropertyGridProperty *pInas = new CMFCPropertyGridProperty(inasName.c_str(), inas->m_name.GetRCIDasWstring().c_str());
			pInasParent->AddSubItem(pInas);

			MultiData *multiData = InsertPropertyMultiData(200, pInasParent, (DWORD_PTR)inas);

			pInas->SetName(inasName.c_str());
			pInas->SetData((DWORD_PTR)multiData);
			pInas->AllowEdit(FALSE);

		}
	}

	for (auto itorParent = r->m_cuco.begin(); itorParent != r->m_cuco.end(); itorParent++)
	{
		F_CUCO* cucoParent = *itorParent;

		for (auto itor = cucoParent->m_arr.begin(); itor != cucoParent->m_arr.end(); itor++)
		{
			CUCO* cuco = *itor;
			R_VectorRecord* vectorRecord = nullptr;

			if (120 == cuco->m_name.RCNM)
			{
				vectorRecord = cell->GetCurveRecord(cuco->m_name.GetName());
			}
			else if (125 == cuco->m_name.RCNM)
			{
				vectorRecord = cell->GetCurveRecord(cuco->m_name.GetName());	
			}

			if (vectorRecord)
			{
				SetVector(cuco->m_name.RCNM, vectorRecord, pProperty);
			}
		}
	}
}

void CDockablePaneEditWindow::SetVector(R_SurfaceRecord* r, CMFCPropertyGridProperty *pSuperProperty)
{
	int inasCount = (int)r->m_inas.size();
	CString count;
	count.Format(L"%d", inasCount);

	S101Cell* cell = (S101Cell*)m_cell;
	std::wstring surName = L"Surface (RCID: " + r->GetRCIDasWstring() + L", Relation: " + std::wstring(count) + L")";

	CMFCPropertyGridProperty *pProperty = new CMFCPropertyGridProperty(surName.c_str());

	if (pSuperProperty)
	{
		pSuperProperty->AddSubItem(pProperty);
	}
	else
	{
		m_wndListVector.AddProperty(pProperty);
	}

	if (inasCount > 0) // If there's even one number,
	{
		CMFCPropertyGridProperty *pInasParent = new CMFCPropertyGridProperty(L"INAS");

		pProperty->AddSubItem(pInasParent);
		for (F_INAS* inas : r->m_inas)
		{
			std::wstring inasName = L"Information id";
			CMFCPropertyGridProperty *pInas = new CMFCPropertyGridProperty(inasName.c_str(), inas->m_name.GetRCIDasWstring().c_str());
			pInasParent->AddSubItem(pInas);

			MultiData *multiData = InsertPropertyMultiData(200, pInasParent, (DWORD_PTR)inas);

			pInas->SetName(inasName.c_str());
			pInas->SetData((DWORD_PTR)multiData);
			pInas->AllowEdit(FALSE);

		}
	}

	for (auto itorParent = r->m_rias.begin(); itorParent != r->m_rias.end(); itorParent++)
	{
		F_RIAS* riasParent = *itorParent;

		for (auto itor = riasParent->m_arr.begin(); itor != riasParent->m_arr.end(); itor++)
		{
			RIAS* rias = *itor;

			R_VectorRecord* vectorRecord = nullptr;

			if (120 == rias->m_name.RCNM)
			{
				vectorRecord = cell->GetCurveRecord(rias->m_name.GetName());
			}
			else if (125 == rias->m_name.RCNM)
			{
				vectorRecord = cell->GetCompositeCurveRecord(rias->m_name.GetName());
			}

			if (vectorRecord)
			{
				SetVector(rias->m_name.RCNM, vectorRecord, pProperty);
			}
		}
	}
}
//
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
	HWND thisHandle = this->m_hWnd;
	HWND attributeHandle = m_wndListAttribute.m_hWnd;

	int index = 0;
	R_VectorRecord* vr = NULL;
	IC2D* ic = NULL;
	F_C2IL* fic = NULL;
	F_C2IT* it = NULL;

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