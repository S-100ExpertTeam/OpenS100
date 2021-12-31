#pragma once

namespace TextPackage
{
	enum TextFlag
	{
		underLine = 1,
		strikeThrough,
		upperLine
	};

	enum HorizontalAlignment
	{
		start = 1,
		end,
		h_center  // standard=> center.
	};

	enum VerticalAlignment
	{
		top = 1,
		bottom,
		v_center // standard=> center.
	};

	enum FontProportion
	{
		monoSpaced = 1,
		proportional
	};

	enum FontSlant
	{
		upright = 1,
		italics
	};

	enum FontWeight
	{
		light = 1,
		medium,
		bold
	};

	enum _FontSubClassType
	{
		_Font,
		_FontReference,
		_FontCharacteristics
	};
}