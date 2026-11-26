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

// No header of an operating system here: the sources are the same on every platform, and
// the few that call Windows include <windows.h> themselves, inside #ifdef _WIN32.

#include "Common.h"
