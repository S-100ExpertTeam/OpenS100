# Doxygen Documentation Guide

This project uses Doxygen to generate API documentation from source code comments.

## Prerequisites

Download and install Doxygen:
- **Windows**: Download from [doxygen.nl](https://www.doxygen.nl/download.html)
- Or via Chocolatey: `choco install doxygen.install`
- Or via vcpkg: `vcpkg install doxygen`

Optional: Install Graphviz for generating diagrams
- Download from [graphviz.org](https://graphviz.org/download/)
- Or via Chocolatey: `choco install graphviz`

## Generating Documentation

### Command Line

From the repository root:

```powershell
# Generate documentation
doxygen Doxyfile
```

This will create documentation in the `docs/html` directory.

### View Documentation

Open the generated documentation:

```powershell
# Open in default browser
start docs/html/index.html
```

Or navigate to `c:\Git\OpenS100\docs\html\index.html` in your browser.

## Documentation Style

This project uses Javadoc-style Doxygen comments:

### File Documentation
```cpp
/**
 * @file FileName.h
 * @brief Brief description
 * @details Detailed description
 */
```

### Function Documentation
```cpp
/**
 * @brief Brief description of the function
 * @param paramName Description of parameter
 * @param[out] outParam Description of output parameter
 * @param[in,out] inOutParam Description of in-out parameter
 * @return Description of return value
 * @note Additional notes
 * @warning Important warnings
 */
```

### Class/Struct Documentation
```cpp
/**
 * @class ClassName
 * @brief Brief description
 * @details Detailed description
 */
```

### Member Documentation
```cpp
double value;  ///< Brief description of member
```

## Configuration

The `Doxyfile` in the repository root contains all Doxygen settings:

- **INPUT**: Specifies source directories (current directory and `LatLonUtility`)
- **OUTPUT_DIRECTORY**: Documentation output location (`./docs`)
- **EXTRACT_ALL**: Extracts documentation even for undocumented items
- **SOURCE_BROWSER**: Includes source code in documentation
- **GENERATE_TREEVIEW**: Creates a navigation tree
- **RECURSIVE**: Processes subdirectories

## Customization

To modify documentation generation:

1. Edit `Doxyfile`
2. Common settings to adjust:
   - `PROJECT_NAME`: Project name
   - `PROJECT_NUMBER`: Version number
   - `OUTPUT_DIRECTORY`: Output location
   - `EXTRACT_PRIVATE`: Include private members
   - `HAVE_DOT`: Enable Graphviz diagrams (requires Graphviz)

## Excluding Files

To exclude specific files or directories, edit the `EXCLUDE_PATTERNS` in `Doxyfile`:

```
EXCLUDE_PATTERNS       = */vcpkg_installed/* \
                         */build/* \
                         */out/* \
                         */.vs/*
```

## CI/CD Integration

To generate documentation automatically in CI/CD:

```yaml
# Example for GitHub Actions
- name: Install Doxygen
  run: choco install doxygen.install -y

- name: Generate Documentation
  run: doxygen Doxyfile

- name: Deploy to GitHub Pages
  uses: peaceiris/actions-gh-pages@v3
  with:
    github_token: ${{ secrets.GITHUB_TOKEN }}
    publish_dir: ./docs/html
```

## References

- [Doxygen Manual](https://www.doxygen.nl/manual/)
- [Doxygen Special Commands](https://www.doxygen.nl/manual/commands.html)
- [Documenting the code](https://www.doxygen.nl/manual/docblocks.html)
