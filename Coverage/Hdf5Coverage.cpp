#include "Hdf5Coverage.h"

#include <hdf5.h>

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cmath>
#include <cstring>
#include <format>
#include <limits>
#include <map>
#include <stdexcept>

namespace s100::coverage
{
    static_assert(sizeof(hid_t) == sizeof(std::int64_t), "hid_t is kept as std::int64_t in the header");

    namespace
    {
        constexpr std::size_t ValueCacheSize = 48;

        // --- HDF5 handles and errors ---------------------------------------------------------

        // Closes an HDF5 identifier when it goes out of scope.
        class Handle
        {
        public:
            using Closer = herr_t (*)(hid_t);
            Handle() = default;
            Handle(hid_t id, Closer closer) noexcept : id_(id), closer_(closer) {}
            Handle(const Handle&) = delete;
            Handle& operator=(const Handle&) = delete;
            Handle(Handle&& other) noexcept : id_(std::exchange(other.id_, H5I_INVALID_HID)), closer_(other.closer_) {}
            Handle& operator=(Handle&& other) noexcept
            {
                if (this != &other)
                {
                    reset();
                    id_ = std::exchange(other.id_, H5I_INVALID_HID);
                    closer_ = other.closer_;
                }
                return *this;
            }
            ~Handle() { reset(); }

            [[nodiscard]] hid_t get() const noexcept { return id_; }
            [[nodiscard]] explicit operator bool() const noexcept { return id_ >= 0; }

        private:
            void reset() noexcept
            {
                if (id_ >= 0 && closer_)
                {
                    closer_(id_);
                }
                id_ = H5I_INVALID_HID;
            }

            hid_t id_ = H5I_INVALID_HID;
            Closer closer_ = nullptr;
        };

        Handle group(hid_t location, const std::string& path) { return { H5Gopen2(location, path.c_str(), H5P_DEFAULT), H5Gclose }; }
        Handle openDataset(hid_t location, const std::string& path) { return { H5Dopen2(location, path.c_str(), H5P_DEFAULT), H5Dclose }; }
        Handle datatype(hid_t id) { return { id, H5Tclose }; }
        Handle dataspace(hid_t id) { return { id, H5Sclose }; }

        // The library prints every failed call to stderr by default; a missing attribute is an
        // answer here, not an error.
        class QuietErrors
        {
        public:
            QuietErrors() noexcept
            {
                H5Eget_auto2(H5E_DEFAULT, &handler_, &data_);
                H5Eset_auto2(H5E_DEFAULT, nullptr, nullptr);
            }
            QuietErrors(const QuietErrors&) = delete;
            QuietErrors& operator=(const QuietErrors&) = delete;
            ~QuietErrors() { H5Eset_auto2(H5E_DEFAULT, handler_, data_); }

        private:
            H5E_auto2_t handler_ = nullptr;
            void* data_ = nullptr;
        };

        // --- names and text ------------------------------------------------------------------

        std::string lower(std::string text)
        {
            std::ranges::transform(text, text.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            return text;
        }

        std::string trimmed(std::string text)
        {
            while (!text.empty() && (text.back() == '\0' || std::isspace(static_cast<unsigned char>(text.back()))))
            {
                text.pop_back();
            }
            std::size_t start = 0;
            while (start < text.size() && std::isspace(static_cast<unsigned char>(text[start])))
            {
                ++start;
            }
            return text.substr(start);
        }

        std::optional<double> parseNumber(std::string_view text)
        {
            while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) text.remove_prefix(1);
            while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back()))) text.remove_suffix(1);
            if (text.empty())
            {
                return std::nullopt;
            }
            if (text.front() == '+') text.remove_prefix(1);
            double value = 0;
            const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);
            if (error != std::errc() || end != text.data() + text.size())
            {
                return std::nullopt;
            }
            return value;
        }

        // Names compared with their digit runs as numbers: Group_2 before Group_10, as
        // localeCompare(..., { numeric: true }) orders them.
        bool naturalLess(const std::string& a, const std::string& b)
        {
            std::size_t i = 0;
            std::size_t j = 0;
            while (i < a.size() && j < b.size())
            {
                const bool da = std::isdigit(static_cast<unsigned char>(a[i])) != 0;
                const bool db = std::isdigit(static_cast<unsigned char>(b[j])) != 0;
                if (da && db)
                {
                    std::size_t ei = i;
                    std::size_t ej = j;
                    while (ei < a.size() && std::isdigit(static_cast<unsigned char>(a[ei]))) ++ei;
                    while (ej < b.size() && std::isdigit(static_cast<unsigned char>(b[ej]))) ++ej;
                    std::string_view na(a.data() + i, ei - i);
                    std::string_view nb(b.data() + j, ej - j);
                    while (na.size() > 1 && na.front() == '0') na.remove_prefix(1);
                    while (nb.size() > 1 && nb.front() == '0') nb.remove_prefix(1);
                    if (na.size() != nb.size()) return na.size() < nb.size();
                    if (na != nb) return na < nb;
                    i = ei;
                    j = ej;
                    continue;
                }
                const char ca = static_cast<char>(std::tolower(static_cast<unsigned char>(a[i])));
                const char cb = static_cast<char>(std::tolower(static_cast<unsigned char>(b[j])));
                if (ca != cb) return ca < cb;
                ++i;
                ++j;
            }
            return a.size() - i < b.size() - j;
        }

        // --- groups and attributes -----------------------------------------------------------

        std::vector<std::string> childNames(hid_t location)
        {
            std::vector<std::string> names;
            H5Literate2(location, H5_INDEX_NAME, H5_ITER_NATIVE, nullptr,
                [](hid_t, const char* name, const H5L_info2_t*, void* data) -> herr_t {
                    static_cast<std::vector<std::string>*>(data)->emplace_back(name);
                    return 0;
                }, &names);
            return names;
        }

        bool isGroup(hid_t location, const std::string& name)
        {
            H5O_info2_t info{};
            return H5Oget_info_by_name3(location, name.c_str(), &info, H5O_INFO_BASIC, H5P_DEFAULT) >= 0 && info.type == H5O_TYPE_GROUP;
        }

        std::vector<std::string> attributeNames(hid_t object)
        {
            std::vector<std::string> names;
            H5Aiterate2(object, H5_INDEX_NAME, H5_ITER_NATIVE, nullptr,
                [](hid_t, const char* name, const H5A_info_t*, void* data) -> herr_t {
                    static_cast<std::vector<std::string>*>(data)->emplace_back(name);
                    return 0;
                }, &names);
            return names;
        }

        // Every element of a string attribute or dataset (fixed or variable length).
        template <typename Read>
        std::optional<std::vector<std::string>> readStrings(hid_t type, hid_t space, Read&& read)
        {
            const hssize_t count = H5Sget_simple_extent_npoints(space);
            if (count < 0)
            {
                return std::nullopt;
            }
            std::vector<std::string> values;
            if (H5Tis_variable_str(type) > 0)
            {
                const Handle memory = datatype(H5Tcopy(H5T_C_S1));
                H5Tset_size(memory.get(), H5T_VARIABLE);
                H5Tset_cset(memory.get(), H5Tget_cset(type));
                std::vector<char*> buffer(static_cast<std::size_t>(count), nullptr);
                if (read(memory.get(), buffer.data()) < 0)
                {
                    return std::nullopt;
                }
                for (char* text : buffer)
                {
                    values.push_back(trimmed(text ? std::string(text) : std::string()));
                    H5free_memory(text);
                }
                return values;
            }
            const std::size_t size = H5Tget_size(type);
            const Handle memory = datatype(H5Tcopy(H5T_C_S1));
            H5Tset_size(memory.get(), size);
            H5Tset_strpad(memory.get(), H5T_STR_NULLPAD);
            std::vector<char> buffer(static_cast<std::size_t>(count) * size);
            if (read(memory.get(), buffer.data()) < 0)
            {
                return std::nullopt;
            }
            for (hssize_t i = 0; i < count; ++i)
            {
                values.push_back(trimmed(std::string(buffer.data() + static_cast<std::size_t>(i) * size, size)));
            }
            return values;
        }

        // Every element of a numeric attribute (integer, float or enumeration) as double.
        std::optional<std::vector<double>> readAttributeNumbers(hid_t attribute)
        {
            const Handle type = datatype(H5Aget_type(attribute));
            const Handle space = dataspace(H5Aget_space(attribute));
            const H5T_class_t typeClass = H5Tget_class(type.get());
            const hssize_t count = H5Sget_simple_extent_npoints(space.get());
            if (count < 0 || (typeClass != H5T_INTEGER && typeClass != H5T_FLOAT && typeClass != H5T_ENUM))
            {
                return std::nullopt;
            }
            std::vector<double> values(static_cast<std::size_t>(count));
            if (typeClass != H5T_ENUM)
            {
                if (H5Aread(attribute, H5T_NATIVE_DOUBLE, values.data()) < 0) return std::nullopt;
                return values;
            }
            // An enumeration is read as the integers it is made of.
            const Handle base = datatype(H5Tget_super(type.get()));
            const Handle native = datatype(H5Tget_native_type(base.get(), H5T_DIR_ASCEND));
            const std::size_t size = H5Tget_size(native.get());
            const bool isSigned = H5Tget_sign(native.get()) == H5T_SGN_2;
            std::vector<unsigned char> raw(static_cast<std::size_t>(count) * size);
            if (H5Aread(attribute, native.get(), raw.data()) < 0) return std::nullopt;
            for (std::size_t i = 0; i < values.size(); ++i)
            {
                const unsigned char* at = raw.data() + i * size;
                std::int64_t value = 0;
                switch (size)
                {
                case 1: value = isSigned ? static_cast<std::int8_t>(*at) : static_cast<std::int64_t>(*at); break;
                case 2: { std::int16_t v; std::memcpy(&v, at, 2); value = isSigned ? v : static_cast<std::uint16_t>(v); break; }
                case 4: { std::int32_t v; std::memcpy(&v, at, 4); value = isSigned ? v : static_cast<std::uint32_t>(v); break; }
                default: { std::int64_t v; std::memcpy(&v, at, 8); value = v; break; }
                }
                values[i] = static_cast<double>(value);
            }
            return values;
        }

        struct Attribute
        {
            std::optional<std::vector<double>> numbers;
            std::optional<std::vector<std::string>> strings;

            [[nodiscard]] std::optional<double> number() const
            {
                if (numbers && numbers->size() == 1) return numbers->front();
                if (strings && strings->size() == 1) return parseNumber(strings->front());
                return std::nullopt;
            }
            [[nodiscard]] std::optional<std::string> text() const
            {
                if (strings && strings->size() == 1) return strings->front();
                return std::nullopt;
            }
            // The value kept in CoverageDataset::metadata: a scalar as it is, an array joined by ", ".
            [[nodiscard]] std::optional<MetadataValue> scalar() const
            {
                if (numbers && numbers->size() == 1) return numbers->front();
                if (strings && strings->size() == 1) return strings->front();
                std::string joined;
                if (numbers && !numbers->empty())
                {
                    for (const double value : *numbers) joined += (joined.empty() ? "" : ", ") + std::format("{}", value);
                    return joined;
                }
                if (strings && !strings->empty())
                {
                    for (const auto& value : *strings) joined += (joined.empty() ? "" : ", ") + value;
                    return joined;
                }
                return std::nullopt;
            }
        };

        using Attributes = std::map<std::string, Attribute, std::less<>>;

        Attributes readAttributes(hid_t object)
        {
            Attributes attributes;
            for (const std::string& name : attributeNames(object))
            {
                const Handle attribute{ H5Aopen(object, name.c_str(), H5P_DEFAULT), H5Aclose };
                if (!attribute)
                {
                    continue;
                }
                Attribute value;
                const Handle type = datatype(H5Aget_type(attribute.get()));
                if (H5Tget_class(type.get()) == H5T_STRING)
                {
                    const Handle space = dataspace(H5Aget_space(attribute.get()));
                    value.strings = readStrings(type.get(), space.get(),
                        [&](hid_t memory, void* buffer) { return H5Aread(attribute.get(), memory, buffer); });
                }
                else
                {
                    value.numbers = readAttributeNumbers(attribute.get());
                }
                if (value.numbers || value.strings)
                {
                    attributes.emplace(name, std::move(value));
                }
            }
            return attributes;
        }

        std::optional<double> numberOf(const Attributes& attributes, std::string_view name)
        {
            const auto found = attributes.find(name);
            return found == attributes.end() ? std::nullopt : found->second.number();
        }

        std::optional<std::string> textOf(const Attributes& attributes, std::string_view name)
        {
            const auto found = attributes.find(name);
            return found == attributes.end() ? std::nullopt : found->second.text();
        }

        // --- datasets ------------------------------------------------------------------------

        std::vector<hsize_t> shapeOf(hid_t data)
        {
            const Handle space = dataspace(H5Dget_space(data));
            const int rank = H5Sget_simple_extent_ndims(space.get());
            std::vector<hsize_t> dims(static_cast<std::size_t>(rank > 0 ? rank : 0));
            if (rank > 0)
            {
                H5Sget_simple_extent_dims(space.get(), dims.data(), nullptr);
            }
            return dims;
        }

        std::vector<std::string> memberNames(hid_t type)
        {
            std::vector<std::string> names;
            if (H5Tget_class(type) != H5T_COMPOUND)
            {
                return names;
            }
            const int count = H5Tget_nmembers(type);
            for (int i = 0; i < count; ++i)
            {
                char* name = H5Tget_member_name(type, static_cast<unsigned>(i));
                names.emplace_back(name ? name : "");
                H5free_memory(name);
            }
            return names;
        }

        // One string member of every row of a compound dataset (Group_F); std::nullopt when the
        // member is missing or not a string.
        std::optional<std::vector<std::string>> readStringMember(hid_t data, hid_t fileType, const std::string& member)
        {
            const int index = H5Tget_member_index(fileType, member.c_str());
            if (index < 0)
            {
                return std::nullopt;
            }
            const Handle memberType = datatype(H5Tget_member_type(fileType, static_cast<unsigned>(index)));
            if (H5Tget_class(memberType.get()) != H5T_STRING)
            {
                return std::nullopt;
            }
            const Handle space = dataspace(H5Dget_space(data));
            const bool variable = H5Tis_variable_str(memberType.get()) > 0;
            return readStrings(memberType.get(), space.get(), [&](hid_t memory, void* buffer) {
                const Handle row = datatype(H5Tcreate(H5T_COMPOUND, variable ? sizeof(char*) : H5Tget_size(memory)));
                H5Tinsert(row.get(), member.c_str(), 0, memory);
                return H5Dread(data, row.get(), H5S_ALL, H5S_ALL, H5P_DEFAULT, buffer);
            });
        }

        struct Description
        {
            std::string code;
            std::string name;
            std::string uom;
            std::optional<double> fillValue;
            std::optional<double> lower;
            std::optional<double> upper;
        };

        // Group_F rows, keyed by lower-cased code.
        std::map<std::string, Description> readFeatureDescriptions(hid_t groupF)
        {
            std::map<std::string, Description> descriptions;
            for (const std::string& key : childNames(groupF))
            {
                if (key == "featureCode")
                {
                    continue;
                }
                const Handle table = openDataset(groupF, key);
                if (!table)
                {
                    continue;
                }
                const Handle type = datatype(H5Dget_type(table.get()));
                if (H5Tget_class(type.get()) != H5T_COMPOUND)
                {
                    continue;
                }
                const auto codes = readStringMember(table.get(), type.get(), "code");
                if (!codes)
                {
                    continue;
                }
                const auto column = [&](const char* name) { return readStringMember(table.get(), type.get(), name); };
                const auto names = column("name");
                const auto uoms = column("uom.name");
                const auto fills = column("fillValue");
                const auto lowers = column("lower");
                const auto uppers = column("upper");
                const auto cell = [](const std::optional<std::vector<std::string>>& values, std::size_t row) -> std::optional<std::string> {
                    if (!values || row >= values->size()) return std::nullopt;
                    return (*values)[row];
                };
                for (std::size_t row = 0; row < codes->size(); ++row)
                {
                    const std::string& code = (*codes)[row];
                    if (code.empty())
                    {
                        continue;
                    }
                    Description description{ .code = code };
                    const auto name = cell(names, row);
                    description.name = name && !name->empty() ? *name : code;
                    description.uom = cell(uoms, row).value_or("");
                    if (const auto text = cell(fills, row)) description.fillValue = parseNumber(*text);
                    if (const auto text = cell(lowers, row)) description.lower = parseNumber(*text);
                    if (const auto text = cell(uppers, row)) description.upper = parseNumber(*text);
                    descriptions[lower(code)] = std::move(description);
                }
            }
            return descriptions;
        }

        // S-100 Part 10c Table 10c-4, for a writer that stores the name rather than the number.
        std::optional<int> codingFormat(const Attributes& attributes)
        {
            const auto found = attributes.find("dataCodingFormat");
            if (found == attributes.end())
            {
                return std::nullopt;
            }
            if (const auto number = found->second.number())
            {
                return static_cast<int>(*number);
            }
            static constexpr std::string_view names[] = { "fixedstations", "regulargrid", "ungeorectifiedgrid", "movingplatform",
                "irregulargrid", "variablecellsize", "tin", "stationwisefixed" };
            if (const auto text = found->second.text())
            {
                const std::string name = lower(*text);
                for (std::size_t i = 0; i < std::size(names); ++i)
                {
                    if (names[i] == name) return static_cast<int>(i + 1);
                }
            }
            return std::nullopt;
        }

        // Where a cell's data point lies from the grid point, in cells along each axis (Part 10c
        // 10c-9.6.1): dataOffsetVector, else dataOffsetCode 2 (1,1), 3 (1,0), 4 (0,1), 5 (0.5,0.5).
        GridIndex dataPointOffset(const Attributes& instance, const Attributes& container)
        {
            const auto pick = [&](std::string_view name) -> const Attribute* {
                if (const auto found = instance.find(name); found != instance.end()) return &found->second;
                if (const auto found = container.find(name); found != container.end()) return &found->second;
                return nullptr;
            };
            if (const Attribute* vector = pick("dataOffsetVector"); vector && vector->numbers && vector->numbers->size() >= 2)
            {
                return GridIndex{ (*vector->numbers)[0], (*vector->numbers)[1] };
            }
            const Attribute* code = pick("dataOffsetCode");
            switch (code && code->number() ? static_cast<int>(*code->number()) : 1)
            {
            case 2: return { 1, 1 };
            case 3: return { 1, 0 };
            case 4: return { 0, 1 };
            case 5: return { 0.5, 0.5 };
            default: return { 0, 0 };
            }
        }

        // minDataset<X>/maxDataset<X> pairs of a feature container, keyed by lower-cased X.
        std::map<std::string, ValueRange> datasetRanges(const Attributes& attributes)
        {
            std::map<std::string, ValueRange> ranges;
            constexpr std::string_view prefix = "minDataset";
            for (const auto& [key, value] : attributes)
            {
                if (!key.starts_with(prefix) || key.size() == prefix.size())
                {
                    continue;
                }
                const std::string what = key.substr(prefix.size());
                const auto min = value.number();
                const auto max = numberOf(attributes, "maxDataset" + what);
                if (min && max && *min <= *max)
                {
                    ranges[lower(what)] = ValueRange{ *min, *max };
                }
            }
            return ranges;
        }

        // Equal within float32 rounding: the fill value is text in Group_F and a float elsewhere.
        bool nearly(double a, double b)
        {
            return std::abs(a - b) <= 1e-6 * std::max(std::abs(a), std::abs(b));
        }

        bool endsWithDirection(const std::string& name)
        {
            const std::string text = lower(name);
            return text.ends_with("dir") || text.ends_with("direction");
        }

        // Reads `member` (or the whole value, when empty) of a values dataset as float32, all
        // rows, or the given depth level of a three-dimensional one.
        std::vector<float> readChannel(hid_t data, const std::string& member, std::size_t cells, const std::optional<hsize_t>& depthIndex)
        {
            std::vector<float> values(cells, std::numeric_limits<float>::quiet_NaN());
            Handle fileSpace = dataspace(H5Dget_space(data));
            Handle memorySpace;
            if (depthIndex)
            {
                const std::vector<hsize_t> shape = shapeOf(data);
                const hsize_t start[3] = { 0, 0, *depthIndex };
                const hsize_t count[3] = { shape[0], shape[1], 1 };
                H5Sselect_hyperslab(fileSpace.get(), H5S_SELECT_SET, start, nullptr, count, nullptr);
                const hsize_t flat = static_cast<hsize_t>(cells);
                memorySpace = dataspace(H5Screate_simple(1, &flat, nullptr));
            }
            const hid_t memSpace = depthIndex ? memorySpace.get() : H5S_ALL;
            const hid_t fileSel = depthIndex ? fileSpace.get() : H5S_ALL;
            if (member.empty())
            {
                if (H5Dread(data, H5T_NATIVE_FLOAT, memSpace, fileSel, H5P_DEFAULT, values.data()) < 0)
                {
                    std::ranges::fill(values, std::numeric_limits<float>::quiet_NaN());
                }
                return values;
            }
            const Handle row = datatype(H5Tcreate(H5T_COMPOUND, sizeof(float)));
            H5Tinsert(row.get(), member.c_str(), 0, H5T_NATIVE_FLOAT);
            if (H5Dread(data, row.get(), memSpace, fileSel, H5P_DEFAULT, values.data()) >= 0)
            {
                return values;
            }
            // An enumerated member the library will not convert to float: read its integers.
            const Handle fileType = datatype(H5Dget_type(data));
            const int index = H5Tget_member_index(fileType.get(), member.c_str());
            if (index < 0)
            {
                std::ranges::fill(values, std::numeric_limits<float>::quiet_NaN());
                return values;
            }
            const Handle memberType = datatype(H5Tget_member_type(fileType.get(), static_cast<unsigned>(index)));
            const Handle base = datatype(H5Tget_class(memberType.get()) == H5T_ENUM ? H5Tget_super(memberType.get()) : H5Tcopy(memberType.get()));
            const Handle native = datatype(H5Tget_native_type(base.get(), H5T_DIR_ASCEND));
            const Handle enumRow = datatype(H5Tcreate(H5T_COMPOUND, H5Tget_size(memberType.get())));
            H5Tinsert(enumRow.get(), member.c_str(), 0, memberType.get());
            std::vector<unsigned char> raw(cells * H5Tget_size(memberType.get()));
            if (H5Dread(data, enumRow.get(), memSpace, fileSel, H5P_DEFAULT, raw.data()) < 0)
            {
                std::ranges::fill(values, std::numeric_limits<float>::quiet_NaN());
                return values;
            }
            const std::size_t size = H5Tget_size(memberType.get());
            const bool isSigned = H5Tget_sign(native.get()) == H5T_SGN_2;
            for (std::size_t i = 0; i < cells; ++i)
            {
                const unsigned char* at = raw.data() + i * size;
                std::int64_t value = 0;
                switch (size)
                {
                case 1: value = isSigned ? static_cast<std::int8_t>(*at) : static_cast<std::int64_t>(*at); break;
                case 2: { std::int16_t v; std::memcpy(&v, at, 2); value = isSigned ? v : static_cast<std::uint16_t>(v); break; }
                case 4: { std::int32_t v; std::memcpy(&v, at, 4); value = isSigned ? v : static_cast<std::uint32_t>(v); break; }
                default: { std::int64_t v; std::memcpy(&v, at, 8); value = v; break; }
                }
                values[i] = static_cast<float>(value);
            }
            return values;
        }

        std::string utf8(const std::filesystem::path& path)
        {
            const std::u8string text = path.u8string();
            return std::string(text.begin(), text.end());
        }
    }

    std::unique_ptr<Hdf5Coverage> Hdf5Coverage::open(const std::filesystem::path& file)
    {
        const std::string name = utf8(file.filename());
        const auto fail = [&](const std::string& reason) {
            return std::runtime_error(std::format("{}: not a readable S-100 HDF5 dataset ({})", name, reason));
        };

        std::unique_ptr<Hdf5Coverage> coverage(new Hdf5Coverage());
        const std::lock_guard lock(coverage->mutex_);
        const QuietErrors quiet;
        coverage->file_ = H5Fopen(utf8(file).c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
        if (coverage->file_ < 0)
        {
            throw fail("the file cannot be opened as HDF5");
        }

        CoverageDataset& dataset = coverage->dataset_;
        dataset.name = name;
        const Attributes root = readAttributes(coverage->file_);
        for (const auto& [key, value] : root)
        {
            if (auto scalar = value.scalar())
            {
                dataset.metadata.emplace(key, std::move(*scalar));
            }
        }
        dataset.productSpecification = textOf(root, "productSpecification").value_or("");
        auto productId = productIdFromSpecification(dataset.productSpecification);
        if (!productId && name.size() >= 3 && std::isdigit(static_cast<unsigned char>(name[0])) && std::isdigit(static_cast<unsigned char>(name[1]))
            && std::isdigit(static_cast<unsigned char>(name[2])))
        {
            productId = "S-" + name.substr(0, 3);
        }
        if (!productId)
        {
            throw fail("productSpecification is missing");
        }
        dataset.productId = *productId;
        dataset.title = textOf(root, "datasetName");

        coverage->readFeatures();
        if (dataset.features.empty())
        {
            std::string reasons;
            for (const auto& [code, reason] : coverage->skipped_)
            {
                reasons += (reasons.empty() ? "" : "; ") + code + ": " + reason;
            }
            throw fail(reasons.empty() ? std::string("no coverage features") : "no regular-grid coverage (" + reasons + ")");
        }

        // The stated bounds of a UTM dataset can fall short of its grid (NOAA's span two opposite
        // corners; the other two lie outside by the grid convergence): the cells' bounds instead.
        const bool utm = dataset.features.front().grid.utm.has_value();
        const auto west = numberOf(root, "westBoundLongitude");
        const auto east = numberOf(root, "eastBoundLongitude");
        const auto south = numberOf(root, "southBoundLatitude");
        const auto north = numberOf(root, "northBoundLatitude");
        if (!utm && west && east && south && north)
        {
            dataset.bounds = GeoBounds{ std::min(*west, *east), std::min(*south, *north), std::max(*west, *east), std::max(*south, *north) };
        }
        else
        {
            dataset.bounds = utm ? gridCellBounds(dataset.features.front().grid) : gridBounds(dataset.features.front().grid);
            for (const auto& feature : dataset.features)
            {
                const GeoBounds box = utm ? gridCellBounds(feature.grid) : gridBounds(feature.grid);
                dataset.bounds = GeoBounds{ std::min(dataset.bounds.minLon, box.minLon), std::min(dataset.bounds.minLat, box.minLat),
                    std::max(dataset.bounds.maxLon, box.maxLon), std::max(dataset.bounds.maxLat, box.maxLat) };
            }
        }
        return coverage;
    }

    Hdf5Coverage::~Hdf5Coverage()
    {
        if (file_ >= 0)
        {
            const std::lock_guard lock(mutex_);
            H5Fclose(file_);
        }
    }

    void Hdf5Coverage::readFeatures()
    {
        const hid_t file = file_;
        const Attributes root = readAttributes(file);
        const auto horizontalCrs = numberOf(root, "horizontalCRS");
        const std::optional<UtmZone> utm = horizontalCrs ? utmZoneFromEpsg(static_cast<int>(*horizontalCrs)) : std::nullopt;

        std::map<std::string, Description> descriptions;
        std::vector<std::string> codes;
        if (const Handle groupF = group(file, "Group_F"))
        {
            descriptions = readFeatureDescriptions(groupF.get());
            if (const Handle list = openDataset(groupF.get(), "featureCode"))
            {
                const Handle type = datatype(H5Dget_type(list.get()));
                const Handle space = dataspace(H5Dget_space(list.get()));
                if (H5Tget_class(type.get()) == H5T_STRING)
                {
                    if (auto listed = readStrings(type.get(), space.get(),
                            [&](hid_t memory, void* buffer) { return H5Dread(list.get(), memory, H5S_ALL, H5S_ALL, H5P_DEFAULT, buffer); }))
                    {
                        std::erase(*listed, std::string());
                        codes = std::move(*listed);
                    }
                }
            }
        }
        if (codes.empty())
        {
            for (const std::string& key : childNames(file))
            {
                if (key != "Group_F") codes.push_back(key);
            }
        }

        for (const std::string& code : codes)
        {
            const Handle featureGroup = group(file, code);
            if (!featureGroup)
            {
                continue;
            }
            const Attributes container = readAttributes(featureGroup.get());
            const auto coding = codingFormat(container);
            if (coding && *coding != 2)
            {
                skipped_.push_back({ code, std::format("dataCodingFormat {} is not supported", *coding) });
                continue;
            }
            std::vector<std::string> instances;
            for (const std::string& key : childNames(featureGroup.get()))
            {
                if (key.starts_with(code + ".")) instances.push_back(key);
            }
            std::ranges::sort(instances, naturalLess);
            const auto statedRanges = datasetRanges(container);
            const auto featureDescription = [&]() -> const Description* {
                const auto found = descriptions.find(lower(code));
                return found == descriptions.end() ? nullptr : &found->second;
            }();

            for (const std::string& instance : instances)
            {
                const std::string id = code + "/" + instance;
                const auto skip = [&](const std::string& reason) { skipped_.push_back({ id, reason }); };
                const Handle instanceGroup = group(file, id);
                if (!instanceGroup)
                {
                    continue;
                }
                const Attributes attributes = readAttributes(instanceGroup.get());

                std::vector<std::string> timeGroupNames;
                for (const std::string& key : childNames(instanceGroup.get()))
                {
                    const std::string name = lower(key);
                    if (name.starts_with("group_") && name.size() > 6
                        && std::all_of(name.begin() + 6, name.end(), [](unsigned char c) { return std::isdigit(c) != 0; })
                        && isGroup(instanceGroup.get(), key))
                    {
                        timeGroupNames.push_back(key);
                    }
                }
                if (timeGroupNames.empty())
                {
                    skip("no value groups");
                    continue;
                }
                std::ranges::sort(timeGroupNames, naturalLess);

                CoverageFeature feature{ .id = id, .code = code };
                for (const std::string& name : timeGroupNames)
                {
                    CoverageTimeGroup timeGroup{ .name = name };
                    if (const Handle timeGroupHandle = group(instanceGroup.get(), name))
                    {
                        const Attributes groupAttributes = readAttributes(timeGroupHandle.get());
                        for (const char* key : { "validTime", "timePoint", "forecastTime" })
                        {
                            const auto text = textOf(groupAttributes, key);
                            const auto time = text ? parseS100DateTime(*text) : std::nullopt;
                            // S-102 writes 00010101T000000Z where a coverage has no time.
                            if (time && std::chrono::year_month_day{ std::chrono::floor<std::chrono::days>(*time) }.year() > std::chrono::year{ 1 })
                            {
                                timeGroup.time = time;
                                break;
                            }
                        }
                    }
                    feature.timeGroups.push_back(std::move(timeGroup));
                }

                const Handle sample = openDataset(instanceGroup.get(), timeGroupNames.front() + "/values");
                if (!sample)
                {
                    skip("values dataset missing");
                    continue;
                }
                const std::vector<hsize_t> shape = shapeOf(sample.get());
                if (shape.size() != 2 && shape.size() != 3)
                {
                    skip(std::format("values have rank {}", shape.size()));
                    continue;
                }
                const auto rows = numberOf(attributes, "numPointsLatitudinal").value_or(static_cast<double>(shape[0]));
                const auto cols = numberOf(attributes, "numPointsLongitudinal").value_or(static_cast<double>(shape[1]));
                if (static_cast<double>(shape[0]) != rows || static_cast<double>(shape[1]) != cols)
                {
                    skip(std::format("values shape {}x{} does not match {}x{} grid", shape[0], shape[1], rows, cols));
                    continue;
                }
                const auto originLon = numberOf(attributes, "gridOriginLongitude");
                const auto originLat = numberOf(attributes, "gridOriginLatitude");
                const auto spacingLon = numberOf(attributes, "gridSpacingLongitudinal");
                const auto spacingLat = numberOf(attributes, "gridSpacingLatitudinal");
                if (!originLon || !originLat || !spacingLon || !spacingLat || *spacingLon == 0 || *spacingLat == 0)
                {
                    skip("grid origin or spacing missing");
                    continue;
                }
                // Eastings and northings read as degrees would put the grid off the chart without a word.
                if (!utm && (std::abs(*originLon) > 360 || std::abs(*originLat) > 90))
                {
                    skip(std::format("horizontalCRS EPSG:{} is not supported", horizontalCrs ? std::to_string(static_cast<int>(*horizontalCrs)) : "?"));
                    continue;
                }
                // The nodes are where the values are: KHOA's dataOffsetCode 5 moves them half a cell.
                const GridIndex offset = dataPointOffset(attributes, container);
                feature.grid = RegularGrid{ .originLon = *originLon + offset.col * *spacingLon, .originLat = *originLat + offset.row * *spacingLat,
                    .spacingLon = *spacingLon, .spacingLat = *spacingLat, .cols = static_cast<int>(cols), .rows = static_cast<int>(rows), .utm = utm };

                if (shape.size() == 3)
                {
                    std::vector<double> levels;
                    if (const Handle levelData = openDataset(instanceGroup.get(), timeGroupNames.front() + "/depthLevels"))
                    {
                        const hsize_t count = shapeOf(levelData.get()).empty() ? 0 : shapeOf(levelData.get())[0];
                        levels.resize(static_cast<std::size_t>(count));
                        if (H5Dread(levelData.get(), H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, levels.data()) < 0) levels.clear();
                    }
                    if (levels.empty())
                    {
                        if (const auto found = root.find("depthLevels"); found != root.end() && found->second.numbers) levels = *found->second.numbers;
                    }
                    if (levels.size() != shape[2])
                    {
                        levels.resize(static_cast<std::size_t>(shape[2]));
                        for (std::size_t i = 0; i < levels.size(); ++i) levels[i] = static_cast<double>(i);
                    }
                    feature.depthLevels = std::move(levels);
                }

                const Handle sampleType = datatype(H5Dget_type(sample.get()));
                const std::vector<std::string> members = memberNames(sampleType.get());
                const Attributes sampleAttributes = readAttributes(sample.get());
                const auto valueFill = numberOf(sampleAttributes, "_FillValue");
                const auto component = [&](const std::string& componentName, const Description* description) {
                    CoverageComponent result;
                    result.name = componentName;
                    result.label = description ? description->name : componentName;
                    result.uom = description ? description->uom : std::string();
                    result.fillValue = description && description->fillValue ? *description->fillValue
                        : valueFill.value_or(std::numeric_limits<double>::quiet_NaN());
                    result.lower = description ? description->lower : std::nullopt;
                    result.upper = description ? description->upper : std::nullopt;
                    // The stated range that names the component (waterLevelHeight ends in height),
                    // unless the writer folded its fill value into it or it leaves the bounds.
                    const std::string lowered = lower(componentName);
                    for (const auto& [what, range] : statedRanges)
                    {
                        if (!lowered.ends_with(what)) continue;
                        if (!nearly(range.min, result.fillValue) && !nearly(range.max, result.fillValue)
                            && (!result.lower || range.min >= *result.lower) && (!result.upper || range.max <= *result.upper))
                        {
                            result.datasetRange = range;
                        }
                        break;
                    }
                    return result;
                };
                if (!members.empty())
                {
                    for (const std::string& member : members)
                    {
                        const auto own = descriptions.find(lower(member));
                        const Description* description = own != descriptions.end() ? &own->second : featureDescription;
                        CoverageComponent shared = component(member, description);
                        // A compound feature described by one row carries one unit for all members;
                        // directions are always in degrees.
                        if (own == descriptions.end() && endsWithDirection(member)) shared.uom = "degrees";
                        feature.components.push_back(std::move(shared));
                    }
                }
                else
                {
                    feature.components.push_back(component(featureDescription ? featureDescription->code : code, featureDescription));
                }
                feature.compound = !members.empty();
                feature.label = featureDescription ? featureDescription->name : code;
                dataset_.features.push_back(std::move(feature));
            }
        }
    }

    Values Hdf5Coverage::readValues(std::string_view featureId, const CoverageSelection& selection) const
    {
        const CoverageFeature* feature = dataset_.feature(featureId);
        if (!feature || selection.timeIndex >= feature->timeGroups.size() || selection.component >= feature->components.size())
        {
            return nullptr;
        }
        const std::size_t depthIndex = feature->depthLevels ? selection.depthIndex : 0;
        if (feature->depthLevels && depthIndex >= feature->depthLevels->size())
        {
            return nullptr;
        }
        const std::string& groupName = feature->timeGroups[selection.timeIndex].name;
        const auto key = [&](std::size_t component) { return std::format("{}|{}|{}|{}", featureId, groupName, depthIndex, component); };

        const std::lock_guard lock(mutex_);
        const std::string wanted = key(selection.component);
        if (const auto found = std::ranges::find(cache_, wanted, &std::pair<std::string, Values>::first); found != cache_.end())
        {
            cache_.splice(cache_.begin(), cache_, found);
            return cache_.front().second;
        }

        const QuietErrors quiet;
        const Handle data = openDataset(file_, std::string(featureId) + "/" + groupName + "/values");
        if (!data)
        {
            return nullptr;
        }
        const std::size_t cells = feature->grid.nodeCount();
        const std::optional<hsize_t> depth = feature->depthLevels ? std::optional<hsize_t>(depthIndex) : std::nullopt;
        // A compound read decodes every member anyway: keep them all (direction and speed, depth
        // and uncertainty are wanted together).
        Values result;
        for (std::size_t index = 0; index < feature->components.size(); ++index)
        {
            if (!feature->compound && index > 0)
            {
                break;
            }
            auto values = std::make_shared<const std::vector<float>>(readChannel(data.get(), feature->compound ? feature->components[index].name : std::string(), cells, depth));
            if (index == selection.component)
            {
                result = values;
            }
            cache_.emplace_front(key(index), std::move(values));
        }
        while (cache_.size() > ValueCacheSize)
        {
            cache_.pop_back();
        }
        return result;
    }
}
