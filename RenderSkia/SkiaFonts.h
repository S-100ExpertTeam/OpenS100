#pragma once
// The fonts the Skia backend writes texts with.
//
// The application hands font files over; the package ships none and asks the system for none,
// so a text looks the same wherever the same files are given. Fonts are read by FreeType on
// every platform.
//
// The first font is the display font: it gives a line its height. A character it has no glyph
// for is taken from the first of the later fonts that has one (a fallback, in the order given).
// Glyphs are placed one after another: there is no shaping of scripts that need it (Arabic,
// Indic) and no right-to-left layout.
#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace s100::render
{
	class SkiaFonts
	{
	public:
		SkiaFonts();
		~SkiaFonts();

		SkiaFonts(const SkiaFonts&) = delete;
		SkiaFonts& operator=(const SkiaFonts&) = delete;

		// A TrueType or OpenType file. False when it cannot be read as a font.
		bool Add(const std::filesystem::path& file);

		// A font the system has, by its family name: for an application that shows what its
		// user chose. The text then looks as that system draws the font. Where the package has
		// no way to ask the system (anything but Windows today) this is false.
		bool AddSystem(const std::string& family);

		// The families AddSystem can find, as the system names them for its user. Empty where
		// the package has no way to ask the system.
		static std::vector<std::string> SystemFamilies();

		size_t Size() const;

		// For the renderer.
		struct Faces;
		const Faces& Get() const
		{
			return *faces;
		}

	private:
		std::unique_ptr<Faces> faces;
	};
}
