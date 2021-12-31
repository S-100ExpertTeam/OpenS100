#include "stdafx.h"
#include "S100_SimpleLineStyle.h"

S100_SimpleLineStyle::S100_SimpleLineStyle()
{
	pen = NULL;
	dash = NULL;
}

S100_SimpleLineStyle::~S100_SimpleLineStyle()
{
	if (pen) delete pen;
	if (dash) delete dash;
}

void S100_SimpleLineStyle::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->GetchildNodes();

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"capStyle") == 0)
		{
			capStyle = std::wstring(pNode->Gettext());
		}
		else if (nodeName.compare(L"joinStyle") == 0)
		{
			joinStyle = std::wstring(pNode->Gettext());
		}
		else if (nodeName.compare(L"offset") == 0)
		{
			offset = std::wstring(pNode->Gettext());
		}
		else if (nodeName.compare(L"pen") == 0)
		{
			if (!pen)
			{
				pen = new S100_Pen();
			}

			pen->GetContents(pNode);
		}
		else if (nodeName.compare(L"dash") == 0)
		{
			if (!dash) dash = new S100_Dash();
			dash->GetContents(pNode);
		}
	}
}

void S100_SimpleLineStyle::SetCapStyle(std::wstring& value)
{
	capStyle = value;
}

void S100_SimpleLineStyle::SetJoinStyle(std::wstring& value)
{
	joinStyle = value;
}

void S100_SimpleLineStyle::SetOffset(std::wstring& value)
{
	offset = value;
}

void S100_SimpleLineStyle::SetPen(S100_Pen* value) 
{
	pen = value;
}

void S100_SimpleLineStyle::SetDash(S100_Dash* value) 
{
	dash = value;
}


std::wstring S100_SimpleLineStyle::GetCapStyle()
{
	return capStyle;
}

std::wstring S100_SimpleLineStyle::GetJoinStyle() 
{
	return joinStyle;
}

std::wstring S100_SimpleLineStyle::GetOffset() 
{
	return offset;
}

S100_Pen* S100_SimpleLineStyle::GetPen() 
{
	return pen;
}

S100_Dash* S100_SimpleLineStyle::GetDash() 
{
	return dash;
}