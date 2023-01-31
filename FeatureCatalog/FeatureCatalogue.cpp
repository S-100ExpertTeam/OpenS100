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
		return itor->second;
}

ComplexAttribute* FeatureCatalogue::GetComplexAttributeFromName(std::wstring name)
{
	for (auto i = complexAttributes.GetComplexAttributePointer().begin();
		i != complexAttributes.GetComplexAttributePointer().end();
		i++)
	{
		if (i->second->GetName().compare(name) == 0)
			return i->second;
	}
	return nullptr;
}

FeatureType* FeatureCatalogue::GetFeatureType(std::wstring code)
{
	auto itor = featureTypes.GetFeatureType().find(code);

	if (itor == featureTypes.GetFeatureType().end())
		return NULL; // not found
	else
		return itor->second;
}

FeatureTypes* FeatureCatalogue::GetFeatureTypes()
{
	return &featureTypes;
}

FeatureType* FeatureCatalogue::GetFeatureTypeName(std::wstring name)
{
	for (
		auto i = featureTypes.GetFeatureType().begin();
		i != featureTypes.GetFeatureType().end();
		i++)
	{
		if (i->second->GetName().compare(name) == 0)
		{
			return i->second;
		}
	}
	return nullptr;
}


FeatureType* FeatureCatalogue::GetFeatureTypeFromIndex(int indexnum)
{
	int num = 0;
	for (auto i= featureTypes.GetFeatureType().begin(); i!=featureTypes.GetFeatureType().end(); i++)
	{
		if (num==indexnum)
		{
			auto dfsf = &i->second;
			return i->second;
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
		return itor->second;
}


InformationType* FeatureCatalogue::GetInformationTypeFromName(std::wstring name)
{
	for (auto itor = informationTypes.GetInformationTypePointer().begin();
		itor != informationTypes.GetInformationTypePointer().end();
		itor++)
	{
		if (itor->second->GetName().compare(name) == 0)
			return itor->second;
	}
	return NULL;
}

FeatureAssociation* FeatureCatalogue::GetFeatureAssociation(std::wstring code)
{
	auto itor = featureAssociations.GetFeatureAssociationPointer().find(code);

	if (itor == featureAssociations.GetFeatureAssociationPointer().end())
		return NULL; // not found
	else
		return itor->second;
}

FeatureAssociation* FeatureCatalogue::GetFeatureAssociationFromName(std::wstring name)
{
	for (auto itor = featureAssociations.GetFeatureAssociationPointer().begin();
		itor != featureAssociations.GetFeatureAssociationPointer().end();
		itor++)
	{
		if (itor->second->GetName().compare(name) == 0)
			return itor->second;
	}
	return NULL;
}

InformationAssociation* FeatureCatalogue::GetInformationAssociation(std::wstring code)
{
	auto itor = informationAssociations.GetInformationAssociationPointer().find(code);

	if (itor == informationAssociations.GetInformationAssociationPointer().end())
		return NULL; // not found
	else
		return itor->second;
}

InformationAssociation* FeatureCatalogue::GetInformationAssociationFromName(std::wstring name)
{
	for (auto itor = informationAssociations.GetInformationAssociationPointer().begin();
		itor != informationAssociations.GetInformationAssociationPointer().end();
		itor++)
	{
		if (itor->second->GetName().compare(name) == 0)
			return itor->second;
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
		else if (!strcmp(instructionName, "S100FC:productId"))
		{
			if (instruction.child_value() != nullptr)
			{
				SetProductId(instruction.child_value());
			}
		}
		//else if (!strcmp(instructionName, "S100FC:producer"))
		//{
		//	producer.GetContents(instruction);
		//}
		//else if (!strcmp(instructionName, "S100FC:S100_FC_definitionSources"))
		//{
		//	definitionSources.GetContents(instruction);
		//}
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

	//SetFullAssociations();
}

void FeatureCatalogue::WriteContents(pugi::xml_node& node)
{
	auto features = featureTypes.GetVecFeatureType();

	for (auto i = features.begin(); i != features.end(); i++)
	{
		auto feature = (*i);
		std::string xpath = "S100FC:S100_FC_FeatureCatalogue/S100FC:S100_FC_FeatureTypes/S100FC:S100_FC_FeatureType/S100FC:code[text()='" + feature->GetCode() + "']";
		auto featureNode = node.select_node(xpath.c_str()).node().parent();

		auto ibNodes = featureNode.select_nodes("S100FC:informationBinding");
		for (auto j = ibNodes.begin(); j != ibNodes.end(); j++)
		{
			auto ibNode = (*j).node();
			featureNode.remove_child(ibNode);
		}

		auto userTypeNode = featureNode.select_node("S100FC:featureUseType").node();

		auto informationBindings = feature->GetInformationBindingPointer();
		for (auto j = informationBindings.begin(); j != informationBindings.end(); j++)
		{
			auto ib = (*j);

			auto node_ib = featureNode.insert_child_before("S100FC:informationBinding", userTypeNode);
			node_ib.append_attribute("roleType").set_value(ib->GetRoleTypeAsString().c_str());
			auto nodeMultiplicity = node_ib.append_child("S100FC:multiplicity");
			nodeMultiplicity.append_child("S100Base:lower").text().set(ib->GetMultiplicity().GetLower());
			auto upperNode = nodeMultiplicity.append_child("S100Base:upper");
			auto attrNil = upperNode.append_attribute("xsi:nil");
			auto attrInfinite = upperNode.append_attribute("infinite");

			if (ib->GetMultiplicity().GetUpper().IsInfinite())
			{
				attrNil.set_value("true");
				attrInfinite.set_value("true");
			}
			else
			{
				attrNil.set_value("false");
				attrInfinite.set_value("false");
				upperNode.text().set(ib->GetMultiplicity().GetUpperCount());
			}

			node_ib.append_child("S100FC:association").append_attribute("ref").set_value(ib->GetAssociation().c_str());
			node_ib.append_child("S100FC:role").append_attribute("ref").set_value(ib->GetRole().c_str());

			auto cnt = ib->GetInformationTypeCount();

			if (cnt > 1)
			{
				int a = 0;
			}

			for (int k = 0; k < cnt; k++)
			{
				auto informationType = ib->GetInformationType(k);
				node_ib.append_child("S100FC:informationType").append_attribute("ref").set_value(informationType.c_str());
			}
		}
	}

	for (auto i = features.begin(); i != features.end(); i++)
	{
		auto feature = (*i);
		std::string xpath = "S100FC:S100_FC_FeatureCatalogue/S100FC:S100_FC_FeatureTypes/S100FC:S100_FC_FeatureType/S100FC:code[text()='" + feature->GetCode() + "']";
		auto featureNode = node.select_node(xpath.c_str()).node().parent();
		
		auto fbNodes = featureNode.select_nodes("S100FC:featureBinding");
		for (auto j = fbNodes.begin(); j != fbNodes.end(); j++)
		{
			auto fbNode = (*j).node();
			featureNode.remove_child(fbNode);
		}

		auto primNode = featureNode.select_node("S100FC:permittedPrimitives").node();

		auto featureBindings = feature->GetFeatureBindingPointer();
		for (auto j = featureBindings.begin(); j != featureBindings.end(); j++)
		{
			auto fb = (*j);

			auto node_fb = featureNode.insert_child_before("S100FC:featureBinding", primNode);
			node_fb.append_attribute("roleType").set_value(fb->GetRoleTypeAsString().c_str());
			auto nodeMultiplicity = node_fb.append_child("S100FC:multiplicity");
			nodeMultiplicity.append_child("S100Base:lower").text().set(fb->GetMultiplicity().GetLower());
			auto upperNode = nodeMultiplicity.append_child("S100Base:upper");
			auto attrNil = upperNode.append_attribute("xsi:nil");
			auto attrInfinite = upperNode.append_attribute("infinite");
			
			if (fb->GetMultiplicity().GetUpper().IsInfinite())
			{
				attrNil.set_value("true");
				attrInfinite.set_value("true");
			}
			else
			{
				attrNil.set_value("false");
				attrInfinite.set_value("false");
				upperNode.text().set(fb->GetMultiplicity().GetUpperCount());
			}

			node_fb.append_child("S100FC:association").append_attribute("ref").set_value(fb->GetAssociation().c_str());
			node_fb.append_child("S100FC:role").append_attribute("ref").set_value(fb->GetRole().c_str());
			
			auto cnt = fb->GetFeatureTypeCount();

			for (int k = 0; k < cnt; k++)
			{
				auto featureType = fb->GetFeatureType(k);
				node_fb.append_child("S100FC:featureType").append_attribute("ref").set_value(featureType.c_str());
			}
		}
	}

	

	//auto informations = informationTypes.GetVecInformationType();
	//for (auto i = informations.begin(); i != informations.end(); i++)
	//{
	//	auto information = (*i);
	//	std::string xpath = "S100FC:S100_FC_FeatureCatalogue/S100FC:S100_FC_InformationTypess/S100FC:S100_FC_InformationTypes/S100FC:code[text()='" + information->GetCode() + "']";
	//	auto featureNode = node.select_node(xpath.c_str()).node();
	//	featureNode.remove_child("S100FC:informationBinding");

	//	auto featureBindings = information->GetFeatureBindingPointer();
	//	for (auto j = featureBindings.begin(); j != featureBindings.end(); j++)
	//	{
	//		auto fb = (*j);

	//		auto node_fb = featureNode.append_child("S100FC:featureBinding");
	//		node_fb.append_attribute("roleType").set_value(fb->GetRoleTypeAsString().c_str());
	//		auto nodeMultiplicity = node_fb.append_child("S100FC:multiplicity");
	//		nodeMultiplicity.append_child("S100Base:lower").text().set(fb->GetMultiplicity().GetLower());
	//		auto upperNode = nodeMultiplicity.append_child("S100Base:upper");
	//		auto attrNil = upperNode.append_attribute("xsi:nil");
	//		auto attrInfinite = upperNode.append_attribute("infinite");

	//		if (fb->GetMultiplicity().GetUpper().IsInfinite())
	//		{
	//			attrNil.set_value("true");
	//			attrInfinite.set_value("true");
	//		}
	//		else
	//		{
	//			attrNil.set_value("false");
	//			attrInfinite.set_value("false");
	//			upperNode.text().set(fb->GetMultiplicity().GetUpperCount());
	//		}

	//		node_fb.append_child("S100FC:association").append_attribute("ref").set_value(fb->GetAssociation().c_str());
	//		node_fb.append_child("S100FC:role").append_attribute("ref").set_value(fb->GetRole().c_str());

	//		auto cnt = fb->GetFeatureTypeCount();

	//		for (int k = 0; k < cnt; k++)
	//		{
	//			auto featureType = fb->GetFeatureType(k);
	//			node_fb.append_child("S100FC:featureType").append_attribute("ref").set_value(featureType.c_str());
	//		}
	//	}
	//}
}

//void FeatureCatalogue::SetFullAssociations()
//{
//	for (auto itor = featureTypes.GetFeatureType().begin(); itor != featureTypes.GetFeatureType().end(); itor++)
//	{
//		FeatureType *ft = itor->second;
//		SetSubAssociation(ft);
//	}
//
//	for (auto itor = informationTypes.GetInformationTypePointer().begin(); itor != informationTypes.GetInformationTypePointer().end(); itor++)
//	{
//		InformationType *it = itor->second;
//		SetSubAssociation(it);
//	}
//}

//void FeatureCatalogue::SetSubAssociation(FeatureType* ft)
//{
//	for (auto fbi = ft->GetFeatureBindingPointer().begin();
//		fbi != ft->GetFeatureBindingPointer().end();
//		fbi++)
//	{
//		auto fb = *fbi;
//		auto bindedFeatureName = fb->GetFeatureTypeAsWstring();
//		SetFeatureAssociationFromSuperType(ft, bindedFeatureName, fb->GetRoleAsWstring(), fb->GetAssociationAsWstring());
//	}
//
//	for (auto ibi = ft->GetInformationBindingPointer().begin();
//		ibi != ft->GetInformationBindingPointer().end();
//		ibi++)
//	{
//		auto ib = ibi->second;
//		auto bindedinformationName = ib->GetInformationTypePointer().GetReference();
//		SetInformationAssociationFromSuperType(ft, bindedinformationName, ib->GetRolePointer().GetReference(), ib->GetAssociationPointer().GetReference());
//	}
//	return;
//}
//
//void FeatureCatalogue::SetSubAssociation(InformationType* it)
//{
//	for (auto ibi = it->GetInformationBindingPointer().begin();
//		ibi != it->GetInformationBindingPointer().end();
//		ibi++)
//	{
//		auto ib = ibi->second;
//		auto bindedinformationName = ib->GetInformationTypePointer().GetReference();
//		SetInformationAssociationFromSuperType(it, bindedinformationName, ib->GetRolePointer().GetReference(), ib->GetAssociationPointer().GetReference());
//	}
//	return;
//}

//void FeatureCatalogue::SetFeatureAssociationFromSuperType(FeatureType* ft, std::wstring superTypeName, std::wstring roleName, std::wstring associationName)
//{
//	for (
//		auto fti = featureTypes.GetFeatureType().begin(); 
//		fti != featureTypes.GetFeatureType().end(); 
//		fti++)
//	{
//		auto currentFeautreType = fti->second;
//		auto currentFeautreCode = currentFeautreType->GetCodeAsWString();
//
//		if (superTypeName.compare(currentFeautreCode) == 0)
//			continue;
//
//		auto duplicationCheckIter = ft->GetFeatureBindingPointer().find(currentFeautreCode);
//		if (duplicationCheckIter != ft->GetFeatureBindingPointer().end())
//			continue;
//
//		if (superTypeName.compare(currentFeautreType->GetSuperType()) == 0)
//		{
//			AddFeatureBinding(ft, currentFeautreType->GetCodeAsWString(), roleName, associationName);
//
//			if (currentFeautreType->GetSuperType().size() > 0)
//			{
//				SetFeatureAssociationFromSuperType(ft, currentFeautreType->GetSuperType(), roleName, associationName);
//			}
//		}
//	}
//	return;
//}
//
//void FeatureCatalogue::SetInformationAssociationFromSuperType(FeatureType* ft, std::wstring superTypeName, std::wstring roleName, std::wstring associationName)
//{
//	for (auto iti = informationTypes.GetInformationTypePointer().begin(); iti != informationTypes.GetInformationTypePointer().end(); iti++)
//	{
//		auto currentInformationType = iti->second;
//		auto currentInformationCode = currentInformationType->GetCodeAsWString();
//		
//		if (superTypeName.compare(currentInformationCode) == 0)
//			continue;
//
//		auto duplicationCheckIter = ft->GetInformationBindingPointer().find(currentInformationCode);
//		if (duplicationCheckIter != ft->GetInformationBindingPointer().end())
//			continue;;
//
//		if (superTypeName.compare(currentInformationType->GetSuperType()) == 0)
//		{
//			AddInformationBinding(ft, currentInformationType->GetCodeAsWString(), roleName, associationName);
//			if (currentInformationType->GetSuperType().size() > 0)
//			{
//				SetInformationAssociationFromSuperType(ft, currentInformationType->GetSuperType(), roleName, associationName);
//			}
//		}
//	}
//	return;
//}
//void FeatureCatalogue::SetInformationAssociationFromSuperType(InformationType* it, std::wstring superTypeName, std::wstring roleName, std::wstring associationName)
//{
//	for (auto iti = informationTypes.GetInformationTypePointer().begin(); iti != informationTypes.GetInformationTypePointer().end(); iti++)
//	{
//		auto currentInformationType = iti->second;
//		auto currentInformationCode = currentInformationType->GetCodeAsWString();
//
//		if (superTypeName.compare(currentInformationCode) == 0)
//			continue;
//
//		auto duplicationCheckIter = it->GetInformationBindingPointer().find(currentInformationCode);
//		if (duplicationCheckIter != it->GetInformationBindingPointer().end())
//			continue;;
//
//		if (superTypeName.compare(currentInformationType->GetSuperType()) == 0)
//		{
//			AddInformationBinding(it, currentInformationType->GetCodeAsWString(), roleName, associationName);
//
//			if (currentInformationType->GetSuperType().size() > 0)
//			{
//				SetInformationAssociationFromSuperType(it, currentInformationType->GetSuperType(), roleName, associationName);
//			}
//		}
//	}
//	return;
//}

//void FeatureCatalogue::AddFeatureBinding(FeatureType* ft, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName)
//{
//	auto fb = new FeatureBinding();
//	fb->SetFeatureType(bindedTypeName);
//	fb->GetRolePointer().SetReference(roleName);
//	fb->GetAssociationPointer().SetReference(associationName);
//
//	ft->GetFeatureBindingPointer().insert({ bindedTypeName, fb });
//}

//void FeatureCatalogue::AddInformationBinding(FeatureType* ft, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName)
//{
//	auto ib = new InformationBinding();
//	ib->GetInformationTypePointer().SetReference(bindedTypeName);
//	ib->GetRolePointer().SetReference(roleName);
//	ib->GetAssociationPointer().SetReference(associationName);
//	
//	ft->GetInformationBindingPointer().insert({ bindedTypeName, ib });
//}
//
//void FeatureCatalogue::AddInformationBinding(InformationType* it, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName)
//{
//	auto ib = new InformationBinding();
//	ib->GetInformationTypePointer().SetReference(bindedTypeName);
//	ib->GetRolePointer().SetReference(roleName);
//	ib->GetAssociationPointer().SetReference(associationName);
//
//	it->GetInformationBindingPointer().insert({ bindedTypeName, ib });
//}

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

bool FeatureCatalogue::Save(std::wstring filePath)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(GetFilePath().c_str()); ///Read File

	pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
	decl.append_attribute("version") = "1.0";
	decl.append_attribute("encoding") = "utf-8";

	// Write
	WriteContents(doc);
	doc.save_file(filePath.c_str(), "\t", pugi::format_default, pugi::encoding_utf8);
	return true;
}

InformationTypes* FeatureCatalogue::GetInformationTypes()
{
	return &informationTypes;
}

Roles* FeatureCatalogue::GetRoles()
{
	return &roles;
}

SimpleAttributes* FeatureCatalogue::GetSimpleAttributes()
{
	return &simpleAttributes;
}

ComplexAttributes* FeatureCatalogue::GetComplexAttributes()
{
	return &complexAttributes;
}

InformationAssociations* FeatureCatalogue::GetInformationAssociations()
{
	return &informationAssociations;
}

FeatureAssociations* FeatureCatalogue::GetFeatureAssociations()
{
	return &featureAssociations;
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

const std::wstring& FeatureCatalogue::GetProductId()
{
	return productId;
}

void FeatureCatalogue::SetProductId(std::string value)
{
	productId = pugi::as_wide(value);
}

void FeatureCatalogue::SetProductId(std::wstring value)
{
	productId = value;
}

void FeatureCatalogue::GetPointFeatures(std::vector<FeatureType*>& result)
{
	auto featureMap = featureTypes.GetVecFeatureType();

	for (auto i = featureMap.begin(); i != featureMap.end(); i++)
	{
		auto primList = (*i)->GetPermittedPrimitivesPointer();

		for (auto j = primList.begin(); j != primList.end(); j++)
		{
			if ((*j)->IsPoint())
			{
				result.push_back(*i);
			}
		}
	}
}

void FeatureCatalogue::GetLineFeatures(std::vector<FeatureType*>& result)
{
	auto featureMap = featureTypes.GetVecFeatureType();

	for (auto i = featureMap.begin(); i != featureMap.end(); i++)
	{
		auto primList = (*i)->GetPermittedPrimitivesPointer();

		for (auto j = primList.begin(); j != primList.end(); j++)
		{
			if ((*j)->IsLine())
			{
				result.push_back(*i);
			}
		}
	}
}

void FeatureCatalogue::GetAreaFeatures(std::vector<FeatureType*>& result)
{
	auto featureMap = featureTypes.GetVecFeatureType();

	for (auto i = featureMap.begin(); i != featureMap.end(); i++)
	{
		auto primList = (*i)->GetPermittedPrimitivesPointer();

		for (auto j = primList.begin(); j != primList.end(); j++)
		{
			if ((*j)->IsArea())
			{
				result.push_back(*i);
			}
		}
	}
}

Attribute* FeatureCatalogue::GetAttribute(std::wstring code)
{
	auto simpleAttribute = GetSimpleAttribute(code);
	if (simpleAttribute)
	{
		return simpleAttribute;
	}

	auto complexAttribute = GetComplexAttribute(code);
	if (complexAttribute)
	{
		return complexAttribute;
	}

	return nullptr;
}