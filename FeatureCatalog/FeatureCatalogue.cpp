// FeatureCatalog.cpp : Define the initialization routine of the corresponding DLL.
#include "stdafx.h"
#include "FeatureCatalogue.h"
#include "Role.h"

#include <atlstr.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

FeatureCatalogue::FeatureCatalogue()
{

}
FeatureCatalogue::FeatureCatalogue(std::wstring path)
{
	Read(path);
}
FeatureCatalogue::~FeatureCatalogue()
{
	delete fieldOfApplication;
	fieldOfApplication = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Search infomation */

SimpleAttribute* FeatureCatalogue::GetSimpleAttribute(std::wstring code)
{
	auto itor = simpleAttributes.GetSimpleAttributePointer().find(code);

	if (itor == simpleAttributes.GetSimpleAttributePointer().end())
	{
		return nullptr; // not found
	}
	else
	{
		return itor->second;
	}
}

SimpleAttribute* FeatureCatalogue::GetSimpleAttributeFromName(std::wstring name)
{
	for (auto i = simpleAttributes.GetSimpleAttributePointer().begin();
		i != simpleAttributes.GetSimpleAttributePointer().end();
		i++)
	{
		if (i->second->GetName().compare(name) == 0)
		{
			return i->second;
		}
	}
	return nullptr;
}

ComplexAttribute* FeatureCatalogue::GetComplexAttribute(std::wstring code)
{
	auto itor = complexAttributes.GetComplexAttributePointer().find(code);

	if (itor == complexAttributes.GetComplexAttributePointer().end())
		return NULL; // not found
	else
		return &itor->second;
}

ComplexAttribute* FeatureCatalogue::GetComplexAttributeFromName(std::wstring name)
{
	for (auto i = complexAttributes.GetComplexAttributePointer().begin();
		i != complexAttributes.GetComplexAttributePointer().end();
		i++)
	{
		if (i->second.GetName().compare(name) == 0)
			return &i->second;
	}
	return nullptr;
}

FeatureType* FeatureCatalogue::GetFeatureType(std::wstring code)
{
	auto itor = featureTypes.GetFeatureTypePointer().find(code);

	if (itor == featureTypes.GetFeatureTypePointer().end())
		return NULL; // not found
	else
		return &itor->second;
}

FeatureTypes& FeatureCatalogue::GetFeatureTypesPointer()
{
	return featureTypes;
}

FeatureType* FeatureCatalogue::GetFeatureTypeName(std::wstring name)
{
	for (auto i = featureTypes.GetFeatureTypePointer().begin();
		i != featureTypes.GetFeatureTypePointer().end();
		i++)
	{
		if (i->second.GetName().compare(name) == 0)
			return &i->second;
	}
	return nullptr;
}


FeatureType* FeatureCatalogue::GetFeatureTypeFromIndex(int indexnum)
{
	int num = 0;
	for (auto i= featureTypes.GetFeatureTypePointer().begin(); i!=featureTypes.GetFeatureTypePointer().end(); i++)
	{
		if (num==indexnum)
		{
			auto dfsf = &i->second;
			OutputDebugString(L"");
			return &i->second;
		}
		num++;
	}
	return nullptr;
}

InformationType* FeatureCatalogue::GetInformationType(std::wstring code)
{
	auto itor = informationTypes.GetInformationTypePointer().find(code);

	if (itor == informationTypes.GetInformationTypePointer().end())
		return NULL; // not found
	else
		return &itor->second;
}


InformationType* FeatureCatalogue::GetInformationTypeFromName(std::wstring name)
{
	for (auto itor = informationTypes.GetInformationTypePointer().begin();
		itor != informationTypes.GetInformationTypePointer().end();
		itor++)
	{
		if (itor->second.GetName().compare(name) == 0)
			return &itor->second;
	}
	return NULL;
}

FeatureAssociation* FeatureCatalogue::GetFeatureAssociation(std::wstring code)
{
	auto itor = featureAssociations.GetFeatureAssociationPointer().find(code);

	if (itor == featureAssociations.GetFeatureAssociationPointer().end())
		return NULL; // not found
	else
		return &itor->second;
}

FeatureAssociation* FeatureCatalogue::GetFeatureAssociationFromName(std::wstring name)
{
	for (auto itor = featureAssociations.GetFeatureAssociationPointer().begin();
		itor != featureAssociations.GetFeatureAssociationPointer().end();
		itor++)
	{
		if (itor->second.GetName().compare(name) == 0)
			return &itor->second;
	}
	return NULL;
}

InformationAssociation* FeatureCatalogue::GetInformationAssociation(std::wstring code)
{
	auto itor = informationAssociations.GetInformationAssociationPointer().find(code);

	if (itor == informationAssociations.GetInformationAssociationPointer().end())
		return NULL; // not found
	else
		return &itor->second;
}

InformationAssociation* FeatureCatalogue::GetInformationAssociationFromName(std::wstring name)
{
	for (auto itor = informationAssociations.GetInformationAssociationPointer().begin();
		itor != informationAssociations.GetInformationAssociationPointer().end();
		itor++)
	{
		if (itor->second.GetName().compare(name) == 0)
			return &itor->second;
	}
	return NULL;
}

Role* FeatureCatalogue::GetRole(std::wstring code)
{
	auto itor = roles.GetRolePointer().find(code);

	if (itor == roles.GetRolePointer().end())
		return NULL; // not found
	else
		return itor->second;
}

Role* FeatureCatalogue::GetRoleFromName(std::wstring name)
{
	for (auto itor = roles.GetRolePointer().begin();
		itor != roles.GetRolePointer().end();
		itor++)
	{
		if (itor->second->GetName().compare(name) == 0)
			return itor->second;
	}
	return NULL;
}

void FeatureCatalogue::GetContents(pugi::xml_node& node)
{
	//Modification plan : Check if there are any elements and add the contents.
	//Modification plan : Put a value in the map, search for it, and if not, print out an error message.
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "S100FC:name"))
		{
			if (instruction.child_value() != nullptr) 
			{
				name = pugi::as_wide(instruction.child_value());
			}
		}
		else if (!strcmp(instructionName, "S100FC:scope"))
		{
			if (instruction.child_value() != nullptr)
			{
				scope = pugi::as_wide(instruction.child_value());
			}
		}
		else if (!strcmp(instructionName, "S100FC:fieldOfApplication"))
		{
			if (instruction.child_value() != nullptr)
			{
				std::string value = instruction.child_value();
				SetFieldOfApplication(value);
			}
		} 
		else if (!strcmp(instructionName, "S100FC:versionNumber"))
		{
			if (instruction.child_value() != nullptr)
			{
				versionNumber = pugi::as_wide(instruction.child_value());
			}
		}
		else if (!strcmp(instructionName, "S100FC:versionDate"))
		{
			if (instruction.child_value() != nullptr)
			{
				versionDate = pugi::as_wide(instruction.child_value());
			}
		}
		else if (!strcmp(instructionName, "S100FC:producer"))
		{
			producer.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_definitionSources"))
		{
			definitionSources.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_SimpleAttributes"))
		{
			simpleAttributes.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_ComplexAttributes"))
		{
			complexAttributes.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_Roles"))
		{
			roles.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_InformationAssociations"))
		{
			informationAssociations.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_FeatureAssociations"))
		{
			featureAssociations.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_InformationTypes"))
		{
			informationTypes.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_FeatureTypes"))
		{
			featureTypes.GetContents(instruction);
		}
		else  //Unspecified value
		{
			std::wstring anotherName = pugi::as_wide(instructionName);
			anotherName.append(L"is another FCDateName");
		}
	}

	SetFullAssociations();
}
void FeatureCatalogue::SetFullAssociations()
{
	for (auto itor = featureTypes.GetFeatureTypePointer().begin(); itor != featureTypes.GetFeatureTypePointer().end(); itor++)
	{
		FeatureType *ft = &itor->second;
		SetSubAssociation(ft);
	}

	for (auto itor = informationTypes.GetInformationTypePointer().begin(); itor != informationTypes.GetInformationTypePointer().end(); itor++)
	{
		InformationType *it = &itor->second;
		SetSubAssociation(it);
	}
}

void FeatureCatalogue::SetSubAssociation(FeatureType* ft)
{
	for (auto fbi = ft->GetFeatureBindingPointer().begin();
		fbi != ft->GetFeatureBindingPointer().end();
		fbi++)
	{
		auto fb = &fbi->second;
		auto bindedFeatureName = fb->GetFeatureTypePointer().GetReference();
		SetFeatureAssociationFromSuperType(ft, bindedFeatureName, fb->GetRolePointer().GetReference(), fb->GetAssociationPointer().GetReference());
	}

	for (auto ibi = ft->GetInformationBindingPointer().begin();
		ibi != ft->GetInformationBindingPointer().end();
		ibi++)
	{
		auto ib = &ibi->second;
		auto bindedinformationName = ib->GetInformationTypePointer().GetReference();
		SetInformationAssociationFromSuperType(ft, bindedinformationName, ib->GetRolePointer().GetReference(), ib->GetAssociationPointer().GetReference());
	}
	return;
}

void FeatureCatalogue::SetSubAssociation(InformationType* it)
{
	for (auto ibi = it->GetInformationBindingPointer().begin();
		ibi != it->GetInformationBindingPointer().end();
		ibi++)
	{
		auto ib = &ibi->second;
		auto bindedinformationName = ib->GetInformationTypePointer().GetReference();
		SetInformationAssociationFromSuperType(it, bindedinformationName, ib->GetRolePointer().GetReference(), ib->GetAssociationPointer().GetReference());
	}
	return;
}

void FeatureCatalogue::SetFeatureAssociationFromSuperType(FeatureType* ft, std::wstring superTypeName, std::wstring roleName, std::wstring associationName)
{
	for (auto fti = featureTypes.GetFeatureTypePointer().begin(); fti != featureTypes.GetFeatureTypePointer().end(); fti++)
	{
		auto currentFeautreType = &fti->second;
		auto currentFeautreCode = currentFeautreType->GetCodeAsWString();

		if (superTypeName.compare(currentFeautreCode) == 0)
			continue;

		auto duplicationCheckIter = ft->GetFeatureBindingPointer().find(currentFeautreCode);
		if (duplicationCheckIter != ft->GetFeatureBindingPointer().end())
			continue;;

		if (superTypeName.compare(currentFeautreType->GetSuperType()) == 0)
		{
			AddFeatureBinding(ft, currentFeautreType->GetCodeAsWString(), roleName, associationName);

			if (currentFeautreType->GetSuperType().size() > 0)
			{
				SetFeatureAssociationFromSuperType(ft, currentFeautreType->GetSuperType(), roleName, associationName);
			}
		}
	}
	return;
}

void FeatureCatalogue::SetInformationAssociationFromSuperType(FeatureType* ft, std::wstring superTypeName, std::wstring roleName, std::wstring associationName)
{
	for (auto iti = informationTypes.GetInformationTypePointer().begin(); iti != informationTypes.GetInformationTypePointer().end(); iti++)
	{
		auto currentInformationType = &iti->second;
		auto currentInformationCode = currentInformationType->GetCodeAsWString();
		
		if (superTypeName.compare(currentInformationCode) == 0)
			continue;

		auto duplicationCheckIter = ft->GetInformationBindingPointer().find(currentInformationCode);
		if (duplicationCheckIter != ft->GetInformationBindingPointer().end())
			continue;;

		if (superTypeName.compare(currentInformationType->GetSuperType()) == 0)
		{
			AddInformationBinding(ft, currentInformationType->GetCodeAsWString(), roleName, associationName);
			if (currentInformationType->GetSuperType().size() > 0)
			{
				SetInformationAssociationFromSuperType(ft, currentInformationType->GetSuperType(), roleName, associationName);
			}
		}
	}
	return;
}
void FeatureCatalogue::SetInformationAssociationFromSuperType(InformationType* it, std::wstring superTypeName, std::wstring roleName, std::wstring associationName)
{
	for (auto iti = informationTypes.GetInformationTypePointer().begin(); iti != informationTypes.GetInformationTypePointer().end(); iti++)
	{
		auto currentInformationType = &iti->second;
		auto currentInformationCode = currentInformationType->GetCodeAsWString();

		if (superTypeName.compare(currentInformationCode) == 0)
			continue;

		auto duplicationCheckIter = it->GetInformationBindingPointer().find(currentInformationCode);
		if (duplicationCheckIter != it->GetInformationBindingPointer().end())
			continue;;

		if (superTypeName.compare(currentInformationType->GetSuperType()) == 0)
		{
			AddInformationBinding(it, currentInformationType->GetCodeAsWString(), roleName, associationName);

			if (currentInformationType->GetSuperType().size() > 0)
			{
				SetInformationAssociationFromSuperType(it, currentInformationType->GetSuperType(), roleName, associationName);
			}
		}
	}
	return;
}

void FeatureCatalogue::AddFeatureBinding(FeatureType* ft, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName)
{
	FeatureBinding fb;
	fb.GetFeatureTypePointer().SetReference(bindedTypeName);
	fb.GetRolePointer().SetReference(roleName);
	fb.GetAssociationPointer().SetReference(associationName);
	
	ft->GetFeatureBindingPointer().insert(
		std::unordered_map<std::wstring, FeatureBinding>::value_type(
			bindedTypeName,
			fb));
}

void FeatureCatalogue::AddInformationBinding(FeatureType* ft, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName)
{
	InformationBinding ib;
	ib.GetInformationTypePointer().SetReference(bindedTypeName);
	ib.GetRolePointer().SetReference(roleName);
	ib.GetAssociationPointer().SetReference(associationName);
	
	ft->GetInformationBindingPointer().insert(
		std::unordered_map<std::wstring, InformationBinding>::value_type(
			bindedTypeName,
			ib));
}

void FeatureCatalogue::AddInformationBinding(InformationType* it, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName)
{
	InformationBinding ib;
	ib.GetInformationTypePointer().SetReference(bindedTypeName);
	ib.GetRolePointer().SetReference(roleName);
	ib.GetAssociationPointer().SetReference(associationName);

	it->GetInformationBindingPointer().insert(
		std::unordered_map<std::wstring, InformationBinding>::value_type(
			bindedTypeName,
			ib));
}

void FeatureCatalogue::NullCheckFieldOfApplication()
{
	if (fieldOfApplication == nullptr)
	{
		fieldOfApplication = new std::wstring();
	}
}

bool FeatureCatalogue::Read(std::wstring path)
{
	// FC (FileName) Loading start
	SetFilePath(path);

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(GetFilePath().c_str()); ///Read File
	pugi::xml_node displayList = doc.child("S100FC:S100_FC_FeatureCatalogue");

	if (displayList != nullptr)
	{
		GetContents(displayList);
		// FC (FileName) Loading complete
		return true;
	}
	else
	{
		return false;
	}
}

InformationTypes& FeatureCatalogue::GetInformationTypesPointer()
{
	return informationTypes;
}

Roles& FeatureCatalogue::GetRolesPointer()
{
	return roles;
}

SimpleAttributes& FeatureCatalogue::GetSimpleAttributesPointer()
{
	return simpleAttributes;
}

ComplexAttributes& FeatureCatalogue::GetComplexAttributesPointer()
{
	return complexAttributes;
}

InformationAssociations& FeatureCatalogue::GetInformationAssociationsPointer()
{
	return informationAssociations;
}

FeatureAssociations& FeatureCatalogue::GetFeatureAssociationsPointer()
{
	return featureAssociations;
}


const std::wstring& FeatureCatalogue::GetName()
{
	return name;
}

void FeatureCatalogue::SetName(std::string value)
{
	name = pugi::as_wide(value);
}

void FeatureCatalogue::SetName(std::wstring value)
{
	name = value;
}

const std::wstring& FeatureCatalogue::GetFilePath()
{
	return filePath;
}

void FeatureCatalogue::SetFilePath(std::string value)
{
	filePath = pugi::as_wide(value);
}

void FeatureCatalogue::SetFilePath(std::wstring value)
{
	filePath = value;
}

const std::wstring& FeatureCatalogue::GetScope()
{
	return scope;
}

void FeatureCatalogue::SetScope(std::string value)
{
	scope = pugi::as_wide(value);
}

void FeatureCatalogue::SetScope(std::wstring value)
{
	scope = value;
}

const std::wstring& FeatureCatalogue::GetFieldOfApplication()
{
	return *fieldOfApplication;
}

void FeatureCatalogue::SetFieldOfApplication(std::string& value)
{
	NullCheckFieldOfApplication();
	*fieldOfApplication = pugi::as_wide(value);
}

void FeatureCatalogue::SetFieldOfApplication(std::wstring& value)
{
	NullCheckFieldOfApplication();
	*fieldOfApplication = value;
}

const std::wstring& FeatureCatalogue::GetVersionNumber()
{
	return versionNumber;
}

void FeatureCatalogue::SetVersionNumber(std::string value)
{
	versionNumber = pugi::as_wide(value);
}

void FeatureCatalogue::SetVersionNumber(std::wstring value)
{
	versionNumber = value;
}

const std::wstring& FeatureCatalogue::GetVersionDate()
{
	return versionDate;
}

void FeatureCatalogue::SetVersionDate(std::string value)
{
	versionDate = pugi::as_wide(value);
}

void FeatureCatalogue::SetVersionDate(std::wstring value)
{
	versionDate = value;
}