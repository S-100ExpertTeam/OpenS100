#pragma once

#include "S100SpatialObject.h"
#include "GF_FeatureType.h"
#include "GF_InformationType.h"
#include "GF_ComplexAttributeType.h"
#include "SPoint.h"
#include "SMultiPoint.h"
#include "SCurve.h"
#include "SCompositeCurve.h"
#include "SSurface.h"
#include "GML_Envelop.h"
#include "GML_DatasetIdentificationInformation.h"
#include "GM_Point.h"

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
    
    bool ReadMembers(pugi::xml_node& node);
    bool ReadFeature(pugi::xml_node& node, FeatureCatalogue* fc, FeatureType* featureType);
    bool ReadInformation(pugi::xml_node& node, FeatureCatalogue* fc, InformationType* informationType);
    bool ReadPoint(pugi::xml_node& node);
    bool ReadMultiPoint(pugi::xml_node& node);
    bool ReadCurve(pugi::xml_node& node);
    bool ReadOrientableCurve(pugi::xml_node& node);
    bool ReadCompositeCurve(pugi::xml_node& node);
    bool ReadSurface(pugi::xml_node& node);

    bool ReadObjectAttribute(pugi::xml_node& node, GF::ObjectType* object, FeatureCatalogue* fc);
    bool ReadFeatureGeometry(pugi::xml_node& node, GF::FeatureType* feature);
    bool ReadFeatureRole(pugi::xml_node& node, GF::FeatureType* feature, FeatureCatalogue* fc);
    bool ReadInformationRole(pugi::xml_node& node, GF::InformationType* information, FeatureCatalogue* fc);

    bool AddSubAttribute(pugi::xml_node& node, GF::ComplexAttributeType* complexAttribute, FeatureCatalogue* fc);

    GM::Point* GetPoint(int x, int y);
};