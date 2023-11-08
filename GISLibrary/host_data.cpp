#include "stdafx.h"
#include "host_data.h"
#include "S101Cell.h"
#include "F_CUCO.h"
#include "F_PTAS.h"
#include "F_CCOC.h"
#include "F_SPAS.h"
#include "F_C3IL.h"
#include "F_C2IL.h"
#include "F_C2IT.h"
#include "F_RIAS.h"
#include "F_ATTR.h"
#include "F_INAS.h"
#include "R_SurfaceRecord.h"
#include "R_PointRecord.h"
#include "R_CompositeRecord.h"
#include "R_CurveRecord.h"
#include "R_InformationRecord.h"
#include "R_FeatureRecord.h"
#include "R_MultiPointRecord.h"
#include "ATTR.h"
#include "C3IL.h"
#include "C2IL.h"
#include "CodeWithNumericCode.h"
#include "CUCO.h"
#include "SPAS.h"
#include "PTAS.h"
#include "RIAS.h"
#include "ProcessS101.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../LatLonUtility/LatLonUtility.h"

#include "../LuaScriptingReference/spatial_association.h"
#include "../LuaScriptingReference/composite_curve.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <stdlib.h>
#include <iomanip>
#include <cassert>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

static S101Cell *cell;
static FeatureCatalogue *feature_catalog;

// Input schema data
static std::vector<std::string> s_feature_ids; // Process in input schema order.  Not strictly necessary but can aid in debugging.
static std::vector<std::string> s_information_ids;

static std::map<std::string, GF::FeatureType*> s_feature_nodes;
static std::map<std::string, R_InformationRecord*> s_information_nodes;
static std::map<std::string, R_PointRecord*> s_spatial_point_nodes;
static std::map<std::string, R_MultiPointRecord*> s_spatial_mpoint_nodes;
static std::map<std::string, R_CurveRecord*> s_spatial_curve_nodes;
static std::map<std::string, R_CompositeRecord*> s_spatial_ccurve_nodes;
static std::map<std::string, R_SurfaceRecord*> s_spatial_surface_nodes;
static std::map<std::string, std::set<std::string>*> s_spatial_features;

static spatial_association get_spatial_association(PTAS* ptas);
static spatial_association get_spatial_association(CUCO* cuco);
static spatial_association get_spatial_association(F_SPAS* spatial);
static spatial_association get_spatial_association(RIAS* rias);

static std::string WstringToString(CString& value)
{
	auto cValue = LibMFCUtil::ConvertWCtoC((wchar_t*)std::wstring(value).c_str());
	std::string result(cValue);
	delete[] cValue;
	return result;
}

static void get_referenced_spatials(std::string spatial_id, std::vector<std::string> *spatials)
{
	std::vector<std::string> ret;

	int findOffset = 0;
	int splitOffset = 0;
	std::string token = "|";

	while ((splitOffset = (int)spatial_id.find(token, findOffset)) != std::string::npos)
	{
		ret.push_back(spatial_id.substr(findOffset, splitOffset - findOffset));
		findOffset = splitOffset + (int)token.length();
	}

	ret.push_back(spatial_id.substr(findOffset, spatial_id.length() - findOffset));

	std::string spatial_type;

	if (ret[0] == "Point")
	{
		auto spatial = s_spatial_point_nodes[spatial_id];

		if (spatial) {

			spatial_type = ret[0];
			spatials->push_back(spatial_id);

		}
	}
	else if (ret[0] == "MultiPoint")
	{
		auto spatial = s_spatial_mpoint_nodes[spatial_id];

		if (spatial) {

			spatial_type = ret[0];
			spatials->push_back(spatial_id);

		}
	}
	else if (ret[0] == "Curve")
	{
		auto spatial = s_spatial_curve_nodes[spatial_id];
		
		if (spatial) {

			spatial_type = ret[0];
			spatials->push_back(spatial_id);

			for (auto i = spatial->m_ptas->m_arr.begin(); i != spatial->m_ptas->m_arr.end(); i++)
			{
				auto ptas = *i;
				std::string boundaryType;
				switch (ptas->m_topi)
				{
				case 1:
					boundaryType = "Begin";
					break;
				case 2:
					boundaryType = "End";
					break;
				}
				if (boundaryType == "Begin")
					spatials->push_back(get_spatial_association(ptas).spatial_type); // (1, ptas) => 1="Point"
				else
					spatials->push_back(get_spatial_association(ptas).spatial_type);
			}
		}
	}
	else if (ret[0] == "CompositeCurve")
	{
		auto spatial = s_spatial_ccurve_nodes[spatial_id];

		if (spatial) {
			spatial_type = ret[0];
			spatials->push_back(spatial_id);

			composite_curve composite_curve;

			for (auto itorParent = spatial->m_cuco.begin(); itorParent != spatial->m_cuco.end(); itorParent++)
			{
				F_CUCO* cucoParent = *itorParent;

				for (auto itor = cucoParent->m_arr.begin(); itor != cucoParent->m_arr.end(); itor++)
				{
					CUCO* cuco = *itor;

					get_referenced_spatials(get_spatial_association(cuco).spatial_id, spatials);
				}
			}
		}
	}
	else if (ret[0] == "Surface")
	{
		auto spatial = s_spatial_surface_nodes[spatial_id];

		if (spatial) {

			spatial_type = ret[0];
			spatials->push_back(spatial_id);

			surface surface;

			for (auto itorParent = spatial->m_rias.begin(); itorParent != spatial->m_rias.end(); itorParent++)
			{
				F_RIAS* riasParent = *itorParent;
				for (auto itor = riasParent->m_arr.begin(); itor != riasParent->m_arr.end(); itor++)
				{
					RIAS* rias = *itor;
					std::string usage;

					switch (rias->m_usag)
					{
					case 1:
						usage = "Outer";
						break;
					case 2:
						usage = "Inner";
						break;
					}

					if (usage == "Outer")
					{
						get_referenced_spatials(get_spatial_association(rias).spatial_id, spatials);
					}
					else
					{
						if (!surface.interior_rings.has_value())
						{
							surface.interior_rings = std::vector<spatial_association>();
						}

						get_referenced_spatials(get_spatial_association(rias).spatial_id, spatials);
					}
				}
			}
		}
	}
}

void hd_delete()
{
	if (!s_spatial_features.empty())
	{
		for (auto data : s_spatial_features)
		{
			data.second->clear();
			delete  data.second;
		}
		s_spatial_features.clear();
	}
		
	s_feature_nodes.clear();
	s_information_nodes.clear();
	s_spatial_point_nodes.clear();
	s_spatial_mpoint_nodes.clear();
	s_spatial_curve_nodes.clear();
	s_spatial_ccurve_nodes.clear();
	s_spatial_surface_nodes.clear();

	s_feature_ids.clear();
}

void hd_init(S101Cell *c)
{
	s_feature_ids.clear();
	s_feature_nodes.clear();
	s_information_nodes.clear();
	s_spatial_features.clear();

	__int64 key = 0;
	cell = c;

	auto features = cell->GetVecFeature();
	for (auto i = features.begin(); i != features.end(); i++)
	{
		int id = (*i)->GetRCID();
		std::string sid = std::to_string(id);
		s_feature_ids.push_back(sid);
		s_feature_nodes[sid] = (*i);
	}

	auto informations = cell->GetVecInformation();
	for (auto i = informations.begin(); i != informations.end(); i++)
	{
		int id = (*i)->GetRCID();
		std::string sid = std::to_string(id);
		s_information_ids.push_back(sid);
		s_information_nodes[sid] = (*i);
	}

	auto points = cell->GetVecPoint();
	for (auto i = points.begin(); i != points.end(); i++)
	{
		int id = (*i)->GetRCID();
		std::string sid = "Point|" + std::to_string(id);
		s_spatial_point_nodes[sid] = (*i);
	}

	auto multiPoints = cell->GetVecMultiPoint();
	for (auto i = multiPoints.begin(); i != multiPoints.end(); i++)
	{
		int id = (*i)->GetRCID();
		std::string sid = "MultiPoint|" + std::to_string(id);
		s_spatial_mpoint_nodes[sid] = (*i);
	}

	auto curves = cell->GetVecCurve();
	for (auto i = curves.begin(); i != curves.end(); i++)
	{
		int id = (*i)->GetRCID();
		std::string sid = "Curve|" + std::to_string(id);
		s_spatial_curve_nodes[sid] = (*i);
	}

	auto compositeCurves = cell->GetVecComposite();
	for (auto i = compositeCurves.begin(); i != compositeCurves.end(); i++)
	{
		int id = (*i)->GetRCID();
		std::string sid = "CompositeCurve|" + std::to_string(id);
		s_spatial_ccurve_nodes[sid] = (*i);
	}

	auto surfaces = cell->GetVecSurface();
	for (auto i = surfaces.begin(); i != surfaces.end(); i++)
	{
		int id = (*i)->GetRCID();
		std::string sid = "Surface|" + std::to_string(id);
		s_spatial_surface_nodes[sid] = (*i);
	}

	// Determine which features are referenced by each spatial
	for (auto feature_id : s_feature_ids)
	{
		std::vector<std::string> ref_spatials;

		auto spatials = hd_get_feature_spatial_associations(feature_id);
		for (auto spatial : spatials)
		{
			get_referenced_spatials(spatial.spatial_id, &ref_spatials);
		}

		for (auto spatial : ref_spatials)
		{
			if (!s_spatial_features.count(spatial))
			{
				s_spatial_features[spatial] = new std::set<std::string>();
			}

			auto spatial_features = s_spatial_features[spatial];
			spatial_features->insert(feature_id); 
		}
	}
}

std::vector<std::string>& hd_get_feature_ids()
{
	return s_feature_ids;
}

std::string hd_get_feature_code(std::string id)
{
	//R_FeatureRecord* fr = s_feature_nodes[id];
	auto fr = s_feature_nodes[id];

	//auto f1 = cell->m_dsgir.m_ftcs->m_arr.find(fr->m_frid.m_nftc);
	//if (f1 == cell->m_dsgir.m_ftcs->m_arr.end())
	//{
	//	return "";
	//}

	return fr->GetCode();

	//std::wstring s1 = std::wstring(f1->second->m_code);
	//std::string ret;
	//ret.assign(s1.begin(), s1.end());

	//return ret;
}

static spatial_association get_spatial_association(F_SPAS* spatial)
{
	spatial_association sa;
	for (auto itor = spatial->m_arr.begin(); itor != spatial->m_arr.end(); itor++)
	{
		SPAS* spas = *itor;
		// Leave scaleMin/Max unset since Part 9 input schema doesn't define it.
		///SGJ
		std::string primitive1;
		switch (spas->m_name.RCNM)
		{
		case 110: primitive1 = "Point"; break;
		case 115: primitive1 = "MultiPoint"; break;
		case 120: primitive1 = "Curve"; break;
		case 125: primitive1 = "CompositeCurve"; break;
		case 130: primitive1 = "Surface"; break;
		default: //Err  
			break;
		}
		std::string orientation = "Forward";
		if (spas->m_name.RCNM == 120 || spas->m_name.RCNM == 125)
		{
			switch (spas->m_ornt)
			{
			case 1:
				orientation = "Forward";
				break;
			case 2:
				orientation = "Reverse";
				break;
			}
		}
		sa = { primitive1, primitive1 + "|" + std::to_string(spas->m_name.RCID), orientation };
	}
	return sa;
}

///SGJ
static spatial_association get_spatial_association(PTAS* ptas)
{
	spatial_association sa;
	std::string primitive1 = "Point";
	sa = { primitive1, primitive1 + "|" + std::to_string(ptas->m_name.RCID) };
	return sa;
}

static spatial_association get_spatial_association(CUCO* cuco)
{
	spatial_association sa;
	std::string primitive1;

	switch (cuco->m_name.RCNM)
	{
	case 120: primitive1 = "Curve"; break;
	case 125: primitive1 = "CompositeCurve";
		break;
	default: break;//Err
	}

	std::string orientation;
	switch (cuco->m_ornt)
	{
	case 1:
		orientation = "Forward";
		break;
	case 2:
		orientation = "Reverse";
		break;
	}

	sa = { primitive1, primitive1 + "|" + std::to_string(cuco->m_name.RCID), orientation };
	return sa;
}

static spatial_association get_spatial_association(RIAS* rias)
{
	spatial_association sa;

	std::string primitive1;
	switch (rias->m_name.RCNM)
	{
		case 120: primitive1 = "Curve"; break;
		case 125: primitive1 = "CompositeCurve"; break;
		default: break;//Err
	}

	std::string orientation;

	switch (rias->m_ornt)
	{
	case 1:
		orientation = "Forward";
		break;
	case 2:
		orientation = "Reverse";
		break;
	}
	sa = { primitive1, primitive1 + "|" + std::to_string(rias->m_name.RCID), orientation };
	return sa;
}

///SGJ
std::vector<spatial_association> hd_get_feature_spatial_associations(std::string id)
{
	auto feature = s_feature_nodes[id];

	std::vector<spatial_association> sas;

	for (auto i = feature-)

	for (auto spatial = feature->m_spas.begin(); spatial != feature->m_spas.end(); spatial++)
	{
		spatial_association a = get_spatial_association(*spatial);
		sas.push_back(a);
	}
	return sas;
}

static std::vector<std::string> get_simple_attribute_values(R_FeatureRecord* fr, std::string path, std::string attribute_code)
{
	std::vector<std::string> attr_values;
	std::vector<std::string> path_items;
	std::vector<int> atixs;

	size_t offset = 0;

	if (!path.empty())
		path += ";";

	std::istringstream iss(path);
	std::string token;
	while (std::getline(iss, token, ';'))
	{
		auto colonIndex = token.find_first_of(':');

		if (colonIndex >= 0 && colonIndex < token.length() - 1)
		{
			int atixIndex = (int)colonIndex + 1;
			auto strAtix = token.substr(atixIndex, token.length() - atixIndex);
			auto atix = atoi(strAtix.c_str());
			atixs.push_back(atix);

			auto code = token.substr(0, colonIndex);
			path_items.push_back(code);
		}
	}

	if (atixs.size() == path_items.size())
	{
		int cnt = (int)atixs.size();

		bool find = false;
		int currentAtix = 0;
		int currentPaix = 0;
		if (fr->m_attr.size() > 0)
		{
			auto fattr = fr->m_attr.front();
			if (fattr->m_arr.size() > 0)
			{
				int parentIndex = -1;
				for (int i = 0; i < cnt; i++)
				{
					for (auto j = parentIndex > 0? parentIndex : 0; j < (int)fattr->m_arr.size(); j++)
					{
						auto attr = fattr->m_arr.at(j);

						auto item = cell->m_dsgir.m_atcs->m_arr.find(attr->m_natc);
						if (item != cell->m_dsgir.m_atcs->m_arr.end())
						{
							auto code = WstringToString(item->second->m_code);
							auto atix = attr->m_atix;
							if (path_items.at(i) == code && atixs.at(i) == atix)
							{
								parentIndex = j;
								break;
							}
						}
					}
				}

				parentIndex++;
				if (parentIndex != -1 || cnt == 0)
				{
					if (cnt == 0)
					{
						parentIndex = 0;
					}

					if (parentIndex <= (int)fattr->m_arr.size())
					{
						for (int i = 0; i < (int)fattr->m_arr.size(); i++)
						{
							auto attr = fattr->m_arr.at(i);

							auto item = cell->m_dsgir.m_atcs->m_arr.find(attr->m_natc);
							if (item != cell->m_dsgir.m_atcs->m_arr.end())
							{
								auto code = WstringToString(item->second->m_code);
								if (parentIndex == 0)
								{
									if (code == attribute_code && attr->m_paix == 0)
									{
										auto value = WstringToString(attr->m_atvl);
										if (value.compare("") == 0)
										{
											value = ProcessS101::g_unknown_attribute_value;
										}
										attr_values.push_back(value);
										continue;
									}
								}
								else
								{
									if (code == attribute_code && attr->m_paix == parentIndex)
									{
										auto value = WstringToString(attr->m_atvl);
										if (value.compare("") == 0)
										{
											value = ProcessS101::g_unknown_attribute_value;
										}
										attr_values.push_back(value);
										continue;
									}
								}
							}
							else
							{
								//OutputDebugString(_T("Attribute code error1\n"));
							}
						}
					}
					else
					{
						//OutputDebugString(_T("Parent index error\n"));
					}
				}
				else
				{
					//OutputDebugString(_T("ParentIndex Error\n"));
				}
			}
		}
	}
	else
	{
		//OutputDebugString(_T("Attribute path error\n"));
	}
	return attr_values;
}

static std::vector<std::string> get_simple_attribute_values(R_InformationRecord* Ir, std::string path, std::string attribute_code)
{
	std::vector<std::string> attr_values;
	std::vector<std::string> path_items;

	size_t offset = 0;
	size_t found;

	if (!path.empty())
		path += ";";

	while ((found = path.find_first_of(';', offset)) != std::string::npos)
	{
		auto path_item = path.substr(offset, found - offset);
		path_items.push_back(path_item);
		offset = found + 1;
	}

	for (auto itorParent = Ir->m_attr.begin(); itorParent != Ir->m_attr.end(); itorParent++)
	{
		F_ATTR* attrParent = *itorParent;

		for (auto itor = attrParent->m_arr.begin(); itor != attrParent->m_arr.end(); itor++)
		{
			ATTR* attr = *itor;

			auto aitor = cell->m_dsgir.m_atcs->m_arr.find(attr->m_natc);
			if (aitor != cell->m_dsgir.m_atcs->m_arr.end())
			{
				std::string attributeAcronym = aitor->second->getCodeAsString();

				if (attributeAcronym.compare(attribute_code) == 0)
				{
					if (offset != 0)
					{
						std::string r_value = attr->getValueAsString();

						std::string parent_name = *path_items.begin();
						auto split = parent_name.find(':', 0);
						auto parent_code_in_path = parent_name.substr(0, split);

						ATTR* parent_attribute = attrParent->m_arr[attr->m_paix - 1];

						auto paitor = cell->m_dsgir.m_atcs->m_arr.find(parent_attribute->m_natc);
						if (paitor != cell->m_dsgir.m_atcs->m_arr.end())
						{
							std::string parent_attributeAcronym = paitor->second->getCodeAsString();

							if (parent_code_in_path.compare(parent_attributeAcronym) == 0)
							{
								if (r_value.compare("") == 0)
								{
									r_value = ProcessS101::g_unknown_attribute_value;
								}
								attr_values.push_back(r_value);
							}
						}

					}
					else
					{
						if(attr->m_atvl.GetLength() > 0)
						{
							std::string r_value = attr->getValueAsString();
						}
						else
						{
							std::string r_value = ProcessS101::g_unknown_attribute_value;
							attr_values.push_back(r_value);
						}
					}
				}
			}
		}
	}
	return attr_values;
}


static int get_complex_attribute_count(R_FeatureRecord* fr, std::string path, std::string attribute_code)
{
	int attr_count = 0;

	std::vector<std::string> attr_values;
	std::vector<std::string> path_items;

	size_t offset = 0;
	size_t found;

	if (!path.empty())
		path += ";";

	while ((found = path.find_first_of(';', offset)) != std::string::npos)
	{
		auto path_item = path.substr(offset, found - offset);

		path_items.push_back(path_item);

		offset = found + 1;
	}

	for (auto itorParent = fr->m_attr.begin(); itorParent != fr->m_attr.end(); itorParent++)
	{
		F_ATTR* attrParent = *itorParent;
		for (auto itor = attrParent->m_arr.begin(); itor != attrParent->m_arr.end(); itor++)
		{
			ATTR* attr = *itor;

			auto aitor = cell->m_dsgir.m_atcs->m_arr.find(attr->m_natc);
			if (aitor != cell->m_dsgir.m_atcs->m_arr.end())
			{
				std::string attributeAcronym = aitor->second->getCodeAsString();

				if (attributeAcronym.compare(attribute_code) == 0)
				{
					if (offset != 0)
					{
						std::string r_value = attr->getValueAsString();

						std::string parent_name = *path_items.rbegin();
						auto split = parent_name.find(':', 0);
						auto parent_code_in_path = parent_name.substr(0, split);
						auto parent_sequence_in_path = parent_name.substr(split+1, parent_name.size() - split-1);
						auto n_parent_sequence_in_path = atoi(parent_sequence_in_path.c_str());

						ATTR* parent_attribute = attrParent->m_arr[attr->m_paix - 1];

						auto paitor = cell->m_dsgir.m_atcs->m_arr.find(parent_attribute->m_natc);
						if (paitor != cell->m_dsgir.m_atcs->m_arr.end())
						{
							std::string parent_attributeAcronym = paitor->second->getCodeAsString();

							if (parent_code_in_path.compare(parent_attributeAcronym) == 0 &&
								n_parent_sequence_in_path == parent_attribute->m_atix)
							{
								attr_count++;
							}
						}
					}
					else
					{
						std::string r_value = attr->getValueAsString();

						attr_count++;
					}
				}
			}
		}
	}
	return attr_count;
}

static int get_complex_attribute_count(R_InformationRecord* Ir, std::string path, std::string attribute_code)
{
	int attr_count = 0;

	std::vector<std::string> attr_values;
	std::vector<std::string> path_items;

	size_t offset = 0;
	size_t found;

	if (!path.empty())
		path += ";";

	while ((found = path.find_first_of(';', offset)) != std::string::npos)
	{
		auto path_item = path.substr(offset, found - offset);

		path_items.push_back(path_item);

		offset = found + 1;
	}

	for (auto itorParent = Ir->m_attr.begin(); itorParent != Ir->m_attr.end(); itorParent++)
	{
		F_ATTR* attrParent = *itorParent;

		for (auto itor = attrParent->m_arr.begin(); itor != attrParent->m_arr.end(); itor++)
		{
			ATTR* attr = *itor;

			auto aitor = cell->m_dsgir.m_atcs->m_arr.find(attr->m_natc);
			if (aitor != cell->m_dsgir.m_atcs->m_arr.end())
			{
				std::string attributeAcronym = aitor->second->getCodeAsString();

				if (attributeAcronym.compare(attribute_code) == 0)
				{
					if (offset != 0)
					{
						std::string r_value = attr->getValueAsString();

						std::string parent_name = *path_items.begin();
						auto split = parent_name.find(':', 0);
						auto parent_code_in_path = parent_name.substr(0, split);

						ATTR* parent_attribute = attrParent->m_arr[attr->m_paix - 1];

						auto paitor = cell->m_dsgir.m_atcs->m_arr.find(parent_attribute->m_natc);
						if (paitor != cell->m_dsgir.m_atcs->m_arr.end())
						{
							std::string parent_attributeAcronym = paitor->second->getCodeAsString();

							if (parent_code_in_path.compare(parent_attributeAcronym) == 0)
							{
								attr_count++;
							}
						}
					}
					else
					{
						std::string r_value = attr->getValueAsString();
						attr_count++;
					}
				}
			}
		}
	}
	return attr_count;
}

std::vector<std::string> hd_get_feature_simple_attribute_values(std::string id, std::string path, std::string attribute_code)
{
	return get_simple_attribute_values(s_feature_nodes[id], path, attribute_code);
}

int hd_get_feature_complex_attribute_count(std::string id, std::string path, std::string attribute_code)
{
	int a = get_complex_attribute_count(s_feature_nodes[id], path, attribute_code);
	return a;
}

std::vector<std::string>& hd_get_information_type_ids()
{
	return s_information_ids;
}

std::string hd_get_information_type_code(std::string id)
{
	R_InformationRecord* ir = s_information_nodes[id];
	if (!ir) {
		return std::string("");
	}

	auto i1 = cell->m_dsgir.m_itcs->m_arr.find(ir->m_irid.NITC());
	if (i1 == cell->m_dsgir.m_itcs->m_arr.end())
	{
		return std::string("");
	}
	
	std::wstring s1 =std::wstring(i1->second->m_code);
	std::string ret;
	ret.assign(s1.begin(), s1.end());

	return ret;
}

std::vector<std::string> hd_get_information_type_simple_attribute_values(std::string id, std::string path, std::string attribute_code)
{
	return get_simple_attribute_values(s_information_nodes[id], path, attribute_code);
}

int hd_get_information_type_complex_attribute_count(std::string id, std::string path, std::string attribute_code)
{
	return get_complex_attribute_count(s_information_nodes[id], path, attribute_code);
}

///SGJ
std::vector<std::string> hd_get_feature_associated_information_ids(std::string feature_id, std::string association_code, std::optional<std::string> role_code)
{
	std::vector<std::string> information_ids1;

	auto feature = s_feature_nodes[feature_id];
	if (!feature) {
		return information_ids1;
	}

	if (feature->m_inas.size() > 0)
	{
		for (auto itorParent = feature->m_inas.begin(); itorParent != feature->m_inas.end(); itorParent++)
		{
			F_INAS* f_inas = *itorParent;
			auto asitor = cell->m_dsgir.m_iacs->m_arr.find(f_inas->m_niac);
			auto ritor = cell->m_dsgir.m_arcs->m_arr.find(f_inas->m_narc);

			auto r_asName = asitor->second->getCodeAsString();
			auto r_roleName = ritor->second->getCodeAsString();

			if (!role_code.has_value() || r_roleName == role_code.value())
				information_ids1.push_back(std::to_string(f_inas->m_name.RCID));
		}
	}
	return information_ids1;
}

std::vector<std::string> hd_get_feature_associated_feature_ids(std::string feature_id, std::string association_code, std::optional<std::string> role_code)
{
	std::vector<std::string> feature_ids;

	auto feature = s_feature_nodes[feature_id];
	if (!feature) {
		return feature_ids;
	}

	if (feature->m_inas.size() > 0)
	{
		for (auto itorParent = feature->m_inas.begin(); itorParent != feature->m_inas.end(); itorParent++)
		{
			F_INAS* f_inas = *itorParent;
			auto asitor = cell->m_dsgir.m_iacs->m_arr.find(f_inas->m_niac);
			auto ritor = cell->m_dsgir.m_arcs->m_arr.find(f_inas->m_narc);

			auto r_asName = asitor->second->getCodeAsString();
			auto r_roleName = ritor->second->getCodeAsString();

			if (!role_code.has_value() || r_roleName == role_code.value())
				feature_ids.push_back(std::to_string(f_inas->m_name.RCID));
		}
	}
	return feature_ids;
}

std::vector<std::string> hd_get_spatial_associated_information_ids(std::string spatial_id, std::string association_code, std::optional<std::string> role_code)
{
	std::vector<std::string> ret;

	int findOffset = 0;
	int splitOffset = 0;
	std::string token = "|";
	while ((splitOffset = (int)spatial_id.find(token, findOffset)) != std::string::npos)
	{
		ret.push_back(spatial_id.substr(findOffset, splitOffset - findOffset));
		findOffset = splitOffset + (int)token.length();
	}
	ret.push_back(spatial_id.substr(findOffset, spatial_id.length() - findOffset));
	std::vector<std::string> information_ids1;
	if (ret[0] == "Point")
	{
		auto spatial = s_spatial_point_nodes[spatial_id];
		if (spatial && spatial->m_inas.size() > 0)
		{
			for (auto itorParent = spatial->m_inas.begin(); itorParent != spatial->m_inas.end(); itorParent++)
			{
				F_INAS* f_inas = *itorParent;

				auto asitor = cell->m_dsgir.m_iacs->m_arr.find(f_inas->m_niac);
				auto ritor = cell->m_dsgir.m_arcs->m_arr.find(f_inas->m_narc);

				auto r_asName = asitor->second->getCodeAsString();
				auto r_roleName = ritor->second->getCodeAsString();

				if (!role_code.has_value() || r_roleName == role_code.value())
					information_ids1.push_back(std::to_string(f_inas->m_name.RCID));
			}
		}
	}
	else if (ret[0] == "MultiPoint")
	{
		auto spatial = s_spatial_mpoint_nodes[spatial_id];
		if (spatial && spatial->m_inas.size() > 0)
		{
			for (auto itorParent = spatial->m_inas.begin(); itorParent != spatial->m_inas.end(); itorParent++)
			{
				F_INAS* f_inas = *itorParent;

				auto asitor = cell->m_dsgir.m_iacs->m_arr.find(f_inas->m_niac);
				auto ritor = cell->m_dsgir.m_arcs->m_arr.find(f_inas->m_narc);

				auto r_asName = asitor->second->getCodeAsString();
				auto r_roleName = ritor->second->getCodeAsString();

				if (!role_code.has_value() || r_roleName == role_code.value())
					information_ids1.push_back(std::to_string(f_inas->m_name.RCID));
			}
		}
	}
	else if (ret[0] == "Curve")
	{
		auto spatial = s_spatial_curve_nodes[spatial_id];
		if (spatial && spatial->m_inas.size() > 0)
		{
			for (auto itorParent = spatial->m_inas.begin(); itorParent != spatial->m_inas.end(); itorParent++)
			{
				F_INAS* f_inas = *itorParent;

				auto asitor = cell->m_dsgir.m_iacs->m_arr.find(f_inas->m_niac);
				auto ritor = cell->m_dsgir.m_arcs->m_arr.find(f_inas->m_narc);

				auto r_asName = asitor->second->getCodeAsString();
				auto r_roleName = ritor->second->getCodeAsString();

				if (!role_code.has_value() || r_roleName == role_code.value())
					information_ids1.push_back(std::to_string(f_inas->m_name.RCID));
			}
		}
	}

	else if (ret[0] == "CompositeCurve")
	{
		auto spatial = s_spatial_ccurve_nodes[spatial_id];

		if (spatial && spatial->m_inas.size() > 0)
		{
			for (auto itorParent = spatial->m_inas.begin(); itorParent != spatial->m_inas.end(); itorParent++)
			{
				F_INAS* f_inas = *itorParent;

				auto asitor = cell->m_dsgir.m_iacs->m_arr.find(f_inas->m_niac);
				auto ritor = cell->m_dsgir.m_arcs->m_arr.find(f_inas->m_narc);

				auto r_asName = asitor->second->getCodeAsString();
				auto r_roleName = ritor->second->getCodeAsString();

				if (!role_code.has_value() || r_roleName == role_code.value()) {
					information_ids1.push_back(std::to_string(f_inas->m_name.RCID));
				}
			}
		}
	}

	else if (ret[0] == "Surface")
	{
		auto spatial = s_spatial_surface_nodes[spatial_id];

		if (spatial && spatial->m_inas.size() > 0)
		{
			for (auto itorParent = spatial->m_inas.begin(); itorParent != spatial->m_inas.end(); itorParent++)
			{
				F_INAS* f_inas = *itorParent;

				auto asitor = cell->m_dsgir.m_iacs->m_arr.find(f_inas->m_niac);
				auto ritor = cell->m_dsgir.m_arcs->m_arr.find(f_inas->m_narc);

				auto r_asName = asitor->second->getCodeAsString();
				auto r_roleName = ritor->second->getCodeAsString();

				if (!role_code.has_value() || r_roleName == role_code.value()) {
					information_ids1.push_back(std::to_string(f_inas->m_name.RCID));
				}
			}
		}
	}
	return information_ids1;
}

std::vector<std::string> hd_get_spatial_associated_feature_ids(std::string spatial_id)
{
	std::vector<std::string> feature_ids;

	if (s_spatial_features.count(spatial_id))
		for (auto feature_id : *s_spatial_features[spatial_id])
			feature_ids.push_back(feature_id);

	return feature_ids;
}

std::vector<std::string> hd_get_spatial_ids()
{
	// to be implement
	std::vector<std::string> spatial_ids;
	return spatial_ids;
}

spatial_type hd_get_spatial_type(std::string spatial_id)
{
	std::vector<std::string> ret;

	int findOffset = 0;
	int splitOffset = 0;
	std::string token = "|";

	while ((splitOffset = (int)spatial_id.find(token, findOffset)) != std::string::npos)
	{
		ret.push_back(spatial_id.substr(findOffset, splitOffset - findOffset));
		findOffset = splitOffset + (int)token.length();
	}
	ret.push_back(spatial_id.substr(findOffset, spatial_id.length() - findOffset));
	std::string spatial_type = ret[0];

	if (spatial_type == "Point")
		return spatial_type::point;
	else if (spatial_type == "MultiPoint")
		return spatial_type::multi_point;
	else if (spatial_type == "Curve")
		return spatial_type::curve;
	else if (spatial_type == "CompositeCurve")
		return spatial_type::composite_curve;
	else if (spatial_type == "Surface")
		return spatial_type::surface;

	assert(false);
	return spatial_type::unknown;
}

static point get_point(F_C2IT *coordinate)
{
	point point;

	if (coordinate)
	{
		point =
		{
			std::to_string(coordinate->m_xcoo),
			std::to_string(coordinate->m_ycoo)
		};
	}
	else
	{
		assert(false);
	}
	return point;
}

static point get_point(C3IL *coordinate)
{
	point point;
	if (!coordinate)
	{
		return point;
	}

	double x = coordinate->m_xcoo / (double)cell->m_dsgir.m_dssi.m_cmfx;
	double y = coordinate->m_ycoo / (double)cell->m_dsgir.m_dssi.m_cmfy;
	double z = coordinate->m_zcoo / (double)cell->m_dsgir.m_dssi.m_cmfz;

	std::wstringstream sstreamVal;
	sstreamVal.setf(std::ios::fixed);
	sstreamVal.setf(std::ios::showpoint);

	sstreamVal.precision(7);
	sstreamVal << x;
	std::wstring wsx = sstreamVal.str();
	std::string sx;
	sx.assign(wsx.begin(), wsx.end());
	sstreamVal.str(L"");

	sstreamVal << y;
	std::wstring wsy = sstreamVal.str();
	std::string sy;
	sy.assign(wsy.begin(), wsy.end());
	sstreamVal.str(L"");

	sstreamVal.precision(2);
	sstreamVal << z;
	std::wstring wsz = sstreamVal.str();
	std::string sz;
	sz.assign(wsz.begin(), wsz.end());
	sstreamVal.str(L"");

	point =
	{
		sx,
		sy,
		sz
	};
	return point;
}

point hd_get_point(std::string spatial_id)
{
	auto spatial = s_spatial_point_nodes[spatial_id];
	if (spatial) {
		auto coordinate = spatial->m_c2it;
		return get_point(coordinate);
	}
	
	return point();
}

multipoint hd_get_multipoint(std::string spatial_id)
{
	multipoint multipoint;
	auto spatial = s_spatial_mpoint_nodes[spatial_id];

	if (spatial) {
		for (auto itor = spatial->m_c3il.begin(); itor != spatial->m_c3il.end(); itor++)
		{
			F_C3IL* c3il = *itor;
			for (auto itor = c3il->m_arr.begin(); itor != c3il->m_arr.end(); itor++)
			{
				C3IL* c3il = *itor;
				multipoint.points.push_back(get_point(c3il));
			}
		}
	}
	return multipoint;
}
 
curve hd_get_curve(std::string spatial_id)
{
	curve curve;

	auto spatial = s_spatial_curve_nodes[spatial_id];

	if (spatial) {
		for (auto i = spatial->m_ptas->m_arr.begin(); i != spatial->m_ptas->m_arr.end(); i++)
		{
			auto ptas = *i;
			std::string boundaryType;
			switch (ptas->m_topi)
			{
			case 1:
				boundaryType = "Begin";
				break;
			case 2:
				boundaryType = "End";
				break;
			}
			if (boundaryType == "Begin")
				curve.start_point = get_spatial_association(ptas);
			else
				curve.end_point = get_spatial_association(ptas);
		}

		for (auto itorParent = spatial->m_c2il.begin(); itorParent != spatial->m_c2il.end(); itorParent++)
		{
			F_C2IL* c2il = *itorParent;

			curve_segment curve_segment;

			curve_segment.interpolation = "Loxodromic";
			for (auto itor = c2il->m_arr.begin(); itor != c2il->m_arr.end(); itor++)
			{
				C2IL* ic2d = *itor;
				curve_segment.control_points.push_back({ std::to_string(ic2d->m_xcoo), std::to_string(ic2d->m_ycoo) });
			}

			curve.segments.push_back(curve_segment);
		}
	}
	return curve;
}

composite_curve hd_get_composite_curve(std::string spatial_id)
{
	composite_curve composite_curve;

	auto spatial = s_spatial_ccurve_nodes[spatial_id];

	if (spatial) {
		for (auto itorParent = spatial->m_cuco.begin(); itorParent != spatial->m_cuco.end(); itorParent++)
		{
			F_CUCO* cucoParent = *itorParent;

			std::string spatialType;

			for (auto itor = cucoParent->m_arr.begin(); itor != cucoParent->m_arr.end(); itor++)
			{
				CUCO* cuco = *itor;

				composite_curve.curve_associations.push_back(get_spatial_association(cuco));
			}
		}
	}
	return composite_curve;
}

surface hd_get_surface(std::string spatial_id)
{
	surface surface;

	auto spatial = s_spatial_surface_nodes[spatial_id];

	if (spatial) {
		for (auto itorParent = spatial->m_rias.begin(); itorParent != spatial->m_rias.end(); itorParent++)
		{
			F_RIAS* riasParent = *itorParent;
			for (auto itor = riasParent->m_arr.begin(); itor != riasParent->m_arr.end(); itor++)
			{
				RIAS* rias = *itor;
				std::string usage;

				switch (rias->m_usag)
				{
				case 1:
					usage = "Outer";
					break;
				case 2:
					usage = "Inner";
					break;
				}

				if (usage == "Outer")
				{
					surface.exterior_ring = get_spatial_association(rias);
				}
				else
				{
					if (!surface.interior_rings.has_value())
						surface.interior_rings = std::vector<spatial_association>();

					surface.interior_rings.value().push_back(get_spatial_association(rias));
				}
			}
		}
	}
	return surface;
}