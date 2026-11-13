#pragma once
#include "afxdialogex.h"
#include "resource.h"

#include "../SystemDatabase/ProtectedImport.h"

#include <filesystem>
#include <optional>

namespace GISLibrary
{
	class SystemDatabaseSession;
}

// "Load Protected Data": installs S-100 Part 15 protected data (an exchange set folder, its
// CATALOG.XML, a .zip of it, or one protected dataset file) into the System Database. Asks for
// PERMIT.XML and the credentials (M_ID, M_KEY, HW_ID, USERPERMIT), none of which are stored.
// The dialog stays open when the request itself is refused (credentials that do not agree, no
// PERMIT.XML) so that it can be corrected; it closes with IDOK once something was imported.
class ProtectedDataDlg final : public CDialogEx
{
	DECLARE_DYNAMIC(ProtectedDataDlg)

public:
	explicit ProtectedDataDlg(GISLibrary::SystemDatabaseSession& session, CWnd* parent = nullptr);

	enum { IDD = IDD_DIALOG_PROTECTED_DATA };

	// Filled in before DoModal to start from a source the user already chose.
	std::filesystem::path source;
	std::filesystem::path permitFile;

	// What the import did, after DoModal returned IDOK.
	[[nodiscard]] const std::optional<s100::sdb::ProtectedImportReport>& report() const noexcept { return report_; }

protected:
	BOOL OnInitDialog() override;
	void OnOK() override;

	afx_msg void OnBrowseSourceFolder();
	afx_msg void OnBrowseSourceFile();
	afx_msg void OnBrowsePermit();
	DECLARE_MESSAGE_MAP()

private:
	[[nodiscard]] std::wstring text(int control) const;

	GISLibrary::SystemDatabaseSession* session_;
	std::optional<s100::sdb::ProtectedImportReport> report_;
};

// The PERMIT.XML that comes with data loaded as ordinary data, which makes it protected data.
// Looked for in the folder (a few levels down), beside it and in an INFO folder beside it (where
// delivered exchange sets keep it; for a CATALOG.XML, around its folder), and in the directory at
// the end of a .zip. For a .zip the result is an empty path: importProtected finds it inside.
// Nothing when there is none. Reads names only, so it is cheap for large data.
[[nodiscard]] std::optional<std::filesystem::path> FindPermitFor(const std::filesystem::path& path);
