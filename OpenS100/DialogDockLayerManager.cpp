#include "pch.h"
#include "OpenS100.h"
#include "OpenS100View.h"
#include "DialogDockLayerManager.h"
#include "afxdialogex.h"
#include "resource.h"

#include "..\\GISLibrary\\Layer.h"
#include "..\\GISLibrary\\GISLibrary.h"

IMPLEMENT_DYNAMIC(CDialogDockLayerManager, CDialog)

CDialogDockLayerManager::CDialogDockLayerManager(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDockLayerManager::IDD, pParent)
{
}

CDialogDockLayerManager::~CDialogDockLayerManager()
{
}

void CDialogDockLayerManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogDockLayerManager, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()

	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CDialogDockLayerManager message handlers
int CDialogDockLayerManager::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}

void CDialogDockLayerManager::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	AdjustLayout();
}


void CDialogDockLayerManager::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);
}

// layerManager
BOOL CDialogDockLayerManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*
	** init List Ctrl
	*/

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}
	InitPropList();
	AdjustLayout();

	return TRUE; 
}

void CDialogDockLayerManager::OnButtonDelete()  //delete layer
{
	DeleteLayer();

}

void CDialogDockLayerManager::FocusLayerRange()
{
	Layer *layer = gisLib->GetLayer();
	auto layerMBR = layer->GetMBR();

	gisLib->GetLayerManager()->GetScaler()->SetMap(layerMBR);
	gisLib->AdjustScreenMap();
	theApp.MapRefresh();
}

void CDialogDockLayerManager::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient, rectCombo;
	GetClientRect(rectClient);

	CRect      rectENCs;

	int cyCmb = rectCombo.Size().cy;
	if (m_wndPropList.GetSafeHwnd())
		m_wndPropList.SetWindowPos(NULL, rectClient.left, 0, rectClient.Width(), rectClient.Size().cy, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CDialogDockLayerManager::InitPropList() //Data Set Identification included in Layer Manager
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);

	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Data Set Identification"));
	CMFCPropertyGridProperty* Encoding_specification = new CMFCPropertyGridProperty(_T("Encoding specification"), (_variant_t)_T("-"), L"Encoding specification that defines the encoding");

	Encoding_specification->Enable(false);

	pGroup1->AddSubItem(Encoding_specification);

	CMFCPropertyGridProperty* Encoding_specification_edition = new CMFCPropertyGridProperty(_T("Encoding specification edition"), (_variant_t)_T("-"), L"Edition of the encoding specification");

	Encoding_specification_edition->Enable(false);

	pGroup1->AddSubItem(Encoding_specification_edition);

	CMFCPropertyGridProperty* Product_identifier = new CMFCPropertyGridProperty(_T("Product identifier"), (_variant_t)_T("-"), L"Unique identifier for the data product as specified in the product specification");

	Product_identifier->Enable(false);

	pGroup1->AddSubItem(Product_identifier);

	CMFCPropertyGridProperty* Product_edition = new CMFCPropertyGridProperty(_T("Product edition"), (_variant_t)_T("-"), L"The file identifier of the dataset");

	Product_edition->Enable(false);

	pGroup1->AddSubItem(Product_edition);

	CMFCPropertyGridProperty* Application_profile = new CMFCPropertyGridProperty(_T("Application profile"), (_variant_t)_T("-"), L"Identifier that specifies a profile within the data product");

	Application_profile->Enable(false);

	pGroup1->AddSubItem(Application_profile);

	CMFCPropertyGridProperty* Dataset_file_identifier = new CMFCPropertyGridProperty(_T("Dataset file identifier"), (_variant_t)_T("-"), L"The file identifier of the dataset");

	Dataset_file_identifier->Enable(false);

	pGroup1->AddSubItem(Dataset_file_identifier);

	CMFCPropertyGridProperty* Dataset_title = new CMFCPropertyGridProperty(_T("Dataset title"), (_variant_t)_T("-"), L"The title of the dataset");

	Dataset_title->Enable(false);

	pGroup1->AddSubItem(Dataset_title);

	CMFCPropertyGridProperty* Dataset_reference_date = new CMFCPropertyGridProperty(_T("Dataset reference date"), (_variant_t)_T("-"), L"The reference date of the dataset Format: YYYYMMDD according to ISO 8601");

	Dataset_reference_date->Enable(false);

	pGroup1->AddSubItem(Dataset_reference_date);

	CMFCPropertyGridProperty* Dataset_language = new CMFCPropertyGridProperty(_T("Dataset language"), (_variant_t)_T("-"), L"The (primary) language used in this dataset");

	Dataset_language->Enable(false);

	pGroup1->AddSubItem(Dataset_language);

	CMFCPropertyGridProperty* Dataset_abstract = new CMFCPropertyGridProperty(_T("Dataset abstract"), (_variant_t)_T("-"), L"The abstract of the dataset");

	Dataset_abstract->Enable(false);

	pGroup1->AddSubItem(Dataset_abstract);

	CMFCPropertyGridProperty* Dataset_edition = new CMFCPropertyGridProperty(_T("Dataset edition"), (_variant_t)_T("-"), L"The edition of the dataset");

	Dataset_edition->Enable(false);

	pGroup1->AddSubItem(Dataset_edition);

	CMFCPropertyGridProperty* Dataset_topic_category = new CMFCPropertyGridProperty(_T("Dataset topic category"), (_variant_t)_T("-"), L"A set of topic categories");

	Dataset_topic_category->Enable(false);

	pGroup1->AddSubItem(Dataset_topic_category);
	m_wndPropList.AddProperty(pGroup1);



	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Data Set Structure Information"));
	CMFCPropertyGridProperty* Dataset_Coordinate_Origin_X = new CMFCPropertyGridProperty(_T("Dataset Coordinate Origin X"), (_variant_t)_T("-"), L"Shift used to adjust x-coordinate before encoding");

	Dataset_Coordinate_Origin_X->Enable(false);

	pGroup2->AddSubItem(Dataset_Coordinate_Origin_X);

	CMFCPropertyGridProperty* Dataset_Coordinate_Origin_Y = new CMFCPropertyGridProperty(_T("Dataset Coordinate Origin Y"), (_variant_t)_T("-"), L"Shift used to adjust y-coordinate before encoding");

	Dataset_Coordinate_Origin_Y->Enable(false);

	pGroup2->AddSubItem(Dataset_Coordinate_Origin_Y);

	CMFCPropertyGridProperty* Dataset_Coordinate_Origin_Z = new CMFCPropertyGridProperty(_T("Dataset Coordinate Origin Z"), (_variant_t)_T("-"), L"Shift used to adjust z-coordinate before encoding");

	Dataset_Coordinate_Origin_Z->Enable(false);

	pGroup2->AddSubItem(Dataset_Coordinate_Origin_Z);

	CMFCPropertyGridProperty* Coordinate_multiplication_x_coordinate = new CMFCPropertyGridProperty(_T("Coordinate multiplication factor for x-coordinate"), (_variant_t)_T("-"), L"Floating point to integer multiplication factor for the x-coordinate or longitude");

	Coordinate_multiplication_x_coordinate->Enable(false);

	pGroup2->AddSubItem(Coordinate_multiplication_x_coordinate);

	CMFCPropertyGridProperty* Coordinate_multiplication_y_coordinate = new CMFCPropertyGridProperty(_T("Coordinate multiplication factor for y-coordinate"), (_variant_t)_T("-"), L"Floating point to integer multiplication factor for the y-coordinate or latitude");

	Coordinate_multiplication_y_coordinate->Enable(false);

	pGroup2->AddSubItem(Coordinate_multiplication_y_coordinate);

	CMFCPropertyGridProperty* Coordinate_multiplication_z_coordinate = new CMFCPropertyGridProperty(_T("Coordinate multiplication factor for z-coordinate"), (_variant_t)_T("-"), L"Floating point to integer multiplication factor for the z-coordinate or depths or height");

	Coordinate_multiplication_z_coordinate->Enable(false);

	pGroup2->AddSubItem(Coordinate_multiplication_z_coordinate);

	CMFCPropertyGridProperty* Number_of_Information_Type_records = new CMFCPropertyGridProperty(_T("Number of Information Type records"), (_variant_t)_T("-"), L"Number of information records in the data set");

	Number_of_Information_Type_records->Enable(false);

	pGroup2->AddSubItem(Number_of_Information_Type_records);

	CMFCPropertyGridProperty* Number_of_Point_records = new CMFCPropertyGridProperty(_T("Number of Point records"), (_variant_t)_T("-"), L"Number of point records in the data set");

	Number_of_Point_records->Enable(false);

	pGroup2->AddSubItem(Number_of_Point_records);

	CMFCPropertyGridProperty* Number_of_Multi_Point_records = new CMFCPropertyGridProperty(_T("Number of Multi Point records"), (_variant_t)_T("-"), L"Number of multi point records in the data set");

	Number_of_Multi_Point_records->Enable(false);

	pGroup2->AddSubItem(Number_of_Multi_Point_records);

	CMFCPropertyGridProperty* Number_of_Curve_records = new CMFCPropertyGridProperty(_T("Number of Curve records"), (_variant_t)_T("-"), L"Number of curve records in the data set");

	Number_of_Curve_records->Enable(false);

	pGroup2->AddSubItem(Number_of_Curve_records);

	CMFCPropertyGridProperty* Number_of_Composite_Curve_records = new CMFCPropertyGridProperty(_T("Number of Composite Curve records"), (_variant_t)_T("-"), L"Number of composite curve records in the data set");

	Number_of_Composite_Curve_records->Enable(false);

	pGroup2->AddSubItem(Number_of_Composite_Curve_records);

	CMFCPropertyGridProperty* Number_of_Surface_records = new CMFCPropertyGridProperty(_T("Number of Surface records"), (_variant_t)_T("-"), L"Number of surface records in the data set");

	Number_of_Surface_records->Enable(false);

	pGroup2->AddSubItem(Number_of_Surface_records);

	CMFCPropertyGridProperty* Number_of_Feature_Type_records = new CMFCPropertyGridProperty(_T("Number of Feature Type records"), (_variant_t)_T("-"), L"Number of feature records in the data set");

	Number_of_Feature_Type_records->Enable(false);

	pGroup2->AddSubItem(Number_of_Feature_Type_records);
	m_wndPropList.AddProperty(pGroup2);
}

void CDialogDockLayerManager::DeleteLayer() {

	//gisLib->ClearInformationLayer(0);
	gisLib->DeleteLayer(); //delete layer

	theApp.m_DockablePaneEditWindow.DeleteAllItems();
	RemoveAllPropList(); //delete Dataset identification
	theApp.m_DockablePaneCurrentSelection.RemoveAll(); //delete Current Selection 
	theApp.m_DockablePaneRelation.UpdateList();

	UpdateList();
	theApp.pView->ClearPickReport();
	theApp.MapRefresh();
}
void CDialogDockLayerManager::UpdateList()
{
	if (gisLib->GetLayer()!=nullptr)
	{
		Layer *layer = (Layer *)gisLib->GetLayer();
		if (layer->m_spatialObject->m_FileType == FILE_S_100_VECTOR)
		{
			S101Cell* c = (S101Cell*)layer->m_spatialObject;
			FillPropList(c);
		}
	}
}

void CDialogDockLayerManager::RemoveAllPropList()
{
	m_wndPropList.RemoveAll();

	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();


	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Data Set Identification"));
	CMFCPropertyGridProperty* Encoding_specification = new CMFCPropertyGridProperty(_T("Encoding specification"), (_variant_t)_T("-"), L"Encoding specification that defines the encoding");
	Encoding_specification->Enable(false);
	pGroup1->AddSubItem(Encoding_specification);

	CMFCPropertyGridProperty* Encoding_specification_edition = new CMFCPropertyGridProperty(_T("Encoding specification edition"), (_variant_t)_T("-"), L"Edition of the encoding specification");
	Encoding_specification_edition->Enable(false);
	pGroup1->AddSubItem(Encoding_specification_edition);

	CMFCPropertyGridProperty* Product_identifier = new CMFCPropertyGridProperty(_T("Product identifier"), (_variant_t)_T("-"), L"Unique identifier for the data product as specified in the product specification");
	Product_identifier->Enable(false);
	pGroup1->AddSubItem(Product_identifier);

	CMFCPropertyGridProperty* Product_edition = new CMFCPropertyGridProperty(_T("Product edition"), (_variant_t)_T("-"), L"The file identifier of the dataset");
	Product_edition->Enable(false);
	pGroup1->AddSubItem(Product_edition);

	CMFCPropertyGridProperty* Application_profile = new CMFCPropertyGridProperty(_T("Application profile"), (_variant_t)_T("-"), L"Identifier that specifies a profile within the data product");
	Application_profile->Enable(false);
	pGroup1->AddSubItem(Application_profile);

	CMFCPropertyGridProperty* Dataset_file_identifier = new CMFCPropertyGridProperty(_T("Dataset file identifier"), (_variant_t)_T("-"), L"The file identifier of the dataset");
	Dataset_file_identifier->Enable(false);
	pGroup1->AddSubItem(Dataset_file_identifier);

	CMFCPropertyGridProperty* Dataset_title = new CMFCPropertyGridProperty(_T("Dataset title"), (_variant_t)_T("-"), L"The title of the dataset");
	Dataset_title->Enable(false);
	pGroup1->AddSubItem(Dataset_title);

	CMFCPropertyGridProperty* Dataset_reference_date = new CMFCPropertyGridProperty(_T("Dataset reference date"), (_variant_t)_T("-"), L"The reference date of the dataset Format: YYYYMMDD according to ISO 8601");
	Dataset_reference_date->Enable(false);
	pGroup1->AddSubItem(Dataset_reference_date);

	CMFCPropertyGridProperty* Dataset_language = new CMFCPropertyGridProperty(_T("Dataset language"), (_variant_t)_T("-"), L"The (primary) language used in this dataset");
	Dataset_language->Enable(false);
	pGroup1->AddSubItem(Dataset_language);

	CMFCPropertyGridProperty* Dataset_abstract = new CMFCPropertyGridProperty(_T("Dataset abstract"), (_variant_t)_T("-"), L"The abstract of the dataset");
	Dataset_abstract->Enable(false);
	pGroup1->AddSubItem(Dataset_abstract);

	CMFCPropertyGridProperty* Dataset_edition = new CMFCPropertyGridProperty(_T("Dataset edition"), (_variant_t)_T("-"), L"The edition of the dataset");
	Dataset_edition->Enable(false);
	pGroup1->AddSubItem(Dataset_edition);

	CMFCPropertyGridProperty* Dataset_topic_category = new CMFCPropertyGridProperty(_T("Dataset topic category"), (_variant_t)_T("-"), L"A set of topic categories");
	Dataset_topic_category->Enable(false);
	pGroup1->AddSubItem(Dataset_topic_category);
	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Data Set Structure Information"));
	CMFCPropertyGridProperty* Dataset_Coordinate_Origin_X = new CMFCPropertyGridProperty(_T("Dataset Coordinate Origin X"), (_variant_t)_T("-"), L"Shift used to adjust x-coordinate before encoding");
	Dataset_Coordinate_Origin_X->Enable(false);
	pGroup2->AddSubItem(Dataset_Coordinate_Origin_X);

	CMFCPropertyGridProperty* Dataset_Coordinate_Origin_Y = new CMFCPropertyGridProperty(_T("Dataset Coordinate Origin Y"), (_variant_t)_T("-"), L"Shift used to adjust y-coordinate before encoding");
	Dataset_Coordinate_Origin_Y->Enable(false);
	pGroup2->AddSubItem(Dataset_Coordinate_Origin_Y);

	CMFCPropertyGridProperty* Dataset_Coordinate_Origin_Z = new CMFCPropertyGridProperty(_T("Dataset Coordinate Origin Z"), (_variant_t)_T("-"), L"Shift used to adjust z-coordinate before encoding");
	Dataset_Coordinate_Origin_Z->Enable(false);
	pGroup2->AddSubItem(Dataset_Coordinate_Origin_Z);

	CMFCPropertyGridProperty* Coordinate_multiplication_x_coordinate = new CMFCPropertyGridProperty(_T("Coordinate multiplication factor for x-coordinate"), (_variant_t)_T("-"), L"Floating point to integer multiplication factor for the x-coordinate or longitude");
	Coordinate_multiplication_x_coordinate->Enable(false);
	pGroup2->AddSubItem(Coordinate_multiplication_x_coordinate);

	CMFCPropertyGridProperty* Coordinate_multiplication_y_coordinate = new CMFCPropertyGridProperty(_T("Coordinate multiplication factor for y-coordinate"), (_variant_t)_T("-"), L"Floating point to integer multiplication factor for the y-coordinate or latitude");
	Coordinate_multiplication_y_coordinate->Enable(false);
	pGroup2->AddSubItem(Coordinate_multiplication_y_coordinate);

	CMFCPropertyGridProperty* Coordinate_multiplication_z_coordinate = new CMFCPropertyGridProperty(_T("Coordinate multiplication factor for z-coordinate"), (_variant_t)_T("-"), L"Floating point to integer multiplication factor for the z-coordinate or depths or height");
	Coordinate_multiplication_z_coordinate->Enable(false);
	pGroup2->AddSubItem(Coordinate_multiplication_z_coordinate);

	CMFCPropertyGridProperty* Number_of_Information_Type_records = new CMFCPropertyGridProperty(_T("Number of Information Type records"), (_variant_t)_T("-"), L"Number of information records in the data set");
	Number_of_Information_Type_records->Enable(false);
	pGroup2->AddSubItem(Number_of_Information_Type_records);

	CMFCPropertyGridProperty* Number_of_Point_records = new CMFCPropertyGridProperty(_T("Number of Point records"), (_variant_t)_T("-"), L"Number of point records in the data set");
	Number_of_Point_records->Enable(false);
	pGroup2->AddSubItem(Number_of_Point_records);

	CMFCPropertyGridProperty* Number_of_Multi_Point_records = new CMFCPropertyGridProperty(_T("Number of Multi Point records"), (_variant_t)_T("-"), L"Number of multi point records in the data set");
	Number_of_Multi_Point_records->Enable(false);
	pGroup2->AddSubItem(Number_of_Multi_Point_records);

	CMFCPropertyGridProperty* Number_of_Curve_records = new CMFCPropertyGridProperty(_T("Number of Curve records"), (_variant_t)_T("-"), L"Number of curve records in the data set");
	Number_of_Curve_records->Enable(false);
	pGroup2->AddSubItem(Number_of_Curve_records);

	CMFCPropertyGridProperty* Number_of_Composite_Curve_records = new CMFCPropertyGridProperty(_T("Number of Composite Curve records"), (_variant_t)_T("-"), L"Number of composite curve records in the data set");
	Number_of_Composite_Curve_records->Enable(false);
	pGroup2->AddSubItem(Number_of_Composite_Curve_records);

	CMFCPropertyGridProperty* Number_of_Surface_records = new CMFCPropertyGridProperty(_T("Number of Surface records"), (_variant_t)_T("-"), L"Number of surface records in the data set");
	Number_of_Surface_records->Enable(false);
	pGroup2->AddSubItem(Number_of_Surface_records);

	CMFCPropertyGridProperty* Number_of_Feature_Type_records = new CMFCPropertyGridProperty(_T("Number of Feature Type records"), (_variant_t)_T("-"), L"Number of feature records in the data set");
	Number_of_Feature_Type_records->Enable(false);
	pGroup2->AddSubItem(Number_of_Feature_Type_records);
	m_wndPropList.AddProperty(pGroup2);
}

//add identifier info
void CDialogDockLayerManager::FillPropList(S101Cell* cell)
{
	m_wndPropList.RemoveAll();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Data Set Identification"));
	CMFCPropertyGridProperty* Encoding_specification = new CMFCPropertyGridProperty(_T("Encoding specification"), (_variant_t)cell->m_dsgir.m_dsid.m_ensp, L"Encoding specification that defines the encoding");
	Encoding_specification->Enable(false);
	
	pGroup1->AddSubItem(Encoding_specification);

	CMFCPropertyGridProperty* Encoding_specification_edition = new CMFCPropertyGridProperty(_T("Encoding specification edition"), (_variant_t)cell->m_dsgir.m_dsid.m_ened, L"Edition of the encoding specification");
	Encoding_specification_edition->Enable(false);

	pGroup1->AddSubItem(Encoding_specification_edition);

	CMFCPropertyGridProperty* Product_identifier = new CMFCPropertyGridProperty(_T("Product identifier"), (_variant_t)cell->m_dsgir.m_dsid.m_prsp, L"Unique identifier for the data product as specified in the product specification");
	Product_identifier->Enable(false);
	
	pGroup1->AddSubItem(Product_identifier);

	CMFCPropertyGridProperty* Product_edition = new CMFCPropertyGridProperty(_T("Product edition"), (_variant_t)cell->m_dsgir.m_dsid.m_pred, L"The file identifier of the dataset");
	Product_edition->Enable(false);
	
	pGroup1->AddSubItem(Product_edition);

	CMFCPropertyGridProperty* Application_profile = new CMFCPropertyGridProperty(_T("Application profile"), (_variant_t)cell->m_dsgir.m_dsid.m_prof, L"Identifier that specifies a profile within the data product");
	Application_profile->Enable(false);
	
	pGroup1->AddSubItem(Application_profile);

	CMFCPropertyGridProperty* Dataset_file_identifier = new CMFCPropertyGridProperty(_T("Dataset file identifier"), (_variant_t)cell->m_dsgir.m_dsid.m_dsnm, L"The file identifier of the dataset");
	Dataset_file_identifier->Enable(false);
	pGroup1->AddSubItem(Dataset_file_identifier);

	CMFCPropertyGridProperty* Dataset_title = new CMFCPropertyGridProperty(_T("Dataset title"), (_variant_t)cell->m_dsgir.m_dsid.m_dstl, L"The title of the dataset");
	Dataset_title->Enable(false);
	pGroup1->AddSubItem(Dataset_title);

	CMFCPropertyGridProperty* Dataset_reference_date = new CMFCPropertyGridProperty(_T("Dataset reference date"), (_variant_t)cell->m_dsgir.m_dsid.m_dsrd, L"The reference date of the dataset Format: YYYYMMDD according to ISO 8601");
	Dataset_reference_date->Enable(false);
	pGroup1->AddSubItem(Dataset_reference_date);

	CMFCPropertyGridProperty* Dataset_language = new CMFCPropertyGridProperty(_T("Dataset language"), (_variant_t)cell->m_dsgir.m_dsid.m_dslg, L"The (primary) language used in this dataset");
	Dataset_language->Enable(false);
	pGroup1->AddSubItem(Dataset_language);

	CMFCPropertyGridProperty* Dataset_abstract = new CMFCPropertyGridProperty(_T("Dataset abstract"), (_variant_t)L"-", L"The abstract of the dataset");
	Dataset_abstract->Enable(false);
	pGroup1->AddSubItem(Dataset_abstract);

	CMFCPropertyGridProperty* Dataset_edition = new CMFCPropertyGridProperty(_T("Dataset edition"), (_variant_t)cell->m_dsgir.m_dsid.m_dsed, L"The edition of the dataset");
	Dataset_edition->Enable(false);
	pGroup1->AddSubItem(Dataset_edition);

	CMFCPropertyGridProperty* Dataset_topic_category = new CMFCPropertyGridProperty(_T("Dataset topic category"), (_variant_t)L"{14}{18}", L"A set of topic categories");
	Dataset_topic_category->Enable(false);
	pGroup1->AddSubItem(Dataset_topic_category);
	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Data Set Structure Information"));
	CMFCPropertyGridProperty* Dataset_Coordinate_Origin_X = new CMFCPropertyGridProperty(_T("Dataset Coordinate Origin X"), (_variant_t)cell->m_dsgir.m_dssi.m_dcox, L"Shift used to adjust x-coordinate before encoding");

	Dataset_Coordinate_Origin_X->Enable(false);

	pGroup2->AddSubItem(Dataset_Coordinate_Origin_X);

	CMFCPropertyGridProperty* Dataset_Coordinate_Origin_Y = new CMFCPropertyGridProperty(_T("Dataset Coordinate Origin Y"), (_variant_t)cell->m_dsgir.m_dssi.m_dcoy, L"Shift used to adjust y-coordinate before encoding");
	Dataset_Coordinate_Origin_Y->Enable(false);
	pGroup2->AddSubItem(Dataset_Coordinate_Origin_Y);

	CMFCPropertyGridProperty* Dataset_Coordinate_Origin_Z = new CMFCPropertyGridProperty(_T("Dataset Coordinate Origin Z"), (_variant_t)cell->m_dsgir.m_dssi.m_dcoz, L"Shift used to adjust z-coordinate before encoding");
	Dataset_Coordinate_Origin_Z->Enable(false);

	pGroup2->AddSubItem(Dataset_Coordinate_Origin_Z);

	CMFCPropertyGridProperty* Coordinate_multiplication_x_coordinate = new CMFCPropertyGridProperty(_T("Coordinate multiplication factor for x-coordinate"), (_variant_t)cell->m_dsgir.m_dssi.m_cmfx, L"Floating point to integer multiplication factor for the x-coordinate or longitude");
	Coordinate_multiplication_x_coordinate->Enable(false);
	
	pGroup2->AddSubItem(Coordinate_multiplication_x_coordinate);

	CMFCPropertyGridProperty* Coordinate_multiplication_y_coordinate = new CMFCPropertyGridProperty(_T("Coordinate multiplication factor for y-coordinate"), (_variant_t)cell->m_dsgir.m_dssi.m_cmfy, L"Floating point to integer multiplication factor for the y-coordinate or latitude");

	Coordinate_multiplication_y_coordinate->Enable(false);
	pGroup2->AddSubItem(Coordinate_multiplication_y_coordinate);

	CMFCPropertyGridProperty* Coordinate_multiplication_z_coordinate = new CMFCPropertyGridProperty(_T("Coordinate multiplication factor for z-coordinate"), (_variant_t)cell->m_dsgir.m_dssi.m_cmfz, L"Floating point to integer multiplication factor for the z-coordinate or depths or height");
	Coordinate_multiplication_z_coordinate->Enable(false);
	pGroup2->AddSubItem(Coordinate_multiplication_z_coordinate);

	CMFCPropertyGridProperty* Number_of_Information_Type_records = new CMFCPropertyGridProperty(_T("Number of Information Type records"), (_variant_t)cell->m_dsgir.m_dssi.m_noir, L"Number of information records in the data set");
	Number_of_Information_Type_records->Enable(false);
	pGroup2->AddSubItem(Number_of_Information_Type_records);

	CMFCPropertyGridProperty* Number_of_Point_records = new CMFCPropertyGridProperty(_T("Number of Point records"), (_variant_t)cell->m_dsgir.m_dssi.m_nopn, L"Number of point records in the data set");
	Number_of_Point_records->Enable(false);
	pGroup2->AddSubItem(Number_of_Point_records);

	CMFCPropertyGridProperty* Number_of_Multi_Point_records = new CMFCPropertyGridProperty(_T("Number of Multi Point records"), (_variant_t)cell->m_dsgir.m_dssi.m_nomn, L"Number of multi point records in the data set");
	Number_of_Multi_Point_records->Enable(false);
	pGroup2->AddSubItem(Number_of_Multi_Point_records);

	CMFCPropertyGridProperty* Number_of_Curve_records = new CMFCPropertyGridProperty(_T("Number of Curve records"), (_variant_t)cell->m_dsgir.m_dssi.m_nocn, L"Number of curve records in the data set");
	Number_of_Curve_records->Enable(false);
	pGroup2->AddSubItem(Number_of_Curve_records);

	CMFCPropertyGridProperty* Number_of_Composite_Curve_records = new CMFCPropertyGridProperty(_T("Number of Composite Curve records"), (_variant_t)cell->m_dsgir.m_dssi.m_noxn, L"Number of composite curve records in the data set");
	Number_of_Composite_Curve_records->Enable(false);
	pGroup2->AddSubItem(Number_of_Composite_Curve_records);

	CMFCPropertyGridProperty* Number_of_Surface_records = new CMFCPropertyGridProperty(_T("Number of Surface records"), (_variant_t)cell->m_dsgir.m_dssi.m_nosn, L"Number of surface records in the data set");
	Number_of_Surface_records->Enable(false);
	pGroup2->AddSubItem(Number_of_Surface_records);

	CMFCPropertyGridProperty* Number_of_Feature_Type_records = new CMFCPropertyGridProperty(_T("Number of Feature Type records"), (_variant_t)cell->m_dsgir.m_dssi.m_nofr, L"Number of feature records in the data set");
	Number_of_Feature_Type_records->Enable(false);
	pGroup2->AddSubItem(Number_of_Feature_Type_records);

	m_wndPropList.AddProperty(pGroup2);
}


void CDialogDockLayerManager::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}