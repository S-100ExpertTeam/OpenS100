#pragma once

#include <variant>

#include "S100_CIExyL.h"
#include "S100_CIEXYZ.h"

//#include <pugixml.hpp>

using S100_CIE = std::variant<S100_CIExyL, S100_CIEXYZ>;



//class S100_CIE
//{
//public:
//	S100_CIE();
//	virtual ~S100_CIE();
//
//private:
//
//public:
//	void GetContents(pugi::xml_node Node);
//	
//	void SetX(float value);
//	float GetX();
//
//	void SetY(float value);
//	float GetY();
//
//	void SetL(float value);
//	float GetL();
//};