#pragma once
#include "LuaScriptingReference.h"
#include "rapidxml.hpp"

#include "..\\FeatureCatalog\\Item.h"
#include "..\\FeatureCatalog\\Attribute.h"
#include "..\\FeatureCatalog\\ListedValue.h"
#include "..\\FeatureCatalog\\SimpleAttribute.h"
#include "..\\FeatureCatalog\\AttributeBinding.h"
#include "..\\FeatureCatalog\\ComplexAttribute.h"
#include "..\\FeatureCatalog\\Role.h"
#include "..\\FeatureCatalog\\InformationAssociation.h"
#include "..\\FeatureCatalog\\FeatureAssociation.h"
#include "..\\FeatureCatalog\\InformationType.h"
#include "..\\FeatureCatalog\\FeatureBinding.h"
#include "..\\FeatureCatalog\\FeatureType.h"

#include <string>
#include <optional>
#include <vector>
#include <map>

template <class T> T enum_by_name(std::map<T, std::string> m, std::string s)
{
	for (auto kvp : m)
		if (kvp.second == s)
			return kvp.first;

	return T::unknown;
}

class S100_FC_SimpleAttribute;
class S100_FC_ComplexAttribute;
class S100_FC_Role;
class S100_FC_InformationAssociation;
class S100_FC_FeatureAssociation;
class S100_FC_InformationType;
class S100_FC_FeatureType;
class FeatureCatalogue;

class KRS_LUA_SCRIPT_REFERENCE_API S100_FC
{
	template <class T>
	friend class S100_FC_ItemReference;

public:
	S100_FC() = default;
	S100_FC(std::string feature_catalog);

	void load_catalog(std::string feature_catalog);
	void load_catalog(FeatureCatalogue* feature_catalog);
	void unload_catalog();

	bool b_load = false;

	std::vector<std::string> get_simple_attribute_codes();
	std::vector<std::string> get_complex_attribute_codes();
	std::vector<std::string> get_role_codes();
	std::vector<std::string> get_information_association_codes();
	std::vector<std::string> get_feature_association_codes();
	std::vector<std::string> get_information_type_codes();
	std::vector<std::string> get_feature_type_codes();

	const S100_FC_SimpleAttribute* get_simple_attribute(std::string code);
	const S100_FC_ComplexAttribute* get_complex_attribute(std::string code);
	const S100_FC_Role* get_role(std::string code);
	const S100_FC_InformationAssociation* get_information_association(std::string code);
	const S100_FC_FeatureAssociation* get_feature_association(std::string code);
	const S100_FC_InformationType* get_information_type(std::string code);
	const S100_FC_FeatureType* get_feature_type(std::string code);

private:
	std::map<std::string, const S100_FC_SimpleAttribute*> m_simple_attributes;
	std::map<std::string, const S100_FC_ComplexAttribute*> m_complex_attributes;
	std::map<std::string, const S100_FC_Role*> m_roles;
	std::map<std::string, const S100_FC_InformationAssociation*> m_information_associations;
	std::map<std::string, const S100_FC_FeatureAssociation*> m_feature_associations;
	std::map<std::string, const S100_FC_InformationType*> m_information_types;
	std::map<std::string, const S100_FC_FeatureType*> m_feature_types;
};

//extern S100_FC theFC;
extern KRS_LUA_SCRIPT_REFERENCE_API FeatureCatalogue *pTheFC;