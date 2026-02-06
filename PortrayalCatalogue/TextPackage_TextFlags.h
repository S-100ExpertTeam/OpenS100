#pragma once

#include "TextPackage_TextFlag.h"

#include <vector>

namespace TextPackage
{

	class TextFlags
	{
	public:
		TextFlags() = default;
		virtual ~TextFlags() = default;

	private:
		std::vector<TextFlag> flag;

	public:
		void AddFlag(const TextFlag& textFlag);
		void ClearFlags();
		const std::vector<TextFlag>& GetFlags() const;
		TextFlag GetFlag(int index) const;
		size_t Size() const;
	};

}