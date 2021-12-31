#include "StdAfx.h"
#include "FeatureTypes.h"

FeatureTypes::FeatureTypes(void)
{

}

FeatureTypes::~FeatureTypes(void)
{

}

void FeatureTypes::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:S100_FC_FeatureType"))
		{
			FeatureType sa;
			sa.GetContents(instruction);
			featureType[sa.GetCodeAsWString()] = sa;

			if (instruction.attribute("isAbstract"))
			{
				XML_Attribute attr;
				attr.Setname("isAbstract");
				attr.Setvalue((char*)instruction.attribute("isAbstract").value());
				SetAttributes(attr);
			}
		}
	}
	ApplySuperType();
}

void FeatureTypes::ApplySuperType()
{
	for (auto itor = featureType.begin(); itor != featureType.end(); itor++)
	{
		FeatureType *ft = &itor->second;
		SetAttributeFromSuperType(ft);
		SetAssociationFromSuperType(ft);
	}
}

bool FeatureTypes::SetAttributeFromSuperType(FeatureType* ft)
{
	if (ft->GetSuperType().size() > 0)
	{
		auto itor = featureType.find(ft->GetSuperType());
		if (itor == featureType.end())
		{
			return false;
		}
		else
		{
			FeatureType* sft = &itor->second;
			if (SetAttributeFromSuperType(sft))
			{
				ft->GetAttributeBindingPointer().insert(ft->GetAttributeBindingPointer().begin(), sft->GetAttributeBindingPointer().begin(), sft->GetAttributeBindingPointer().end());
			}
			return true;
		}

	}
	else
		return true;

	return false;

}


bool FeatureTypes::SetAssociationFromSuperType(FeatureType* ft)
{
	if (ft->GetSuperType().size() > 0)
	{
		auto itor = featureType.find(ft->GetSuperType());
		if (itor == featureType.end())
		{
			return false;
		}
		else
		{
			FeatureType* sft = &itor->second;
			if (SetAssociationFromSuperType(sft))
			{
				ft->GetFeatureBindingPointer().insert(sft->GetFeatureBindingPointer().begin(), sft->GetFeatureBindingPointer().end());
				ft->GetInformationBindingPointer().insert(sft->GetInformationBindingPointer().begin(), sft->GetInformationBindingPointer().end());
			}
			return true;
		}

	}
	else
		return true;

	return false;

}

std::unordered_map<std::wstring, FeatureType>& FeatureTypes::GetFeatureTypePointer()
{
	return featureType;
}