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
#include "StringUtil.h"

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
	std::wstring GetNameW() { return toWide(GetName()); }
	void SetName(std::string value);
	void SetName(std::wstring value) { SetName(toUtf8(value)); }

	const std::string& GetFilePath();
	std::wstring GetFilePathW() { return toWide(GetFilePath()); }
	void SetFilePath(std::string value);
	void SetFilePath(std::wstring value) { SetFilePath(toUtf8(value)); }
	
	const std::string& GetScope();
	std::wstring GetScopeW() { return toWide(GetScope()); }
	void SetScope(std::string value);
	void SetScope(std::wstring value) { SetScope(toUtf8(value)); }

	const std::string* GetFieldOfApplication();
	std::wstring GetFieldOfApplicationW() { return toWide(GetFieldOfApplication()); }
	void SetFieldOfApplication(std::string& value);
	void SetFieldOfApplication(std::wstring value) { SetFieldOfApplication(toUtf8(value)); }

	const std::string& GetVersionNumber();
	std::wstring GetVersionNumberW() { return toWide(GetVersionNumber()); }
	void SetVersionNumber(std::string value);
	void SetVersionNumber(std::wstring value) { SetVersionNumber(toUtf8(value)); }
	Version getVersion() const;

	const std::string& GetVersionDate();
	std::wstring GetVersionDateW() { return toWide(GetVersionDate()); }
	void SetVersionDate(std::string value);
	void SetVersionDate(std::wstring value) { SetVersionDate(toUtf8(value)); }

	std::string getProductId() const;
	std::wstring getProductIdW() const { return toWide(getProductId()); }
	const std::string& GetProductId();
	std::wstring GetProductIdW() { return toWide(GetProductId()); }
	void SetProductId(std::string value);
	void SetProductId(std::wstring value) { SetProductId(toUtf8(value)); }

	SimpleAttribute* GetSimpleAttribute(std::string code);
	SimpleAttribute* GetSimpleAttribute(std::wstring code) { return GetSimpleAttribute(toUtf8(code)); }
	SimpleAttribute* GetSimpleAttribute(std::string Code);
	SimpleAttribute* GetSimpleAttribute(std::wstring Code) { return GetSimpleAttribute(toUtf8(Code)); }
	SimpleAttribute* GetSimpleAttributeFromName(std::string name);
	SimpleAttribute* GetSimpleAttributeFromName(std::wstring name) { return GetSimpleAttributeFromName(toUtf8(name)); }
	SimpleAttributes* GetSimpleAttributes();
	
	ComplexAttribute* GetComplexAttribute(std::string code);
	ComplexAttribute* GetComplexAttribute(std::wstring code) { return GetComplexAttribute(toUtf8(code)); }
	ComplexAttribute* GetComplexAttribute(std::string Code);
	ComplexAttribute* GetComplexAttribute(std::wstring Code) { return GetComplexAttribute(toUtf8(Code)); }
	ComplexAttribute* GetComplexAttributeFromName(std::string name);
	ComplexAttribute* GetComplexAttributeFromName(std::wstring name) { return GetComplexAttributeFromName(toUtf8(name)); }
	ComplexAttributes* GetComplexAttributes();

	Role* GetRole(std::string Code);
	Role* GetRole(std::wstring Code) { return GetRole(toUtf8(Code)); }
	Role* GetRoleFromName(std::string name);
	Role* GetRoleFromName(std::wstring name) { return GetRoleFromName(toUtf8(name)); }
	Roles* GetRoles();

	InformationAssociation* GetInformationAssociation(std::string Code);
	InformationAssociation* GetInformationAssociation(std::wstring Code) { return GetInformationAssociation(toUtf8(Code)); }
	InformationAssociation* GetInformationAssociationFromName(std::string name);
	InformationAssociation* GetInformationAssociationFromName(std::wstring name) { return GetInformationAssociationFromName(toUtf8(name)); }
	InformationAssociations* GetInformationAssociations();

	FeatureAssociation* GetFeatureAssociation(std::string Code);
	FeatureAssociation* GetFeatureAssociation(std::wstring Code) { return GetFeatureAssociation(toUtf8(Code)); }
	FeatureAssociation* GetFeatureAssociationFromName(std::string name);
	FeatureAssociation* GetFeatureAssociationFromName(std::wstring name) { return GetFeatureAssociationFromName(toUtf8(name)); }
	FeatureAssociations* GetFeatureAssociations();

	InformationType* GetInformationType(std::string Code);
	InformationType* GetInformationType(std::wstring Code) { return GetInformationType(toUtf8(Code)); }
	InformationType* GetInformationTypeFromName(std::string name);
	InformationType* GetInformationTypeFromName(std::wstring name) { return GetInformationTypeFromName(toUtf8(name)); }
	InformationTypes* GetInformationTypes();

	FeatureType* GetFeatureType(std::string Code);
	FeatureType* GetFeatureType(std::wstring Code) { return GetFeatureType(toUtf8(Code)); }
	FeatureType* GetFeatureTypeName(std::string Name);
	FeatureType* GetFeatureTypeName(std::wstring Name) { return GetFeatureTypeName(toUtf8(Name)); }
	FeatureType* GetFeatureTypeFromIndex(int indexnum);
	FeatureTypes* GetFeatureTypes();

	FCD::S100_CD_AttributeValueType getSimpleAttributeType(std::string code);
	FCD::S100_CD_AttributeValueType getSimpleAttributeType(std::wstring code) { return getSimpleAttributeType(toUtf8(code)); }

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
	Attribute* GetAttribute(std::wstring code) { return GetAttribute(toUtf8(code)); }
};
