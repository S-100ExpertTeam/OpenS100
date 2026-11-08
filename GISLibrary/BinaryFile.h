#pragma once
// Platform-neutral whole-file reading for the binary readers (ISO 8211 and friends).
//
// Replaces MFC CFile in the core so the readers compile without <afx*.h> on any platform.
// Phase 1 step 3-1 (Documents/TechDocs/2026-11-08-phase1-core-decoupling.md).
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <optional>
#include <vector>

namespace s100
{
	namespace io
	{
		// Reads the whole file into memory. Returns std::nullopt when the file cannot be
		// opened or read completely; an existing empty file yields an empty vector.
		inline std::optional<std::vector<std::uint8_t>> ReadAllBytes(const std::filesystem::path& path)
		{
			std::error_code ec;
			const auto size = std::filesystem::file_size(path, ec);
			if (ec)
			{
				return std::nullopt;
			}

			std::ifstream in(path, std::ios::binary);
			if (!in)
			{
				return std::nullopt;
			}

			std::vector<std::uint8_t> bytes(static_cast<size_t>(size));
			if (size > 0 && !in.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(size)))
			{
				return std::nullopt;
			}
			return bytes;
		}
	}
}
