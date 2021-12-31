#include "stdafx.h"
#include "FeatureType.h"

FeatureType::FeatureType()
{

}

FeatureType::~FeatureType()
{

}

void FeatureType::GetContents(pugi::xml_node& node)
{
	((S100ObjectType*)this)->S100ObjectType::GetContents(node);

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:featureUseType"))
		{
			featureUseType.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:featureBinding"))
		{
			FeatureBinding feature;
			feature.GetContents(instruction);

			std::wstring associatename = feature.GetFeatureTypePointer().Getvalue();
			if (associatename.compare(L"") == 0)
			{
				auto pointer = feature.GetFeatureTypePointer().GetattributesPointer();
				for (auto itor = pointer.begin();
					itor != pointer.end();
					itor++)
				{
					auto attri = &(*itor);
					if (attri->Getname().compare(L"ref") == 0)
					{
						associatename = attri->Getvalue();
					}
				}
			}
			featureBinding[associatename] = feature;
		}
		else if (!strcmp(instructionName, "S100FC:permittedPrimitives"))
		{
			permittedPrimitives.push_back(SpatialPrimitiveType());
			permittedPrimitives.back().GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:superType"))
		{
			superType = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:subType"))
		{
			subType.push_back(pugi::as_wide(instruction.child_value()));
		}
	}
}

const std::wstring& FeatureType::GetSuperType()
{
	return superType;
}

void FeatureType::SetSuperType(std::wstring value)
{
	superType = value;
}

std::list<std::wstring>& FeatureType::GetSubTypePointer()
{
	return subType;
}

FeatureUseType& FeatureType::GetFeatureUseTypePointer()
{
	return featureUseType;
}

std::unordered_map<std::wstring, FeatureBinding>& FeatureType::GetFeatureBindingPointer()
{
	return featureBinding;
}

std::list<SpatialPrimitiveType>& FeatureType::GetPermittedPrimitivesPointer()
{
	return permittedPrimitives;
}