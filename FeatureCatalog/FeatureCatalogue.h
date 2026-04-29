#pragma once

#include "ResponsibleParty.h"
#include "DefinitionSources.h"
#include "SimpleAttributes.h"
#include "ComplexAttributes.h"
#include "Roles.h"
#include "InformationAssociations.h"
#include "FeatureAssociations.h"
#include "InformationTypes.h"
#include "FeatureTypes.h"
#include "Version.h"

#include <pugixml.hpp>

#include <string>

class SimpleAttribute;
class ComplexAttribute;
class FeatureType;
class InformationType;

class FeatureCatalogue
{
public:
	FeatureCatalogue();
	//FeatureCatalogue(std::string path);
	virtual ~FeatureCatalogue();

protected:
	std::string name = "";
	std::string scope = "";

protected:
	std::string* fieldOfApplication = nullptr;  

protected:
	//std::string versionNumber = "";
	Version versionNumber;
	std::string versionDate = ""; 
	std::string productId = "";
	
	std::string filePath = "";

	// CI_Responsibility 
	//ResponsibleParty producer; 
	// FC_DefinitionSource 
	//DefinitionSources definitionSources; 
	SimpleAttributes simpleAttributes;
	ComplexAttributes complexAttributes;
	Roles roles;
	InformationAssociations informationAssociations;
	FeatureAssociations featureAssociations;
	InformationTypes informationTypes;
	FeatureTypes featureTypes; 

public:
	bool Read(std::string filePath);
	bool Save(std::string filePath);

	void GetContents(pugi::xml_node& node);
	void WriteContents(pugi::xml_node& node);

	const std::string& GetName();
	void SetName(std::string value);

	const std::string& GetFilePath();
	void SetFilePath(std::string value);
	
	const std::string& GetScope();
	void SetScope(std::string value);

	const std::string* GetFieldOfApplication();
	void SetFieldOfApplication(std::string& value);

	const std::string& GetVersionNumber();
	void SetVersionNumber(std::string value);
	Version getVersion() const;

	const std::string& GetVersionDate();
	void SetVersionDate(std::string value);

	std::string getProductId() const;
	const std::string& GetProductId();
	void SetProductId(std::string value);

	SimpleAttribute* GetSimpleAttribute(std::string code);
	SimpleAttribute* GetSimpleAttribute(std::string Code);
	SimpleAttribute* GetSimpleAttributeFromName(std::string name);
	SimpleAttributes* GetSimpleAttributes();
	
	ComplexAttribute* GetComplexAttribute(std::string code);
	ComplexAttribute* GetComplexAttribute(std::string Code);
	ComplexAttribute* GetComplexAttributeFromName(std::string name);
	ComplexAttributes* GetComplexAttributes();

	Role* GetRole(std::string Code);
	Role* GetRole(std::string Code);
	Role* GetRoleFromName(std::string name);
	Roles* GetRoles();

	InformationAssociation* GetInformationAssociation(std::string Code);
	InformationAssociation* GetInformationAssociation(std::string Code);
	InformationAssociation* GetInformationAssociationFromName(std::string name);
	InformationAssociations* GetInformationAssociations();

	FeatureAssociation* GetFeatureAssociation(std::string Code);
	FeatureAssociation* GetFeatureAssociation(std::string Code);
	FeatureAssociation* GetFeatureAssociationFromName(std::string name);
	FeatureAssociations* GetFeatureAssociations();

	InformationType* GetInformationType(std::string Code);
	InformationType* GetInformationType(std::string Code);
	InformationType* GetInformationTypeFromName(std::string name);
	InformationTypes* GetInformationTypes();

	FeatureType* GetFeatureType(std::string Code);
	FeatureType* GetFeatureType(std::string Code);
	FeatureType* GetFeatureTypeName(std::string Name);
	FeatureType* GetFeatureTypeFromIndex(int indexnum);
	FeatureTypes* GetFeatureTypes();

	FCD::S100_CD_AttributeValueType getSimpleAttributeType(std::string code);

	//void SetFullAssociations();
	//void SetSubAssociation(FeatureType* ft);
	//void SetSubAssociation(InformationType* it);
	//void SetFeatureAssociationFromSuperType(FeatureType* ft, std::string superTypeName, std::string roleName, std::string associationName);
	//void SetInformationAssociationFromSuperType(FeatureType* ft, std::string superTypeName, std::string roleName, std::string associationName);
	//void SetInformationAssociationFromSuperType(InformationType* it, std::string superTypeName, std::string roleName, std::string associationName);

	//void AddFeatureBinding(FeatureType* ft, std::string bindedTypeName, std::string roleName, std::string associationName);
	//void AddInformationBinding(FeatureType* ft, std::string bindedTypeName, std::string roleName, std::string associationName);
	//void AddInformationBinding(InformationType* it, std::string bindedTypeName, std::string roleName, std::string associationName);

	void NullCheckFieldOfApplication();

	// Don't release result.
	void GetPointFeatures(std::vector<FeatureType*>& result);
	void GetMultiPointFeatures(std::vector<FeatureType*>& result);
	void GetLineFeatures(std::vector<FeatureType*>& result);
	void GetAreaFeatures(std::vector<FeatureType*>& result);
	void GetCoverageFeatures(std::vector<FeatureType*>& result);
	void GetNoGeometryFeatures(std::vector<FeatureType*>& result);

	Attribute* GetAttribute(std::string code);
	Attribute* GetAttribute(std::string code);
};