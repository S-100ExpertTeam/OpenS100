#pragma once
// Platform-neutral byte sink for the binary writers (ISO 8211 records and fields).
//
// Replaces MFC CFile* in WriteRecord/WriteField/WriteDirectory/WriteLeader. The call
// shape `file->Write(ptr, length)` is kept so the record classes stay readable; only the
// type behind it changed. Phase 1 step 3-1 (Documents/TechDocs/2026-11-08-phase1-core-decoupling.md).
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ostream>
#include <vector>

namespace s100
{
	namespace io
	{
		class BinaryWriter
		{
		public:
			explicit BinaryWriter(std::ostream& out) : m_out(&out) {}

			// Appends `length` bytes starting at `data`.
			void Write(const void* data, std::size_t length)
			{
				if (data && length > 0)
				{
					m_out->write(static_cast<const char*>(data), static_cast<std::streamsize>(length));
				}
			}

			bool Good() const { return m_out->good(); }

		private:
			std::ostream* m_out;
		};

		// Convenience: owns an output file stream and exposes it as a BinaryWriter.
		class BinaryFileWriter : public BinaryWriter
		{
		public:
			explicit BinaryFileWriter(const std::filesystem::path& path)
				: BinaryWriter(m_file), m_file(path, std::ios::binary | std::ios::trunc) {}

			bool IsOpen() const { return m_file.is_open(); }

		private:
			std::ofstream m_file;
		};
	}
}
