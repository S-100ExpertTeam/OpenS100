#include "stdafx.h"
#include "CreateInputSchemaS101.h"

#include "..\\GISLibrary\\S101Cell.h"
#include "..\\GISLibrary\\F_CUCO.h"
#include "..\\GISLibrary\\F_PTAS.h"
#include "..\\GISLibrary\\F_SPAS.h"
#include "..\\GISLibrary\\F_FASC.h"
#include "..\\GISLibrary\\F_C3IL.h"
#include "..\\GISLibrary\\F_C2IL.h"
#include "..\\GISLibrary\\F_C2IT.h"
#include "..\\GISLibrary\\F_ATTR.h"
#include "..\\GISLibrary\\F_INAS.h"
#include "..\\GISLibrary\\F_RIAS.h"
#include "..\\GISLibrary\\R_InformationRecord.h"
#include "..\\GISLibrary\\R_FeatureRecord.h"
#include "..\\GISLibrary\\R_MultiPointRecord.h"
#include "..\\GISLibrary\\R_CurveRecord.h"
#include "..\\GISLibrary\\R_SurfaceRecord.h"
#include "..\\GISLibrary\\R_PointRecord.h"
#include "..\\GISLibrary\\R_CompositeRecord.h"
#include "..\\GISLibrary\\ATTR.h"
#include "..\\GISLibrary\\C3IL.h"
#include "..\\GISLibrary\\IC2D.h"
#include "..\\GISLibrary\\\CodeWithNumericCode.h"
#include "..\\GISLibrary\\CUCO.h"
#include "..\\GISLibrary\\SPAS.h"
#include "..\\GISLibrary\\RIAS.h"
#include "..\\GISLibrary\\PTAS.h"

#include "..\\S100Geometry\\SGeometry.h"

#include <sstream> 

std::wstring get_wstring_from_coordinate(int value)
{
	std::wstringstream wss;    
		wss.setf(std::ios::fixed);
	wss.setf(std::ios::showpoint);
	wss.precision(7);

	wss << value / 10000000.;
	return wss.str();
}

std::wstring get_wstring_from_double(double value) 
{
	std::wstringstream wss;
	wss.precision(2);

	wss << value;
	return wss.str();
}

CreateInputSchemaS101::CreateInputSchemaS101()
{

}
CreateInputSchemaS101::~CreateInputSchemaS101()
{

}

void CreateInputSchemaS101::CreatePortrayalCatalogueInputData(S101Cell* cell/*, bool isNew*/)
{
	//function that creates XML files.
	std::wstring inputPath_n = L"..\\ProgramData\\S100_PC_IO_XML\\INPUT\\";
	inputPath_n.append(std::wstring(cell->GetFileName()));
	inputPath_n.erase(inputPath_n.end() - 4, inputPath_n.end());
	inputPath_n.append(L".xml");

	pugi::xml_document doc;
	auto declarationNode = doc.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "1.0";
	declarationNode.append_attribute("encoding") = "UTF-8";

	auto root = doc.append_child("Dataset"); //Top node.
	root.append_attribute("xmlns:xsi") = "http://www.w3.org/2001/XMLSchema-instance";
	root.append_attribute("xmlns:S100") = "http://www.iho.int/S100BaseModel";
	root.append_attribute("xmlns:noNamespaceSchemaLocation") = "S101DataModel.xsd";

	pugi::xml_node nodeChild = root.append_child("InformationTypes");
	SetInformationsType(cell, nodeChild);

	nodeChild = root.append_child("Points");
	SetVectorPointsType(cell, nodeChild);

	nodeChild = root.append_child("MultiPoints");
	SetVectorMultiPointsType(cell, nodeChild);

	nodeChild = root.append_child("Curves");
	SetVectorCurvesType(cell, nodeChild);

	nodeChild = root.append_child("CompositeCurves");
	SetVectorCompositeCurvesType(cell, nodeChild);

	nodeChild = root.append_child("Surfaces");
	SetVectorSurfacesType(cell, nodeChild);

	nodeChild = root.append_child("Features");
	SetFeaturesType(cell, nodeChild);

	bool saveSucceeded = doc.save_file(inputPath_n.c_str(), PUGIXML_TEXT("  "));

}

#pragma warning(disable:4244)
#pragma warning(disable:4018)
void CreateInputSchemaS101::SetFeaturesType(S101Cell* cell, pugi::xml_node node)
{
	__int64 key = 0;
	R_FeatureRecord* fr = NULL;
	POSITION pos = cell->GetFeatureStartPosition();
	while (pos != NULL)
	{
		cell->GetNextAssoc(pos, key, fr);

		auto fi = cell->m_dsgir.m_ftcs->m_arr.find(fr->m_frid.m_nftc);
		if (cell->m_dsgir.m_ftcs->m_arr.end()== fi)
		{
			continue;

		}
		CString featureAcronym = fi->second->m_code;

		pugi::xml_node pFeatureElement = node.append_child((CStringA)featureAcronym);
		pFeatureElement.append_attribute("id").set_value(fr->m_frid.m_name.RCID);

		//featureAcronym.ReleaseBuffer();
		std::wstring spatialType;

		if (fr->m_geometry)
		{
			switch (fr->m_geometry->type)
			{
			case 1:
				spatialType = L"Point";
				break;
			case 2:
				spatialType = L"Curve";
				break;
			case 3:
				spatialType = L"Surface";
				break;
			case 4:
				spatialType = L"MultiPoint";
				break;
			}
			pFeatureElement.append_attribute("primitive").set_value((CStringA)spatialType.c_str());

			for (auto itorParent = fr->m_spas.begin(); itorParent != fr->m_spas.end(); itorParent++)
			{
				F_SPAS *spasParent = *itorParent;

				for (auto itor = spasParent->m_arr.begin(); itor != spasParent->m_arr.end(); itor++)
				{
					SPAS* spas = *itor;
					CString spatialAssociation;
					if (spas->m_name.RCNM == 110)
					{
						spatialAssociation.Format(L"Point");
					}
					else if (spas->m_name.RCNM == 115)
					{
						spatialAssociation.Format(L"MultiPoint");
					}
					else if (spas->m_name.RCNM == 120)
					{
						spatialAssociation.Format(L"Curve");
					}
					else if (spas->m_name.RCNM == 125)
					{
						CompositeCurveToCurve(cell, pFeatureElement, spas->m_name.RCID, spas->m_ornt);
						continue;
					}
					else if (spas->m_name.RCNM == 130)
					{
						spatialAssociation.Format(L"Surface");
					}

					pugi::xml_node pSpatialAssociation = pFeatureElement.append_child((CStringA)spatialAssociation);
					//spatialAssociation.ReleaseBuffer();

					CString spatialAssociationValue;
					spatialAssociationValue.Format(L"%s|%d", spatialAssociation, spas->m_name.RCID);
					pSpatialAssociation.append_attribute("ref").set_value((CStringA)spatialAssociationValue);
					//spatialAssociationValue.ReleaseBuffer();

					if (spas->m_name.RCNM == 120 || spas->m_name.RCNM == 125)
					{
						std::wstring orientation;
						switch (spas->m_ornt)
						{
						case 1:
							orientation = L"Forward";
							break;
						case 2:
							orientation = L"Reverse";
							break;
						}

						pSpatialAssociation.append_attribute("orientation").set_value((CStringA)orientation.c_str());
					}
				}
			}
		}
		else
		{
			spatialType = L"None";
			pFeatureElement.append_attribute("primitive").set_value((CStringA)spatialType.c_str());
		}

		if (fr->m_attr.size() > 0)
		{
			std::vector<pugi::xml_node> attributeVector;

			for (auto itorParent = fr->m_attr.begin(); itorParent != fr->m_attr.end(); itorParent++)
			{
				F_ATTR* attrParent = *itorParent;

				for (auto itor = attrParent->m_arr.begin(); itor != attrParent->m_arr.end(); itor++)
				{
					ATTR* attr = *itor;
					auto aitor = cell->m_dsgir.m_atcs->m_arr.find(attr->m_natc);

					if (aitor != cell->m_dsgir.m_atcs->m_arr.end())
					{
						CString attributeAcronym = aitor->second->m_code;

						pugi::xml_node pAttributeElement = pFeatureElement.append_child((CStringA)attributeAcronym);
						pAttributeElement.append_child(pugi::node_pcdata).set_value(pugi::as_utf8(attr->m_atvl).c_str());

						//attributeAcronym.ReleaseBuffer();

						if (attr->m_paix) //In the case of entering as a child,
						{
							unsigned nodeNumber = (unsigned)attributeVector.size();

							if (attr->m_paix <= nodeNumber)
							{
								pugi::xml_node item = attributeVector[attr->m_paix - 1];
								attributeVector.push_back(pAttributeElement);
								item.append_move(pAttributeElement);
							}
							else
							{
								OutputDebugString(L"Attribute Item Error");
							}
						}
						else
						{
							attributeVector.push_back(pAttributeElement);
							pFeatureElement.append_move(pAttributeElement);
						}
					}
				}
			}
		}

		if (fr->m_fasc.size() > 0) //Add attribute value.
		{
			for (auto itorParent = fr->m_fasc.begin(); itorParent != fr->m_fasc.end(); itorParent++)
			{
				F_FASC* f_fasc = *itorParent;

				auto asitor = cell->m_dsgir.m_facs->m_arr.find(f_fasc->m_nfac);
				auto ritor = cell->m_dsgir.m_arcs->m_arr.find(f_fasc->m_narc);

				std::wstring asName = asitor->second->m_code;
				std::wstring roleName = ritor->second->m_code;

				pugi::xml_node pElement = pFeatureElement.append_child((CStringA)asName.c_str());
				pElement.append_attribute("role").set_value((CStringA)roleName.c_str());
				pElement.append_attribute("featureRef").set_value(f_fasc->m_name.RCID);

			}
		}

		if (fr->m_inas.size() > 0) //Add attribute value.
		{
			for (auto itorParent = fr->m_inas.begin(); itorParent != fr->m_inas.end(); itorParent++)
			{
				F_INAS* f_inas = *itorParent;

				auto asitor = cell->m_dsgir.m_iacs->m_arr.find(f_inas->m_niac);

				auto ritor = cell->m_dsgir.m_arcs->m_arr.find(f_inas->m_narc);

				std::wstring asName = asitor->second->m_code;
				std::wstring roleName = ritor->second->m_code;

				pugi::xml_node pElement = pFeatureElement.append_child((CStringA)asName.c_str());
				pElement.append_attribute("role").set_value((CStringA)roleName.c_str());
				pElement.append_attribute("informationRef").set_value(f_inas->m_name.RCID);
			}
		}
	}


}

#pragma warning(disable:4244)
#pragma warning(disable:4018)
void CreateInputSchemaS101::SetInformationsType(S101Cell* cell, pugi::xml_node node)
{
	__int64 key = 0;
	R_InformationRecord* ir = NULL;
	POSITION pos = cell->GetInfoStartPosition();

	while (pos != NULL)
	{
		cell->GetNextAssoc(pos, key, ir);
		auto fi = cell->m_dsgir.m_itcs->m_arr.find(ir->m_irid.m_nitc);
		if(cell->m_dsgir.m_itcs->m_arr.end()==fi)
		{
			continue;
		}
		CString informationAcronym = fi->second->m_code;


		//NauticalInformation Element
		pugi::xml_node NauticalInformation = node.append_child((CStringA)informationAcronym);
		NauticalInformation.append_attribute("id") = ir->m_irid.m_name.RCID; //Add attributes.

		//informationAcronym.ReleaseBuffer();

		if (ir->m_attr.size() > 0)
		{
			std::vector<pugi::xml_node> attributeVector;

			for (auto itorParent = ir->m_attr.begin(); itorParent != ir->m_attr.end(); itorParent++)
			{
				F_ATTR* attrParent = *itorParent;

				for (auto itor = attrParent->m_arr.begin(); itor != attrParent->m_arr.end(); itor++)
				{
					ATTR* attr = *itor;
					auto aitor = cell->m_dsgir.m_atcs->m_arr.find(attr->m_natc);
					if (aitor != cell->m_dsgir.m_atcs->m_arr.end())
					{
						CString attributeAcronym = aitor->second->m_code;

						pugi::xml_node informationchild = NauticalInformation.append_child((CStringA)attributeAcronym);
						informationchild.append_child(pugi::node_pcdata).set_value(pugi::as_utf8(attr->m_atvl).c_str());

						if (attr->m_paix)
						{
							unsigned nodeNumber = (unsigned)attributeVector.size();

							if (attr->m_paix <= nodeNumber)
							{
								pugi::xml_node item = attributeVector[attr->m_paix - 1];
								item.append_move(informationchild);
								attributeVector.push_back(informationchild);
							}
							else
							{
								OutputDebugString(L"Attribute Item Error");
							}

						}
						else
						{
							attributeVector.push_back(informationchild);
						}


					}
				}
			}
		}
		if (ir->m_inas.size() > 0)
		{
			for (auto itorParent = ir->m_inas.begin(); itorParent != ir->m_inas.end(); itorParent++)
			{
				//This belongs to Feature's child value.
				F_INAS* f_inas = *itorParent;

				auto asitor = cell->m_dsgir.m_iacs->m_arr.find(f_inas->m_niac);
				auto ritor = cell->m_dsgir.m_arcs->m_arr.find(f_inas->m_narc);

				std::wstring asName = asitor->second->m_code;
				std::wstring roleName = ritor->second->m_code;

				pugi::xml_node pAttributeElement = NauticalInformation.append_child((CStringA)asName.c_str());
				pAttributeElement.append_attribute("role").set_value((CStringA)roleName.c_str());

				CString strRCID;
				strRCID.Format(_T("%d"), f_inas->m_name.RCID);
				pAttributeElement.append_attribute("informationRef").set_value((CStringA)strRCID);

			}
		}

	}
}

void CreateInputSchemaS101::SetVectorPointsType(S101Cell* cell, pugi::xml_node node)
{
	__int64 key = 0;
	R_PointRecord* sr = NULL;
	POSITION pos = cell->GetpointStartPosition();
	while (pos != NULL)
	{
		cell->GetNextAssoc(pos, key, sr);
		pugi::xml_node point = node.append_child("Point");

		std::string id = "Point|" + std::to_string(sr->GetRCID());
		
		point.append_attribute("id").set_value(id.c_str());

		std::wstring pointtxt = L"";
		pointtxt.assign(id.begin(), id.end());
		pointtxt.append(L" input xml");
		pointtxt.append(L" Making Clear");

		if (sr->m_c2it)
		{
			pugi::xml_node Coordinate2D = point.append_child("Coordinate2D");

			pugi::xml_node XElement = Coordinate2D.append_child("x");
			XElement.append_child(pugi::node_pcdata).set_value((CStringA)get_wstring_from_coordinate(sr->m_c2it->m_xcoo).c_str());

			pugi::xml_node YElement = Coordinate2D.append_child("y");
			YElement.append_child(pugi::node_pcdata).set_value((CStringA)get_wstring_from_coordinate(sr->m_c2it->m_ycoo).c_str());
		}
	}
}

void CreateInputSchemaS101::SetVectorMultiPointsType(S101Cell* cell, pugi::xml_node node)
{
	__int64 key = 0;
	R_MultiPointRecord* sr = NULL;
	POSITION pos = cell->GetMultStartPosition();

	while (pos != NULL)
	{
		cell->GetNextAssoc(pos, key, sr);

		pugi::xml_node pSpatialElement = node.append_child("MultiPoint");
		std::string id = "MultiPoint|" + std::to_string(sr->GetRCID());
		pSpatialElement.append_attribute("id").set_value(id.c_str());

		std::wstring MultiPoint = L"";
		MultiPoint.assign(id.begin(), id.end());
		MultiPoint.append(L" input xml");
		MultiPoint.append(L" Making Clear");

		for (auto itor = sr->m_c3il.begin(); itor != sr->m_c3il.end(); itor++)
		{
			F_C3IL *c3il = *itor;
			for (auto itor = c3il->m_arr.begin(); itor != c3il->m_arr.end(); itor++)
			{
				pugi::xml_node pCoordinates = pSpatialElement.append_child("Coordinate3D");

				C3IL* c3il = *itor;

				pugi::xml_node pX = pCoordinates.append_child("x");
				pX.append_child(pugi::node_pcdata).set_value((CStringA)get_wstring_from_coordinate(c3il->m_xcoo).c_str());

				pugi::xml_node pY = pCoordinates.append_child("y");
				pY.append_child(pugi::node_pcdata).set_value((CStringA)get_wstring_from_coordinate(c3il->m_ycoo).c_str());

				pugi::xml_node pZ = pCoordinates.append_child("z");
				pZ.append_child(pugi::node_pcdata).set_value((CStringA)get_wstring_from_double(c3il->m_zcoo / (double)cell->m_dsgir.m_dssi.m_cmfz).c_str());

			}
		}

	}

}

void CreateInputSchemaS101::SetCurveElement(pugi::xml_node node, R_CurveRecord* curveRecord)
{
	CString spatialType = L"Curve";
	pugi::xml_node pSpatialElement = node.append_child("Curve");
	//spatialType.ReleaseBuffer();

	std::string id= "Curve|"+ std::to_string(curveRecord->GetRCID());
	pSpatialElement.append_attribute("id").set_value(id.c_str());
	

	for (auto i = curveRecord->m_ptas->m_arr.begin(); i != curveRecord->m_ptas->m_arr.end(); i++)
	{
		PTAS* ptas = *i;

		std::wstring boundaryType;

		switch (ptas->m_topi)
		{
		case 1:
			boundaryType = L"Begin";
			break;
		case 2:
			boundaryType = L"End";
			break;
		}
		pugi::xml_node pBoundary = pSpatialElement.append_child("Boundary");

		std::string nodeSpatialType = "Point|" + std::to_string(ptas->m_name.RCID);

		std::wstring pointtxt = L"";
		pointtxt.assign(id.begin(), id.end());
		pointtxt.append(L" input xml");
		pointtxt.append(L" Making Clear");


		pBoundary.append_attribute("ref").set_value(nodeSpatialType.c_str());
		pBoundary.append_attribute("boundaryType").set_value((CStringA)boundaryType.c_str());
	}

	for (auto itorParent = curveRecord->m_c2il.begin(); itorParent != curveRecord->m_c2il.end(); itorParent++)
	{
		F_C2IL* c2il = *itorParent;
		pugi::xml_node pSegment = pSpatialElement.append_child("Segment");
		pSegment.append_attribute("interpolation").set_value("Loxodromic");

		for (auto itor = c2il->m_arr.begin(); itor != c2il->m_arr.end(); itor++)
		{
			pugi::xml_node pCoordinates = pSegment.append_child("ControlPoint");

			IC2D* ic2d = *itor;
			pugi::xml_node pX = pCoordinates.append_child("X");
			pX.append_child(pugi::node_pcdata).set_value((CStringA)get_wstring_from_coordinate(ic2d->m_xcoo).c_str());

			pugi::xml_node pY = pCoordinates.append_child("Y");
			pY.append_child(pugi::node_pcdata).set_value((CStringA)get_wstring_from_coordinate(ic2d->m_ycoo).c_str());
		}
	}
}

void CreateInputSchemaS101::SetVectorCurvesType(S101Cell* cell, pugi::xml_node node)
{
	__int64 key = 0;
	R_CurveRecord* cr = NULL;
	POSITION pos = cell->GetCurStartPosition();

	while (pos != NULL)
	{
		cell->GetNextAssoc(pos, key, cr);
		SetCurveElement(node, cr);
	}

}

void CreateInputSchemaS101::SetVectorCompositeCurvesType(S101Cell* cell, pugi::xml_node node)
{
	__int64 key = 0;
	R_CompositeRecord* sr = NULL;
	POSITION pos = cell->GetComStartPosition();

	while (pos != NULL)
	{
		cell->GetNextAssoc(pos, key, sr);

		pugi::xml_node pSpatialElement = node.append_child("CompositeCurve");
		std::string 	id = "CompositeCurve|" + std::to_string(sr->GetRCID());
		pSpatialElement.append_attribute("id").set_value(id.c_str());

		std::wstring pointtxt = L"";
		pointtxt.assign(id.begin(),id.end());
		pointtxt.append(L" input xml");
		pointtxt.append(L" Making Clear");

		for (auto itorParent = sr->m_cuco.begin(); itorParent != sr->m_cuco.end(); itorParent++)
		{
			F_CUCO* cucoParent = *itorParent;

			for (auto itor = cucoParent->m_arr.begin(); itor != cucoParent->m_arr.end(); itor++)
			{
				CUCO* cuco = *itor;

				switch (cuco->m_name.RCNM)
				{
				case 120:
					break;
				case 125:
					CompositeCurveToCurve(cell, pSpatialElement, cuco->m_name.RCID, cuco->m_ornt);
					continue;
				}

				std::wstring orientation;
				switch (cuco->m_ornt)
				{
				case 1:
					orientation = L"Forward";
					break;
				case 2:
					orientation = L"Reverse";
					break;
				}
				//Curve 
				pugi::xml_node pSpatialReference = pSpatialElement.append_child("Curve");
				std::string id1 = "Curve|" + std::to_string(cuco->m_name.RCID);
				pSpatialReference.append_attribute("ref").set_value(id1.c_str());
				pSpatialReference.append_attribute("orientation").set_value((CStringA)orientation.c_str());
			
				std::wstring curve = L"";
				curve.assign(id1.begin(), id1.end());
				curve.append(L" input xml");
				curve.append(L" Making Clear");
			}
		}
	}
}

void CreateInputSchemaS101::SetVectorSurfacesType(S101Cell* cell, pugi::xml_node node)
{
	__int64 key = 0;
	R_SurfaceRecord* sr = NULL;
	POSITION pos = cell->GetSurStartPosition();
	while (pos != NULL)
	{
		cell->GetNextAssoc(pos, key, sr);
		CString spatialType = L"Surface";
		pugi::xml_node pSpatialElement = node.append_child((CStringA)spatialType);
		//spatialType.ReleaseBuffer();
		CString id;
		id.Format(L"%s|%d", spatialType, sr->m_srid.m_name.RCID);
		pSpatialElement.append_attribute("id").set_value((CStringA)id);

		std::wstring pointtxt = L"";
		pointtxt.append(id);
		pointtxt.append(L" input xml");
		pointtxt.append(L" Making Clear");

		//id.ReleaseBuffer();
		for (auto itorParent = sr->m_rias.begin(); itorParent != sr->m_rias.end(); itorParent++)
		{
			F_RIAS* riasParent = *itorParent;

			for (auto itor = riasParent->m_arr.begin(); itor != riasParent->m_arr.end(); itor++)
			{
				RIAS* rias = *itor;

				std::wstring usage;
				switch (rias->m_usag)
				{
				case 1:
					usage = L"Outer";
					break;
				case 2:
					usage = L"Inner";
					break;
				}
				pugi::xml_node pRing = pSpatialElement.append_child("Ring");
				pRing.append_attribute("type").set_value((CStringA)usage.c_str());

				std::wstring spatialType;
				switch (rias->m_name.RCNM)
				{
				case 120:
					spatialType = L"Curve";
					break;
				case 125:
					spatialType = L"CompositeCurve";
					break;
				}

				std::wstring orientation;
				switch (rias->m_ornt)
				{
				case 1:
					orientation = L"Forward";
					break;
				case 2:
					orientation = L"Reverse";
					break;
				}
				pugi::xml_node pSpatialReference = pRing.append_child((CStringA)spatialType.c_str());

				CString id;
				id.Format(L"%s|%d", spatialType.c_str(), rias->m_name.RCID);

				pSpatialReference.append_attribute("ref").set_value((CStringA)id);
				//id.ReleaseBuffer();

				pSpatialReference.append_attribute("orientation").set_value((CStringA)orientation.c_str());
			}
		}
	}
}

void CreateInputSchemaS101::CompositeCurveToCurve(S101Cell* cell, pugi::xml_node node, int RCID, int ORNT)
{
	__int64 iKey = ((__int64)125) << 32 | ((__int64)RCID);
	auto itorFind = cell->m_vecMap.find(iKey);
	if (itorFind != cell->m_vecMap.end())
	{
		R_CompositeRecord* ccr = (R_CompositeRecord*)itorFind->second;

		CUCO *cuco = NULL;
		R_CurveRecord* cr = NULL;

		if (ORNT == 1)
		{
			for (auto itorParent = ccr->m_cuco.begin(); itorParent != ccr->m_cuco.end(); itorParent++)
			{
				F_CUCO* cucoParent = *itorParent;

				for (auto itor = cucoParent->m_arr.begin(); itor != cucoParent->m_arr.end(); itor++)
				{
					cuco = *itor;
					int curORNT = ORNT;

					if (cuco->m_ornt == 2)
					{
						curORNT = ORNT == 1 ? 2 : 1;
					}

					if (cuco->m_name.RCNM == 120)
					{
						iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;

						cr=cell->GetCurveRecord(iKey);
						if (cr)
						{
							std::wstring spatialType;

							spatialType = L"Curve";

							std::wstring orientation;
							switch (curORNT)
							{
							case 1:
								orientation = L"Forward";
								break;
							case 2:
								orientation = L"Reverse";
								break;
							}
							pugi::xml_node pSpatialReference = node.append_child((CStringA)spatialType.c_str());

							CString id;
							id.Format(L"%s|%d", spatialType.c_str(), cuco->m_name.RCID);


							std::wstring pointtxt = L"";
							pointtxt.append(id);
							pointtxt.append(L" input xml");
							pointtxt.append(L" Making Clear");


							pSpatialReference.append_attribute("ref").set_value((CStringA)id);
							//id.ReleaseBuffer();

							pSpatialReference.append_attribute("orientation").set_value((CStringA)orientation.c_str());

						}
					}
					else if (cuco->m_name.RCNM == 125)
					{
						CompositeCurveToCurve(cell, node, cuco->m_name.RCID, ORNT);
					}
				}
			}
		}
		else if (ORNT == 2)
		{
			for (auto itorParent = ccr->m_cuco.begin(); itorParent != ccr->m_cuco.end(); itorParent++)
			{
				F_CUCO* cucoParent = *itorParent;

				for (auto itor = cucoParent->m_arr.rbegin(); itor != cucoParent->m_arr.rend(); itor++)
				{
					cuco = *itor;
					iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;

					int curORNT = ORNT;

					if (cuco->m_ornt == 2)
					{
						curORNT = ORNT == 1 ? 2 : 1;
					}

					if (cuco->m_name.RCNM == 120)
					{
						std::wstring spatialType;

						spatialType = L"Curve";

						std::wstring orientation;
						switch (curORNT)
						{
						case 1:
							orientation = L"Forward";
							break;
						case 2:
							orientation = L"Reverse";
							break;
						}
						pugi::xml_node pSpatialReference = node.append_child((CStringA)spatialType.c_str());

						CString id;
						id.Format(L"%s|%d", spatialType.c_str(), cuco->m_name.RCID);

						std::wstring pointtxt = L"";
						pointtxt.append(id);
						pointtxt.append(L" input xml");
						pointtxt.append(L" Making Clear");


						pSpatialReference.append_attribute("ref").set_value((CStringA)id);
						//id.ReleaseBuffer();

						pSpatialReference.append_attribute("orientation").set_value((CStringA)orientation.c_str());
					}
					else if (cuco->m_name.RCNM == 125)
					{
						if (cuco->m_ornt == 2)
						{
							ORNT = ORNT == 1 ? 2 : 1;
						}
						CompositeCurveToCurve(cell, node, cuco->m_name.RCID, ORNT);
					}
				}
			}
		}
	}
	else
	{
	}

}

void CreateInputSchemaS101::SurfaceToCurve(S101Cell* cell, pugi::xml_node node)
{

}