#include "stdafx.h"
#include "ProcessS101.h"
#include "S101Layer.h"
#include "S101Cell.h"
#include "PCOutputSchemaManager.h"
#include "SENC_DisplayList.h"
#include "host_data.h"
#include "host_functions.h"

#include "../LuaScriptingReference/lua_functions.h"
#include "..\\LuaScriptingReference\\lua_session.h"
#include "..\\LuaScriptingReference\\portrayal_catalog.h"

#include "..\\FeatureCatalog\\FeatureCatalogue.h"

#include "..\\GeoMetryLibrary\\ENCCommon.h"

#include "..\\PortrayalCatalogue\\S100_ColorFill.h"
#include "..\\PortrayalCatalogue\\S100_AreaFillReference.h"
#include "..\\PortrayalCatalogue\\S100_DisplayList.h"
#include "..\\PortrayalCatalogue\\S100_AreaInstruction.h"
#include "..\\PortrayalCatalogue\\S100_TextInstruction.h"
#include "..\\PortrayalCatalogue\\S100_AlertReference.h"
#include "..\\PortrayalCatalogue\\S100_AugmentedRay.h"
#include "..\\PortrayalCatalogue\\S100_AugmentedPath.h"
#include "..\\PortrayalCatalogue\\S100_LineInstruction.h"
#include "..\\PortrayalCatalogue\\S100_PointInstruction.h"
#include "..\\PortrayalCatalogue\\S100_SymbolFill.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/ViewingGroupLayer.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

#include <sstream> 

using namespace LatLonUtility;

S101LuaScriptingReference ProcessS101::theInstance;
std::string ProcessS101::g_unknown_attribute_value = "";

wchar_t* ConvertCtoWC2(char* str)
{
	wchar_t* pStr = nullptr;
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);

	if (strSize <= 0)
	{
		OutputDebugString(_T("Failed to MultiByteToWideChar()\n"));
		return nullptr;
	}

	pStr = new WCHAR[strSize];
	MultiByteToWideChar(CP_ACP, 0, str, (int)strlen(str) + 1, pStr, strSize);
	return pStr;
}

ProcessS101::ProcessS101()
{

}

ProcessS101::~ProcessS101()
{

}

void dump_com_error(_com_error &e)
{
	printf("Error\n");
	printf("\a\t Code = %08lx\n", e.Error());
	printf("\a\t Code meaning = %ws", e.ErrorMessage());
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	printf("\a\tSource = %s\n", (LPCSTR)bstrSource);
	printf("\a\tDescription = %s\n", (LPCSTR)bstrDescription);
}

int ProcessS101::ProcessS101_LUA(std::wstring luaRulePath, S101Layer* layer)
{
	try {
		auto fc = layer->GetFeatureCatalog();
		auto pc = layer->GetPC();
		if (nullptr == fc || nullptr == pc)
		{
			return 0;
		}

		std::wstring featureCatalogPath = fc->GetFilePath();

		std::string featureCatalogPath_string(featureCatalogPath.begin(), featureCatalogPath.end());
		std::string luaRulePath_string(luaRulePath.begin(), luaRulePath.end());

		InitPortrayal(
			luaRulePath_string.c_str(),
			(S101Cell*)layer->m_spatialObject,
			fc,
			pc);

		std::string two_shades = ENCCommon::TWO_SHADES ? "true" : "false";
		std::string national_language = ENCCommon::SHOW_NOBJNM ? "eng" : "kor";
		std::string ignore_scamin = ENCCommon::APPLY_SCALE_MIN ? "true" : "false";
		std::string shallow_pattern = ENCCommon::SHALLOW_PATTERN ? "true" : "false";
		std::string simplified_points = ENCCommon::SIMPLIFIED_POINT_SYMBOL ? "true" : "false";
		std::string show_isolated_dangers_in_shallow_waters = ENCCommon::SHOW_ISOLATED_DANGER_IN_SHALLOW_WATER ? "true" : "false";
		std::string plain_boundaries = ENCCommon::SymbolizedAreaBoundary ? "false" : "true";
		std::string safety_depth = std::to_string(ENCCommon::SAFETY_DEPTH);
		std::string sallow_contour = std::to_string(ENCCommon::SHALLOW_CONTOUR);
		std::string safety_contour = std::to_string(ENCCommon::SAFETY_CONTOUR);
		std::string deep_contour = std::to_string(ENCCommon::DEEP_CONTOUR);
		std::string full_sector = ENCCommon::FULL_SECTORS ? "true" : "false";
		std::string radar_overlay = ENCCommon::DISPLAY_PLANE ? "true" : "false";

		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("SafetyDepth", safety_depth.c_str());
		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("ShallowContour", sallow_contour.c_str());
		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("SafetyContour", safety_contour.c_str());
		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("TwoShades", two_shades.c_str());
		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("DeepContour", deep_contour.c_str());
		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("ShallowPattern", shallow_pattern.c_str());
		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("ShowIsolatedDangersInShallowWaters",
			show_isolated_dangers_in_shallow_waters.c_str());
		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("PlainBoundaries", plain_boundaries.c_str());
		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("SimplifiedPoints", simplified_points.c_str());
		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("FullSectors", full_sector.c_str());
		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("RadarOverlay", radar_overlay.c_str());
		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("IgnoreScamin", ignore_scamin.c_str());
		KRS_LUA_SCRIPT_REFERENCE::PortrayalSetContextParameter("NationalLanguage", national_language.c_str());

		std::list<Result_DrawingInstruction>* drawingInstructionResult = KRS_LUA_SCRIPT_REFERENCE::GetResultDrawingInstructions();

		int t = (int)drawingInstructionResult->size();

		S101Cell* c = (S101Cell*)layer->m_spatialObject;
		c->InitDrawingInstruction();

		for (auto i = drawingInstructionResult->begin(); i != drawingInstructionResult->end(); i++)
		{
			std::string drawingInstructions = i->drawingInstructions;
			std::vector<std::string> di_splited = Split(drawingInstructions, ";");

			LUA_ParsingDrawingInstructions(i->featureID, di_splited, c->pcManager);
		}

		c->pcManager->GenerateSENCInstruction(c, layer->GetPC());

		c->pcManager->InitDisplayList();

		KRS_LUA_SCRIPT_REFERENCE::SaveDrawingInstructions("..\\TEMP\\drawingCommands.txt");
		KRS_LUA_SCRIPT_REFERENCE::RemoveResultDrawingInstructions();
	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	}
	return 0;
}

bool ProcessS101::LUA_ParsingDrawingInstructions(std::string featureID, std::vector<std::string> elements, PCOutputSchemaManager* pcm)
{
	// Visibility
	std::string v_ViewingGroup;
	std::string v_DisplayPlane;
	std::string v_DrawingPriority;
	std::string v_ScaleMinimum;
	std::string v_ScaleMaximum;

	// Transform
	std::string v_LocalOffset;
	std::string v_LinePlacement;
	std::string v_AreaPlacement;
	std::string v_AreaCRS;
	std::string v_Rotation;
	std::string v_ScaleFactor;

	// Pen Style
	std::string v_PenColor;
	std::string v_PenWidth;

	// Line Style
	std::string v_LineStyle;
	std::string v_LineSymbol;
	std::string v_Dash;

	// Text Style
	std::string v_FontColor;
	std::string v_FontSize;
	std::string v_FontProportion;
	std::string v_FontWeight;
	std::string v_FontSlant;
	std::string v_FontSerifs;
	std::string v_FontUnderline;
	std::string v_FontStrikethrough;
	std::string v_FontUpperline;
	std::string v_FontReference;
	std::string v_TextAlignHorizontal;
	std::string v_TextAlignVertical;
	std::string v_TextVerticalOffset;

	// Colour Override
	std::string v_OverrideColor;
	std::string v_OverrideAll;

	// Geometry
	std::string v_SpatialReference;
	std::string v_AugmentedPoint;
	std::string v_AugmentedRay;
	std::string v_AugmentedPath;
	std::string v_Polyline;
	std::string v_Arc3Points;
	std::string v_ArcByRadius;
	std::string v_Annulus;
	std::string v_ClearAugmented;

	// Coverage
	std::string v_LookupEntry;
	std::string v_NumericAnnotation;
	std::string v_SymbolAnnotation;
	std::string v_CoverageColor;

	std::string v_ColorFill;
	std::string v_TextInstruction;
	std::string v_LineInstruction;
	std::string v_PointInstruction;
	std::list<std::string> vl_SpatialReference;
	std::string v_AreaFillReference;
	std::string v_AlertReference;
	S100_Dash dash;

	S100_LineStyle lineStyle;

	for (auto i = elements.begin(); i != elements.end(); i++)
	{
		std::string element = *i;
		std::vector<std::string> di_splited = Split(element, ":");

		int splitedSize = (int)di_splited.size();

		if (splitedSize == 1)
		{
			// "NullInstruction"
			// "ClearGeometry"
			if (di_splited[0].compare("ClearGeometry") == 0)
			{
				v_AugmentedPoint = "";
				vl_SpatialReference.clear();
				v_AreaFillReference = "";
				v_AugmentedRay = "";
				v_AugmentedPath = "";
				v_ArcByRadius = "";
			}
		}
		else
		{
			// Splited Size 2
			std::string tag = di_splited[0];
			std::string value = di_splited[1];

			int sizeForIndex = (int)tag.size();

			if (sizeForIndex == 2)
			{
			}
			else if (sizeForIndex == 4)
			{
				// "Dash:0,3.6"
				if (tag.compare("Dash") == 0)
				{
					v_Dash = value;
					dash.ParseValue(value);
				}
				else
				{
					CString str;
					str.Format(_T("Lua Parser error - %S(%d)"), tag.c_str(), sizeForIndex);
					OutputDebugString(str);
				}
			}
			else if (sizeForIndex == 5)
			{
				if (tag.compare("Hover") == 0)
				{
				}
			}
			else if (sizeForIndex == 6)
			{
				if (tag.compare("Parent") == 0)
				{
				}
			}
			else if (sizeForIndex == 8)
			{
				// "FontSize:10"
				if (tag.compare("FontSize") == 0)
				{
					v_FontSize = value;
				}
				// "Rotation:PortrayalCRS,135"
				else if (tag.compare("Rotation") == 0)
				{
					v_Rotation = value;
				}
				else
				{
					CString str;
					str.Format(_T("Lua Parser error - %S(%d)"), tag.c_str(), sizeForIndex);
					OutputDebugString(str);
				}
			}
			else if (sizeForIndex == 7)
			{
				if (tag.compare("AreaCRS") == 0)
				{
				}
			}
			else if (sizeForIndex == 9)
			{
				// "ColorFill:DEPVS"
				if (tag.compare("ColorFill") == 0)
				{
					v_ColorFill = value;

					S100_AreaInstruction *in = new S100_AreaInstruction();
					pcm->displayList->SetDisplayInstruction((S100_Instruction*)in);

					in->GetFeatureReference()->SetReference(std::wstring(featureID.begin(), featureID.end()));
					in->SetDrawingProiority(LUA_GetPriority(v_DrawingPriority));
					in->SetDisplayPlane(std::wstring(v_DisplayPlane.begin(), v_DisplayPlane.end()));
					in->SetViewingGroup(std::wstring(v_ViewingGroup.begin(), v_ViewingGroup.end()));
					in->SetScaleMinimum(std::wstring(v_ScaleMinimum.begin(), v_ScaleMinimum.end()));

					if (v_ColorFill.size() > 0)
					{
						std::vector<std::string> v_splited = Split(v_ColorFill, ",");

						if (!in->GetAreaFill()) in->SetAreaFill(new S100_ColorFill());
						S100_ColorFill* cf = (S100_ColorFill*)in->GetAreaFill();

						if (!cf->GetColor()) cf->SetColor(new S100_Color());
						cf->GetColor()->SetToken(std::wstring(v_splited[0].begin(), v_splited[0].end()));
						cf->GetColor()->SetName(std::wstring(v_splited[0].begin(), v_splited[0].end()));

						if (v_splited.size() > 2)
						{
							cf->GetColor()->SetTransparency(std::wstring(v_splited[1].begin(), v_splited[1].end()));
						}
					}
					v_ColorFill = "";
				}
				else if (tag.compare("LineStyle") == 0)
				{
					v_LineStyle = value;
					lineStyle.ParseValue(value);

					if (false == dash.IsEmpty())
					{
						lineStyle.SetDash(&dash);
						dash.SetEmpty();
					}
				}
				else if (tag.compare("FontColor") == 0)
				{
					v_FontColor = value;
				}
				else if (tag.compare("FontSlant") == 0)
				{
					v_FontSlant = value;
				}
				else
				{
					CString str;
					str.Format(_T("Lua Parser error - %S(%d)"), tag.c_str(), sizeForIndex);
					OutputDebugString(str);
				}
			}
			else if (sizeForIndex == 11)
			{
				// "LocalOffset:-3.51,3.51"
				if (tag.compare("LocalOffset") == 0)
				{
					v_LocalOffset = value;
				}
				// "ScaleFactor:0.311"
				else if (tag.compare("ScaleFactor") == 0)
				{
					v_ScaleFactor = value;
				}
				// "ArcByRadius:0,0,20,24,199"
				else if (tag.compare("ArcByRadius") == 0)
				{
					v_ArcByRadius = value;
				}
				else
				{
					CString str;
					str.Format(_T("Lua Parser error - %S(%d)"), tag.c_str(), sizeForIndex);
					OutputDebugString(str);
				}
			}
			else if (sizeForIndex == 12)
			{
				// "ScaleMinimum:179999"
				if (tag.compare("ScaleMinimum") == 0)
				{
					v_ScaleMinimum = value;
				}
				// "ViewingGroup:27020"
				else if (tag.compare("ViewingGroup") == 0)
				{
					v_ViewingGroup = value;
				}
				// "DisplayPlane:OverRADAR"
				else if (tag.compare("DisplayPlane") == 0)
				{
					v_DisplayPlane = value;
				}
				// "AugmentedRay:GeographicCRS,24,GeographicCRS,16668"
				else if (tag.compare("AugmentedRay") == 0)
				{
					v_AugmentedRay = value;

					v_AugmentedPath = "";
				}
				else
				{
					CString str;
					str.Format(_T("Lua Parser error - %S(%d)"), tag.c_str(), sizeForIndex);
					OutputDebugString(str);
				}
			}
			else if (sizeForIndex == 13)
			{
				if (tag.compare("LinePlacement") == 0)
				{
					v_LinePlacement = value;
				}
				else if (tag.compare("AugmentedPath") == 0)
				{
					v_AugmentedPath = value;
					v_AugmentedRay = "";
				}
				else if (tag.compare("AreaPlacement") == 0)
				{
				}
				else
				{
					CString str;
					str.Format(_T("Lua Parser error - %S(%d)"), tag.c_str(), sizeForIndex);
					OutputDebugString(str);
				}
			}
			else if (sizeForIndex == 14)
			{
				if (tag.compare("AugmentedPoint") == 0)
				{
					v_AugmentedPoint = value;
				}
				else if (tag.compare("AlertReference") == 0)
				{
					v_AlertReference = value;

					S100_AlertReference* in = new S100_AlertReference();
					pcm->displayList->AddAlertInstruction((S100_Instruction*)in);

					in->GetFeatureReference()->SetReference(std::wstring(featureID.begin(), featureID.end()));

					std::vector<std::string> v_splited = Split(v_AlertReference, ",");
					if (v_splited.size() > 0)
					{
						in->alertType = std::wstring(v_splited[0].begin(), v_splited[0].end());
					}
					if (v_splited.size() > 1)
					{
						in->plan = std::wstring(v_splited[1].begin(), v_splited[1].end());
					}
					if (v_splited.size() > 2)
					{
						in->monitor = std::wstring(v_splited[2].begin(), v_splited[2].end());
					}

					v_AlertReference = "";
				}
				else
				{
					CString str;
					str.Format(_T("Lua Parser error - %S(%d)"), tag.c_str(), sizeForIndex);
					OutputDebugString(str);
				}
			}
			else if (sizeForIndex == 15)
			{
				// "DrawingPriority:8"
				if (tag.compare("DrawingPriority") == 0)
				{
					v_DrawingPriority = value;
				}
				// "TextInstruction:bn Alligator River Light 16,21,8"
				else if (tag.compare("TextInstruction") == 0)
				{
					v_TextInstruction = value;
					S100_TextInstruction *in = new S100_TextInstruction();
					pcm->displayList->SetDisplayInstruction((S100_Instruction*)in);

					in->GetFeatureReference()->SetReference(std::wstring(featureID.begin(), featureID.end()));
					in->SetDrawingProiority(LUA_GetPriority(v_DrawingPriority));
					in->SetDisplayPlane(std::wstring(v_DisplayPlane.begin(), v_DisplayPlane.end()));
					in->SetViewingGroup(std::wstring(v_ViewingGroup.begin(), v_ViewingGroup.end()));
					in->SetScaleMinimum(std::wstring(v_ScaleMinimum.begin(), v_ScaleMinimum.end()));

					in->SetTextPoint(new S100_TextPoint());

					if (v_TextAlignVertical.size() > 0)
					{
						in->GetTextPoint()->SetVerticalAlignment(std::wstring(v_TextAlignVertical.begin(), v_TextAlignVertical.end()));
					}
					if (v_TextAlignHorizontal.size() > 0)
					{
						in->GetTextPoint()->SetHorizontalAlignment(std::wstring(v_TextAlignHorizontal.begin(), v_TextAlignHorizontal.end()));
					}

					if (v_LocalOffset.size() > 0)
					{
						in->GetTextPoint()->SetOffset(new S100_VectorPoint());

						std::vector<std::string> v_splited = Split(v_LocalOffset, ",");
						if (v_splited.size() == 2)
						{
							in->GetTextPoint()->GetOffset()->SetX(std::wstring(v_splited[0].begin(), v_splited[0].end()));
							in->GetTextPoint()->GetOffset()->SetY(std::wstring(v_splited[1].begin(), v_splited[1].end()));
						}
						else
						{
							OutputDebugString(L"Error : Offset Value should have 2 arguments.");
						}

						if (v_TextInstruction.size() > 0)
						{
							S100_Element *element = new S100_Element();

							in->GetTextPoint()->SetElement(element);

							if (!element->GetText()) element->SetText(new S100_Text());

							std::vector<std::string> v_splited_text = Split(v_TextInstruction, ",");

							auto wValue = ConvertCtoWC2((char*)v_splited_text[0].c_str());
							std::wstring wstrValue = wValue;
							delete[] wValue;
							element->GetText()->SetValue(wstrValue);

							if (v_FontSize.size() > 0)
							{
								element->SetBodySize(std::wstring(v_FontSize.begin(), v_FontSize.end()));
							}
							if (v_FontSize.size() > 0)
							{
								if (!element->GetFont()) element->SetFont(new S100_Font());
								element->GetFont()->SetSlant(std::wstring(v_FontSlant.begin(), v_FontSlant.end()));
							}
						}
					}
					v_TextInstruction = "";
				}
				// "LineInstruction:QUESMRK1"	
				else if (tag.compare("LineInstruction") == 0)
				{
					v_LineInstruction = value;

					if (v_AugmentedRay.size() > 0)
					{
						S100_AugmentedRay *in = new S100_AugmentedRay();
						pcm->displayList->SetDisplayInstruction((S100_Instruction*)in);

						in->GetFeatureReference()->SetReference(std::wstring(featureID.begin(), featureID.end()));
						in->SetDrawingProiority(LUA_GetPriority(v_DrawingPriority));
						in->SetDisplayPlane(std::wstring(v_DisplayPlane.begin(), v_DisplayPlane.end()));
						in->SetViewingGroup(std::wstring(v_ViewingGroup.begin(), v_ViewingGroup.end()));
						in->SetScaleMinimum(std::wstring(v_ScaleMinimum.begin(), v_ScaleMinimum.end()));

						if (false == lineStyle.IsEmpty())
						{
							in->SetLineStyle(new S100_LineStyle(lineStyle));
						}

						if (v_AugmentedRay.size() > 0)
						{
							std::vector<std::string> v_splited = Split(v_AugmentedRay, ",");
							if (v_splited.size() == 4)
							{
								in->SetDirection(std::wstring(v_splited[1].begin(), v_splited[1].end()));
								in->SetLength(std::wstring(v_splited[3].begin(), v_splited[3].end()));
							}
						}
					}
					else if (v_AugmentedPath.size() > 0)
					{
						S100_AugmentedPath *in = new S100_AugmentedPath();
						pcm->displayList->SetDisplayInstruction((S100_Instruction*)in);

						in->GetFeatureReference()->SetReference(std::wstring(featureID.begin(), featureID.end()));
						in->SetDrawingProiority(LUA_GetPriority(v_DrawingPriority));
						in->SetDisplayPlane(std::wstring(v_DisplayPlane.begin(), v_DisplayPlane.end()));
						in->SetViewingGroup(std::wstring(v_ViewingGroup.begin(), v_ViewingGroup.end()));
						in->SetScaleMinimum(std::wstring(v_ScaleMinimum.begin(), v_ScaleMinimum.end()));

						if (false == lineStyle.IsEmpty())
						{
							in->SetLineStyle(new S100_LineStyle(lineStyle));
						}

						if (v_ArcByRadius.size() > 0)
						{
							std::vector<std::string> v_splited = Split(v_ArcByRadius, ",");
							if (v_splited.size() == 5)
							{

								if (!in->GetPath()) in->SetPath(new S100_Path());

								S100_ArcByRadius arcByRadius;

								arcByRadius.GetCenter()->SetX(std::wstring(v_splited[0].begin(), v_splited[0].end()));
								arcByRadius.GetCenter()->SetY(std::wstring(v_splited[1].begin(), v_splited[1].end()));
								arcByRadius.SetRadius(std::wstring(v_splited[2].begin(), v_splited[2].end()));

								if (arcByRadius.GetSector() == nullptr)arcByRadius.SetSector(new S100_Sector());
								arcByRadius.GetSector()->SetStartAngle(std::wstring(v_splited[3].begin(), v_splited[3].end()));
								arcByRadius.GetSector()->SetAnglearDistance(std::wstring(v_splited[4].begin(), v_splited[4].end()));


								in->GetPath()->SetArcByRadiuses(&arcByRadius);
							}
						}
					}
					else
					{
						S100_LineInstruction *in = new S100_LineInstruction();
						pcm->displayList->SetDisplayInstruction((S100_Instruction*)in);

						in->GetFeatureReference()->SetReference(std::wstring(featureID.begin(), featureID.end()));
						in->SetDrawingProiority(LUA_GetPriority(v_DrawingPriority));
						in->SetDisplayPlane(std::wstring(v_DisplayPlane.begin(), v_DisplayPlane.end()));
						in->SetViewingGroup(std::wstring(v_ViewingGroup.begin(), v_ViewingGroup.end()));
						in->SetScaleMinimum(std::wstring(v_ScaleMinimum.begin(), v_ScaleMinimum.end()));

						if (v_LineInstruction.size() > 0)
						{
							if (!in->GetLineStyleReference())
							{
								in->SetLineStyleReference(new S100_LineStyleReference());
								in->GetLineStyleReference()->SetReference(std::wstring(v_LineInstruction.begin(), v_LineInstruction.end()));
							}
						}

						if (false == lineStyle.IsEmpty())
						{
							in->SetLineStyle(new S100_LineStyle(lineStyle));
						}

						if (vl_SpatialReference.size() > 0)
						{
							for (auto it = vl_SpatialReference.begin(); it != vl_SpatialReference.end(); it++)
							{
								S100_SpatialReference* sref = new S100_SpatialReference();
								in->SetSpatialReference(sref);
								std::string v_SpatialReference = *it;

								std::vector<std::string> v_splited = Split(v_SpatialReference, "|");
								if (v_splited.size() == 2)
								{
									sref->SetType(v_splited[0]);
									sref->SetReference(v_splited[1]);
								}
							}
							v_LineInstruction = "";
						}
					}
				}
			}
			else if (sizeForIndex == 16)
			{
				// PointInstruction
				if (tag.compare("PointInstruction") == 0)
				{
					v_PointInstruction = value;
					S100_PointInstruction *in = new S100_PointInstruction();
					pcm->displayList->SetDisplayInstruction((S100_Instruction*)in);

					in->GetFeatureReference()->SetReference(std::wstring(featureID.begin(), featureID.end()));
					in->SetDrawingProiority(LUA_GetPriority(v_DrawingPriority));
					in->SetDisplayPlane(std::wstring(v_DisplayPlane.begin(), v_DisplayPlane.end()));
					in->SetViewingGroup(std::wstring(v_ViewingGroup.begin(), v_ViewingGroup.end()));
					in->SetScaleMinimum(std::wstring(v_ScaleMinimum.begin(), v_ScaleMinimum.end()));

					if (v_PointInstruction.size() > 0)
					{
						in->SetSymbol(new S100_Symbol());
						in->GetSymbol()->SetReference(std::wstring(v_PointInstruction.begin(), v_PointInstruction.end()));

						std::vector<std::string> r_splited = Split(v_Rotation, ",");
						if (r_splited.size() == 2)
						{
							in->GetSymbol()->SetRotation(std::stod(r_splited[1]));

						}
					}

					if (v_AugmentedPoint.size() > 1)
					{
						std::vector<std::string> v_splited = Split(v_AugmentedPoint, ",");
						if (v_splited.size() == 3)
						{
							if (!in->GetVectorPoint()) in->SetVectorPoint(new S100_VectorPoint());

							in->GetVectorPoint()->SetX(std::wstring(v_splited[1].begin(), v_splited[1].end()));
							in->GetVectorPoint()->SetY(std::wstring(v_splited[2].begin(), v_splited[2].end()));
						}
						else
						{
							OutputDebugString(L"Error : Vector Point Value should have 3 arguments.");
						}
					}

					if (vl_SpatialReference.size() > 0)
					{
						for (auto it = vl_SpatialReference.begin(); it != vl_SpatialReference.end(); it++)
						{
							S100_SpatialReference* sref = new S100_SpatialReference();
							in->SetSpatialReference(sref);
							std::string v_SpatialReference = *it;

							std::vector<std::string> v_splited = Split(v_SpatialReference, "|");
							if (v_splited.size() == 2)
							{
								sref->SetType(v_splited[0]);
								sref->SetReference(v_splited[1]);
							}
						}
					}

					v_PointInstruction = "";
				}
				// "SpatialReference:Curve|107" 
				else if (tag.compare("SpatialReference") == 0)
				{
					std::string v_SpatialReference = value;
					vl_SpatialReference.push_back(v_SpatialReference);
				}
				else
				{
					CString str;
					str.Format(_T("Lua Parser error - %S(%d)"), tag.c_str(), sizeForIndex);
					OutputDebugString(str);
				}
			}
			else if (sizeForIndex == 17)
			{
				// "TextAlignVertical:Center"
				if (tag.compare("TextAlignVertical") == 0)
				{
					v_TextAlignVertical = value;
				}
				// "AreaFillReference:MARSHES1"
				else if (tag.compare("AreaFillReference") == 0)
				{
					v_AreaFillReference = value;

					S100_AreaInstruction *in = new S100_AreaInstruction();
					pcm->displayList->SetDisplayInstruction((S100_Instruction*)in);

					in->GetFeatureReference()->SetReference(std::wstring(featureID.begin(), featureID.end()));
					in->SetDrawingProiority(LUA_GetPriority(v_DrawingPriority));
					in->SetDisplayPlane(std::wstring(v_DisplayPlane.begin(), v_DisplayPlane.end()));
					in->SetViewingGroup(std::wstring(v_ViewingGroup.begin(), v_ViewingGroup.end()));
					in->SetScaleMinimum(std::wstring(v_ScaleMinimum.begin(), v_ScaleMinimum.end()));

					if (v_AreaFillReference.size() > 0)
					{
						if (!in->GetAreaFill()) in->SetAreaFill(new S100_AreaFillReference());
						S100_AreaFillReference* cf = (S100_AreaFillReference*)in->GetAreaFill();

						cf->SetReference(std::wstring(v_AreaFillReference.begin(), v_AreaFillReference.end()));

						std::wstring path = cf->GetReference();
						path.append(L".xml");
						S100_SymbolFill* tempAreaFill = new S100_SymbolFill();
						tempAreaFill->SetFileTitle(cf->GetReference());

						//여기를 pugi로 변경하면 심볼이 나오지않습니다
						if (!tempAreaFill->ReadFileByPugiXml(path))
						{
							delete tempAreaFill;
						}
						else
						{
							delete (S100_AreaFillReference*)cf;

							in->SetAreaFill(tempAreaFill);
						}
					}
					v_AreaFillReference = "";
				}
				else
				{
					CString str;
					str.Format(_T("Lua Parser error - %S(%d)"), tag.c_str(), sizeForIndex);
					OutputDebugString(str);
				}
			}
			else if (sizeForIndex == 19)
			{
				// "TextAlignHorizontal:End"
				if (tag.compare("TextAlignHorizontal") == 0)
				{
					v_TextAlignHorizontal = value;
				}
				else
				{
					CString str;
					str.Format(_T("Lua Parser error - %S(%d)"), tag, sizeForIndex);
					OutputDebugString(str);
				}
			}
			else
			{
				CString str;
				str.Format(_T("Lua Parser error - %S(%d)"), tag.c_str(), sizeForIndex);
				OutputDebugString(str);
			}
		}
	}
	return true;
}

std::wstring ProcessS101::LUA_GetPriority(std::string lua_priority)
{
	return std::wstring(lua_priority.begin(), lua_priority.end());
}

void ProcessS101::InitPortrayal(const char* topLevelRule, S101Cell* cell, FeatureCatalogue* fc, PortrayalCatalogue* pc)
{
	if (theInstance.m_lua_session)
	{
		delete theInstance.m_lua_session;
	}

	if (cell)
	{
		theInstance.m_s101_cell = cell;
	}

	if (fc)
	{
		theInstance.m_s101_feature_catalogue = fc;
	}

	theInstance.m_lua_session = NULL;

	std::string top_level_rule(topLevelRule);
	pTheFC = fc;

	hd_init(cell);

	pc_init("..\\ProgramData\\S101_Portrayal\\portrayal_catalogue.xml");

	//
	// //Initialize Lua library
	//

	theInstance.m_lua_session = new lua_session();

	//
	// Register host functions.
	//
	theInstance.m_lua_session->register_function("HostGetFeatureIDs", HostGetFeatureIDs);
	theInstance.m_lua_session->register_function("HostFeatureGetCode", HostFeatureGetCode);
	theInstance.m_lua_session->register_function("HostGetInformationTypeIDs", HostGetInformationTypeIDs);
	theInstance.m_lua_session->register_function("HostInformationTypeGetCode", HostInformationTypeGetCode);
	theInstance.m_lua_session->register_function("HostFeatureGetSimpleAttribute", HostFeatureGetSimpleAttribute);
	theInstance.m_lua_session->register_function("HostFeatureGetComplexAttributeCount", HostFeatureGetComplexAttributeCount);
	theInstance.m_lua_session->register_function("HostFeatureGetSpatialAssociations", HostFeatureGetSpatialAssociations);
	theInstance.m_lua_session->register_function("HostFeatureGetAssociatedFeatureIDs", HostFeatureGetAssociatedFeatureIDs);
	theInstance.m_lua_session->register_function("HostFeatureGetAssociatedInformationIDs", HostFeatureGetAssociatedInformationIDs);
	theInstance.m_lua_session->register_function("HostGetSpatialIDs", HostGetSpatialIDs);
	theInstance.m_lua_session->register_function("HostGetSpatial", HostGetSpatial);
	theInstance.m_lua_session->register_function("HostSpatialGetAssociatedInformationIDs", HostSpatialGetAssociatedInformationIDs);
	theInstance.m_lua_session->register_function("HostSpatialGetAssociatedFeatureIDs", HostSpatialGetAssociatedFeatureIDs);
	theInstance.m_lua_session->register_function("HostInformationTypeGetSimpleAttribute", HostInformationTypeGetSimpleAttribute);
	theInstance.m_lua_session->register_function("HostInformationTypeGetComplexAttributeCount", HostInformationTypeGetComplexAttributeCount);

	theInstance.m_lua_session->register_function("HostGetFeatureTypeCodes", HostGetFeatureTypeCodes);
	theInstance.m_lua_session->register_function("HostGetInformationTypeCodes", HostGetInformationTypeCodes);
	theInstance.m_lua_session->register_function("HostGetSimpleAttributeTypeCodes", HostGetSimpleAttributeTypeCodes);
	theInstance.m_lua_session->register_function("HostGetComplexAttributeTypeCodes", HostGetComplexAttributeTypeCodes);
	theInstance.m_lua_session->register_function("HostGetRoleTypeCodes", HostGetRoleTypeCodes);
	theInstance.m_lua_session->register_function("HostGetInformationAssociationTypeCodes", HostGetInformationAssociationTypeCodes);
	theInstance.m_lua_session->register_function("HostGetFeatureAssociationTypeCodes", HostGetFeatureAssociationTypeCodes);
	theInstance.m_lua_session->register_function("HostGetFeatureTypeInfo", HostGetFeatureTypeInfo);
	theInstance.m_lua_session->register_function("HostGetInformationTypeInfo", HostGetInformationTypeInfo);
	theInstance.m_lua_session->register_function("HostGetSimpleAttributeTypeInfo", HostGetSimpleAttributeTypeInfo);
	theInstance.m_lua_session->register_function("HostGetComplexAttributeTypeInfo", HostGetComplexAttributeTypeInfo);

	theInstance.m_lua_session->register_function("HostSpatialRelate", HostSpatialRelate);

	theInstance.m_lua_session->register_function("HostDebuggerEntry", HostDebuggerEntry);

	theInstance.m_lua_session->register_function("HostPortrayalEmit", HostPortrayalEmit);

	//
	// Extract rules path from top level rule file name
	//
	std::string rules_path = top_level_rule.substr(0, top_level_rule.find_last_of('\\'));

	//
	// Setup path so Lua can find the rules files
	//
	std::string path = rules_path + (std::string)"\\?.lua";

	theInstance.m_lua_session->set_field("package", "path", path.c_str());

	//
	// Load top level rule file
	//
	theInstance.m_lua_session->load_file(top_level_rule);

	//
	// Get unknown attribute value string
	//
	g_unknown_attribute_value = theInstance.m_lua_session->call<std::string>("GetUnknownAttributeString", { 1.0 });

	//
	// Turn on type system checks
	//
	theInstance.m_lua_session->call<bool>("TypeSystemChecks", { true });

	//
	// Initialize portrayal context parameters
	//
	theInstance.m_lua_session->get_function("PortrayalInitializeContextParameters");

	auto cps = pc_get_context_parameters();

	std::vector<lua_variant> cpts;

	for (auto cp : cps)
		cpts.push_back(theInstance.m_lua_session->call("PortrayalCreateContextParameter", { cp.name, cp.type, cp.default_value }));

	theInstance.m_lua_session->push(cpts);

	theInstance.m_lua_session->call_raw(1);

	pc_delete();
}

void ProcessS101::PortrayalSetContextParameter(const char*  parameterName, const char*  parameterValue)
{
	theInstance.m_lua_session->call("PortrayalSetContextParameter", { parameterName, parameterValue });
}