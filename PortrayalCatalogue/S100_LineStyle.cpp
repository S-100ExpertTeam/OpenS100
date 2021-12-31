#include "stdafx.h"
#include "S100_LineStyle.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

S100_LineStyle::S100_LineStyle()
{
	capStyle = L"Butt";
	joinStyle = L"Miter";
	offset = L"0.0";
	SetType(1);
}

S100_LineStyle::~S100_LineStyle()
{

}

void S100_LineStyle::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;

	MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
	MSXML2::IXMLDOMNodePtr pAttrNP;
	VARIANT value;

	pAttrNP = pAttr->getNamedItem(L"capStyle");
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);
		capStyle = std::wstring(value.bstrVal).c_str();
	}
	pAttrNP = pAttr->getNamedItem(L"joinStyle");
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);
		joinStyle = std::wstring(value.bstrVal).c_str();
	}
	pAttrNP = pAttr->getNamedItem(L"offset");
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);
		offset = std::wstring(value.bstrVal).c_str();
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->GetchildNodes();

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"intervalLength") == 0)
		{
			std::wstring str((wchar_t*)pNode->Gettext());
			intervalLength = L"12";
			intervalLength = str.c_str();
		}
		else if (nodeName.compare(L"pen") == 0)
		{
			m_pen.GetContents(pNode);
		}
		else if (nodeName.compare(L"dash") == 0)
		{
			S100_Dash dash;
			dash.GetContents(pNode);
			m_dash.push_back(dash);
		}
		else if (nodeName.compare(L"symbol") == 0)
		{
			S100_LineSymbol symbol;
			symbol.GetContents(pNode);
			m_symbol.push_back(symbol);
		}
	}
}
void S100_LineStyle::GetContents(pugi::xml_node& node) 
{
	if (!node)
	{
		return;
	}
	
	for each (pugi::xml_attribute attri in node.attributes())
	{
		auto attriName = attri.name();
		if (!strcmp(attriName, "capStyle"))
		{
			capStyle = pugi::as_wide(attri.value());
		}
		else if (!strcmp(attriName, "joinStyle"))
		{
			joinStyle = pugi::as_wide(attri.value());
		}
		else if (!strcmp(attriName, "offset"))
		{
			offset = pugi::as_wide(attri.value());
		}
	}
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "intervalLength"))
		{
			intervalLength = pugi::as_wide(instruction.child_value());
		}

		else if (!strcmp(instructionName, "pen"))
		{
			m_pen.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "dash"))
		{
			S100_Dash dash;
			dash.GetContents(instruction);
			m_dash.push_back(dash);
		}

		else if (!strcmp(instructionName, "symbol"))
		{
			S100_LineSymbol symbol;
			symbol.GetContents(instruction);
			m_symbol.push_back(symbol);
		}
	}
}

void S100_LineStyle::ParseValue(std::string value)
{
	SetEmpty();

	std::vector<std::string> v_splited = LatLonUtility::Split(value, ",");
	if (v_splited.size() >= 4)
	{
		name = std::wstring(v_splited[0].begin(), v_splited[0].end());
		intervalLength = std::wstring(v_splited[1].begin(), v_splited[1].end());
		width = std::wstring(v_splited[2].begin(), v_splited[2].end());
		token = std::wstring(v_splited[3].begin(), v_splited[3].end());

		if (v_splited.size() >= 5)
		{
			transparency = std::wstring(v_splited[4].begin(), v_splited[4].end());

			if (v_splited.size() >= 6)
			{
				capStyle = std::wstring(v_splited[5].begin(), v_splited[5].end());

				if (v_splited.size() >= 7)
				{
					joinStyle = std::wstring(v_splited[6].begin(), v_splited[6].end());

					if (v_splited.size() >= 8)
					{
						offset = std::wstring(v_splited[7].begin(), v_splited[7].end());
					}
				}
			}
		}

		m_pen.SetWidth(width);
		m_pen.GetColor().SetToken(token);
		m_pen.GetColor().SetTransparency(_wtof(transparency.c_str()));
	}
}

bool S100_LineStyle::IsEmpty()
{
	return width.empty();
}

void S100_LineStyle::SetEmpty()
{
	name = L"";
	intervalLength = L"";
	width = L"";
	token = L"";
	transparency = L"0";
	capStyle = L"Butt";
	joinStyle = L"Miter";
	offset = L"0.0";

	m_dash.clear();
	m_symbol.clear();
}

void S100_LineStyle::SetName(std::wstring& value)
{
	name = value;
}

void S100_LineStyle::SetIntervalLength(std::wstring& value)
{
	intervalLength = value;
}

void S100_LineStyle::SetWidth(std::wstring& value)
{
	width = value;
}

void S100_LineStyle::SetToken(std::wstring& value)
{
	token = value;
}

void S100_LineStyle::SetTransparency(std::wstring& value)
{
	transparency = value;
}
void S100_LineStyle::SetCapStyle(std::wstring& value)
{
	capStyle = value;
}
void S100_LineStyle::SetJoinStyle(std::wstring& value) 
{
	joinStyle = value;
}
void S100_LineStyle::SetOffset(std::wstring& value)
{
	offset = value;
}
void S100_LineStyle::SetPen(S100_Pen* value) 
{
	m_pen = *value;
}

void S100_LineStyle::SetDash(S100_Dash* value)
{
	m_dash.push_back(*value);
}

void S100_LineStyle::SetDash(std::list<S100_Dash> value) 
{
	m_dash = value;
}

void S100_LineStyle::SetSymbol(S100_LineSymbol* value) 
{
	m_symbol.push_back(*value);
}
void S100_LineStyle::SetSymbol(std::list<S100_LineSymbol> value)
{
	m_symbol = value;
}

std::wstring S100_LineStyle::GetName()
{
	return name;
}
std::wstring S100_LineStyle::GetIntervalLength()
{
	return intervalLength;
}
std::wstring S100_LineStyle::GetWidth() 
{
	return width;
}
std::wstring S100_LineStyle::GetToken()
{
	return token;
}
std::wstring S100_LineStyle::GetTransparency() 
{
	return transparency;
}
std::wstring S100_LineStyle::GetCapStyle()
{
	return capStyle;
}
std::wstring S100_LineStyle::GetJoinStyle() 
{
	return joinStyle;
}
std::wstring S100_LineStyle::GetOffset()
{
	return offset;
}

S100_Pen* S100_LineStyle::GetPen() {
	return &m_pen;
}
S100_Dash* S100_LineStyle::GetDash(int index) 
{
	auto it = m_dash.begin();
	advance(it, index);
	return &(*it);
}
std::list<S100_Dash> S100_LineStyle::GetDashs()
{
	return m_dash;
}
S100_LineSymbol* S100_LineStyle::GetSymbol(int index)
{
	auto it = m_symbol.begin();
	advance(it, index);
	return &(*it);
}
std::list<S100_LineSymbol> S100_LineStyle::GetSymbols() 
{
	return m_symbol;
}