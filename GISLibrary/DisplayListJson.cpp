#include "stdafx.h"
#include "DisplayListJson.h"

#include "SENC_AlertReference.h"
#include "SENC_Annulus.h"
#include "SENC_Arc3Points.h"
#include "SENC_ArcByRadius.h"
#include "SENC_AreaFillReference.h"
#include "SENC_AreaInstruction.h"
#include "SENC_AugmentedPath.h"
#include "SENC_AugmentedRay.h"
#include "SENC_ColorFill.h"
#include "SENC_CompositeLineStyle.h"
#include "SENC_Dash.h"
#include "SENC_Define.h"
#include "SENC_DisplayList.h"
#include "SENC_Hatch.h"
#include "SENC_HatchFill.h"
#include "SENC_Instruction.h"
#include "SENC_InstructionVisitor.h"
#include "SENC_LineInstruction.h"
#include "SENC_LineStyle.h"
#include "SENC_LineStyleReference.h"
#include "SENC_LineSymbol.h"
#include "SENC_NullInstruction.h"
#include "SENC_Path.h"
#include "SENC_PointInstruction.h"
#include "SENC_Polyline.h"
#include "SENC_Sector.h"
#include "SENC_SpatialReference.h"
#include "SENC_Symbol.h"
#include "SENC_SymbolFill.h"
#include "SENC_TextInstruction.h"
#include "SENC_TextPoint.h"
#include "SENC_pixmapFill.h"

#include <pugixml.hpp>

#include <charconv>
#include <cmath>
#include <vector>

namespace
{
	// A JSON text built front to back. Key() and Element() place the separator; the value that
	// follows is written bare.
	class JsonWriter
	{
	public:
		std::string text;

		JsonWriter& Key(std::string_view name)
		{
			Separate();
			Quote(name);
			text += ':';
			return *this;
		}

		JsonWriter& Element()
		{
			Separate();
			return *this;
		}

		void Open(char bracket)
		{
			text += bracket;
			first.push_back(true);
		}

		void Close(char bracket)
		{
			first.pop_back();
			text += bracket;
		}

		void Null() { text += "null"; }
		void Bool(bool value) { text += value ? "true" : "false"; }
		void Integer(long long value) { text += std::to_string(value); }

		// Shortest text that reads back to the same value; std::to_chars does not look at the locale.
		template <typename Floating>
		void Number(Floating value)
		{
			if (!std::isfinite(value))
			{
				Null();
				return;
			}
			if (value == 0)
			{
				text += '0';   // no "-0"
				return;
			}
			char buffer[40];
			const auto end = std::to_chars(buffer, buffer + sizeof(buffer), value).ptr;
			text.append(buffer, end);
		}

		void String(std::string_view value) { Quote(value); }
		void String(const std::wstring& value) { Quote(pugi::as_utf8(value)); }

	private:
		std::vector<bool> first;

		void Separate()
		{
			if (first.empty())
			{
				return;
			}
			if (!first.back())
			{
				text += ',';
			}
			first.back() = false;
		}

		void Quote(std::string_view value)
		{
			static const char hex[] = "0123456789abcdef";
			text += '"';
			for (const char c : value)
			{
				const auto u = static_cast<unsigned char>(c);
				switch (c)
				{
				case '"': text += "\\\""; break;
				case '\\': text += "\\\\"; break;
				case '\n': text += "\\n"; break;
				case '\r': text += "\\r"; break;
				case '\t': text += "\\t"; break;
				default:
					if (u < 0x20)
					{
						text += "\\u00";
						text += hex[u >> 4];
						text += hex[u & 0xF];
					}
					else
					{
						text += c;
					}
				}
			}
			text += '"';
		}
	};

	// The model keeps the enumerations of Part 9 as the numbers of SENC_Define.h. The text has
	// the names of the standard; a number that is none of them is written as the number.
	struct Name
	{
		int value;
		const char* name;
	};

	template <std::size_t N>
	void Enumeration(JsonWriter& json, int value, const Name(&names)[N])
	{
		for (const auto& entry : names)
		{
			if (entry.value == value)
			{
				json.String(entry.name);
				return;
			}
		}
		json.Integer(value);
	}

	const Name crsNames[] = {
		{ GEOGRAPHIC_CRS, "GeographicCRS" }, { PORTRAYAL_CRS, "PortrayalCRS" },
		{ LOCAL_CRS, "LocalCRS" }, { LINE_CRS, "LineCRS" } };
	const Name areaCrsNames[] = {
		{ GLOBAL, "Global" }, { LOCAL_GEOMETRY, "LocalGeometry" }, { GLOBAL_GEOMETRY, "GlobalGeometry" } };
	const Name capNames[] = { { BUTT, "Butt" }, { SQUARE, "Square" }, { ROUND, "Round" } };
	const Name joinNames[] = { { BEBEL, "Bevel" }, { MITER, "Miter" }, { ROUND, "Round" } };
	const Name horizontalNames[] = { { START, "Start" }, { END, "End" }, { CENTER, "Center" } };
	const Name verticalNames[] = { { TOP, "Top" }, { BOTTOM, "Bottom" }, { CENTER, "Center" } };
	const Name weightNames[] = { { LIGHT, "Light" }, { MEDIUM, "Medium" }, { BOLD, "Bold" } };
	const Name slantNames[] = { { 0, "Upright" }, { 1, "Italics" } };
	const Name proportionNames[] = { { MONOSPACES, "MonoSpaced" }, { PROPORTIONAL, "Proportional" } };
	const Name planeNames[] = { { 0, "UnderRadar" }, { 1, "OverRadar" } };

	void Offset(JsonWriter& json, const SENC_VectorPoint& point)
	{
		json.Open('{');
		json.Key("x").Number(point.x);
		json.Key("y").Number(point.y);
		json.Close('}');
	}

	template <typename Transparency>
	void Color(JsonWriter& json, const std::wstring& token, Transparency transparency)
	{
		json.Open('{');
		json.Key("token").String(token);
		json.Key("transparency").Number(transparency);
		json.Close('}');
	}

	void Sector(JsonWriter& json, const SENC_Sector* sector)
	{
		if (!sector)
		{
			json.Null();
			return;
		}
		json.Open('{');
		json.Key("startAngle").Number(sector->startAngle);
		json.Key("angularDistance").Number(sector->angularDistance);
		json.Close('}');
	}

	void Symbol(JsonWriter& json, const SENC_Symbol* symbol)
	{
		if (!symbol)
		{
			json.Null();
			return;
		}
		json.Open('{');
		json.Key("reference").String(symbol->reference);
		json.Key("rotation").Number(symbol->rotation);
		json.Key("rotationCRS");
		Enumeration(json, symbol->rotationCRS, crsNames);
		json.Key("scaleFactor").Number(symbol->scaleFactor);
		json.Key("offset");
		Offset(json, symbol->offset);
		json.Close('}');
	}

	class LineStyleWriter : public SENC_LineStyleVisitor
	{
	public:
		explicit LineStyleWriter(JsonWriter& json) : json(json) {}

		void Write(SENC_LineStyleBase* style)
		{
			if (!style)
			{
				json.Null();
				return;
			}
			const auto before = json.text.size();
			style->Accept(*this);
			if (json.text.size() == before)
			{
				json.Null();   // a base object of no kind
			}
		}

		void WriteAll(const std::vector<SENC_LineStyleBase*>& styles)
		{
			json.Open('[');
			for (auto* style : styles)
			{
				json.Element();
				Write(style);
			}
			json.Close(']');
		}

		void Visit(SENC_LineStyle& style) override
		{
			json.Open('{');
			json.Key("type").String("lineStyle");
			if (!style.lineStyleName.empty())
			{
				json.Key("name").String(style.lineStyleName);
			}
			json.Key("width").Number(style.pen_width);
			json.Key("color");
			Color(json, style.colorToken, style.pen_transparency);
			json.Key("intervalLength");
			if (style.intervalLength == 0)
			{
				json.Null();   // a solid line
			}
			else
			{
				json.Number(style.intervalLength);
			}
			json.Key("dashes").Open('[');
			for (const auto* dash : style.dashes)
			{
				json.Element().Open('{');
				json.Key("start").Number(dash->start);
				json.Key("length").Number(dash->length);
				json.Close('}');
			}
			json.Close(']');
			json.Key("symbols").Open('[');
			for (const auto* symbol : style.symbols)
			{
				json.Element().Open('{');
				json.Key("reference").String(symbol->reference);
				json.Key("position").Number(symbol->position);
				json.Key("rotation").Number(symbol->rotation);
				json.Key("scaleFactor").Number(symbol->scaleFactor);
				json.Key("crsType");
				Enumeration(json, symbol->crsType, crsNames);
				json.Close('}');
			}
			json.Close(']');
			json.Key("capStyle");
			Enumeration(json, style.capStyle, capNames);
			json.Key("joinStyle");
			Enumeration(json, style.joinStyle, joinNames);
			json.Key("offset").Number(style.offset);
			json.Close('}');
		}

		void Visit(SENC_LineStyleReference& style) override
		{
			json.Open('{');
			json.Key("type").String("lineStyleReference");
			json.Key("reference").String(style.reference);
			json.Close('}');
		}

		void Visit(SENC_CompositeLineStyle& style) override
		{
			json.Open('{');
			json.Key("type").String("compositeLineStyle");
			json.Key("styles");
			WriteAll(style.lineStyles);
			json.Close('}');
		}

	private:
		JsonWriter& json;
	};

	void AreaFill(JsonWriter& json, SENC_AreaFillBase* fill)
	{
		if (!fill)
		{
			json.Null();
			return;
		}
		json.Open('{');
		if (const auto* color = dynamic_cast<SENC_ColorFill*>(fill))
		{
			json.Key("type").String("colorFill");
			json.Key("color");
			if (color->color)
			{
				Color(json, color->color->token, color->color->transparency);
			}
			else
			{
				json.Null();
			}
		}
		else if (const auto* symbol = dynamic_cast<SENC_SymbolFill*>(fill))
		{
			json.Key("type").String("symbolFill");
			json.Key("areaCRS");
			Enumeration(json, symbol->areaCRSType, areaCrsNames);
			json.Key("symbol");
			Symbol(json, symbol->symbol);
			json.Key("v1");
			Offset(json, symbol->v1);
			json.Key("v2");
			Offset(json, symbol->v2);
			json.Key("clipSymbols").Bool(symbol->clipSymbols);
		}
		else if (const auto* hatch = dynamic_cast<SENC_HatchFill*>(fill))
		{
			json.Key("type").String("hatchFill");
			json.Key("areaCRS");
			Enumeration(json, hatch->areaCRSType, areaCrsNames);
			json.Key("hatches").Open('[');
			for (const auto* line : hatch->hatches)
			{
				json.Element().Open('{');
				json.Key("direction");
				Offset(json, line->direction);
				json.Key("distance").Number(line->distance);
				json.Key("lineStyle");
				LineStyleWriter(json).Write(line->lineStyle);
				json.Close('}');
			}
			json.Close(']');
		}
		else if (const auto* pixmap = dynamic_cast<SENC_PixmapFill*>(fill))
		{
			json.Key("type").String("pixmapFill");
			json.Key("areaCRS");
			Enumeration(json, pixmap->areaCRSType, areaCrsNames);
			json.Key("reference").Integer(pixmap->pixmap.reference);
		}
		else if (dynamic_cast<SENC_AreaFillReference*>(fill))
		{
			json.Key("type").String("areaFillReference");
			json.Key("reference").String(fill->fileStem);   // the name; the number is never set
			json.Close('}');
			return;
		}
		else
		{
			json.Key("type").String("unknown");
		}
		if (!fill->fileStem.empty())
		{
			json.Key("fileStem").String(fill->fileStem);
		}
		json.Close('}');
	}

	class InstructionWriter : public SENC_InstructionVisitor
	{
	public:
		explicit InstructionWriter(JsonWriter& json) : json(json) {}

		void Write(SENC_Instruction& instruction)
		{
			const auto before = json.text.size();
			instruction.Accept(*this);
			if (json.text.size() == before)
			{
				// A kind that takes no visitor (a base object): the common part alone.
				json.Open('{');
				json.Key("type").Integer(instruction.type);
				Common(instruction);
				json.Close('}');
			}
		}

		void Visit(SENC_NullInstruction& instruction) override
		{
			Begin("null", instruction);
			json.Close('}');
		}

		void Visit(SENC_PointInstruction& instruction) override
		{
			Begin("point", instruction);
			json.Key("symbol");
			Symbol(json, instruction.symbol);
			if (instruction.vectorPoint)
			{
				json.Key("vectorPoint");
				Offset(json, *instruction.vectorPoint);
			}
			json.Close('}');
		}

		void Visit(SENC_LineInstruction& instruction) override
		{
			Begin("line", instruction);
			json.Key("suppression").Bool(instruction.GetSuppression());
			json.Key("styles");
			LineStyleWriter(json).WriteAll(instruction.lineStyles);
			json.Close('}');
		}

		void Visit(SENC_AreaInstruction& instruction) override
		{
			Begin("area", instruction);
			json.Key("fill");
			AreaFill(json, instruction.areaFill);
			json.Close('}');
		}

		void Visit(SENC_TextInstruction& instruction) override
		{
			Begin("text", instruction);
			json.Key("textPoint");
			if (const auto* point = instruction.textPoint)
			{
				json.Open('{');
				json.Key("horizontalAlignment");
				Enumeration(json, point->horizontalAlignment, horizontalNames);
				json.Key("verticalAlignment");
				Enumeration(json, point->verticalAlignment, verticalNames);
				json.Key("elements").Open('[');
				for (const auto* element : point->elements)
				{
					json.Element().Open('{');
					json.Key("text").String(element->text.value);
					json.Key("fontColor");
					if (element->hasForeground)
					{
						Color(json, element->foreground.token, element->foreground.transparency);
					}
					else
					{
						json.Null();
					}
					json.Key("bodySize").Integer(element->bodySize);
					json.Key("fontWeight");
					Enumeration(json, element->font.weight, weightNames);
					json.Key("fontSlant");
					Enumeration(json, element->font.slant, slantNames);
					json.Key("fontSerifs").Bool(element->font.serifs);
					json.Key("fontProportion");
					Enumeration(json, element->font.proportion, proportionNames);
					json.Key("verticalOffset").Number(element->verticalOffset);
					json.Close('}');
				}
				json.Close(']');
				json.Key("offset");
				Offset(json, point->offset);
				json.Close('}');
			}
			else
			{
				json.Null();
			}
			json.Close('}');
		}

		void Visit(SENC_AugmentedRay& instruction) override
		{
			Begin("augmentedRay", instruction);
			json.Key("crs");
			Enumeration(json, instruction.areaCRSType, areaCrsNames);
			json.Key("direction").Number(instruction.direction);
			json.Key("length").Number(instruction.length);
			json.Key("styles");
			LineStyleWriter(json).WriteAll(instruction.lineStyles);
			json.Close('}');
		}

		void Visit(SENC_AugmentedPath& instruction) override
		{
			Begin("augmentedPath", instruction);
			json.Key("crs");
			Enumeration(json, instruction.areaCRSType, areaCrsNames);
			json.Key("path");
			if (const auto* path = instruction.path)
			{
				json.Open('{');
				json.Key("polylines").Open('[');
				for (const auto* polyline : path->polylines)
				{
					json.Element().Open('[');
					for (const auto& point : polyline->points)
					{
						json.Element();
						Offset(json, point);
					}
					json.Close(']');
				}
				json.Close(']');
				json.Key("arc3Points").Open('[');
				for (const auto* arc : path->arc3Pointses)
				{
					json.Element().Open('{');
					json.Key("startPoint");
					Offset(json, arc->point);
					json.Key("medianPoint");
					Offset(json, arc->medianPoint);
					json.Key("endPoint");
					Offset(json, arc->endPoint);
					json.Close('}');
				}
				json.Close(']');
				json.Key("arcByRadius").Open('[');
				for (const auto* arc : path->arcByRadiuses)
				{
					json.Element().Open('{');
					json.Key("center");
					Offset(json, arc->center);
					json.Key("radius").Number(arc->radius);
					json.Key("sector");
					Sector(json, arc->sector);
					json.Close('}');
				}
				json.Close(']');
				json.Key("annulus").Open('[');
				for (const auto* annulus : path->annuluses)
				{
					json.Element().Open('{');
					json.Key("center");
					Offset(json, annulus->point);
					json.Key("innerRadius");
					if (annulus->innerRadius)
					{
						json.Number(*annulus->innerRadius);
					}
					else
					{
						json.Null();
					}
					json.Key("outerRadius").Number(annulus->outerRaius);
					json.Key("sector");
					Sector(json, annulus->sector);
					json.Close('}');
				}
				json.Close(']');
				json.Close('}');
			}
			else
			{
				json.Null();
			}
			json.Key("styles");
			LineStyleWriter(json).WriteAll(instruction.lineStyles);
			json.Close('}');
		}

	private:
		JsonWriter& json;

		void Begin(const char* kind, SENC_Instruction& instruction)
		{
			json.Open('{');
			json.Key("type").String(kind);
			Common(instruction);
		}

		// What every instruction carries. A scale of 0 is "no limit" in the model and null here.
		void Common(SENC_Instruction& instruction)
		{
			if (instruction.id)
			{
				json.Key("id").String(*instruction.id);
			}
			if (instruction.parentId)
			{
				json.Key("parentId").String(*instruction.parentId);
			}
			if (instruction.hover)
			{
				json.Key("hover").Bool(*instruction.hover);
			}
			json.Key("featureReference").String(instruction.featureReference);
			json.Key("spatialReferences").Open('[');
			for (const auto* reference : instruction.spatialReference)
			{
				json.Element().Open('{');
				json.Key("rcnm").Integer(reference->RCNM);
				json.Key("reference").Integer(reference->reference);
				json.Key("forward").Bool(reference->bForward);
				json.Close('}');
			}
			json.Close(']');
			json.Key("viewingGroups").Open('[');
			for (const int group : instruction.viewingGroup)
			{
				json.Element().Integer(group);
			}
			json.Close(']');
			json.Key("displayPlane");
			Enumeration(json, instruction.displayPlane, planeNames);
			json.Key("drawingPriority").Integer(instruction.drawingPriority);
			json.Key("scaleMinimum");
			Scale(instruction.scaleMinimum);
			json.Key("scaleMaximum");
			Scale(instruction.scaleMaximum);
			if (instruction.alertReference)
			{
				json.Key("alertReference").Open('{');
				json.Key("reference").String(instruction.alertReference->reference);
				if (instruction.alertReference->plan)
				{
					json.Key("plan").String(*instruction.alertReference->plan);
				}
				if (instruction.alertReference->monitoring)
				{
					json.Key("monitoring").String(*instruction.alertReference->monitoring);
				}
				json.Close('}');
			}
		}

		void Scale(unsigned value)
		{
			if (value == 0)
			{
				json.Null();
			}
			else
			{
				json.Integer(value);
			}
		}
	};
}

namespace GISLibrary
{
	std::string DisplayListToJson(SENC_DisplayList& displayList)
	{
		JsonWriter json;
		InstructionWriter writer(json);
		json.Open('{');
		json.Key("instructions").Open('[');
		for (auto& [key, instruction] : displayList.displayInstructions)
		{
			if (instruction)
			{
				json.Element();
				json.text += '\n';   // one instruction a line: a text diff shows the instruction
				writer.Write(*instruction);
			}
		}
		json.text += '\n';
		json.Close(']');
		json.Close('}');
		json.text += '\n';
		return std::move(json.text);
	}

	std::string InstructionToJson(SENC_Instruction& instruction)
	{
		JsonWriter json;
		InstructionWriter(json).Write(instruction);
		return std::move(json.text);
	}

	std::uint64_t JsonHash(std::string_view json)
	{
		std::uint64_t hash = 14695981039346656037ull;
		for (const char c : json)
		{
			hash ^= static_cast<unsigned char>(c);
			hash *= 1099511628211ull;
		}
		return hash;
	}
}
