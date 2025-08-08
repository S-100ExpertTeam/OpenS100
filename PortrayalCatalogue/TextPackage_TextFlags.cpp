#include "stdafx.h"
#include "TextPackage_TextFlags.h"

namespace TextPackage
{
	void TextFlags::AddFlag(const TextFlag& textFlag)
	{
		flag.push_back(textFlag);
	}

	void TextFlags::ClearFlags()
	{
		flag.clear();
	}

	const std::vector<TextFlag>& TextFlags::GetFlags() const
	{
		return flag;
	}

	TextFlag TextFlags::GetFlag(int index) const
	{
		if (index >= 0 && index < flag.size())
		{
			return flag[index];
		}

		return TextFlag::none;
	}

	size_t TextFlags::Size() const
	{
		return flag.size();
	}
}
