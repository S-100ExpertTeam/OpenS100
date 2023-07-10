#pragma once

namespace TextPackage
{
	enum class TextFlag
	{
		underLine = 1,
		strikeThrough,
		upperLine
	};

	enum class HorizontalAlignment
	{
		start = 1,
		end,
		h_center  // standard=> center.
	};

	enum class VerticalAlignment
	{
		top = 1,
		bottom,
		v_center // standard=> center.
	};

	enum class FontProportion
	{
		monoSpaced = 1,
		proportional
	};

	enum class FontSlant
	{
		upright = 1,
		italics
	};

	enum class FontWeight
	{
		light = 1,
		medium,
		bold
	};

	enum class _FontSubClassType
	{
		_Font,
		_FontReference,
		_FontCharacteristics
	};
}