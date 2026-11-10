#include "ResourceStore.h"

#include "Sha256.h"
#include "Utf8Path.h"

#include <algorithm>
#include <stdexcept>

namespace s100::sdb
{
    namespace fs = std::filesystem;

    namespace
    {
        std::string join(std::initializer_list<std::string_view> segments)
        {
            std::string path;
            for (const std::string_view segment : segments)
            {
                if (!path.empty())
                {
                    path += '/';
                }
                path += ResourceStore::safeSegment(segment);
            }
            return path;
        }

        // A sibling of `target` to write into before renaming over it.
        fs::path partialFile(const fs::path& target)
        {
            fs::path partial = target;
            partial += ".part";
            return partial;
        }
    }

    ResourceStore::ResourceStore(fs::path root) : root_(std::move(root))
    {
    }

    std::string ResourceStore::safeSegment(std::string_view text)
    {
        std::string segment;
        segment.reserve(text.size());
        for (const char c : text)
        {
            const auto byte = static_cast<unsigned char>(c);
            const bool reserved = byte < 0x20 || c == '/' || c == '\\' || c == ':' || c == '*' || c == '?'
                || c == '"' || c == '<' || c == '>' || c == '|';
            segment += reserved ? '_' : c;
        }
        // Windows drops trailing dots and spaces from names, which would merge distinct segments.
        while (!segment.empty() && (segment.back() == '.' || segment.back() == ' '))
        {
            segment.back() = '_';
        }
        if (segment.empty())
        {
            segment = "_";
        }
        return segment;
    }

    std::string ResourceStore::featureCataloguePath(std::string_view product, std::string_view version, std::string_view fileName)
    {
        return join({ "FC", product, version, fileName });
    }

    std::string ResourceStore::portrayalCatalogueDirectory(std::string_view product, std::string_view version)
    {
        return join({ "PC", product, version });
    }

    std::string ResourceStore::datasetDirectory(std::string_view product, std::string_view name)
    {
        return join({ "DATASET", product, name });
    }

    std::string ResourceStore::datasetFilePath(std::string_view product, std::string_view name, std::string_view fileName)
    {
        return join({ "DATASET", product, name, fileName });
    }

    std::string ResourceStore::supportFilePath(std::string_view product, std::string_view fileName)
    {
        return join({ "SUPPORT", product, fileName });
    }

    fs::path ResourceStore::absolute(std::string_view relative) const
    {
        const fs::path path = fromUtf8(relative);
        if (path.empty() || path.has_root_name() || path.has_root_directory())
        {
            throw std::invalid_argument("not a path relative to the System Database root: " + std::string(relative));
        }
        for (const fs::path& part : path)
        {
            if (part == "..")
            {
                throw std::invalid_argument("path leaves the System Database root: " + std::string(relative));
            }
        }
        return (root_ / path).lexically_normal();
    }

    bool ResourceStore::exists(std::string_view relative) const
    {
        std::error_code error;
        return fs::exists(absolute(relative), error);
    }

    std::string ResourceStore::copyFile(const fs::path& source, std::string_view relative)
    {
        const fs::path target = absolute(relative);
        fs::create_directories(target.parent_path());
        const fs::path partial = partialFile(target);
        fs::copy_file(source, partial, fs::copy_options::overwrite_existing);
        try
        {
            const std::string digest = sha256File(partial);
            fs::rename(partial, target);
            return digest;
        }
        catch (...)
        {
            std::error_code ignored;
            fs::remove(partial, ignored);
            throw;
        }
    }

    std::vector<std::string> ResourceStore::copyDirectory(const fs::path& source, std::string_view relativeDirectory)
    {
        const fs::path target = absolute(relativeDirectory);
        fs::remove_all(target);
        fs::create_directories(target);

        std::vector<std::string> copied;
        for (const auto& entry : fs::recursive_directory_iterator(source))
        {
            const fs::path relativeToSource = entry.path().lexically_relative(source);
            const fs::path destination = target / relativeToSource;
            if (entry.is_directory())
            {
                fs::create_directories(destination);
            }
            else if (entry.is_regular_file())
            {
                fs::create_directories(destination.parent_path());
                fs::copy_file(entry.path(), destination, fs::copy_options::overwrite_existing);
                copied.push_back(toGenericUtf8(fromUtf8(relativeDirectory) / relativeToSource));
            }
        }
        std::ranges::sort(copied);
        return copied;
    }

    void ResourceStore::move(std::string_view fromRelative, std::string_view toRelative)
    {
        const fs::path from = absolute(fromRelative);
        const fs::path to = absolute(toRelative);
        fs::create_directories(to.parent_path());
        if (fs::is_directory(from))
        {
            fs::remove_all(to);
        }
        fs::rename(from, to);
        pruneEmptyParents(from.parent_path());
    }

    bool ResourceStore::remove(std::string_view relative)
    {
        const fs::path target = absolute(relative);
        const bool removed = fs::remove_all(target) > 0;
        pruneEmptyParents(target.parent_path());
        return removed;
    }

    void ResourceStore::pruneEmptyParents(fs::path directory) const
    {
        const fs::path root = root_.lexically_normal();
        std::error_code error;
        for (;;)
        {
            // Stop at the root and at anything outside it.
            const fs::path below = directory.lexically_normal().lexically_relative(root);
            if (below.empty() || below == "." || *below.begin() == "..")
            {
                return;
            }
            if (!fs::is_directory(directory, error) || !fs::is_empty(directory, error))
            {
                return;
            }
            fs::remove(directory, error);
            if (error)
            {
                return;
            }
            directory = directory.parent_path();
        }
    }
}
