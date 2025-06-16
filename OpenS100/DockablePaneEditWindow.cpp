#include "pch.h"
#include "MainFrm.h"
#include "DockablePaneEditWindow.h"
#include "resource.h"
#include "OpenS100.h"
#include "OpenS100View.h"

#include "../GeoMetryLibrary/GeoCommonFuc.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../GISLibrary/S100Layer.h"
#include "../GISLibrary/SpatialObject.h"
#include "../GISLibrary/GISLibrary.h"
#include "../GISLibrary/S100SpatialObject.h"
#include "../GISLibrary/S100Layer.h"
#include "../GISLibrary/GeometryType.h"
#include "../GISLibrary/GM_SurfacePatch.h"
#include "../GISLibrary/GM_Polygon.h"
#include "../GISLibrary/SPoint.h"
#include "../GISLibrary/SMultiPoint.h"
#include "../GISLibrary/SCurve.h"
#include "../GISLibrary/SCompositeCurve.h"
#include "../GISLibrary/SSurface.h"

#include <vector>
#include <sstream>
#include <string>

const UINT WM_FILTER_LIST = ::RegisterWindowMessage(_T("WM_FILTER_LIST"));
const CString tabNameAttribute = L"Attributes";
const CString tabNameVector = L"Vector";

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
	//auto selectedID = pugi::as_utf8(selectedFeatureID);

	DeleteAttributeItems();
	
	std::vector<CMFCPropertyGridProperty*> pAttrItemList;

	if (m_cell == nullptr) {
		return;
	}

	auto s100Layer = (S100Layer*)m_cell->GetLayer();
	auto fc = s100Layer->GetFeatureCatalog();
	if (nullptr == fc) {
		return;
	}

	std::string id = "";
	int type = 0;
	if (m_selectedObjectType == L"Feature") {
		id = pugi::as_utf8(selectedFeatureID);
		type = 1;
	}
	else if (m_selectedObjectType == L"Information") {
		id = pugi::as_utf8(selectedInformationID);
		type = 2;
	}

	auto object = m_cell->GetObjectType(type, id);

	if (object)
	{
		int attrCnt = object->GetAttributeCount();
		for (int i = 0; i < attrCnt; i++)
		{
			auto thematicAttribute = object->GetAttribute(i);
			if (thematicAttribute) 
			{

				auto code = thematicAttribute->GetCode();
				//Attribute* attribute = fc->GetSimpleAttribute(code);
				//if (!attribute) {
				//	attribute = fc->GetComplexAttribute(code);
				//	if (!attribute)
				//	{
				//		continue;
				//	}
				//}
				
				if (thematicAttribute->IsSimple()) 
				{
					addSimpleAttribute(nullptr, thematicAttribute->GetCode(), thematicAttribute->GetValue(), fc);
					
					//auto gf_sa = (GF::SimpleAttributeType*)thematicAttribute;
					//auto value = object->GetAttributeValue(i);
					//CString strValue;

					//auto sa = (SimpleAttribute*)attribute;

					//if (sa->GetValueType() == FCD::S100_CD_AttributeValueType::enumeration)
					//{
					//	auto iValue = atoi(value.c_str());
					//	auto listedValue = sa->GetListedValue(iValue);
					//	if (listedValue)
					//	{
					//		strValue.Format(L"%d. %s", listedValue->GetCode(), listedValue->GetLabel().c_str());
					//	}
					//}
					//else
					//{
					//	strValue = LibMFCUtil::StringToWString(value).c_str();
					//}

					//auto pAttribute = new CMFCPropertyGridProperty(sa->GetName().c_str(), strValue);
					//pAttrItemList.push_back(pAttribute);
					//pAttribute->SetDescription(attribute->GetDefinition().c_str());
				}
				else 
				{
					//auto gf_ca = (GF::ComplexAttributeType*)thematicAttribute;

					//auto ca = (ComplexAttribute*)attribute;
					//auto pAttribute = new CMFCPropertyGridProperty(ca->GetName().c_str());
					//pAttrItemList.push_back(pAttribute);
					//pAttribute->SetDescription(attribute->GetDefinition().c_str());
					addComplexAttribute(nullptr, (GF::ComplexAttributeType*)thematicAttribute, fc);
				}
				
				//auto value = object->GetAttributeValue(i);
				//auto code = m_cell->GetObjectAttributeCode(type, id, i);
				//CString strValue;
				//CMFCPropertyGridProperty* pAttribute = nullptr;

				//Attribute* attribute = fc->GetSimpleAttribute(code);
				//bool isSimple = true;
				//if (!attribute)
				//{
				//	attribute = fc->GetComplexAttribute(code);
				//	isSimple = false;
				//	if (!attribute)
				//	{
				//		continue;
				//	}
				//}

				//if (isSimple)
				//{
				//	auto sa = (SimpleAttribute*)attribute;

				//	if (sa->GetValueType() == FCD::S100_CD_AttributeValueType::enumeration)
				//	{
				//		auto iValue = atoi(value.c_str());
				//		auto listedValue = sa->GetListedValue(iValue);
				//		if (listedValue)
				//		{
				//			strValue.Format(L"%d. %s", listedValue->GetCode(), listedValue->GetLabel().c_str());
				//		}
				//	}
				//	else
				//	{
				//		strValue = LibMFCUtil::StringToWString(value).c_str();
				//	}

				//	pAttribute = new CMFCPropertyGridProperty(sa->GetName().c_str(), strValue);
				//}
				//else
				//{
				//	auto ca = (ComplexAttribute*)attribute;
				//	if (ca)
				//	{
				//		pAttribute = new CMFCPropertyGridProperty(ca->GetName().c_str());
				//	}
				//}

				//pAttrItemList.push_back(pAttribute);
				//pAttribute->SetDescription(attribute->GetDefinition().c_str());

				//auto parentIndex = object->GetParentAttributeIndex(i);

				//if (parentIndex != 0)
				//{
				//	pAttrItemList[parentIndex - 1]->AddSubItem(pAttribute);
				//}
				//else
				//{
				//	m_wndListAttribute.AddProperty(pAttribute);
				//}
			}
		}
	}

	m_wndListAttribute.ExpandAll();
}

void CDockablePaneEditWindow::addSimpleAttribute(CMFCPropertyGridProperty* parent, std::string code, std::string value, FeatureCatalogue* fc)
{
	auto sa = fc->GetSimpleAttribute(code);
	if (sa) {
		CString strCode = pugi::as_wide(code).c_str();
		CString strValue = pugi::as_wide(value).c_str();
		CString strDescription = sa->GetDefinition().c_str();

		auto child = new CMFCPropertyGridProperty(strCode, strValue);
		child->SetDescription(strDescription);

		if (parent) {
			parent->AddSubItem(child);
		}
		else {
			m_wndListAttribute.AddProperty(child);
		}
	}
}

void CDockablePaneEditWindow::addComplexAttribute(CMFCPropertyGridProperty* parent, GF::ComplexAttributeType* complexAttribute, FeatureCatalogue* fc)
{
	CString strCode = pugi::as_wide(complexAttribute->GetCode()).c_str();

	auto ca = fc->GetComplexAttribute(complexAttribute->GetCode());

	if (ca) 
	{
		CString strDescription = ca->GetDefinition().c_str();
		auto child = new CMFCPropertyGridProperty(strCode);
		child->SetDescription(strDescription);

		for (int i = 0; i < complexAttribute->GetSubAttributeCount(); i++) 
		{
			auto subAttribute = complexAttribute->GetSubAttribute(i);
			if (subAttribute->IsSimple()) 
			{
				auto code = subAttribute->GetCode();
				auto value = subAttribute->GetValue();
				addSimpleAttribute(child, code, value, fc);
			}
			else 
			{
				addComplexAttribute(child, (GF::ComplexAttributeType*)subAttribute, fc);
			}
		}

		if (parent) 
		{
			parent->AddSubItem(child);
		}
		else 
		{
			m_wndListAttribute.AddProperty(child);
		}
	}
}

void CDockablePaneEditWindow::SetVectors()
{
	auto m_pFeature = m_cell->GetFeatureType(pugi::as_utf8(selectedFeatureID));

	m_wndListVector.ShowWindow(FALSE);
	DeleteVectorItems();

	if (m_cell == nullptr || m_pFeature == nullptr)
	{
		m_wndListVector.ShowWindow(TRUE);
		return;
	}

	auto geom = m_pFeature->GetGeometry();
	if (geom)
	{
		SetVector(geom);
	}

	m_wndListVector.ExpandAll(FALSE);
	m_wndListVector.ShowWindow(TRUE);
	
}

void CDockablePaneEditWindow::SetVector(SGeometry* geom, CMFCPropertyGridProperty* pSuperProperty)
{
	auto type = geom->GetType();
	if (type == SGeometryType::Point)
	{
		SetVector((SPoint*)geom, pSuperProperty);
	}
	else if (type == SGeometryType::MultiPoint)
	{
		SetVector((SMultiPoint*)geom, pSuperProperty);
	}
	else if (type == SGeometryType::Curve)
	{
		SetVector((SCurve*)geom, pSuperProperty);
	}
	else if (type == SGeometryType::CompositeCurve)
	{
		SetVector((SCompositeCurve*)geom, pSuperProperty);
	}
	else if (type == SGeometryType::Surface)
	{
		SetVector((SSurface*)geom, pSuperProperty);
	}
}

void CDockablePaneEditWindow::SetVector(GM::Point* r, CMFCPropertyGridProperty* pSuperProperty)
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
	auto infoCount = geom->GetInformationTypeCount();

	CString strInfoCount;
	strInfoCount.Format(L"%d", infoCount);

	std::wstring pointName = L"Point (ID : " + geom->GetIDAsWString() + L", Relation: " + std::wstring(strInfoCount) + L")";

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
			auto info = geom->GetInformationType(i);
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

	double x = geom->GetX();
	double y = geom->GetY();

	inverseProjection(x, y);

	wss.precision(precision_x);
	wss.precision(precision_y);

	wss << y << L"," << x;

	CMFCPropertyGridProperty* pChildProperty = new CMFCPropertyGridProperty(L"Position", wss.str().c_str());
	pProperty->AddSubItem(pChildProperty);

	pChildProperty->SetName(L"Point");
}

void CDockablePaneEditWindow::SetVector(GM::MultiPoint* r, CMFCPropertyGridProperty* pSuperProperty)
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

void CDockablePaneEditWindow::SetVector(SMultiPoint* geom, CMFCPropertyGridProperty* pSuperProperty)
{
	int infoCount = geom->GetInformationTypeCount();
	CString count;
	count.Format(L"%d", infoCount);

	std::wstring multpointName = L"MultiPoint (RCID : " + geom->GetIDAsWString() + L", Relation: " + std::wstring(count) + L")";
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
			auto info = geom->GetInformationType(i);

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

	for (auto i = 0; i < geom->GetNumPoints(); i++)
	{
		std::wstringstream wss;
		wss.setf(std::ios::fixed, std::ios::floatfield);
		double x = geom->GetX(i);
		double y = geom->GetY(i);
		double z = geom->GetZ(i);

		inverseProjection(x, y);

		wss.precision(precision_x);
		wss << x << L",";
		
		wss.precision(precision_y);
		wss << y << L",";
		
		wss.precision(precision_z);
		wss << z;

		CMFCPropertyGridProperty* pChildProperty = new CMFCPropertyGridProperty(L"Position", wss.str().c_str());
		pProperty->AddSubItem(pChildProperty);

		pChildProperty->SetName(L"Point");
	}
}

void CDockablePaneEditWindow::SetVector(GM::Curve* r, CMFCPropertyGridProperty* pSuperProperty)
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

	SetVector(r->boundary.startPoint, pProperty);

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

	SetVector(r->boundary.endPoint, pProperty);
}

void CDockablePaneEditWindow::SetVector(SCurve* geom, CMFCPropertyGridProperty* pSuperProperty)
{
	int infoCount = geom->GetInformationTypeCount();
	CString count;
	count.Format(L"%d", infoCount);

	std::wstring curveName = L"Curve (RCID : " + geom->GetIDAsWString() + L", Relation: " + std::wstring(count) + L")";

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
			auto info = geom->GetInformationType(i);
			
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

	SetVector(geom->GetFirstPoint(), pProperty);

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

	for (int i = 1; i < geom->GetNumPoints() - 1; i++)
	{
		std::wstringstream wss;
		wss.setf(std::ios::fixed, std::ios::floatfield);

		double x = geom->GetX(i);
		double y = geom->GetY(i);

		inverseProjection(x, y);

		wss.precision(precision_x);
		wss.precision(precision_y);

		wss << y << L"," << x;

		CMFCPropertyGridProperty* pChildProperty = new CMFCPropertyGridProperty(L"Position", wss.str().c_str());
		pProperty->AddSubItem(pChildProperty);
		pChildProperty->SetName(L"C2IL");
	}

	SetVector(geom->GetLastPoint(), pProperty);
}

void CDockablePaneEditWindow::SetVector(GM::CompositeCurve* r, CMFCPropertyGridProperty* pSuperProperty)
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
		if (r->component.at(i)->GetType() == GM::GeometryType::Curve)
		{
			SetVector((GM::Curve*)&r->component.at(i), pProperty);
		}
		else if (r->component.at(i)->GetType() == GM::GeometryType::CompositeCurve)
		{
			SetVector((GM::CompositeCurve*)&r->component.at(i), pProperty);
		}
	}
}

void CDockablePaneEditWindow::SetVector(SCompositeCurve* geom, CMFCPropertyGridProperty* pSuperProperty)
{
	int infoCount = (int)geom->GetInformationTypeCount();
	CString count;
	count.Format(L"%d", infoCount);

	S101Cell* cell = (S101Cell*)m_cell;
	std::wstring comName = L"Composite Curve (RCID : " + geom->GetIDAsWString() + L", Relation: " + std::wstring(count) + L")";
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
			auto info = geom->GetInformationType(i);
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

	for (int i = 0; i < geom->GetCurveCount(); i++)
	{
		auto ring = geom->GetCurve(i);
		if (ring->GetType() == SGeometryType::Curve)
		{
			SetVector(ring, pProperty);
		}
		else if (ring->GetType() == SGeometryType::CompositeCurve)
		{
			SetVector((SCompositeCurve*)ring, pProperty);
		}
	}
}

void CDockablePaneEditWindow::SetVector(GM::Surface* r, CMFCPropertyGridProperty* pSuperProperty)
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

	auto exteriorCompositeCurve = r->GetPolygon().boundary.exterior;
	SetVector(exteriorCompositeCurve, pProperty);

	for (int i = 0; i < r->GetPolygon().boundary.interior.size(); i++)
	{
		auto interiorCompositeCurve = r->GetPolygon().boundary.interior.at(i);
		SetVector(interiorCompositeCurve, pProperty);
	}
}

void CDockablePaneEditWindow::SetVector(SSurface* geom, CMFCPropertyGridProperty* pSuperProperty)
{
	int infoCount = (int)geom->GetInformationTypeCount();
	CString count;
	count.Format(L"%d", infoCount);

	std::wstring surName = L"Surface (RCID: " + geom->GetIDAsWString() + L", Relation: " + std::wstring(count) + L")";

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
			auto info = geom->GetInformationType(i);

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

	for (int i = 0; i < geom->GetRingCount(); i++)
	{
		auto ring = geom->GetRing(i);
		if (ring->GetType() == SGeometryType::Curve)
		{
			SetVector(ring, pProperty);
		}
		else if (ring->GetType() == SGeometryType::CompositeCurve)
		{
			SetVector((SCompositeCurve*)ring, pProperty);
		}
	}
}

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
}

BOOL CDockablePaneEditWindow::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CDockablePaneEditWindow::SetSpatialObject(S100SpatialObject *object)
{
	m_cell = object;
}

void CDockablePaneEditWindow::ClearVector()
{
	m_wndListVector.ShowWindow(FALSE);
	DeleteVectorItems();
	m_wndListVector.ShowWindow(TRUE);
}