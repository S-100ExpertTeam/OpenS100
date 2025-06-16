#include "stdafx.h"

#include "PCOutputSchemaManager.h"
#include "S101Cell.h"

#include "R_FeatureRecord.h"
#include "GISLibrary.h"
#include "S100Layer.h"
#include "SENC_CommonFuc.h"
#include "SENC_LineStyle.h"
#include "SENC_ColorFill.h"
#include "SENC_AreaFillReference.h"
#include "SENC_SymbolFill.h"
#include "SENC_PixmapFill.h"
#include "SENC_HatchFill.h"
#include "SENC_SpatialReference.h"
#include "SENC_DisplayList.h"
#include "SENC_Instruction.h"
#include "SENC_NullInstruction.h"
#include "SENC_PointInstruction.h"
#include "SENC_LineInstruction.h"
#include "SENC_AreaInstruction.h"
#include "SENC_TextInstruction.h"
#include "SENC_AugmentedRay.h"
#include "SENC_AugmentedPath.h"
#include "SENC_AlertReference.h"
#include "SENC_ArcByRadius.h"
#include "SPoint.h"
#include "SCoverage.h"
#include "StringUtil.hpp"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/DisplayFactory.h"

#include "../GeoMetryLibrary/GeoCommonFuc.h"

#include <mmsystem.h> 

PCOutputSchemaManager::PCOutputSchemaManager()
{
	displayList = CDisplayFactory::createDisplayList();
	displayListSENC = new SENC_DisplayList();
}

PCOutputSchemaManager::~PCOutputSchemaManager()
{
	CDisplayFactory::destroyDisplayList(displayList);
	displayList = nullptr;

	delete displayListSENC;
	displayListSENC = nullptr;
}

void PCOutputSchemaManager::InitDisplayList()
{
	CDisplayFactory::destroyDisplayList(displayList);
	displayList = nullptr;
}

void PCOutputSchemaManager::GenerateSENCInstruction(S100SpatialObject* s100so, PortrayalCatalogue* pc)
{
	if (!displayListSENC)
		displayListSENC = new SENC_DisplayList();

	for (const auto& iter : displayList->GetDisplayInstructions())
	{
		SENC_Instruction* sit = SENC_Instruction::S1002SENC(iter, pc, this, s100so);
		if (!sit)
			continue;

		auto featureID = pugi::as_utf8(iter->GetFeatureReference());
		auto featureType = s100so->GetFeatureType(featureID);
		if (!featureType)
			continue;

		displayListSENC->AddInstruction(sit);
		sit->fr = featureType;
	}
}

void PCOutputSchemaManager::ParsingSENCInstruction(S100Layer* layer, std::string featureID, std::vector<std::string> elements)
{
	if (!displayListSENC)
		displayListSENC = new SENC_DisplayList();

	if ((!layer) ||
		(elements.empty()))
		return;

	S101Cell* enc = (S101Cell*)layer->m_spatialObject;
	PortrayalCatalogue* pc = layer->GetPC();

	if ((!enc) ||
		(!pc))
		return;

	auto featureType = enc->GetFeatureType(featureID);
	if (!featureType)
		return;

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
		std::string v_FontSize = "10";
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

		std::list<std::string> vl_SpatialReference;

		for (auto& element : elements)
		{
			std::vector<std::string> di_splited = split(element, ':');

			if ((int)di_splited.size() == 0)
				continue;
			if ((int)di_splited.size() == 1)
			{
				// "NullInstruction"
				// "ClearGeometry"
				if (di_splited[0].compare("ClearGeometry") == 0)
				{
					v_AugmentedPoint = "";
					vl_SpatialReference.clear();
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

				switch (tag.size())
				{
				case 4:
				{
					if (tag == "Dash")
						v_Dash = value;
				}
				break;
				case 8:
				{
					// "FontSize:10"
					if (tag == "FontSize")
						v_FontSize = value;
					// "Rotation:PortrayalCRS,135"
					else if (tag == "Rotation")
						v_Rotation = value;
				}
				break;
				case 9:
				{
					// "ColorFill:DEPVS"
					if (tag == "ColorFill")
					{
						SENC_AreaInstruction* sit = new SENC_AreaInstruction();

						GetSENCFromString(sit, featureID, v_DrawingPriority, v_DisplayPlane, v_ViewingGroup, v_ScaleMinimum, v_ScaleMaximum, vl_SpatialReference);

						sit->areaFill = new SENC_ColorFill();

						std::vector<std::string> vColorFill = split(value, ',');
						if (vColorFill.size() > 0)
						{
							SENC_Color* color = new SENC_Color();

							color = new SENC_Color();
							color->SetToken(string2wstring(vColorFill[0]));
							if (vColorFill.size() > 1)
								color->SetTransparency(string2float(vColorFill[1]));

							auto colorProfile = pc->GetS100PCManager()->GetS100ColorProfile();
							if (colorProfile)
								color->RGBColor = colorProfile->GetColor(string2wstring(vColorFill[0]));

							((SENC_ColorFill*)sit->areaFill)->color = color;
						}
						vColorFill.clear();

						sit->fr = featureType;

						displayListSENC->AddInstruction(sit);
					}
					else if (tag == "LineStyle")
						v_LineStyle = value;
					else if (tag == "FontColor")
						v_FontColor = value;
					else if (tag == "FontSlant")
						v_FontSlant = value;
				}
				break;
				case 11:
				{
					// "LocalOffset:-3.51,3.51"
					if (tag.compare("LocalOffset") == 0)
						v_LocalOffset = value;
					// "ScaleFactor:0.311"
					else if (tag.compare("ScaleFactor") == 0)
						v_ScaleFactor = value;
					// "ArcByRadius:0,0,20,24,199"
					else if (tag.compare("ArcByRadius") == 0)
						v_ArcByRadius = value;
				}
				break;
				case 12:
				{
					// "ScaleMinimum:179999"
					if (tag.compare("ScaleMinimum") == 0)
						v_ScaleMinimum = value;
					if (tag.compare("ScaleMaximum") == 0)
						v_ScaleMaximum = value;
					// "ViewingGroup:27020"
					else if (tag.compare("ViewingGroup") == 0)
						v_ViewingGroup = value;
					// "DisplayPlane:OverRADAR"
					else if (tag.compare("DisplayPlane") == 0)
						v_DisplayPlane = value;
					// "AugmentedRay:GeographicCRS,24,GeographicCRS,16668"
					else if (tag.compare("AugmentedRay") == 0)
					{
						v_AugmentedRay = value;
						v_AugmentedPath = "";
					}
				}
				break;
				case 13:
				{
					if (tag.compare("LinePlacement") == 0)
						v_LinePlacement = value;
					else if (tag.compare("AugmentedPath") == 0)
					{
						v_AugmentedPath = value;
						v_AugmentedRay = "";
					}
				}
				break;
				case 14:
				{
					if (tag.compare("AugmentedPoint") == 0)
						v_AugmentedPoint = value;
				}
				break;
				case 15:
				{
					// "DrawingPriority:8"
					if (tag.compare("DrawingPriority") == 0)
						v_DrawingPriority = value;
					// "TextInstruction:bn Alligator River Light 16,21,8"
					else if (tag.compare("TextInstruction") == 0)
					{
						SENC_TextInstruction* sit = new SENC_TextInstruction();

						GetSENCFromString(sit, featureID, v_DrawingPriority, v_DisplayPlane, v_ViewingGroup, v_ScaleMinimum, v_ScaleMaximum, vl_SpatialReference);

						sit->textPoint = new SENC_TextPoint();
						sit->textPoint->horizontalAlignment = SENC_CommonFuc::GetHorizontalAlignment(string2wstring(v_TextAlignHorizontal));
						sit->textPoint->verticalAlignment = SENC_CommonFuc::GetVerticalAlignment(string2wstring(v_TextAlignVertical));
						std::vector<std::string> vecLocalOffset = split(v_LocalOffset, ',');
						if (vecLocalOffset.size() == 2)
						{
							sit->textPoint->offset.x = string2double(vecLocalOffset[0]);
							sit->textPoint->offset.y = string2double(vecLocalOffset[1]);
						}
						vecLocalOffset.clear();

						std::vector<std::string> vText = split(value, ',');
						if (!vText.empty())
						{
							SENC_Element* element = new SENC_Element();
							sit->textPoint->elements.push_back(element);

							element->bodySize = string2int(v_FontSize);
							std::vector<std::string> vFontColor = split(v_FontColor, ',');
							if (vFontColor.size() > 0)
							{
								element->foreground.token = string2wstring(vFontColor[0]);
								if (vFontColor.size() > 1)
									element->foreground.transparency = string2int(vFontColor[1]) * 100;

								element->pColor = new D2D1_COLOR_F();
								element->pColor->a = 255;
								element->pColor->r = 0;
								element->pColor->g = 0;
								element->pColor->b = 0;

								auto colorProfile = pc->GetS100PCManager()->GetS100ColorProfile();
								if (colorProfile)
								{
									auto color = colorProfile->GetColor(element->foreground.token);
									if (colorProfile->currentPalette->IsItem(element->foreground.token))
									{
										auto item = colorProfile->currentPalette->GetItem(element->foreground.token);
										if (item)
										{
											element->pColor->r = (FLOAT)(item->srgb.red / 255.0);
											element->pColor->g = (FLOAT)(item->srgb.green / 255.0);
											element->pColor->b = (FLOAT)(item->srgb.blue / 255.0);
										}
									}
								}
							}
							vFontColor.clear();

							element->text.value = string2wstring(vText[0]);
						}	
						vText.clear();

						sit->fr = featureType;

						displayListSENC->AddInstruction(sit);
					}
					else if (tag.compare("LineInstruction") == 0)
					{
						if (!v_AugmentedRay.empty())
						{
							SENC_AugmentedRay* sit = new SENC_AugmentedRay();

							GetSENCFromString(sit, featureID, v_DrawingPriority, v_DisplayPlane, v_ViewingGroup, v_ScaleMinimum, v_ScaleMaximum, vl_SpatialReference);

							std::vector<std::string> vAugmentedRay = split(v_AugmentedRay, ',');
							if (vAugmentedRay.size() == 4)
							{
								sit->direction = string2double(vAugmentedRay[1]);
								sit->length = string2double(vAugmentedRay[3]);
							}
							vAugmentedRay.clear();

							sit->areaCRSType = 0;

							if (!v_LineStyle.empty())
							{
								SENC_LineStyle* ls = new SENC_LineStyle();
								ls->GetStyleFromString(v_LineStyle, v_Dash, pc);
								sit->lineStyles.push_back(ls);
							}

							sit->fr = featureType;

							displayListSENC->AddInstruction(sit);
						}
						else if (v_AugmentedPath.size() > 0)
						{
							SENC_AugmentedPath* sit = new SENC_AugmentedPath();

							GetSENCFromString(sit, featureID, v_DrawingPriority, v_DisplayPlane, v_ViewingGroup, v_ScaleMinimum, v_ScaleMaximum, vl_SpatialReference);

							std::vector<std::string> vArcByRadius = split(v_ArcByRadius, ',');
							if (vArcByRadius.size() == 5)
							{
								sit->path = new SENC_Path();

								SENC_ArcByRadius* sp = new SENC_ArcByRadius();
								sp->center.x = string2double(vArcByRadius[0]);
								sp->center.y = string2double(vArcByRadius[1]);
								sp->radius = string2float(vArcByRadius[2]);
								sp->sector = new SENC_Sector();
								sp->sector->angularDistance = string2float(vArcByRadius[3]);
								sp->sector->startAngle = string2float(vArcByRadius[4]);
								sit->path->arcByRadiuses.push_back(sp);
							}
							vArcByRadius.clear();

							if (!v_LineStyle.empty())
							{
								SENC_LineStyle* ls = new SENC_LineStyle();
								ls->GetStyleFromString(v_LineStyle, v_Dash, pc);
								sit->lineStyles.push_back(ls);
							}

							sit->fr = featureType;

							displayListSENC->AddInstruction(sit);
						}
						else
						{
							SENC_LineInstruction* sit = new SENC_LineInstruction();

							GetSENCFromString(sit, featureID, v_DrawingPriority, v_DisplayPlane, v_ViewingGroup, v_ScaleMinimum, v_ScaleMaximum, vl_SpatialReference);

							std::wstring reference = string2wstring(value);
							std::unordered_map<std::wstring, S100_LineStyleBase*> lineStyles = pc->GetLineStyles();
							auto lineStyle = lineStyles.find(reference);
							if (lineStyle != lineStyles.end())
							{
								S100_LineStyleBase* base = lineStyle->second;
								if (base->GetType() == 1)
								{
									SENC_LineStyle* ls = new SENC_LineStyle();
									ls->SetLineStyleName(reference);
									if (lineStyle != lineStyles.end())
									{
										ls->GetStyleFromS100((S100_LineStyle*)lineStyle->second, pc);
										sit->lineStyles.push_back(ls);
									}
								}
								else if (base->GetType() == 3)
								{
									S100_CompositeLineStyle* cls = (S100_CompositeLineStyle*)base;
									for (const auto& iter : cls->GetLineStyles())
									{
										SENC_LineStyle* ls = new SENC_LineStyle();
										ls->GetStyleFromS100(iter, pc);
										sit->lineStyles.push_back(ls);
									}
								}
							}

							if (!v_LineStyle.empty())
							{
								SENC_LineStyle* ls = new SENC_LineStyle();
								ls->GetStyleFromString(v_LineStyle, v_Dash, pc);
								sit->lineStyles.push_back(ls);
							}

							sit->fr = featureType;

							displayListSENC->AddInstruction(sit);
						}
					}
				}
				break;
				case 16:
				{
					// PointInstruction
					if (tag.compare("PointInstruction") == 0)
					{
						SENC_PointInstruction* sit = new SENC_PointInstruction();

						GetSENCFromString(sit, featureID, v_DrawingPriority, v_DisplayPlane, v_ViewingGroup, v_ScaleMinimum, v_ScaleMaximum, vl_SpatialReference);

						sit->symbol = new SENC_Symbol();
						sit->symbol->reference = string2wstring(value);
						std::vector<std::string> vRotation = split(v_Rotation, ',');
						if (vRotation.size() > 2)
							sit->symbol->rotation = string2double(vRotation[1]);
						vRotation.clear();
						sit->symbol->scaleFactor = 1.0;

						std::vector<std::string> vAugmentedPoint = split(v_AugmentedPoint, ',');
						if (vAugmentedPoint.size() == 3)
						{
							sit->vectorPoint = new SENC_VectorPoint();
							sit->vectorPoint->x = string2double(vAugmentedPoint[1]);
							sit->vectorPoint->y = string2double(vAugmentedPoint[2]);
						}

						sit->fr = featureType;

						displayListSENC->AddInstruction(sit);
					}
					// "SpatialReference:Curve|107" 
					else if (tag.compare("SpatialReference") == 0)
						vl_SpatialReference.push_back(value);
				}
				break;
				case 17:
				{
					// "TextAlignVertical:Center"
					if (tag.compare("TextAlignVertical") == 0)
						v_TextAlignVertical = value;
					else if (tag.compare("AreaFillReference") == 0)
					{
						SENC_AreaInstruction* sit = new SENC_AreaInstruction();

						GetSENCFromString(sit, featureID, v_DrawingPriority, v_DisplayPlane, v_ViewingGroup, v_ScaleMinimum, v_ScaleMaximum, vl_SpatialReference);
												
						std::wstring title = string2wstring(value);
						title.append(_T(".xml"));

						sit->areaFill = new SENC_AreaFillReference();
						sit->areaFill->SetFileTitle(title);

						sit->fr = featureType;

						displayListSENC->AddInstruction(sit);
					}
				}
				break;
				case 19:
				{
					// "TextAlignHorizontal:End"
					if (tag.compare("TextAlignHorizontal") == 0)
						v_TextAlignHorizontal = value;
				}
				break;
				}
			}

			di_splited.clear();
		}

		vl_SpatialReference.clear();
	}

	return;
}

void PCOutputSchemaManager::GetSENCFromS100Common(S100_Instruction* tp, SENC_Instruction* si)
{
	si->displayPlane = tp->GetDisplayPlane().compare(L"UNDERRADAR") == 0 ? 0 : 1;
	si->drawingPriority = tp->GetDrawingProiority();
	si->viewingGroup = _wtoi(tp->GetViewingGroup(0).c_str());
	si->featureReference = pugi::as_utf8(tp->GetFeatureReference());
	si->scaleMinimum = tp->GetScaleMinimum();
	si->scaleMaximum = tp->GetScaleMaximum();

	auto spatialReference = tp->GetSpatialReference();
	for (auto itor = spatialReference.begin(); itor != spatialReference.end(); itor++)
	{
		S100_SpatialReference* sr = *itor;
		SENC_SpatialReference* ssr = new SENC_SpatialReference();

		ssr->SetRCNM(sr->GetType());
		ssr->reference = _wtoi(sr->GetReference().c_str());
		ssr->bForward = sr->isForward();

		si->spatialReference.push_back(ssr);
	}
}

void PCOutputSchemaManager::GetSENCFromString(SENC_Instruction* sit, std::string fid, std::string dpt, std::string dp, std::string vg, std::string min, std::string max, std::list<std::string> sr)
{
	if (!sit)
		return;

	sit->featureReference = fid;
	sit->drawingPriority = string2int(dpt);
	sit->displayPlane = dp.compare("UNDERRADAR") == 0 ? 0 : 1;
	sit->viewingGroup = string2int(vg);
	sit->scaleMinimum = string2int(min);
	sit->scaleMaximum = string2int(max);

	for (const auto& iter : sr)
	{
		std::vector<std::string> vSpatailRef = split(iter, '|');
		if (vSpatailRef.size() == 2)
		{
			SENC_SpatialReference* sr = new SENC_SpatialReference();

			sr->SetRCNM(string2wstring(vSpatailRef[0]));
			sr->reference = string2int(vSpatailRef[1]);

			sit->spatialReference.push_back(sr);
		}
		vSpatailRef.clear();
	}
}

void PCOutputSchemaManager::ChangePallete(PortrayalCatalogue* pc)
{
	if (displayListSENC)
	{
		displayListSENC->ChangePallete(pc);
	}
}

