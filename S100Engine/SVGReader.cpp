#include "stdafx.h"
#include "SVGReader.h"
#include "Rectangle.h"

#include <pugixml.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace simpleUse
{
	//Divide strings (separate strings using char characters)
	std::vector<CString> Split(CString _data, char parsingChar)
	{
		std::vector<CString> result;

		CString data = _data;
		while (data.GetLength() != 0)
		{
			data.TrimLeft(parsingChar);
			int nFind2 = data.Find(parsingChar);
			if (nFind2 == -1)
			{
				result.push_back(data);
				break;
			}
			CString strNum = data.Mid(0, nFind2);
			result.push_back(strNum);
			data = data.Mid(nFind2);
		}
		return result;
	}

	template<typename T>
	void split(const T& line, const T& separators, std::vector<T>& tokens)
	{
		size_t start = line.find_first_not_of(separators);
		size_t end = line.length();

		while ((start >= 0) && (start < end)) {
			size_t stop = line.find_first_of(separators, start);
			if ((stop < 0) || (stop > end)) stop = end;
			tokens.push_back(line.substr(start, stop - start));
			start = line.find_first_not_of(separators, stop + 1);
		}
	}


	//Divide strings (separate strings by specifying the number of characters)
	std::vector<CString> SplitBySpace(CString _data, int _space)
	{
		std::vector<CString> result;

		CString data = _data;
		for (int i = 0; i < _data.GetLength(); i += _space)
		{
			CString cs;
			for (int j = 0; j < _space; j++)
			{
				cs.AppendChar(_data[i + j]);
			}
			result.push_back(cs);
		}
		return result;
	}

	// Converting to hexadecimal to decimal.
	int Conert16to10(char* ch)
	{
		int result = strtol(ch, NULL, 16);
		return result;
	}

	//CSRING -> char* (please enter delete!)
	//char* ConvertCstringToChar(CString inCstring)
	//{
	//	char* result;
	//	wchar_t *wchar_str;
	//	int char_str_len;

	//	// 1. CString to wchar * conversion
	//	wchar_str = inCstring.GetBuffer(inCstring.GetLength());
	//	char_str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
	//	//char_str = new char[char_str_len];

	//	result = (char*)malloc(char_str_len);

	//	// 2. wchar_t* to char* conversion
	//	WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, result, char_str_len, 0, 0);

	//	//delete result;
	//	return result;
	//}

	//char* -> wstring
	std::wstring ConvertCharToWstring(char* inChar)
	{
		std::string str = inChar;
		std::wstring result;

		result.assign(str.begin(), str.end());
		return result;
	}

	//wchar_t to char.
	char* ConvertWCtoC(wchar_t* str)
	{
		//Declare the char* variable to return
		char* pStr = nullptr;

		// Calculates the length of the input wchar_t variable.
		int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

		if (strSize <= 0)
		{
			//OutputDebugString(_T("Failed to WideCharToMultiByte()\n"));
			return nullptr;
		}

		//char* memory allocation
		pStr = new char[strSize];

		//Changing the type.
		WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
		return pStr;
	}
}

#pragma region GetFunction
bool GetClass(char* attributeContent, bool& fill, std::wstring& colorName)
{
	std::wstring wsTemp = std::wstring(attributeContent, attributeContent + strlen(attributeContent));
	std::wstring temp;
	std::wstring wsColor;
	std::wstring wsFill;
	std::vector<std::wstring> wsVecTemp;

	if (wsTemp.size() == 6)
	{
		temp = wsTemp;
		wsColor = temp.substr(1, temp.size());
		wsFill = temp.substr(0, 1);
	}
	else
	{
		simpleUse::split<std::wstring>(std::wstring(attributeContent, attributeContent + strlen(attributeContent)),
			L" ",
			wsVecTemp);
		temp = wsVecTemp[wsVecTemp.size() - 1].c_str();
		wsColor = temp.substr(1, temp.size());
		wsFill = temp.substr(0, 1);
	}

	if (&colorName != nullptr)
	{
		colorName = wsColor;
	}
	else
	{
		return false;
	}

	if (&fill != nullptr)
	{
		if (!wcscmp(wsFill.c_str(), L"f"))
		{
			fill = true;
			return true;
		}
		else
		{
			fill = false;
			return true;
		}
	}
	else
	{
		return false;
	}

	return false;
}

bool GetRotation(char* attributeContent, int& rotation)
{
	try
	{
		std::vector<std::wstring> wsVecTemp;
		simpleUse::split<std::wstring>(std::wstring(attributeContent, attributeContent + strlen(attributeContent)), L"rotate ()", wsVecTemp);
		rotation = _wtoi(wsVecTemp[0].c_str());
	}
	catch (std::exception ex)
	{
		return false;
	}
	return true;
}

#pragma warning(disable:4244)
bool GetStyle(char* attributeContent, double& strokeWidth, std::wstring& strokeDasharray, float& alpha)
{
	std::vector<std::wstring> wsVecTemp;
	simpleUse::split<std::wstring>(std::wstring(attributeContent, attributeContent + strlen(attributeContent)), L":; ", wsVecTemp);
	for (int i = 0; i < (int)wsVecTemp.size(); i += 2)
	{
		const wchar_t* a = wsVecTemp[i].c_str();
		std::wstring sss = wsVecTemp[i + 1];
		if (!wcscmp(wsVecTemp[i].c_str(), L"stroke-width") && (&strokeWidth != nullptr))
		{
			strokeWidth = _wtof(wsVecTemp[i + 1].c_str());
		}
		else if (!wcscmp(wsVecTemp[i].c_str(), L"stroke-dasharray") && (&strokeDasharray != nullptr))
		{
			strokeDasharray = wsVecTemp[i + 1];
		}
		else if (wsVecTemp[i].compare(L"fill-opacity") == 0)
		{
			alpha = _wtof(wsVecTemp[i + 1].c_str());
		}
		else if (wsVecTemp[i].compare(L"stroke-opacity") == 0)
		{
			alpha = _wtof(wsVecTemp[i + 1].c_str());
		}
	}
	return true;
}

bool GetDouble(char* attributeContent, double& dataSpace)
{
	try
	{
		dataSpace = atof(attributeContent);
	}
	catch (std::exception ex)
	{
		return false;
	}
	return true;
}
#pragma endregion

SVGReader::SVGReader()
{
}

bool SVGReader::OpenByPugi(char* path)
{
	auto document = pugi::xml_document();
	auto result = document.load_file(path);

	pugi::xml_node displayList = document.child("svg");
	if (displayList.empty())
	{
		auto wpath = pugi::as_wide(path);
		CString errStr;
		errStr.Format(L"Failed to call load svg in SVGReader::OpenByPugi() %s \n", wpath.c_str());
		//OutputDebugString(errStr);
		return false;
	}

	for (pugi::xml_node instruction = displayList.first_child(); instruction; instruction = instruction.next_sibling())
	{
		if (instruction == nullptr)
		{
			return false;
		}

		std::string instructionName = instruction.name();

		if (!instructionName.compare("path"))
		{
			libS100Engine::Line* line = new libS100Engine::Line();
			std::vector<std::wstring> wsVecTemp;
			for (pugi::xml_attribute attri = instruction.first_attribute(); attri; attri = attri.next_attribute())
			{
				auto attriName = attri.name();
				auto attriValue = attri.value();
				std::wstring wsAttributeContent = std::wstring(attriValue, attriValue + strlen(attriValue));
				wsVecTemp.clear();

				if (!strcmp(attriName, "d"))
				{
					simpleUse::split<std::wstring>(wsAttributeContent, L" ,ML", wsVecTemp);
					line->m_points.clear();
					line->AddPoints(wsVecTemp);
				}
				else if (!strcmp(attriName, "class"))
				{
					char* value = (char*)attriValue;
					GetClass(value, line->fill, line->colorName);
				}
				else if (!strcmp(attriName, "style"))
				{
					char* value = (char*)attriValue;
					GetStyle(value, line->strokeWidth, line->strokeDasharray, line->alpha);
				}
				else if (!strcmp(attriName, "transform"))
				{
					char* value = (char*)attriValue;
					GetRotation(value, line->rotation);
				}
				else if (!strcmp(attriName, "stroke-width"))
				{
					line->SetStrokeWidth(attri.as_double());
				}
				else if (!strcmp(attriName, "fill-opacity"))
				{
					line->alpha = attri.as_double();
				}
			}
			figures.push_back(line);
		}
		else if (!instructionName.compare("line"))
		{

		}
		else if (!instructionName.compare("rect"))
		{
			bool bSymbolBoxLayout = false;
			libS100Engine::Line* line = nullptr;
			double x = 0;
			double y = 0;
			double width = 0;
			double height = 0;
			bool fill = false;
			std::string classValue;
			std::string styleValue;

			auto classAttribute = instruction.attribute("class");
			if (classAttribute != nullptr)
			{
				classValue = classAttribute.value();
				if (0 == classValue.compare("svgBox layout"))
				{
					bSymbolBoxLayout = true;
				}
				// If rect is not layout,
				else if (std::string::npos == std::string(classValue).find("layout"))
				{
					line = new libS100Engine::Line();
				}
			}

			// read attributes
			for (pugi::xml_attribute attri = instruction.first_attribute(); attri; attri = attri.next_attribute())
			{
				auto attriName = attri.name();
				auto attriValue = attri.value();

				if (0 == strcmp(attriName, "x"))
				{
					if (true == bSymbolBoxLayout)
					{
						bounding_x = stof(pugi::as_wide(attri.value()));
					}
					else if (nullptr != line)
					{
						x = attri.as_double();
					}
				}
				else if (0 == strcmp(attriName, "y"))
				{
					if (true == bSymbolBoxLayout)
					{
						bounding_y = stof(pugi::as_wide(attri.value()));
					}
					else if (nullptr != line)
					{
						y = attri.as_double();
					}
				}
				else if (0 == strcmp(attriName, "height"))
				{
					if (true == bSymbolBoxLayout)
					{
						bounding_height = stof(pugi::as_wide(attri.value()));
					}
					else if (nullptr != line)
					{
						height = attri.as_double();
					}
				}
				else if (0 == strcmp(attriName, "width"))
				{
					if (true == bSymbolBoxLayout)
					{
						bounding_width = stof(pugi::as_wide(attri.value()));
					}
					else if (nullptr != line)
					{
						width = attri.as_double();
					}
				}
				else if (0 == strcmp(attriName, "fill"))
				{
					if (0 != strcmp(attriValue, "none"))
					{
						fill = true;
					}
				}
				else if (0 == strcmp(attriName, "style"))
				{
					styleValue = attriValue;
				}
				else if (0 == strcmp(attriName, "stroke-width"))
				{
					line->strokeWidth = attri.as_double();
				}
				else if (!strcmp(attriName, "fill-opacity"))
				{
					line->alpha = attri.as_double();
				}
			}

			if (nullptr != line)
			{
				GetClass((char*)classValue.c_str(), line->fill, line->colorName);
				GetStyle((char*)styleValue.c_str(), line->strokeWidth, line->strokeDasharray, line->alpha);
				POINTF points[5];

				points[0].x = x;
				points[0].y = y;

				points[1].x = x + width;
				points[1].y = y;

				points[2].x = x + width;
				points[2].y = y + height;

				points[3].x = x;
				points[3].y = y + height;

				points[4].x = x;
				points[4].y = y;

				line->AddPoint(points[0]);
				line->AddPoint(points[1]);
				line->AddPoint(points[2]);
				line->AddPoint(points[3]);
				line->AddPoint(points[4]);

				figures.push_back(line);
			}
		}
		else if (!instructionName.compare("circle"))
		{
			libS100Engine::Circle* circle = new libS100Engine::Circle();
			std::vector<std::wstring> wsVecTemp;

			for (pugi::xml_attribute attri = instruction.first_attribute(); attri; attri = attri.next_attribute())
			{
				auto attriName = attri.name();
				auto attriValue = attri.value();
				std::wstring wsAttributeContent;

				if (!strcmp(attriName, "cx"))
				{
					char* value = (char*)attriValue;
					GetDouble(value, circle->cx);
				}
				else if (!strcmp(attriName, "cy"))
				{
					char* value = (char*)attriValue;
					GetDouble(value, circle->cy);
				}
				else if (!strcmp(attriName, "r"))
				{
					char* value = (char*)attriValue;
					GetDouble(value, circle->radius);
				}
				else if (!strcmp(attriName, "class"))
				{
					char* value = (char*)attriValue;
					if (!strcmp(value, "pivotPoint layout"))
					{

					}
					else
					{
						GetClass(value, circle->fill, circle->colorName);
					}
				}
				else if (!strcmp(attriName, "style"))
				{
					char* value = (char*)attriValue;
					GetStyle(value, circle->strokeWidth, circle->strokeDasharray, circle->alpha);
				}
				else if (!strcmp(attriName, "transform"))
				{
					char* value = (char*)attriValue;
					GetRotation(value, circle->rotation);
				}
				else if (!strcmp(attriName, "stroke-width"))
				{
					circle->strokeWidth = attri.as_double();
				}
				else if (!strcmp(attriName, "fill-opacity"))
				{
					circle->alpha = attri.as_double();
				}
			}

			if (!wcscmp(circle->colorName.c_str(), L""))
			{
				circle->type = libS100Engine::FigureType::pivotPoint;
				figures.push_back(circle);
			}
			else
			{
				figures.push_back(circle);
			}
		}
		else if (!instructionName.compare("ellipse"))
		{

		}
		else if (!instructionName.compare("polyline"))
		{

		}
		else if (!instructionName.compare("polygon"))
		{

		}
	}

	// Get the name of the file without ext.
	auto wPath = LibMFCUtil::ConvertCtoWC(path);
	CString cpath(wPath);
	delete[] wPath;

	name = LibMFCUtil::GetFileName(cpath);

	return true;
}

SVGReader::SVGReader(char* path)
{
}

SVGReader::~SVGReader()
{

}

void SVGReader::Close()
{
	for (int i = 0; i < (int)figures.size(); i++)
	{
		if (figures[i] != nullptr)
		{
			delete figures[i];
			figures[i] = nullptr;
		}
	}

	figures.clear();

	for (auto i = geometry.begin(); i != geometry.end(); i++)
	{
		SafeRelease(&i->pGeometry);
	}
}


void SVGReader::CreateSVGGeometry(ID2D1Factory1* m_pDirect2dFactory)
{
	for (auto i = 0; i < (int)figures.size(); i++)
	{
		switch (figures[i]->type)
		{
		case libS100Engine::FigureType::line:
			geometry.push_back(CreateSVGGeometryFromLine(m_pDirect2dFactory, (libS100Engine::Line*)figures[i]));
			break;
		case libS100Engine::FigureType::circle:
			geometry.push_back(CreateSVGGeometryFromCircle((libS100Engine::Circle*)figures[i]));
			break;
		default:
			break;
		}
	}
}

SVGGeometry SVGReader::CreateSVGGeometryFromLine(ID2D1Factory1* m_pDirect2dFactory, libS100Engine::Line* line)
{
	SVGGeometry svgGeometry;

	svgGeometry.bFill = line->fill;
	svgGeometry.bStoke = !line->fill;
	svgGeometry.width = (FLOAT)line->strokeWidth;
	svgGeometry.strokeColorName = line->colorName;
	svgGeometry.fillColorName = line->colorName;
	svgGeometry.alpha = line->alpha;

	HRESULT hr = m_pDirect2dFactory->CreatePathGeometry(&svgGeometry.pGeometry);

	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink* pSink = nullptr;

		hr = svgGeometry.pGeometry->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
			pSink->BeginFigure(
				D2D1::Point2F(line->m_points.begin()->x, line->m_points.begin()->y),
				D2D1_FIGURE_BEGIN_FILLED
			);

			for (auto k = line->m_points.begin() + 1; k != line->m_points.end(); k++)
			{
				pSink->AddLine(D2D1::Point2F(k->x, k->y));
			}

			pSink->EndFigure(D2D1_FIGURE_END_OPEN);

			hr = pSink->Close();

			if (!SUCCEEDED(hr))
			{
				//OutputDebugString(_T("Faield to close sink\n"));
			}
		}
		else
		{
			//OutputDebugString(_T("Faield to Open Sink\n"));
		}
	}
	else
	{
		//OutputDebugString(_T("Failed to create geometry\n"));
	}

	return svgGeometry;
}


SVGGeometry SVGReader::CreateSVGGeometryFromCircle(libS100Engine::Circle* circle)
{
	SVGGeometry svgGeometry;

	svgGeometry.bFill = circle->fill;
	svgGeometry.bStoke = !circle->fill;
	svgGeometry.width = (FLOAT)circle->strokeWidth;
	svgGeometry.strokeColorName = circle->colorName;
	svgGeometry.fillColorName = circle->colorName;
	svgGeometry.pGeometry = nullptr;
	svgGeometry.alpha = circle->alpha;

	svgGeometry.ellipse.point.x = (FLOAT)circle->cx;
	svgGeometry.ellipse.point.y = (FLOAT)circle->cy;

	svgGeometry.ellipse.radiusX = (FLOAT)circle->radius;
	svgGeometry.ellipse.radiusY = (FLOAT)circle->radius;

	return svgGeometry;
}