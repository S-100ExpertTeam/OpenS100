# Coding Conventions

## String Management

Strings are stored internally as `std::string` (UTF-8) inside classes.

### Encoding

| Type | Encoding |
|------|----------|
| `std::string` | UTF-8 |
| `std::wstring` | UTF-16 |

### Getter / Setter Rules

- Both `string` and `wstring` getter/setter variants must be provided.
- Getters that return `wstring` must have a `W` suffix in their name.
- Setters are provided as overloads for both types.

### Example

```cpp
// Getter
std::string  getName()  const;   // UTF-8
std::wstring getNameW() const;   // UTF-16

// Setter (overload)
void setName(const std::string&  name);  // UTF-8
void setName(const std::wstring& name);  // UTF-16
```

## Modern C++

New code and any code being touched follows the C++20 rules in
`Documents/TechDocs/DIRECTIVES.md` item 25: smart pointers instead of raw `new`/`delete`,
`std::string`/`std::string_view`/`std::filesystem::path` instead of `CString`, `std::span` and
`std::optional` at boundaries, `enum class`, `constexpr`, `[[nodiscard]]`, `override`/`explicit`,
no C-style casts or macros, standard containers instead of `CMap`/`CArray`/`CList`.
Existing code is migrated function by function as it is touched, not in one sweep.
