#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

// The managed store: the directory tree under the System Database root that holds a copy of every
// installed file (Documents/TechDocs/2026-11-10-system-database.md, section 3.3).
//
//   <root>/sdb.sqlite
//   <root>/FC/<product>/<version>/<file>.xml
//   <root>/PC/<product>/<version>/...            (the portrayal catalogue folder as delivered)
//   <root>/DATASET/<product>/<name>/<name>.000, .001, ...
//   <root>/SUPPORT/<product>/<file>
//
// Locations are handed out and taken back as paths relative to the root, UTF-8 with '/'
// separators, which is how the database records them. The store copies and deletes files; it does
// not know what they contain.
namespace s100::sdb
{
    class ResourceStore final
    {
    public:
        // The root need not exist yet; it is created on the first write.
        explicit ResourceStore(std::filesystem::path root);

        [[nodiscard]] const std::filesystem::path& root() const noexcept { return root_; }
        [[nodiscard]] std::filesystem::path databaseFile() const { return root_ / "sdb.sqlite"; }

        // Relative locations in the layout above. Every argument becomes one directory or file
        // name: characters that cannot appear in one are replaced, so no argument can climb out of
        // its place in the tree.
        [[nodiscard]] static std::string featureCataloguePath(std::string_view product, std::string_view version, std::string_view fileName);
        [[nodiscard]] static std::string portrayalCatalogueDirectory(std::string_view product, std::string_view version);
        [[nodiscard]] static std::string datasetDirectory(std::string_view product, std::string_view name);
        [[nodiscard]] static std::string datasetFilePath(std::string_view product, std::string_view name, std::string_view fileName);
        [[nodiscard]] static std::string supportFilePath(std::string_view product, std::string_view fileName);

        // A file or directory name made safe for use as one path segment.
        [[nodiscard]] static std::string safeSegment(std::string_view text);

        // The absolute location of a relative path. Throws std::invalid_argument for a path that
        // is absolute or leaves the root ("..").
        [[nodiscard]] std::filesystem::path absolute(std::string_view relative) const;
        [[nodiscard]] bool exists(std::string_view relative) const;

        // Copies `source` to `relative`, creating directories and replacing what is there. The
        // copy is written next to its destination first and renamed into place, so the destination
        // is never left half written. Returns the SHA-256 (hex) of the copied file. Throws
        // std::filesystem::filesystem_error.
        std::string copyFile(const std::filesystem::path& source, std::string_view relative);
        // Copies the tree under `source` into `relativeDirectory`, which is emptied first.
        // Returns the relative paths of the files copied, in sorted order.
        std::vector<std::string> copyDirectory(const std::filesystem::path& source, std::string_view relativeDirectory);
        // Moves a file or directory inside the store, replacing the destination.
        void move(std::string_view fromRelative, std::string_view toRelative);

        // Removes a file or a directory tree, then the directories above it that are left empty
        // (never the root). Returns false when there was nothing to remove.
        bool remove(std::string_view relative);

    private:
        void pruneEmptyParents(std::filesystem::path directory) const;

        std::filesystem::path root_;
    };
}
