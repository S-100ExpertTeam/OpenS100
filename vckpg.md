# vckpg
vcpkg is C/C++ dependecy manager from Microsoft for all platforms, buildsystems, and workflows  
[vcpkg website](https://vcpkg.io/)

# About vcpkg
vcpkg is a free C/C++ package manager for acquiring and managing libraries. Choose from over 1500 open source libraries to download and build in a single step or add your own private libraries to simplify your build process. Maintained by the Microsoft C++ team and open source contributors.

# Reference
- [Get started with vcpkg](https://vcpkg.io/en/getting-started.html)

# Install vcpkg
Installing vcpkg is a two-step process: first, clone the repo, then run the bootstrapping script to produce the vcpkg binary. The repo can be cloned anywhere, and will include the vcpkg binary after bootstrapping as well as any libraries that are installed from the command line. It is recommended to clone vcpkg as a submodule for CMake projects, but to install it globally for MSBuild projects. If installing globally, we recommend a short install path like: C:\src\vcpkg or C:\dev\vcpkg, since otherwise you may run into path issues for some port build systems.

## Step 1: Clone the vcpkg repo
```cmd
> git clone https://github.com/microsoft/vcpkg.git
```
Make sure you are in the directory you want the tool installed to before doing this.

## Step 2: Run the bootstrap script to build vcpkg
```cmd
> cd vcpkg
> .\bootstrap-vcpkg.bat
``` 

## Step 3: Install packages used in OpenS100
```cmd
> vcpkg install pugixml:x64-windows
> vcpkg install geographiclib:x64-windows
> vcpkg install polyclipping:x64-windows
```
OpenS100 uses 3 packages.
- pugixml
- geographiclib
- polyclipping

## Using vcpkg with MSBuild / Visual Studio
```cmd
> vcpkg integrate install
```
After this, you can create a new project or open an existing one in the IDE. All installed libraries should already be discoverable by IntelliSense and usable in code without additional configuration.
