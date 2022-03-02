#include "stdafx.h"
#include "lua_functions.h"

#include "..\\FeatureCatalog\\FeatureCatalogue.h"
#include "..\\FeatureCatalog\\FeatureType.h"
#include "..\\FeatureCatalog\\InformationType.h"
#include "..\\FeatureCatalog\\InformationBinding.h"
#include "..\\FeatureCatalog\\Role.h"
#include "..\\FeatureCatalog\\InformationAssociation.h"
#include "..\\FeatureCatalog\\FeatureAssociation.h"
#include "..\\FeatureCatalog\\SimpleAttribute.h"


//FeatureCatalogue *pTheFC = nullptr;
FeatureCatalogue *pTheFC = nullptr;
static lua_variant NullIfEmpty(std::string s)
{
	if (s == "")
		return nullptr;
	else
		return s;
}

static lua_variant NullIfEmpty(std::wstring s)
{
	if (s == L"")
		return nullptr;
	else
		return std::string(s.begin(), s.end());
}

lua_ref_ptr CreateSpatialAssociation(lua_session *ls, spatial_association *spatial_association)
{
	return ls->call<lua_ref_ptr>("CreateSpatialAssociation", { spatial_association->spatial_type, spatial_association->spatial_id, spatial_association->orientation, spatial_association->scale_minimum, spatial_association->scale_maximum });
}

lua_ref_ptr CreatePoint(lua_session *ls, point point)
{
	return ls->call<lua_ref_ptr>("CreatePoint", { point.x, point.y, point.z });
}

lua_ref_ptr CreateMultiPoint(lua_session *ls, multipoint multipoint)
{
	std::vector<lua_ref_ptr> points;

	for (auto point : multipoint.points)
		points.push_back(CreatePoint(ls, point));

	return ls->call<lua_ref_ptr>("CreateMultiPoint", { points });
}

lua_ref_ptr CreateCurveSegment(lua_session *ls, curve_segment curve_segment)
{
	std::vector<lua_ref_ptr> control_points;

	for (auto control_point : curve_segment.control_points)
		control_points.push_back(CreatePoint(ls, control_point));

	return ls->call<lua_ref_ptr>("CreateCurveSegment", { control_points, curve_segment.interpolation });
}

lua_ref_ptr CreateCurve(lua_session *ls, curve curve)
{
	std::vector<lua_ref_ptr> segments;

	for (auto segment : curve.segments)
		segments.push_back(CreateCurveSegment(ls, segment));

	return ls->call<lua_ref_ptr>("CreateCurve", { CreateSpatialAssociation(ls, &curve.start_point), CreateSpatialAssociation(ls, &curve.end_point), segments });
}

lua_ref_ptr CreateCompositeCurve(lua_session *ls, composite_curve composite_curve)
{
	std::vector<lua_ref_ptr> curve_associations;

	for (auto curve_association : composite_curve.curve_associations)
		curve_associations.push_back(CreateSpatialAssociation(ls, &curve_association));

	return ls->call<lua_ref_ptr>("CreateCompositeCurve", { curve_associations });
}

lua_ref_ptr CreateSurface(lua_session *ls, surface surface)
{
	if (surface.interior_rings.has_value())
	{
		std::vector<lua_ref_ptr> interior_rings;

		for (auto interior_ring : surface.interior_rings.value())
			interior_rings.push_back(CreateSpatialAssociation(ls, &interior_ring));

		return ls->call<lua_ref_ptr>("CreateSurface", { CreateSpatialAssociation(ls, &surface.exterior_ring), interior_rings });
	}
	else
	{
		return ls->call<lua_ref_ptr>("CreateSurface", { CreateSpatialAssociation(ls, &surface.exterior_ring) });
	}
}

lua_ref_ptr CreateArcByCenterPoint(lua_session *ls, spatial_association center_point, double radius, double start_angle, double angular_distance)
{
	return ls->call<lua_ref_ptr>("CreateArcByCenterPoint", { CreateSpatialAssociation(ls, &center_point), radius, start_angle, angular_distance });
}

lua_ref_ptr CreateCircleByCenterPoint(lua_session *ls, spatial_association center_point, double radius, double start_angle, double angular_distance)
{
	return ls->call<lua_ref_ptr>("CreateArcByCenterPoint", { CreateSpatialAssociation(ls, &center_point), radius, start_angle, angular_distance });
}

lua_ref_ptr CreateItem(lua_session *ls, Item *item) // ------------------------------------------------------------------------------------------------------
{
	std::string code = item->GetCode();
	std::string name = std::string(item->GetName().begin(), item->GetName().end());
	std::string definition = std::string(item->GetDefinition().begin(), item->GetDefinition().end());

	std::optional<std::string> remarks;
	if (true == item->IsEmptyRemarks())
	{
		remarks = item->GetRemarks();
	}

	std::vector<std::string> alias;

	for (auto a : item->GetAliasPointer())
	{
		alias.push_back(std::string(a.begin(), a.end()));
	}

	return ls->call<lua_ref_ptr>("CreateItem", { code, name, definition, remarks, alias });
}

lua_ref_ptr CreateNamedType(lua_session *ls, NamedType *named_type) // ------------------------------------------------------------------------------------------
{
	auto item = CreateItem(ls, named_type);

	std::vector<lua_ref_ptr> attribute_bindings;

	for (auto ab : named_type->GetAttributeBindingPointer())
		attribute_bindings.push_back(CreateAttributeBinding(ls, &ab));

	bool isAbstract = false;

	for (auto a : named_type->GetattributesPointer())
	{
		if (a.Getname() == L"isAbstract")
		{
			isAbstract = named_type->Getvalue() == L"true";
		}
	}

	return ls->call<lua_ref_ptr>("CreateNamedType", { item, isAbstract, attribute_bindings });
}

lua_ref_ptr CreateObjectType(lua_session *ls, S100ObjectType *object_type) 
{
	std::vector<lua_ref_ptr> information_bindings;

	for (auto ib : object_type->GetInformationBindingPointer())
		information_bindings.push_back(CreateInformationBinding(ls, &ib.second));

	return ls->call<lua_ref_ptr>("CreateObjectType", { CreateNamedType(ls, object_type), information_bindings });
}

lua_ref_ptr CreateInformationType(lua_session *ls, InformationType *information_type) 
{
	auto object_type = CreateObjectType(ls, information_type);

	std::vector<std::string> sub_types;
	std::string sub_type;
	for (auto st : information_type->GetSubTypePointer())
	{		
		std::string sub_type = std::string(st.begin(), st.end());
		sub_types.push_back(sub_type);
	}

	return ls->call<lua_ref_ptr>("CreateInformationType", { object_type, NullIfEmpty(information_type->GetSuperType()), sub_types });
}

lua_ref_ptr CreateFeatureType(lua_session *ls, FeatureType *feature_type) 
{
	auto object_type = CreateObjectType(ls, feature_type);

	std::vector<std::string> permitted_primitives;

	for (auto pp : feature_type->GetPermittedPrimitivesPointer())
		permitted_primitives.push_back(std::string(pp.GetValueString().begin(), pp.GetValueString().end())); // E-r??

	std::vector<lua_ref_ptr> feature_bindings;

	for (auto fb : feature_type->GetFeatureBindingPointer())
		feature_bindings.push_back(CreateFeatureBinding(ls, &fb.second));

	auto featureUseType = std::string(feature_type->GetFeatureUseTypePointer().GetValueString().begin(), feature_type->GetFeatureUseTypePointer().GetValueString().end());

	std::vector<std::string> sub_types;
	std::string sub_type;
	for (auto st : feature_type->GetSubTypePointer())
	{
		std::string sub_type = std::string(st.begin(), st.end());
		sub_types.push_back(sub_type);
	}

	return ls->call<lua_ref_ptr>("CreateFeatureType", { object_type, featureUseType, permitted_primitives, feature_bindings, NullIfEmpty(feature_type->GetSuperType()), sub_types });
}

lua_ref_ptr CreateInformationAssociation(lua_session *ls, InformationAssociation *information_association) // -----------------------------------------------------------------------------------
{
	auto named_type = CreateNamedType(ls, information_association);

	std::vector<lua_ref_ptr> ia_roles;

	std::wstring referenceCode;

	for (auto ia_role : information_association->GetRolePointer())
	{
		for (auto a : ia_role.Getattributes())
		{
			if (a.Getname() == L"ref")
			{
				referenceCode = a.Getvalue();
			}
		}

		for (auto s = pTheFC->GetRolesPointer().GetRolePointer().begin(); s != pTheFC->GetRolesPointer().GetRolePointer().end(); s++)
		{
			Role* sa = s->second;
			if (true == sa->CompareCode(referenceCode))
			{
				ia_roles.push_back(CreateRole(ls, sa));
			}
		}
	}

	std::vector<std::string> sub_types;
	std::string sub_type;
	for (auto st : information_association->GetSubTypePointer())
	{
		std::string sub_type = std::string(st.begin(), st.end());
		sub_types.push_back(sub_type);
	}

	return ls->call<lua_ref_ptr>("CreateInformationAssociation", { named_type, ia_roles, NullIfEmpty(information_association->GetSuperType()), sub_types });
}

lua_ref_ptr CreateFeatureAssociation(lua_session *ls, FeatureAssociation *feature_association) // -----------------------------------------------------------------------------------
{
	auto named_type = CreateNamedType(ls, feature_association);

	std::vector<lua_ref_ptr> fa_roles;

	std::wstring referenceCode;

	for (auto fa_role : feature_association->GetRolePointer())
	{
		for (auto a : fa_role.Getattributes())
		{
			if (a.Getname() == L"ref")
			{
				referenceCode = a.Getvalue();
			}
		}

		for (auto s = pTheFC->GetRolesPointer().GetRolePointer().begin(); s != pTheFC->GetRolesPointer().GetRolePointer().end(); s++)
		{
			Role* role = s->second;
			if (true == role->CompareCode(referenceCode))
			{
				fa_roles.push_back(CreateRole(ls, role));
			}
		}
	}

	std::vector<std::string> sub_types;

	for (auto st : feature_association->GetSubTypePointer())
	{
		std::string sub_type = std::string(st.begin(), st.end());
		sub_types.push_back(sub_type);
	}

	return ls->call<lua_ref_ptr>("CreateFeatureAssociation", { named_type, fa_roles, NullIfEmpty(feature_association->GetSuperType()), sub_types });

}

lua_ref_ptr CreateRole(lua_session *ls, Role *role) // ------------------------------------------------------------------------------------------------------------------------
{
	return ls->call<lua_ref_ptr>("CreateRole", { CreateItem(ls, role) });
}

lua_ref_ptr CreateAttributeConstraints(lua_session *ls, AttributeConstraints *attribute_constraints)
{
	std::optional<double> rangeLower;
	std::optional<double> rangeUpper;
	std::optional<std::string> rangeClosure;

	if (std::string(attribute_constraints->range.Getvalue().begin(), attribute_constraints->range.Getvalue().end()) != "")
	{
		rangeLower = attribute_constraints->range.GetLowerBound();
		rangeUpper = attribute_constraints->range.GetUpperBound();
		rangeClosure = std::string(attribute_constraints->range.GetIntervalType().GetValueString().begin(), attribute_constraints->range.GetIntervalType().GetValueString().end());
	}

	std::optional<std::string> textPattern = std::string(attribute_constraints->textPattern.begin(), attribute_constraints->textPattern.end());

	return ls->call<lua_ref_ptr>("CreateAttributeConstraints", { attribute_constraints->stringLength, textPattern, rangeLower, rangeUpper, rangeClosure, attribute_constraints->precision });
}

lua_ref_ptr CreateSimpleAttribute(lua_session *ls, SimpleAttribute *simple_attribute) // ------------------------------------------------------------------------------------------------------------------------
{
	auto item = CreateItem(ls, simple_attribute);

	auto valueType = FCD::S100_CD_AttributeValueTypeToString(simple_attribute->GetValueType());

	std::optional<std::string> uom;

	std::optional<std::string> quantitySpecification;


	std::vector<lua_ref_ptr> listedValues;

	SimpleAttribute* sa = (SimpleAttribute*)simple_attribute;

	if (true == sa->GetUOMRef().GetNameRef().empty())
	{
		uom = pugi::as_utf8(sa->GetUOMRef().GetName());
	}

	quantitySpecification = FCD::S100_CD_QuantitySpecificationToString(simple_attribute->GetQuantitySpecification());

	for (auto a : simple_attribute->GetListedValuesPointer())
	{
		for (auto b = a.GetListedValuePointer().begin(); b != a.GetListedValuePointer().end(); b++)
		{
			listedValues.push_back(CreateListedValue(ls, &(*b).second));//E-r
		}
	}

	lua_ref_ptr attributeContraints = nullptr;

	if (std::string(simple_attribute->GetConstraintsPointer().Getvalue().begin(), simple_attribute->GetConstraintsPointer().Getvalue().end()) != "")//E-r
		attributeContraints = CreateAttributeConstraints(ls, &simple_attribute->GetConstraintsPointer());

	return ls->call<lua_ref_ptr>("CreateSimpleAttribute", { item, valueType, uom, quantitySpecification, attributeContraints, listedValues });
}

lua_ref_ptr CreateComplexAttribute(lua_session *ls, ComplexAttribute *complex_attribute)
{
	auto item = CreateItem(ls, complex_attribute);

	std::vector<lua_ref_ptr> attribute_bindings;

	for (auto ab : complex_attribute->GetSubAttributeBindingPointer())
		attribute_bindings.push_back(CreateAttributeBinding(ls, &ab));

	return ls->call<lua_ref_ptr>("CreateComplexAttribute", { item, attribute_bindings });
}

lua_ref_ptr CreateListedValue(lua_session *ls, ListedValue *listed_value)
{
	std::string label = std::string(listed_value->GetLabel().begin(), listed_value->GetLabel().end());
	std::string definition = std::string(listed_value->GetDefinition().begin(), listed_value->GetDefinition().end());
	int code = listed_value->GetCode().GetvalueInteger();

	std::optional<std::string> remarks;
	if (listed_value->GetRemarks() != L"")
		remarks = std::string(listed_value->GetRemarks().begin(), listed_value->GetRemarks().end());

	std::vector<std::string> alias;
	for (auto a : listed_value->GetAliasPointer())
	{
		alias.push_back(std::string(a.begin(), a.end()));
	}

	return ls->call<lua_ref_ptr>("CreateListedValue", { label, definition, code, remarks, alias });
}

lua_ref_ptr CreateAttributeBinding(lua_session *ls, AttributeBinding *attribute_binding) // -----------------------------------------------------------------------------------------------------------------------
{
	lua_ref_ptr Attributeb;

	std::string referenceCode;

	bool sequential = false;

	for (auto a : attribute_binding->GetAttribute().GetattributesPointer())
	{
		if (a.Getname() == L"ref")
		{
			referenceCode = a.GetvalueString();
		}
	}

	sequential = attribute_binding->GetAttribute().Getvalue().compare(L"true") == 0;

	std::vector<int> permittedValues;

	for (auto s : attribute_binding->GetPermittedValues().Getvalue())
	{
		permittedValues.push_back(s);
	}

	std::optional<int> muluppvalue;

	if (attribute_binding->GetMultiplicity().GetUpper().Getvalue() != L"")
	{
		muluppvalue = std::stoi(attribute_binding->GetMultiplicity().GetUpper().Getvalue());
	}

	return ls->call<lua_ref_ptr>("CreateAttributeBinding", { referenceCode, attribute_binding->GetMultiplicity().GetLower(), muluppvalue, sequential, permittedValues });
}

lua_ref_ptr CreateInformationBinding(lua_session *ls, InformationBinding *information_binding) // -----------------------------------------------------------------------------------------------------------------------
{
	std::string role;
	std::string information_association;

	std::wstring referenceCode;
	std::string itreferenceCode;

	for (auto a : information_binding->GetRolePointer().GetattributesPointer())
	{
		if (a.Getname() == L"ref")
		{
			referenceCode = a.Getvalue();
		}
	}

	role = std::string(information_binding->GetRolePointer().Getvalue().begin(), information_binding->GetRolePointer().Getvalue().end());

	for (auto a : information_binding->GetAssociationPointer().GetattributesPointer())
	{
		if (a.Getname() == L"ref")
		{
			referenceCode = a.Getvalue();
		}
	}

	information_association = std::string(information_binding->GetAssociationPointer().Getvalue().begin(), information_binding->GetAssociationPointer().Getvalue().end());

	for (auto a : information_binding->GetInformationTypePointer().GetattributesPointer())
	{
		if (a.Getname() == L"ref")
		{
			itreferenceCode = a.GetvalueString();
		}
	}

	std::optional<int> muluppvalue;

	if (information_binding->GetMultiplicityPointer().GetUpper().Getvalue() != L"")
		muluppvalue = std::stoi(information_binding->GetMultiplicityPointer().GetUpper().Getvalue());

	auto roleType = std::string(information_binding->GetRolePointer().Getvalue().begin(), information_binding->GetRolePointer().Getvalue().end());

	return ls->call<lua_ref_ptr>("CreateInformationBinding", { itreferenceCode, information_binding->GetMultiplicityPointer().GetLower(), muluppvalue, roleType, role, information_association });

}

lua_ref_ptr CreateFeatureBinding(lua_session *ls, FeatureBinding *feature_binding)
{
	std::string role;
	std::string feature_association;

	std::wstring referenceCode;
	std::string ftreferenceCode;

	for (auto a : feature_binding->GetRolePointer().GetattributesPointer())
	{
		if (a.Getname() == L"ref")
		{
			referenceCode = a.Getvalue();
		}
	}



	role = std::string(feature_binding->GetRolePointer().Getvalue().begin(), feature_binding->GetRolePointer().Getvalue().end());

	for (auto a : feature_binding->GetAssociationPointer().GetattributesPointer())
	{
		if (a.Getname() == L"ref")
		{
			referenceCode = a.Getvalue();
		}
	}

	feature_association = std::string(feature_binding->GetAssociationPointer().Getvalue().begin(), feature_binding->GetAssociationPointer().Getvalue().end());

	for (auto a : feature_binding->GetFeatureTypePointer().GetattributesPointer())
	{
		if (a.Getname()== L"ref")
		{
			ftreferenceCode = a.GetvalueString();
		}
	}

	std::optional<int> muluppvalue;

	if (feature_binding->GetMultiplicityPointer().GetUpper().Getvalue()!= L"")
		muluppvalue = std::stoi(feature_binding->GetMultiplicityPointer().GetUpper().Getvalue());


	auto roleType = std::string(feature_binding->GetRolePointer().Getvalue().begin(), feature_binding->GetRolePointer().Getvalue().end());

	return ls->call<lua_ref_ptr>("CreateFeatureBinding", { ftreferenceCode, feature_binding->GetMultiplicityPointer().GetLower(), muluppvalue, roleType, role, feature_association });
}