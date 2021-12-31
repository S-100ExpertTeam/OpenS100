#pragma once
#include "..\\LuaScriptingReference\\spatial_association.h"
#include "..\\LuaScriptingReference\\spatial_type.h"
#include "..\\LuaScriptingReference\\point.h"
#include "..\\LuaScriptingReference\\multipoint.h"
#include "..\\LuaScriptingReference\\curve.h"
#include "..\\LuaScriptingReference\\composite_curve.h"
#include "..\\LuaScriptingReference\\surface.h"

#include <vector>
#include <optional>

class FeatureCatalogue;
class S101Cell;

void hd_init(S101Cell *c);
void hd_delete();

std::vector<std::string> hd_get_feature_ids();
std::string hd_get_feature_code(std::string id);
std::vector<spatial_association> hd_get_feature_spatial_associations(std::string id);
std::vector<std::string> hd_get_feature_simple_attribute_values(std::string id, std::string path, std::string attribute_code);
int hd_get_feature_complex_attribute_count(std::string id, std::string path, std::string attribute_code);

std::vector<std::string> hd_get_information_type_ids();
std::string hd_get_information_type_code(std::string id);
std::vector<std::string> hd_get_information_type_simple_attribute_values(std::string id, std::string path, std::string attribute_code);
int hd_get_information_type_complex_attribute_count(std::string id, std::string path, std::string attribute_code);

std::vector<std::string> hd_get_feature_associated_information_ids(std::string feature_id, std::string association_code, std::optional<std::string> role_code);
std::vector<std::string> hd_get_feature_associated_feature_ids(std::string feature_id, std::string association_code, std::optional<std::string> role_code);
std::vector<std::string> hd_get_spatial_associated_information_ids(std::string spatial_id, std::string association_code, std::optional<std::string> role_code);
std::vector<std::string> hd_get_spatial_associated_feature_ids(std::string spatial_id);
std::vector<std::string> hd_get_spatial_ids();
spatial_type hd_get_spatial_type(std::string spatial_id);

point hd_get_point(std::string spatial_id);
multipoint hd_get_multipoint(std::string spatial_id);
curve hd_get_curve(std::string spatial_id);
composite_curve hd_get_composite_curve(std::string spatial_id);
surface hd_get_surface(std::string spatial_id);