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
	FeatureCatalogue(std::wstring path);
	virtual ~FeatureCatalogue();

protected:
	std::wstring name = L"";
	std::wstring scope = L"";

protected:
	std::wstring *fieldOfApplication = nullptr;  

protected:
	//std::wstring versionNumber = L"";
	Version versionNumber;
	std::wstring versionDate = L""; 
	std::wstring productId = L"";
	
	std::wstring filePath = L"";

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
	bool Read(std::wstring filePath);
	bool Save(std::wstring filePath);

	void GetContents(pugi::xml_node& node);
	void WriteContents(pugi::xml_node& node);

	const std::wstring& GetName();
	void SetName(std::string value);
	void SetName(std::wstring value);

	const std::wstring& GetFilePath();
	void SetFilePath(std::string value);
	void SetFilePath(std::wstring value);
	
	const std::wstring& GetScope();
	void SetScope(std::string value);
	void SetScope(std::wstring value);

	const std::wstring& GetFieldOfApplication();
	void SetFieldOfApplication(std::string& value);
	void SetFieldOfApplication(std::wstring& value);

	const std::wstring& GetVersionNumber();
	void SetVersionNumber(std::string value);
	void SetVersionNumber(std::wstring value);
	Version getVersion() const;

	const std::wstring& GetVersionDate();
	void SetVersionDate(std::string value);
	void SetVersionDate(std::wstring value);

	std::string getProductId() const;
	const std::wstring& GetProductId();
	void SetProductId(std::string value);
	void SetProductId(std::wstring value);

	SimpleAttribute* GetSimpleAttribute(std::string code);
	SimpleAttribute* GetSimpleAttribute(std::wstring Code);
	SimpleAttribute* GetSimpleAttributeFromName(std::wstring name);
	SimpleAttributes* GetSimpleAttributes();
	
	ComplexAttribute* GetComplexAttribute(std::string code);
	ComplexAttribute* GetComplexAttribute(std::wstring Code);
	ComplexAttribute* GetComplexAttributeFromName(std::wstring name);
	ComplexAttributes* GetComplexAttributes();

	Role* GetRole(std::string Code);
	Role* GetRole(std::wstring Code);
	Role* GetRoleFromName(std::wstring name);
	Roles* GetRoles();

	InformationAssociation* GetInformationAssociation(std::string Code);
	InformationAssociation* GetInformationAssociation(std::wstring Code);
	InformationAssociation* GetInformationAssociationFromName(std::wstring name);
	InformationAssociations* GetInformationAssociations();

	FeatureAssociation* GetFeatureAssociation(std::string Code);
	FeatureAssociation* GetFeatureAssociation(std::wstring Code);
	FeatureAssociation* GetFeatureAssociationFromName(std::wstring name);
	FeatureAssociations* GetFeatureAssociations();

	InformationType* GetInformationType(std::string Code);
	InformationType* GetInformationType(std::wstring Code);
	InformationType* GetInformationTypeFromName(std::wstring name);
	InformationTypes* GetInformationTypes();

	FeatureType* GetFeatureType(std::string Code);
	FeatureType* GetFeatureType(std::wstring Code);
	FeatureType* GetFeatureTypeName(std::wstring Name);
	FeatureType* GetFeatureTypeFromIndex(int indexnum);
	FeatureTypes* GetFeatureTypes();

	//void SetFullAssociations();
	//void SetSubAssociation(FeatureType* ft);
	//void SetSubAssociation(InformationType* it);
	//void SetFeatureAssociationFromSuperType(FeatureType* ft, std::wstring superTypeName, std::wstring roleName, std::wstring associationName);
	//void SetInformationAssociationFromSuperType(FeatureType* ft, std::wstring superTypeName, std::wstring roleName, std::wstring associationName);
	//void SetInformationAssociationFromSuperType(InformationType* it, std::wstring superTypeName, std::wstring roleName, std::wstring associationName);

	//void AddFeatureBinding(FeatureType* ft, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName);
	//void AddInformationBinding(FeatureType* ft, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName);
	//void AddInformationBinding(InformationType* it, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName);

	void NullCheckFieldOfApplication();

	// Don't release result.
	void GetPointFeatures(std::vector<FeatureType*>& result);
	void GetLineFeatures(std::vector<FeatureType*>& result);
	void GetAreaFeatures(std::vector<FeatureType*>& result);

	Attribute* GetAttribute(std::string code);
	Attribute* GetAttribute(std::wstring code);
};