#pragma once
#include <string>

namespace Portrayal
{
	// Where a catalogue puts the symbol, line style, area fill and colour profile files it lists.
	//
	// Reading those files is tied to how they are drawn, so the catalogue does not do it itself:
	// whoever opens the catalogue gives it a store (the renderer's, in the viewer). A catalogue
	// without a store is the catalogue data alone - rules, viewing groups, display modes, context
	// parameters and the lists of resource files.
	class ResourceStore
	{
	public:
		virtual ~ResourceStore() = default;

		virtual bool AddColorProfile(const std::wstring& id, const std::wstring& path) = 0;
		virtual bool AddSymbol(const std::wstring& path) = 0;
		virtual bool AddLineStyle(const std::wstring& path) = 0;
		virtual bool AddAreaFill(const std::wstring& path) = 0;

		// Symbol name is the file name without its extension.
		virtual bool HasSymbol(const std::wstring& name) = 0;

		virtual void ChangePalette(const std::wstring& paletteName) = 0;
	};
}
