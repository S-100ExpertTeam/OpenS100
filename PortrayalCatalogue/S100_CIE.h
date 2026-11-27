#pragma once

#include <variant>

#include "S100_CIExyL.h"
#include "S100_CIEXYZ.h"

//#include <pugixml.hpp>

using S100_CIE = std::variant<S100_CIExyL, S100_CIEXYZ>;



