#pragma once

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

class S10XGML :
    public S100SpatialObject
{
public:
    S10XGML();
    virtual ~S10XGML();

public:
    S100GML::Envelop envelop;
    S100GML::DatasetIdentificationInformation datasetIdentificationInformation;

    std::vector<GF::FeatureType*> features;
    std::vector<GF::InformationType*> informations;

    std::vector<GM::Object*> geometries;

public:
    bool Open(CString _filepath) override;
    bool SaveToInputXML(std::string path) override;

    GM::Object* GetGeometry(std::string id) override;

    bool ReadMembers(pugi::xml_node& node);
    GF::FeatureType* ReadFeature(pugi::xml_node& node, FeatureCatalogue* fc);
    GF::InformationType* ReadInformation(pugi::xml_node& node, FeatureCatalogue* fc);
    GM::Point* ReadPoint(pugi::xml_node& node);
    GM::MultiPoint* ReadMultiPoint(pugi::xml_node& node);
    GM::Curve* ReadCurve(pugi::xml_node& node);
    GM::OrientableCurve* ReadOrientableCurve(pugi::xml_node& node);
    GM::CompositeCurve* ReadCompositeCurve(pugi::xml_node& node);
    GM::Surface* ReadSurface(pugi::xml_node& node);
    bool ReadMember(pugi::xml_node& node);

    bool ReadObjectAttribute(pugi::xml_node& node, GF::ObjectType* object, FeatureCatalogue* fc);
    bool ReadFeatureGeometry(pugi::xml_node& node, GF::FeatureType* feature);
    bool ReadFeatureRole(pugi::xml_node& node, GF::FeatureType* feature, FeatureCatalogue* fc);
    bool ReadInformationRole(pugi::xml_node& node, GF::InformationType* information, FeatureCatalogue* fc);

    bool AddSubAttribute(pugi::xml_node& node, GF::ComplexAttributeType* complexAttribute, FeatureCatalogue* fc);

    GM::Point* GetPoint(int x, int y);
    void AddGeometry(GM::Object* geometry);

    // input xml
    bool WriteInputXML_FeatureTypes(pugi::xml_node& node);
    bool WriteInputXML_FeatureType_SimpleAttribute(pugi::xml_node& node, GF::SimpleAttributeType* simpleAttribute);
    bool WriteInputXML_FeatureType_ComplexAttribute(pugi::xml_node& node, GF::ComplexAttributeType* complexAttribute);

private:
    std::string DeleteXMLNamespace(std::string value);
    std::string getCodeFromMember(std::string nodeName);
};