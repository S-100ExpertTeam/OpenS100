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

//FeatureCatalogue::FeatureCatalogue(std::wstring path)
//{
//	Read(path);
//}

FeatureCatalogue::~FeatureCatalogue()
{
	delete fieldOfApplication;
	fieldOfApplication = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Search infomation */

SimpleAttribute* FeatureCatalogue::GetSimpleAttribute(std::string code)
{
	auto wcode = pugi::as_wide(code);
	return GetSimpleAttribute(wcode);
}

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

ComplexAttribute* FeatureCatalogue::GetComplexAttribute(std::string code)
{
	auto wcode = pugi::as_wide(code);
	return GetComplexAttribute(wcode);
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

FeatureType* FeatureCatalogue::GetFeatureType(std::string Code)
{
	return GetFeatureType(pugi::as_wide(Code));
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

FCD::S100_CD_AttributeValueType FeatureCatalogue::getSimpleAttributeType(std::string code)
{
	auto sa = GetSimpleAttribute(code);
	if (sa) {
		return sa->GetValueType();
	}
	return FCD::S100_CD_AttributeValueType::none;
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

InformationType* FeatureCatalogue::GetInformationType(std::string Code)
{
	return GetInformationType(pugi::as_wide(Code));
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

FeatureAssociation* FeatureCatalogue::GetFeatureAssociation(std::string Code)
{
	return GetFeatureAssociation(pugi::as_wide(Code));
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

InformationAssociation* FeatureCatalogue::GetInformationAssociation(std::string Code)
{
	return GetInformationAssociation(pugi::as_wide(Code));
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

Role* FeatureCatalogue::GetRole(std::string Code)
{
	return GetRole(pugi::as_wide(Code));
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
				SetVersionNumber(instruction.child_value());
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
	auto sa = simpleAttributes.GetSimpleAttributePointer();
	for (auto i = sa.begin(); i != sa.end(); i++)
	{
		auto simple = i->second;
		std::string xpath = "S100FC:S100_FC_FeatureCatalogue/S100FC:S100_FC_SimpleAttributes/S100FC:S100_FC_SimpleAttribute/S100FC:code[text()='" + simple->GetCode() + "']";
		auto simpleNode = node.select_node(xpath.c_str()).node().parent();
		auto node_dr = simpleNode.select_node("S100FC:definitionReference/S100FC:sourceIdentifier").node().text().set(simple->getSourceIdentifier().c_str());
		
	}

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

		auto node_dr = featureNode.select_node("S100FC:definitionReference/S100FC:sourceIdentifier").node().text().set(feature->getSourceIdentifier().c_str());

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
	pugi::xml_parse_result result = doc.load_file(GetFilePath().c_str());
	pugi::xml_node rootNode = doc.child("S100FC:S100_FC_FeatureCatalogue");

	if (rootNode != nullptr)
	{
		GetContents(rootNode);
		return true;
	}

	return false;
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

const std::wstring* FeatureCatalogue::GetFieldOfApplication()
{
	if (fieldOfApplication) 
	{
		return fieldOfApplication;
	}

	return nullptr;
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
	return pugi::as_wide(versionNumber.getSource());
}

void FeatureCatalogue::SetVersionNumber(std::string value)
{
	//versionNumber = pugi::as_wide(value);
	versionNumber.setSource(value);
}

void FeatureCatalogue::SetVersionNumber(std::wstring value)
{
	SetVersionNumber(pugi::as_utf8(value));
	//versionNumber = value;
}

Version FeatureCatalogue::getVersion() const
{
	return versionNumber;
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

std::string FeatureCatalogue::getProductId() const
{
	return pugi::as_utf8(productId);
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
			if (*j == SpatialPrimitiveType::point)
			{
				result.push_back(*i);
				break;
			}
		}
	}
}

void FeatureCatalogue::GetMultiPointFeatures(std::vector<FeatureType*>& result)
{
	auto featureMap = featureTypes.GetVecFeatureType();

	for (auto i = featureMap.begin(); i != featureMap.end(); i++)
	{
		auto primList = (*i)->GetPermittedPrimitivesPointer();

		for (auto j = primList.begin(); j != primList.end(); j++)
		{
			if (*j == SpatialPrimitiveType::pointSet)
			{
				result.push_back(*i);
				break;
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
			if (*j == SpatialPrimitiveType::curve)
			{
				result.push_back(*i);
				break;
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
			if (*j == SpatialPrimitiveType::surface)
			{
				result.push_back(*i);
				break;
			}
		}
	}
}

void FeatureCatalogue::GetCoverageFeatures(std::vector<FeatureType*>& result)
{
	auto featureMap = featureTypes.GetVecFeatureType();

	for (auto i = featureMap.begin(); i != featureMap.end(); i++)
	{
		auto primList = (*i)->GetPermittedPrimitivesPointer();

		for (auto j = primList.begin(); j != primList.end(); j++)
		{
			if (*j == SpatialPrimitiveType::coverage)
			{
				result.push_back(*i);
				break;
			}
		}
	}
}

void FeatureCatalogue::GetNoGeometryFeatures(std::vector<FeatureType*>& result)
{
	auto featureMap = featureTypes.GetVecFeatureType();

	for (auto i = featureMap.begin(); i != featureMap.end(); i++)
	{
		auto primList = (*i)->GetPermittedPrimitivesPointer();

		for (auto j = primList.begin(); j != primList.end(); j++)
		{
			if (*j == SpatialPrimitiveType::noGeometry)
			{
				result.push_back(*i);
				break;
			}
		}
	}
}

Attribute* FeatureCatalogue::GetAttribute(std::string code)
{
	return GetAttribute(pugi::as_wide(code));
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