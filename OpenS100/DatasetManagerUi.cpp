#include "pch.h"
#include "DatasetManagerUi.h"
#include "OpenS100.h"

#include "../GISLibrary/SystemDatabaseSession.h"
#include "../GeoMetryLibrary/TextCodec.h"

#include <shellapi.h>

#include <array>
#include <format>

namespace DatasetManagerUi
{
	GISLibrary::SystemDatabaseSession* Session(CWnd* parent)
	{
		if (!theApp.systemDatabase && parent)
		{
			parent->MessageBox(L"The System Database could not be opened; see the message shown at start-up.",
				L"Dataset Manager", MB_ICONWARNING);
		}
		return theApp.systemDatabase.get();
	}

	std::wstring Widen(const std::string& utf8)
	{
		return s100::text::ToWide(utf8);
	}

	void Report(CWnd* parent, const std::vector<s100::sdb::ImportResult>& results, const std::vector<std::string>& problems,
		const std::vector<std::string>& notes)
	{
		using s100::sdb::ImportOutcome;
		std::array<std::size_t, 7> counts{};
		for (const auto& result : results)
		{
			++counts[static_cast<std::size_t>(result.outcome)];
		}
		std::wstring text;
		for (const auto& note : notes)
		{
			text += Widen(note) + L"\n";
		}
		text += std::format(L"Added {}, replaced {}, updates {}, cancelled {}, removed {}, unchanged {}, not installed {}.",
			counts[static_cast<int>(ImportOutcome::Added)], counts[static_cast<int>(ImportOutcome::Replaced)],
			counts[static_cast<int>(ImportOutcome::UpdateAdded)], counts[static_cast<int>(ImportOutcome::Cancelled)],
			counts[static_cast<int>(ImportOutcome::Removed)], counts[static_cast<int>(ImportOutcome::Unchanged)],
			counts[static_cast<int>(ImportOutcome::Rejected)]);

		constexpr std::size_t listed = 12;
		std::size_t shown = 0;
		for (const auto& result : results)
		{
			if (result.outcome != ImportOutcome::Rejected)
			{
				continue;
			}
			if (shown++ == listed)
			{
				text += L"\n...";
				break;
			}
			text += L"\n" + result.source.filename().wstring() + L": " + Widen(result.message);
		}
		for (const auto& problem : problems)
		{
			text += L"\n" + Widen(problem);
		}
		const bool trouble = counts[static_cast<int>(ImportOutcome::Rejected)] > 0 || !problems.empty();
		if (parent)
		{
			parent->MessageBox(text.c_str(), L"Dataset Manager", trouble ? MB_ICONWARNING : MB_ICONINFORMATION);
		}
	}

	bool ConfirmRemoval(CWnd* parent, std::size_t count)
	{
		const std::wstring text = std::format(L"Remove {} item(s) from the System Database?\n"
			L"Their copies in the System Database are deleted; the files you imported them from are not touched.", count);
		return parent && parent->MessageBox(text.c_str(), L"Dataset Manager", MB_ICONQUESTION | MB_OKCANCEL) == IDOK;
	}

	void ShowInFolder(const std::filesystem::path& file)
	{
		const std::wstring arguments = L"/select,\"" + file.wstring() + L"\"";
		::ShellExecuteW(nullptr, L"open", L"explorer.exe", arguments.c_str(), nullptr, SW_SHOWNORMAL);
	}

	std::vector<std::filesystem::path> DroppedFiles(void* dropInfo)
	{
		const auto drop = static_cast<HDROP>(dropInfo);
		std::vector<std::filesystem::path> files;
		const UINT count = ::DragQueryFileW(drop, 0xFFFFFFFF, nullptr, 0);
		for (UINT i = 0; i < count; ++i)
		{
			std::wstring name(::DragQueryFileW(drop, i, nullptr, 0), L'\0');
			::DragQueryFileW(drop, i, name.data(), static_cast<UINT>(name.size() + 1));
			files.emplace_back(name);
		}
		return files;
	}
}
