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

		// Reads at most `count` bytes from the start of the file (fewer if the file is shorter).
		inline std::optional<std::vector<std::uint8_t>> ReadBytes(const std::filesystem::path& path, std::size_t count)
		{
			std::ifstream in(path, std::ios::binary);
			if (!in)
			{
				return std::nullopt;
			}
			std::vector<std::uint8_t> bytes(count);
			in.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(count));
			bytes.resize(static_cast<size_t>(in.gcount()));
			return bytes;
		}

		// Random-access binary reader for record-oriented formats (shapefile .shp/.shx ...).
		class BinaryFileReader
		{
		public:
			bool Open(const std::filesystem::path& path)
			{
				m_in.open(path, std::ios::binary);
				return m_in.is_open();
			}
			bool IsOpen() const { return m_in.is_open(); }
			void Seek(std::uint64_t offsetFromStart)
			{
				m_in.clear();
				m_in.seekg(static_cast<std::streamoff>(offsetFromStart), std::ios::beg);
			}
			// Returns the number of bytes actually read.
			std::size_t Read(void* buffer, std::size_t size)
			{
				m_in.read(static_cast<char*>(buffer), static_cast<std::streamsize>(size));
				return static_cast<std::size_t>(m_in.gcount());
			}

		private:
			std::ifstream m_in;
		};
	}
}
