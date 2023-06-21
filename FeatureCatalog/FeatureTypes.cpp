#include "stdafx.h"
#include "FeatureTypes.h"

FeatureTypes::FeatureTypes(void)
{

}

FeatureTypes::~FeatureTypes(void)
{
	for (auto i = vecFeatureType.begin(); i != vecFeatureType.end(); i++)
	{
		delete (*i);
	}
}

void FeatureTypes::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:S100_FC_FeatureType"))
		{
			auto sa = new FeatureType();
			sa->GetContents(instruction);
			InsertFeatureType(sa);
		}
	}
	ApplySuperType();
}

void FeatureTypes::ApplySuperType()
{
	for (auto itor = featureType.begin(); itor != featureType.end(); itor++)
	{
		FeatureType *ft = itor->second;
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
			FeatureType* sft = itor->second;

			for (auto i = sft->GetAttributeBindingPointer().begin();
				i != sft->GetAttributeBindingPointer().end();
				i++)
			{
				auto ab = new AttributeBinding();
				*ab = *(*i);
				ft->InsertAttributeBinding(ab);
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
			FeatureType* sft = itor->second;
			if (SetAssociationFromSuperType(sft))
			{
				for (auto i = sft->GetFeatureBindingPointer().begin();
					i != sft->GetFeatureBindingPointer().end();
					i++)
				{
					auto fb = new FeatureBinding();
					*fb = *(*i);
					ft->InsertFeatureBinding(fb);
				}

				for (auto i = sft->GetInformationBindingPointer().begin();
					i != sft->GetInformationBindingPointer().end();
					i++)
				{
					auto ib = new InformationBinding();
					*ib = *(*i);
					ft->InsertInformationBinding(ib);
				}
			}
			return true;
		}

	}
	else
		return true;

	return false;

}

std::vector<FeatureType*>& FeatureTypes::GetVecFeatureType()
{
	return vecFeatureType;
}

std::unordered_map<std::wstring, FeatureType*>& FeatureTypes::GetFeatureType()
{
	return featureType;
}

bool FeatureTypes::InsertFeatureType(FeatureType* value)
{
	auto key = value->GetCodeAsWString();
	if (featureType.find(key) == featureType.end())
	{
		vecFeatureType.push_back(value);
		featureType.insert({ key, value });
		return true;
	}

	delete value;
	return false;
}