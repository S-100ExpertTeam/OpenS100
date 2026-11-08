#pragma once
// Header-only whitespace tokenizer for the core projects (no MFC).
//
// Same behaviour as LibMFCUtil's StringTokenizer: consecutive separators collapse,
// leading/trailing separators produce no empty tokens.
// Phase 1 step 3-4 (Documents/TechDocs/2026-11-08-phase1-core-decoupling.md).
#include <string>
#include <string_view>
#include <vector>

namespace s100
{
	class StringTokenizer
	{
	public:
		StringTokenizer(std::string_view input, std::string_view separators)
		{
			auto start = input.find_first_not_of(separators);
			while (start != std::string_view::npos)
			{
				const auto end = input.find_first_of(separators, start);
				tokens.emplace_back(input.substr(start, end == std::string_view::npos ? std::string_view::npos : end - start));
				start = input.find_first_not_of(separators, end);
			}
		}

		size_t countTokens() const { return tokens.size(); }
		bool hasMoreTokens() const { return next < tokens.size(); }
		std::string nextToken() { return hasMoreTokens() ? tokens[next++] : std::string(); }

	private:
		std::vector<std::string> tokens;
		size_t next = 0;
	};
}
