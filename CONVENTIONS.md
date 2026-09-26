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
