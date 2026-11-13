#pragma once

#include "DataProtection.h"
#include "Importer.h"

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

// Installing S-100 Part 15 protected data: the files a PERMIT.XML grants are decrypted into a
// staging folder, everything else is copied there as it is, and the staged exchange set (or the
// staged dataset files when there is no CATALOG.XML) goes through the Importer like any other.
// The store then holds the decrypted files, so the viewer reads them as it reads unprotected data.
namespace s100::sdb
{
    // What the user supplies to install protected data. M_KEY, HW_ID and USERPERMIT are each
    // optional, as long as a HW_ID can be had: given directly, or decrypted from the user permit
    // with M_KEY. Whatever is supplied is checked against the rest (see resolveCredentials).
    struct ProtectedImportRequest
    {
        std::string manufacturerId;    // M_ID, 6 characters
        std::string manufacturerKey;   // M_KEY, 32 hex
        std::string hwId;              // HW_ID, 32 hex
        std::string userPermit;        // USERPERMIT, 46 characters
        // PERMIT.XML. Empty: the PERMIT.XML found in the source.
        std::filesystem::path permitFile;
        // An exchange set folder (or its CATALOG.XML), a .zip archive of one, or a single
        // protected dataset file.
        std::filesystem::path source;
    };

    struct ProtectedImportOptions
    {
        // Where the staging folder is made (a fresh sub-folder, removed afterwards). Empty: the
        // system's temporary directory.
        std::filesystem::path stagingParent;
        // Today as yyyy-MM-dd for the permit expiry check. Empty: the current UTC date.
        std::string today;
        Importer::Progress progress;
    };

    struct ResolvedCredentials
    {
        std::string hwId;                       // upper case, 32 hex
        std::optional<UserPermit> userPermit;   // when one was supplied or PERMIT.XML has one
    };

    // The HW_ID to decrypt with. Throws ProtectionError when there is none or the inputs disagree:
    // a USERPERMIT other than the one in PERMIT.XML, an M_ID other than the user permit's, or a
    // HW_ID other than the one M_KEY decrypts from the user permit.
    [[nodiscard]] ResolvedCredentials resolveCredentials(const ProtectedImportRequest& request, const PermitFile& permits);

    struct DecryptionResult
    {
        std::string file;      // the protected file, relative to the source ('/' separated)
        bool decrypted = false;
        std::string message;   // why it failed, empty on success
    };

    struct ExpiredPermit
    {
        std::string fileName;
        std::string expiry;   // as PERMIT.XML writes it
    };

    struct ProtectedImportReport
    {
        std::string hwId;
        std::string dataServerName;
        std::vector<DecryptionResult> files;   // one per file a permit was found for
        // Permits that have expired. Their data is installed all the same, as with S-63.
        std::vector<ExpiredPermit> expiredPermits;
        // Permits for files the source does not hold, and archive entries that were skipped.
        std::vector<std::string> warnings;
        ImportReport import;

        [[nodiscard]] std::size_t decryptedCount() const noexcept;
    };

    // Decrypts and installs. Throws ProtectionError for problems with the request itself (no
    // source, no or a bad PERMIT.XML, credentials that do not resolve); a file that does not
    // decrypt is reported in `files` and the importer then rejects it as missing, the other files
    // are installed.
    ProtectedImportReport importProtected(Importer& importer, const ProtectedImportRequest& request,
        const ProtectedImportOptions& options = {});
}
