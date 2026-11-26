#pragma once

// Standard headers the sources of this project rely on through the precompiled header.
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

// On Windows the device-space types of the model (POINT, RECT: DeviceTypes.h) are the Win32
// ones, so <windows.h> is here for their definitions. Nothing else of the operating system
// is: no COM, no ATL. It goes when those types are the library's own.
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // keep commdlg.h and friends out (their function-name macros clash with model methods)
#endif
#include "targetver.h"
#include <windows.h>
#endif

#include "Common.h"
