#include "pch.h"
#include "ProtectedDataDlg.h"

#include "../GISLibrary/SystemDatabaseSession.h"
#include "../SystemDatabase/Utf8Path.h"
#include "DatasetManagerUi.h"

#include <algorithm>
#include <cctype>
#include <cwctype>
#include <fstream>
#include <string_view>

namespace fs = std::filesystem;

namespace
{
	bool namedIgnoringCase(const fs::path& path, std::wstring_view name)
	{
		const std::wstring file = path.filename().wstring();
		return std::ranges::equal(file, name, [](wchar_t a, wchar_t b) { return std::towupper(a) == std::towupper(b); });
	}

	std::optional<fs::path> permitIn(const fs::path& folder, int depth)
	{
		std::error_code error;
		for (auto it = fs::recursive_directory_iterator(folder, fs::directory_options::skip_permission_denied, error);
			 !error && it != fs::recursive_directory_iterator(); it.increment(error))
		{
			if (it.depth() >= depth)
			{
				it.disable_recursion_pending();
			}
			std::error_code fileError;
			if (it->is_regular_file(fileError) && namedIgnoringCase(it->path(), L"PERMIT.XML"))
			{
				return it->path();
			}
		}
		return std::nullopt;
	}

	// Whether the directory at the end of a .zip names a PERMIT.XML. Reads the last few MiB only:
	// the central directory, which lists every entry by name, is at the end.
	bool zipNamesPermit(const fs::path& zip)
	{
		std::ifstream in(zip, std::ios::binary | std::ios::ate);
		if (!in)
		{
			return false;
		}
		constexpr std::streamoff tail = 8 << 20;
		const std::streamoff size = in.tellg();
		const std::streamoff start = std::max<std::streamoff>(0, size - tail);
		std::string bytes(static_cast<std::size_t>(size - start), '\0');
		in.seekg(start);
		in.read(bytes.data(), static_cast<std::streamsize>(bytes.size()));
		std::ranges::transform(bytes, bytes.begin(), [](char c) { return static_cast<char>(std::toupper(static_cast<unsigned char>(c))); });
		return bytes.find("PERMIT.XML") != std::string::npos;
	}

	std::string utf8(const std::wstring& text)
	{
		return s100::sdb::toUtf8(fs::path(text));
	}
}

std::optional<fs::path> FindPermitFor(const fs::path& path)
{
	std::error_code error;
	if (fs::is_directory(path, error))
	{
		if (auto found = permitIn(path, 3))
		{
			return found;
		}
		for (const fs::path& beside : { path.parent_path(), path.parent_path() / L"INFO" })
		{
			const fs::path candidate = beside / L"PERMIT.XML";
			if (!beside.empty() && fs::is_regular_file(candidate, error))
			{
				return candidate;
			}
		}
		return std::nullopt;
	}
	if (namedIgnoringCase(path, L"CATALOG.XML"))
	{
		return FindPermitFor(path.parent_path());
	}
	std::wstring extension = path.extension().wstring();
	std::ranges::transform(extension, extension.begin(), [](wchar_t c) { return static_cast<wchar_t>(std::towupper(c)); });
	if (extension == L".ZIP" && zipNamesPermit(path))
	{
		return fs::path();
	}
	return std::nullopt;
}

IMPLEMENT_DYNAMIC(ProtectedDataDlg, CDialogEx)

ProtectedDataDlg::ProtectedDataDlg(GISLibrary::SystemDatabaseSession& session, CWnd* parent)
	: CDialogEx(IDD_DIALOG_PROTECTED_DATA, parent), session_(&session)
{
}

BEGIN_MESSAGE_MAP(ProtectedDataDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_P15_SOURCE_FOLDER, &ProtectedDataDlg::OnBrowseSourceFolder)
	ON_BN_CLICKED(IDC_BUTTON_P15_SOURCE_FILE, &ProtectedDataDlg::OnBrowseSourceFile)
	ON_BN_CLICKED(IDC_BUTTON_P15_PERMIT, &ProtectedDataDlg::OnBrowsePermit)
END_MESSAGE_MAP()

BOOL ProtectedDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetDlgItemTextW(IDC_EDIT_P15_SOURCE, source.wstring().c_str());
	SetDlgItemTextW(IDC_EDIT_P15_PERMIT, permitFile.wstring().c_str());
	// HW_ID and M_KEY are 32 hex digits, M_ID 6 characters, a user permit 46.
	static_cast<CEdit*>(GetDlgItem(IDC_EDIT_P15_MID))->SetLimitText(6);
	static_cast<CEdit*>(GetDlgItem(IDC_EDIT_P15_MKEY))->SetLimitText(32);
	static_cast<CEdit*>(GetDlgItem(IDC_EDIT_P15_HWID))->SetLimitText(32);
	static_cast<CEdit*>(GetDlgItem(IDC_EDIT_P15_USERPERMIT))->SetLimitText(46);

	// Start where the user has to type: the data when there is none yet, else the credentials.
	GetDlgItem(source.empty() ? IDC_EDIT_P15_SOURCE : IDC_EDIT_P15_MID)->SetFocus();
	return FALSE;
}

std::wstring ProtectedDataDlg::text(int control) const
{
	CString value;
	GetDlgItemTextW(control, value);
	value.Trim();
	return std::wstring(value.GetString());
}

void ProtectedDataDlg::OnBrowseSourceFolder()
{
	CFolderPickerDialog dialog(text(IDC_EDIT_P15_SOURCE).c_str(), 0, this);
	if (dialog.DoModal() != IDOK)
	{
		return;
	}
	const fs::path folder(dialog.GetPathName().GetString());
	SetDlgItemTextW(IDC_EDIT_P15_SOURCE, folder.wstring().c_str());
	if (text(IDC_EDIT_P15_PERMIT).empty())
	{
		if (const auto permit = FindPermitFor(folder))
		{
			SetDlgItemTextW(IDC_EDIT_P15_PERMIT, permit->wstring().c_str());
		}
	}
}

void ProtectedDataDlg::OnBrowseSourceFile()
{
	CFileDialog dialog(TRUE, nullptr, nullptr, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		L"Exchange set (CATALOG.XML, *.zip)|CATALOG.XML;*.zip|Protected dataset files|*.*||", this);
	if (dialog.DoModal() != IDOK)
	{
		return;
	}
	const fs::path file(dialog.GetPathName().GetString());
	SetDlgItemTextW(IDC_EDIT_P15_SOURCE, file.wstring().c_str());
	if (text(IDC_EDIT_P15_PERMIT).empty())
	{
		// A single dataset file: its PERMIT.XML is usually in a folder above it.
		auto permit = FindPermitFor(file);
		for (fs::path folder = file.parent_path(); !permit && !folder.empty() && folder != folder.parent_path(); folder = folder.parent_path())
		{
			const fs::path candidate = folder / L"PERMIT.XML";
			std::error_code error;
			if (fs::is_regular_file(candidate, error))
			{
				permit = candidate;
			}
		}
		if (permit && !permit->empty())
		{
			SetDlgItemTextW(IDC_EDIT_P15_PERMIT, permit->wstring().c_str());
		}
	}
}

void ProtectedDataDlg::OnBrowsePermit()
{
	CFileDialog dialog(TRUE, L"xml", L"PERMIT.XML", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		L"Permit file (PERMIT.XML)|PERMIT.XML;*.xml|All files (*.*)|*.*||", this);
	if (dialog.DoModal() == IDOK)
	{
		SetDlgItemTextW(IDC_EDIT_P15_PERMIT, dialog.GetPathName().GetString());
	}
}

void ProtectedDataDlg::OnOK()
{
	const s100::sdb::ProtectedImportRequest request{
		.manufacturerId = utf8(text(IDC_EDIT_P15_MID)),
		.manufacturerKey = utf8(text(IDC_EDIT_P15_MKEY)),
		.hwId = utf8(text(IDC_EDIT_P15_HWID)),
		.userPermit = utf8(text(IDC_EDIT_P15_USERPERMIT)),
		.permitFile = fs::path(text(IDC_EDIT_P15_PERMIT)),
		.source = fs::path(text(IDC_EDIT_P15_SOURCE)),
	};
	if (request.source.empty())
	{
		MessageBox(L"Choose the protected data to install.", L"Load Protected Data", MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_P15_SOURCE)->SetFocus();
		return;
	}

	try
	{
		CWaitCursor wait;
		report_ = s100::sdb::importProtected(session_->importer(), request);
	}
	catch (const s100::sdb::ProtectionError& error)
	{
		// The request itself was refused: nothing was installed, let the user correct it.
		MessageBox(DatasetManagerUi::Widen(error.what()).c_str(), L"Load Protected Data", MB_ICONWARNING);
		return;
	}
	catch (const std::exception& error)
	{
		MessageBox(DatasetManagerUi::Widen(error.what()).c_str(), L"Load Protected Data", MB_ICONERROR);
		return;
	}
	CDialogEx::OnOK();
}
