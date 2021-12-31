#pragma once
#include <map>

class CClass_S101_Base;
struct ST_FEATURE_RECORD;
struct ST_INFO_RECORD;
struct ST_POINT_RECORD;
struct ST_MULTI_POINT_RECORD;

class CClass_S101_InputXML
{
public:
	CClass_S101_InputXML();
	virtual ~CClass_S101_InputXML();

	// function : Create a pointer for S-101 Input XML.
	// factor : 
	//        CClass_S101_Base* pBase - S-101 data to create Input XML.
	// return : 
	//        create sucsess - Pointers to Input XML.
	//        create fail - nullptr
	MSXML2::IXMLDOMDocument2Ptr GetInputXML(CClass_S101_Base* pBase);

	// function : Create a file of S-101 Input XML
	// factor : 
	//        CClass_S101_Base* pBase        - S-101 data to create Input XML.
	//        CString           pathInputXML - Path of Input XML to be generated
	// return :
	//        create sucsess - true
	//        create fail - false
	bool                        GetInputXML(CClass_S101_Base* pBase, CString pathInputXML);

	// function : Create a pointer for S-101 Output XML.
	// factor :
	//        CString pathInputXML - The path of S-101 Input XML.
	//        CString pathXSL      - The XSLT path of the S-101 PC.
	// return : 
	//        create sucsess - Outut XML pointer.
	//        create fail - nullptr
	MSXML2::IXMLDOMDocument2Ptr GetOutputXML(CString pathInputXML, CString pathXSL);

	// function : Create a file of S-101 Output XML
	// factor :
	//        CString pathInputXML  - File path of S-101 Input XML
	//        CString pathXSL       - The XSLT path of the S-101 PC.
	//        CString pathOutputXML - File path of S-101 Output XML
	// return : 
	//        create sucsess - true
	//        create fail - false
	bool                        GetOutputXML(CString pathInputXML, CString pathXSL, CString pathOutputXML);

	// function : Create a pointer for S-101 Output XML.
	// factor :
	//        MSXML2::IXMLDOMDocument2Ptr pInputXML - The pointer of S-101 Input XML.
	//        CString                     pathXSL   - The XSLT path of the S-101 PC.
	// return : 
	//        create sucsess - The pointer of S-101 Output XML.
	//        create fail - nullptr
	MSXML2::IXMLDOMDocument2Ptr GetOutputXML(MSXML2::IXMLDOMDocument2Ptr pInputXML, CString pathXSL);

	// function : Create a pointer for S-101 Output XML.
	// factor : 
	//        MSXML2::IXMLDOMDocument2Ptr pInputXML     - The pointer of S-101 Input XML.
	//        CString					  pathXSL       - The XSLT path of the S-101 PC.
	//		  CString					  pathOutputXML - File path of S-101 Output XML
	// return : 
	bool                        GetOutputXML(MSXML2::IXMLDOMDocument2Ptr pInputXML, CString pathXSL, CString pathOutputXML);

	// Three are not actually used.
	CString GetInformationAssociationStringCode(CClass_S101_Base* pBase, int nNumeric);
	CString GetFeatureAssociationStringCode(CClass_S101_Base* pBase, int nNumeric);
	CString GetAssociationRoleStringCode(CClass_S101_Base* pBase, int nNumeric);

	// function : Create Processing Instruction of XML
	// factor : 
	//        MSXML2::IXMLDOMDocument2Ptr pDoc - XML DOM Document Pointer to Create Processing Instruction
	// return : always true (TBD)
	bool CreateProcessingInstruction(MSXML2::IXMLDOMDocument2Ptr pDoc);

	// function : Create a Dataset element of S-101 Input XML.
	// factor : 
	//        MSXML2::IXMLDOMDocument2Ptr pDoc -DOM Docu pointer of S-101 Input XML.
	// return : Dataset ø§∑π∏‡∆Æ¿« XML DOM Element ∆˜¿Œ≈Õ
	MSXML2::IXMLDOMElementPtr CreateDatasetElement(MSXML2::IXMLDOMDocument2Ptr pDoc);

	// function : Create Features Element of S-101 Input XML
	// factor : 
	//        CClass_S101_Base*           pBase           - S-101 Data Pointer.
	//        MSXML2::IXMLDOMDocument2Ptr pDoc            - DOM Documment pointer of S-101 Input XML.
	//        MSXML2::IXMLDOMElementPtr   pDatasetElement - DOM Element Pointer for Dataset of S-101 Input XML
	// return : DOM Element Pointer for Features Element of S-101 Input XML
	MSXML2::IXMLDOMElementPtr CreateFeaturesElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pDatasetElement);

	// function :Generating Information Types Element of S-101 Input XML
	// factor : 
	//        CClass_S101_Base* pBase                     -S-101 Data Pointer.
	//        MSXML2::IXMLDOMDocument2Ptr pDoc            - DOM Document pointer of S-101 Input XML.
	//	      MSXML2::IXMLDOMElementPtr   pDatasetElement - DOM Element Pointer for Dataset of S-101 Input XML
	// return : DOM Element Pointer for Information Types Element of S-101 Input XML
	MSXML2::IXMLDOMElementPtr CreateInformationTypesElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pDatasetElement);

	// function : Generating points elements of S-101 Input XML.
	// factor : 
	//       CClass_S101_Base*           pBase           - S-101 Data Pointer.
	//       MSXML2::IXMLDOMDocument2Ptr pDoc            - DOM Documment pointer of S-101 Input XML.
	//       MSXML2::IXMLDOMElementPtr   pDatasetElement - DOM Element Pointer for Dataset of S-101 Input XML
	// return : DOM Element Pointer for Points Element of S-101 Input XML
	MSXML2::IXMLDOMElementPtr CreatePointsElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pDatasetElement);

	// function : Create MultiPoints Element of S-101 Input XML
	// factor : 
	//        CClass_S101_Base* pBase
	//        
	// return : 
	MSXML2::IXMLDOMElementPtr CreateMultiPointsElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pDatasetElement);

	MSXML2::IXMLDOMElementPtr CreateCurvesElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pDatasetElement);
	
	MSXML2::IXMLDOMElementPtr CreateCompositeCurvesElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pDatasetElement);
	
	MSXML2::IXMLDOMElementPtr CreateSurfacesElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pDatasetElement);

	bool CreateFeatureTypeStringCodeElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pFeaturesElement);
	bool CreateAttributeStringCodeElement(CClass_S101_Base* pBase, ST_FEATURE_RECORD* pST_FEATURE_RECORD, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pFeatureTypeElement);
	bool CreateAttributeStringCodeElement(CClass_S101_Base* pBase, ST_INFO_RECORD* pST_INFO_RECORD, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pInformationTypeElement);
	bool CreateInformationTypeStringCodeElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pInformationTypesElement);
	bool CreatePointElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pPointsElement);
	bool CreateMultiPointElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pMultiPointsElement);
	bool CreateCurveElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pCurvesElement);
	bool CreateCompositeCurveElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pCompositeCurveElement);
	bool CreateSurfaceElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pSurfacesElement);

	CString GetFeatureTypeID(ST_FEATURE_RECORD* pST_FEATURE_RECORD);
	CString GetFeatureTypePrimitive(ST_FEATURE_RECORD* pST_FEATURE_RECORD);
	CString GetInformationTypeID(ST_INFO_RECORD* pST_INFO_RECORD);
	CString GetPointID(ST_POINT_RECORD* pST_POINT_RECORD);
	CString GetMultiPointID(ST_MULTI_POINT_RECORD* pST_MULTI_POINT_RECORD);
	CString GetInterpolation(int num);

	CString GetSpatialTypeNameFromRRNM(unsigned int nRRNM);
	CString GetOrientationFromORNT(unsigned int nORNT);

	bool AddParameters(MSXML2::IXSLProcessorPtr pProcessor);

	HRESULT hr_Initialize;
};