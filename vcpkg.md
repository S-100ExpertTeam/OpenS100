# vcpkg for OpenS100 (MFC)
vcpkg is C/C++ dependecy manager from Microsoft for all platforms, buildsystems, and workflows  
[vcpkg website](https://vcpkg.io/)

# Quick start (Manifest mode + Binary cache)

This repository uses vcpkg manifest mode and the system `VCPKG_DEFAULT_BINARY_CACHE` via `vcpkg-configuration.json`.

- `vcpkg.json` lists all dependencies; no per-package installs are needed.
- `vcpkg-configuration.json` points to `${env:VCPKG_DEFAULT_BINARY_CACHE}` so your system cache is used.

Setup
- Choose a cache directory, e.g. `D:\vcpkg-cache`.
- Set the environment variable (PowerShell, current user):
  ```powershell
  [System.Environment]::SetEnvironmentVariable('VCPKG_DEFAULT_BINARY_CACHE','D:\vcpkg-cache','User')
  ```
- Restart Visual Studio or your terminal after setting it.

Visual Studio (MSBuild/MFC)
- Open the solution and build (e.g., `x64`).
- Packages will auto-restore from `vcpkg.json` using your binary cache.

Command line (alternative)
```cmd
> vcpkg install --triplet x64-windows
```
This reads `vcpkg.json` and restores dependencies using the binary cache.

# About vcpkg
vcpkg is a free C/C++ package manager for acquiring and managing libraries. Choose from over 1500 open source libraries to download and build in a single step or add your own private libraries to simplify your build process. Maintained by the Microsoft C++ team and open source contributors.

# Reference
- [Get started with vcpkg](https://vcpkg.io/en/getting-started.html)

# Install vcpkg
{{ ... }}
```cmd
> cd vcpkg
> .\bootstrap-vcpkg.bat
``` 

## Step 3: Restore packages for OpenS100 (manifest mode)
```cmd
> vcpkg install --triplet x64-windows
```

## Using vcpkg with MSBuild / Visual Studio
```cmd
> vcpkg integrate install
```
After this, you can create a new project or open an existing one in the IDE. All installed libraries should already be discoverable by IntelliSense and usable in code without additional configuration.
