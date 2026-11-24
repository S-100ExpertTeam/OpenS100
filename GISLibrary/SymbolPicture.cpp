#include "stdafx.h"
#include "SymbolPicture.h"

#include "../GeoMetryLibrary/ENCCommon.h"
#include "../PortrayalCatalogue/ExternalFile.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/S100_Symbols.h"

#include <pugixml.hpp>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>

namespace
{
	// By name: the precompiled header of the viewer build brings in another PointF.
	using s100::render::PathFigure;
	using s100::render::PointF;
	using s100::render::SymbolShape;

	// The numbers and command letters of a path, one after another.
	class PathText
	{
	public:
		explicit PathText(const char* text) : at(text)
		{
		}

		// The next command letter, or 0 when a number (or the end) comes first.
		char Command()
		{
			Skip();
			const char letter = *at;
			if ((letter >= 'A' && letter <= 'Z' && letter != 'E') || (letter >= 'a' && letter <= 'z' && letter != 'e'))
			{
				at++;
				return letter;
			}
			return 0;
		}

		bool Number(float& value)
		{
			Skip();
			char* end = nullptr;
			const double read = std::strtod(at, &end);
			if (end == at)
			{
				return false;
			}
			at = end;
			value = static_cast<float>(read);
			return true;
		}

		bool AtEnd()
		{
			Skip();
			return *at == 0;
		}

	private:
		void Skip()
		{
			while (*at == ' ' || *at == ',' || *at == '\t' || *at == '\r' || *at == '\n')
			{
				at++;
			}
		}

		const char* at;
	};

	// The figures of a path. False when the path has a command that is not read; the figures up
	// to it are kept.
	bool ReadPath(const char* text, std::vector<PathFigure>& figures)
	{
		PathText path(text);
		PointF current;
		PointF start;
		bool open = false;   // a figure is being added to
		char command = 0;

		const auto lineTo = [&](PointF to) {
			if (!open)
			{
				PathFigure figure;
				figure.start = current;
				figures.push_back(std::move(figure));
				start = current;
				open = true;
			}
			figures.back().lines.push_back(to);
			current = to;
		};

		while (!path.AtEnd())
		{
			// A number where a command is expected repeats the command (a move goes on as lines).
			if (const char next = path.Command())
			{
				command = next;
			}
			else if (command == 'M')
			{
				command = 'L';
			}
			else if (command == 'm')
			{
				command = 'l';
			}
			else if (command == 'Z' || command == 'z' || command == 0)
			{
				return false;
			}

			float x = 0.0f;
			float y = 0.0f;
			switch (command)
			{
			case 'M':
			case 'm':
				if (!path.Number(x) || !path.Number(y))
				{
					return false;
				}
				current = command == 'M' ? PointF{ x, y } : PointF{ current.x + x, current.y + y };
				start = current;
				open = false;
				break;
			case 'L':
			case 'l':
				if (!path.Number(x) || !path.Number(y))
				{
					return false;
				}
				lineTo(command == 'L' ? PointF{ x, y } : PointF{ current.x + x, current.y + y });
				break;
			case 'H':
			case 'h':
				if (!path.Number(x))
				{
					return false;
				}
				lineTo({ command == 'H' ? x : current.x + x, current.y });
				break;
			case 'V':
			case 'v':
				if (!path.Number(y))
				{
					return false;
				}
				lineTo({ current.x, command == 'V' ? y : current.y + y });
				break;
			case 'Z':
			case 'z':
				if (open)
				{
					figures.back().closed = true;
					open = false;
				}
				current = start;
				break;
			default:
				return false;
			}
		}
		return true;
	}

	// The last word of a class says how the shape is coloured: "fCHBLK", "sl f0 sCHBLK".
	void ReadClass(const char* text, SymbolShape& shape)
	{
		std::istringstream words(text);
		std::string word;
		std::string last;
		while (words >> word)
		{
			last = word;
		}
		if (last.empty())
		{
			return;
		}
		shape.fill = last[0] == 'f';
		shape.colorToken = last.substr(1);
	}

	// "stroke-width:0.32;fill-opacity:0.5"
	void ReadStyle(const char* text, SymbolShape& shape)
	{
		std::string style(text);
		std::replace(style.begin(), style.end(), ':', ' ');
		std::replace(style.begin(), style.end(), ';', ' ');
		std::istringstream words(style);
		std::string name;
		std::string value;
		while (words >> name >> value)
		{
			if (name == "stroke-width")
			{
				shape.strokeWidth = static_cast<float>(std::strtod(value.c_str(), nullptr));
			}
			else if (name == "fill-opacity" || name == "stroke-opacity")
			{
				shape.opacity = static_cast<float>(std::strtod(value.c_str(), nullptr));
			}
		}
	}

	// What a path, a rectangle and a circle have alike.
	void ReadPresentation(const pugi::xml_node& node, SymbolShape& shape)
	{
		for (pugi::xml_attribute attribute = node.first_attribute(); attribute; attribute = attribute.next_attribute())
		{
			const char* name = attribute.name();
			if (!std::strcmp(name, "class"))
			{
				ReadClass(attribute.value(), shape);
			}
			else if (!std::strcmp(name, "style"))
			{
				ReadStyle(attribute.value(), shape);
			}
			else if (!std::strcmp(name, "stroke-width"))
			{
				shape.strokeWidth = attribute.as_float();
			}
			else if (!std::strcmp(name, "fill-opacity") || !std::strcmp(name, "stroke-opacity"))
			{
				shape.opacity = attribute.as_float();
			}
		}
	}

	// A circle as a closed figure of chords no longer than half a pixel on the screen.
	PathFigure CircleFigure(PointF center, float radius, float scale)
	{
		const double pi = 3.14159265358979323846;
		const double pixels = static_cast<double>(radius) * scale;
		const int count = std::clamp(static_cast<int>(std::ceil(4.0 * pi * pixels)), 16, 720);

		PathFigure figure;
		figure.start = { center.x + radius, center.y };
		figure.closed = true;
		for (int i = 1; i < count; i++)
		{
			const double angle = 2.0 * pi * i / count;
			figure.lines.push_back({ center.x + static_cast<float>(radius * std::cos(angle)),
				center.y + static_cast<float>(radius * std::sin(angle)) });
		}
		return figure;
	}
}

namespace s100::render
{
	bool ReadSymbolPicture(const std::filesystem::path& path, SymbolPicture& picture)
	{
		// Read by the standard library, which takes the path as it is on every platform.
		std::ifstream file(path, std::ios::binary);
		if (!file)
		{
			return false;
		}
		pugi::xml_document document;
		if (!document.load(file))
		{
			return false;
		}
		const pugi::xml_node svg = document.child("svg");
		if (!svg)
		{
			return false;
		}

		picture = SymbolPicture{};
		picture.name = path.stem().string();

		for (pugi::xml_node node = svg.first_child(); node; node = node.next_sibling())
		{
			const char* element = node.name();
			if (!std::strcmp(element, "path"))
			{
				SymbolShape shape;
				ReadPresentation(node, shape);
				if (!ReadPath(node.attribute("d").value(), shape.figures))
				{
					picture.unsupported++;
				}
				if (!shape.figures.empty())
				{
					picture.shapes.push_back(std::move(shape));
				}
			}
			else if (!std::strcmp(element, "rect"))
			{
				const std::string kind = node.attribute("class").value();
				const float x = node.attribute("x").as_float();
				const float y = node.attribute("y").as_float();
				const float width = node.attribute("width").as_float();
				const float height = node.attribute("height").as_float();
				if (kind == "svgBox layout")
				{
					picture.x = x;
					picture.y = y;
					picture.width = width;
					picture.height = height;
				}
				else if (!kind.empty() && kind.find("layout") == std::string::npos)
				{
					SymbolShape shape;
					ReadPresentation(node, shape);
					PathFigure figure;
					figure.start = { x, y };
					figure.lines = { { x + width, y }, { x + width, y + height }, { x, y + height } };
					figure.closed = true;
					shape.figures.push_back(std::move(figure));
					picture.shapes.push_back(std::move(shape));
				}
			}
			else if (!std::strcmp(element, "circle"))
			{
				const std::string kind = node.attribute("class").value();
				if (kind.empty() || kind.find("layout") != std::string::npos)
				{
					continue;   // the pivot: where the symbol is placed, not a part of it
				}
				SymbolShape shape;
				ReadPresentation(node, shape);
				shape.center = { node.attribute("cx").as_float(), node.attribute("cy").as_float() };
				shape.radius = node.attribute("r").as_float();
				if (shape.radius > 0.0f)
				{
					picture.shapes.push_back(std::move(shape));
				}
			}
		}
		return true;
	}

	SymbolLibrary::SymbolLibrary(PortrayalCatalogue& catalogue)
	{
		const auto directory = std::filesystem::path(catalogue.GetRootPathW()) / L"Symbols";
		if (auto symbols = catalogue.GetSymbols()->GetSymbolsVector())
		{
			for (auto file : *symbols)
			{
				const std::filesystem::path path = directory / file->GetFileNameW();
				files.emplace(path.stem().string(), path);
			}
		}
	}

	const SymbolPicture* SymbolLibrary::Find(const std::string& name)
	{
		const auto read = pictures.find(name);
		if (read != pictures.end())
		{
			return read->second.get();
		}
		std::unique_ptr<SymbolPicture> picture;
		const auto file = files.find(name);
		if (file != files.end())
		{
			picture = std::make_unique<SymbolPicture>();
			if (!ReadSymbolPicture(file->second, *picture))
			{
				picture.reset();
			}
		}
		return pictures.emplace(name, std::move(picture)).first->second.get();
	}

	float SymbolStrokePixels(float millimetres)
	{
		return millimetres * 0.03937f * 96.0f;
	}

	void DrawSymbolPicture(IRenderer& renderer, const SymbolPicture& picture, PointF at, float rotationDegrees, float scale,
		const ColorLookup& color, float strokeScale)
	{
		renderer.SetTransform({ TransformStep::Scale(scale), TransformStep::Rotate(rotationDegrees),
			TransformStep::Translate(at.x, at.y) });

		for (const SymbolShape& shape : picture.shapes)
		{
			ColorF ink = color(pugi::as_wide(shape.colorToken));
			ink.a = shape.opacity;

			Path path;
			if (shape.radius > 0.0f)
			{
				path.figures.push_back(CircleFigure(shape.center, shape.radius, scale));
			}
			else
			{
				path.figures = shape.figures;
			}

			if (shape.fill)
			{
				Fill fill;
				fill.color = ink;
				fill.smooth = true;
				renderer.FillPath(path, fill);
			}
			else if (shape.strokeWidth > 0.0f)
			{
				Stroke stroke;
				stroke.width = SymbolStrokePixels(shape.strokeWidth) * strokeScale;
				renderer.StrokePath(path, ink, stroke);
			}
		}
	}

	bool DrawSymbol(IRenderer& renderer, SymbolLibrary& symbols, const std::string& name, PointF at, float rotationDegrees,
		float scale, const ColorLookup& color)
	{
		if (name == "INFORM01" && !ENCCommon::Show_INFORM01)
		{
			return false;
		}
		const SymbolPicture* picture = symbols.Find(name);
		if (!picture)
		{
			picture = symbols.Find("QUESMRK1");
			if (!picture)
			{
				return false;
			}
		}
		DrawSymbolPicture(renderer, *picture, at, rotationDegrees, scale, color);
		return true;
	}
}
