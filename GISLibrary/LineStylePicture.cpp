#include "stdafx.h"
#include "LineStylePicture.h"

#include "../GeoMetryLibrary/ENCCommon.h"
#include "../PortrayalCatalogue/ExternalFile.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/S100_LineStyles.h"

#include <pugixml.hpp>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>

namespace
{
	// By name: the precompiled header of the viewer build brings in another PointF.
	using s100::render::LineStylePicture;
	using s100::render::PathFigure;
	using s100::render::PointF;

	// Pixels to a millimetre along a line style, and across a composite one.
	const float kAlong = 2.0f / 0.32f;
	const float kAcross = 1.0f / 0.32f;

	// <pen width="0.32"><color transparency="0.5">CHMGD</color></pen>
	void ReadPen(const pugi::xml_node& pen, std::string& token, float& opacity, float* width)
	{
		const pugi::xml_node color = pen.child("color");
		token = color.text().get();
		while (!token.empty() && (token.back() == ' ' || token.back() == '\n' || token.back() == '\r' || token.back() == '\t'))
		{
			token.pop_back();
		}
		token.erase(0, token.find_first_not_of(" \t\r\n"));
		opacity = 1.0f - color.attribute("transparency").as_float(0.0f);
		if (width)
		{
			*width = pen.attribute("width").as_float(0.32f);
		}
	}

	void ReadSimple(const pugi::xml_node& node, LineStylePicture& lineStyle)
	{
		// Whole pixels, cut as the viewer has always cut them.
		lineStyle.length = std::floor(node.child("intervalLength").text().as_float() * kAlong);
		ReadPen(node.child("pen"), lineStyle.colorToken, lineStyle.opacity, nullptr);
		for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
		{
			if (!std::strcmp(child.name(), "dash"))
			{
				LineStylePicture::Dash dash;
				dash.start = child.child("start").text().as_float() * kAlong;
				dash.length = child.child("length").text().as_float() * kAlong;
				lineStyle.dashes.push_back(dash);
			}
			else if (!std::strcmp(child.name(), "symbol"))
			{
				LineStylePicture::Symbol symbol;
				symbol.reference = child.attribute("reference").value();
				symbol.position = child.child("position").text().as_float() * kAlong;
				symbol.rotation = child.attribute("rotation").as_float(0.0f);
				lineStyle.symbols.push_back(std::move(symbol));
			}
		}
	}
}

namespace s100::render
{
	bool ReadLineStyle(const std::filesystem::path& path, LineStylePicture& lineStyle)
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

		lineStyle = LineStylePicture{};
		lineStyle.name = path.stem().string();

		const pugi::xml_node root = document.document_element();   // ls:lineStyle, ls:compositeLineStyle
		const std::string kind = root.name();
		if (kind.find("compositeLineStyle") != std::string::npos)
		{
			for (pugi::xml_node part = root.child("lineStyle"); part; part = part.next_sibling("lineStyle"))
			{
				LineStylePicture::Line line;
				line.offset = part.attribute("offset").as_float(0.0f) * kAcross;
				float width = 0.32f;
				ReadPen(part.child("pen"), line.colorToken, line.opacity, &width);
				line.width = width * kAcross;
				lineStyle.lines.push_back(std::move(line));
			}
			return !lineStyle.lines.empty();
		}
		if (kind.find("lineStyle") != std::string::npos)
		{
			ReadSimple(root, lineStyle);
			return true;
		}
		return false;
	}

	LineStyleLibrary::LineStyleLibrary(PortrayalCatalogue& catalogue)
	{
		const auto directory = std::filesystem::path(catalogue.GetRootPathW()) / L"LineStyles";
		if (auto files = catalogue.GetTreeLineStyles()->GetLineStyleFilesVector())
		{
			for (auto file : *files)
			{
				LineStylePicture lineStyle;
				if (ReadLineStyle(directory / file->GetFileNameW(), lineStyle))
				{
					lineStyles.emplace(lineStyle.name, std::move(lineStyle));
				}
			}
		}
	}

	const LineStylePicture* LineStyleLibrary::Find(const std::string& name) const
	{
		const auto found = lineStyles.find(name);
		return found == lineStyles.end() ? nullptr : &found->second;
	}

	void DrawLineStyle(IRenderer& renderer, const LineStylePicture& lineStyle, const std::vector<PointF>& points,
		const ColorF* shortStretches, const ColorLookup& color)
	{
		const float symbolScale = ENCCommon::DISPLAY_SYMBOL_SCALE;

		for (size_t i = 1; i < points.size(); i++)
		{
			const PointF from = points[i - 1];
			const PointF to = points[i];
			const float dx = to.x - from.x;
			const float dy = to.y - from.y;
			const float length = std::sqrt(dx * dx + dy * dy);
			if (!(length > 0.0f))
			{
				continue;
			}

			if (shortStretches && length < kShortStretch)
			{
				Path path;
				path.figures.push_back({ from, { to }, false });
				Stroke stroke;
				stroke.width = 1.0f;
				stroke.dashes = { 2.0f, 2.0f };
				renderer.SetTransform({});
				renderer.StrokePath(path, *shortStretches, stroke);
				continue;
			}

			// The stretch as the x axis from its first point.
			const float degrees = std::atan2(dy, dx) * (180.0f / 3.1415f);
			renderer.SetTransform({ TransformStep::RotateAbout(degrees, from.x, from.y) });

			if (!lineStyle.lines.empty())
			{
				for (const LineStylePicture::Line& line : lineStyle.lines)
				{
					Path path;
					path.figures.push_back({ { from.x, from.y + line.offset }, { { from.x + length, from.y + line.offset } }, false });
					ColorF ink = color(pugi::as_wide(line.colorToken));
					ink.a = line.opacity;
					Stroke stroke;
					stroke.width = line.width;
					renderer.StrokePath(path, ink, stroke);
				}
				continue;
			}
			if (lineStyle.length < 1.0f)
			{
				continue;
			}

			// The dashes of every interval on the stretch, cut where the stretch ends.
			Path dashes;
			for (float start = 0.0f; start < length; start += lineStyle.length)
			{
				const float room = (std::min)(lineStyle.length, length - start);
				for (const LineStylePicture::Dash& dash : lineStyle.dashes)
				{
					if (dash.start >= room)
					{
						continue;
					}
					const float end = (std::min)(dash.start + dash.length, room);
					dashes.figures.push_back({ { from.x + start + dash.start, from.y }, { { from.x + start + end, from.y } }, false });
				}
			}
			if (!dashes.figures.empty())
			{
				ColorF ink = color(pugi::as_wide(lineStyle.colorToken));
				ink.a = lineStyle.opacity;
				Stroke stroke;
				stroke.width = 1.0f;
				renderer.StrokePath(dashes, ink, stroke);
			}

			// The symbols, each whole, where its place is still on the stretch. A symbol is
			// placed in device pixels whatever the transform, so its place and its turn are
			// worked out for the surface.
			if (!lineStyle.symbols.empty())
			{
				const float ux = dx / length;
				const float uy = dy / length;
				for (float start = 0.0f; start < length; start += lineStyle.length)
				{
					const float room = (std::min)(lineStyle.length, length - start);
					for (const LineStylePicture::Symbol& symbol : lineStyle.symbols)
					{
						if (symbol.position > room)
						{
							continue;
						}
						const float along = start + symbol.position;
						renderer.DrawSymbol(symbol.reference, { from.x + ux * along, from.y + uy * along },
							symbol.rotation + degrees, symbolScale);
					}
				}
			}
		}
	}

	bool DrawLineStyle(IRenderer& renderer, const LineStyleLibrary& lineStyles, const std::string& name,
		const std::vector<PointF>& points, const ColorF* shortStretches, const ColorLookup& color)
	{
		const LineStylePicture* lineStyle = lineStyles.Find(name);
		if (!lineStyle)
		{
			lineStyle = lineStyles.Find("QUESMRK1");
			if (!lineStyle)
			{
				return false;
			}
		}
		DrawLineStyle(renderer, *lineStyle, points, shortStretches, color);
		return true;
	}
}
