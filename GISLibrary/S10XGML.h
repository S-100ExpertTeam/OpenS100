#pragma once

#include "S100GML_NameSpace.h"
#include "S100SpatialObject.h"
#include "GF_FeatureType.h"
#include "GF_InformationType.h"
#include "GF_ComplexAttributeType.h"
#include "SPoint.h"
#include "SMultiPoint.h"
#include "SCompositeCurve.h"
#include "SSurface.h"
#include "GML_Envelop.h"
#include "GML_DatasetIdentificationInformation.h"
#include "GM_Point.h"
#include "GM_MultiPoint.h"
#include "GM_OrientableCurve.h"
#include "GM_Curve.h"
#include "GM_CompositeCurve.h"
#include "GM_Surface.h"


#include <vector>
#include <unordered_map>

class S10XGML :
    public S100SpatialObject
{
public:
    S10XGML();
    S10XGML(D2D1Resources* d2d1);
    virtual ~S10XGML();

public:
    S100GML::Envelop envelop;
    S100GML::DatasetIdentificationInformation datasetIdentificationInformation;

    std::vector<GF::FeatureType*> features;
    std::vector<GF::InformationType*> informations;

    std::vector<GM::Object*> geometries;

    S100GML_NameSpace s100namespace;
public:
    bool Open(CString _filepath) override;
    bool OpenMetadata(CString _filepath) override;
    bool Open(std::string fileContent);
    bool OpenMetadata(std::string fileContent);
    bool Open(pugi::xml_node doc, pugi::xml_parse_result result);
    bool OpenMetadata(pugi::xml_node doc, pugi::xml_parse_result result);
    bool SaveToInputXML(std::string path) override;
    std::string toInputXML();
    pugi::xml_document toXmlDocument();

    bool ReadMembers(pugi::xml_node& node);
    GF::FeatureType* ReadFeature(pugi::xml_node& node, FeatureCatalogue* fc);
    GF::InformationType* ReadInformation(pugi::xml_node& node, FeatureCatalogue* fc);

    // If node is root of the geometry, id & srsName is null
    GM::Point* ReadPoint(pugi::xml_node& node, std::string id = "", std::string srsName = "");
    GM::MultiPoint* ReadMultiPoint(pugi::xml_node& node, std::string id = "", std::string srsName = "");
    GM::Curve* ReadCurve(pugi::xml_node& node, std::string id = "", std::string srsName = "");
    GM::OrientableCurve* ReadOrientableCurve(pugi::xml_node& node, std::string id = "", std::string srsName = "");
    GM::CompositeCurve* ReadCompositeCurve(pugi::xml_node& node, std::string id = "", std::string srsName = "");
    GM::Curve* ReadLinearRing(pugi::xml_node& node); // read gml:LinearRing
    GM::Surface* ReadSurface(pugi::xml_node& node); // read S100:Surface
    
    // node : Polygon node
    GM::Surface* ReadPolygon(pugi::xml_node& node);
    bool ReadMember(pugi::xml_node& node);
    bool ReadIMember(pugi::xml_node& node);

    bool ReadObjectAttribute(pugi::xml_node& node, GF::ObjectType* object, FeatureCatalogue* fc);
    bool ReadFeatureGeometry(pugi::xml_node& node, GF::FeatureType* feature);
    bool ReadFeatureRole(pugi::xml_node& node, GF::FeatureType* feature, FeatureCatalogue* fc);
    bool ReadInformationRole(pugi::xml_node& node, GF::InformationType* information, FeatureCatalogue* fc);

    bool AddSubAttribute(pugi::xml_node& node, GF::ComplexAttributeType* complexAttribute, FeatureCatalogue* fc);

    GM::Point* GetPoint(int x, int y);
    GM::Object* AddGeometry(GM::Object* geometry);

    // input xml
    bool WriteInputXML_FeatureTypes(pugi::xml_node& node);
    bool WriteInputXML_FeatureType_SimpleAttribute(pugi::xml_node& node, GF::SimpleAttributeType* simpleAttribute);
    bool WriteInputXML_FeatureType_ComplexAttribute(pugi::xml_node& node, GF::ComplexAttributeType* complexAttribute);

    bool WriteInputXML_InformationTypes(pugi::xml_node& node);

    void CalcMBR();

public:
    std::wstring GetFeatureTypeCodeByID(std::wstring id) override;
    std::wstring GetFeatureTypeCodeByID(std::string id) override;
    std::wstring GetFeatureTypeCodeByID(int id) override;

    std::wstring GetInformationTypeCodeByID(std::wstring id) override;
    std::wstring GetInformationTypeCodeByID(std::string id) override;
    std::wstring GetInformationTypeCodeByID(int id) override;
	
    int GetFeatureCount() override;
    int GetInformationCount() override;

    GF::FeatureType* GetFeatureType(std::string id) override;
    GF::FeatureType* GetFeatureTypeByIndex(int index) override;

    GF::InformationType* GetInformationType(std::string id) override;
    GF::InformationType* GetInformationTypeByIndex(int index) override;

    std::string GetFeatureAssociationCode(GF::FeatureType* featureType, int index) override;
    std::string GetFeatureAssociationRoleCode(GF::FeatureType* featureType, int index) override;

    std::string GetInformationAssociationCode(GF::FeatureType* featureType, int index) override;
    std::string GetInformationAssociationRoleCode(GF::FeatureType* featureType, int index) override;

    std::string GetInformationAssociationCode(GF::InformationType* informationType, int index) override;
    std::string GetInformationAssociationRoleCode(GF::InformationType* informationType, int index) override;

    //std::string GetObjectAttributeCode(int type, std::string id, int index) override;

    //int GetFeatureAttributeCount(std::string id) override;
    //std::string GetFeatureAttributeCode(std::string, int index) override;
	
    //int GetInformationAttributeCount(std::string id) override;
    //std::string GetInformationAttributeCode(std::string, int index) override;

    GM::Object* GetGeometry(std::string id) override;

    GM::OrientableCurve* GetOrientableCurve(std::string id);

    void SetGeometry();


    SSurface* SurfaceToSSurface(GM::Surface* surface);

private:
    void Init();
    std::string DeleteXMLNamespace(std::string value);
    std::string getCodeFromMember(std::string nodeName);
    SPoint* PointToSPoint(GM::Point* point);
    SMultiPoint* MultiPointToSMultiPoint(GM::MultiPoint* multiPoint);
    SAbstractCurve* OrientableCurveToSCurve(GM::OrientableCurve* orientableCurve);
    SCurve* CurveToSCurve(GM::Curve* curve);
    SCompositeCurve* CompositeCurveToSCompositeCurve(GM::CompositeCurve* compositeCurve);
    
    // for namespace name
    bool GetNameSpaceName(pugi::xml_attribute, std::string& name);
};