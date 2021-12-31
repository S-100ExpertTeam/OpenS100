#include "stdafx.h"
#include "S100_LineStyle.h"
#include "SVG.h"


namespace S100_SVG_D2D1_DLL 
{
	S100_LineStyle::S100_LineStyle()
	{

	}

	S100_LineStyle::~S100_LineStyle()
	{ 

	}

	S100_Line S100_LineStyle::ReadLineStyle(std::wstring path)
	{
		MSXML2::IXMLDOMDocument2Ptr pDoc;							// XML Document
		if (!path.empty())
		{
			pDoc.CreateInstance(__uuidof(DOMDocument));  // create instance
			pDoc->load((_variant_t)path.c_str());

			m_filepath = path;
			GetLineStyleInfo(pDoc);
		}
		return line;
	}
	S100_Line S100_LineStyle::ReadLineStyleByPugi(std::wstring path)
	{
	
		if (!path.empty())
		{
			pugi::xml_document xmldoc;
			pugi::xml_parse_result result = xmldoc.load_file(path.c_str());

			m_filepath = path;
			GetLineStyleInfo(xmldoc);
		}
		return line;
	}

	void S100_LineStyle::GetLineStyleInfo(MSXML2::IXMLDOMNodePtr pNode)
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

			if (nodeName.compare(L"lineStyle") == 0)
			{
				MSXML2::IXMLDOMNodeListPtr pCNList = pNode->GetchildNodes();
				std::wstring aa;
				for (int j = 0; j < pCNList->Getlength(); j++)
				{
					MSXML2::IXMLDOMNodePtr pCN = pCNList->Getitem(j);
					std::wstring nodeName = (LPCTSTR)pCN->GetnodeName();

					if (nodeName.compare(L"pen") == 0)
					{
						GetPen(pCN);
					}
					else if (nodeName.compare(L"dash") == 0)
					{
						GetDash(pCN->GetchildNodes());
					}
					else if (nodeName.compare(L"symbol") == 0)
					{
						GetSymbol(pCN);
					}
					//intervalLength
					else if (nodeName.compare(L"intervalLength") == 0)
						line.length = (float)_wtof(pCN->Gettext()) * factor;
				}
			}
		}
		return;
	}
	void S100_LineStyle::GetLineStyleInfo(pugi::xml_node node)
	{
		if (node==nullptr)
			return;

		auto linestyleNode = node.first_child();
		const pugi::char_t* linestyleNodeName = linestyleNode.name();

		if (!strcmp(linestyleNodeName, "lineStyle"))
		{
			for (pugi::xml_node instruction = linestyleNode.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();
				if (!strcmp(instructionName, "pen"))
				{
					GetPen(instruction);
				}
				else if (!strcmp(instructionName, "dash"))
				{
					GetDash(instruction);
				}
				else if (!strcmp(instructionName, "symbol"))
				{
					GetSymbol(instruction);
				}
				else if (!strcmp(instructionName, "intervalLength"))
				{
					auto intervalLengthvalue = pugi::as_wide(instruction.child_value());
					if (intervalLengthvalue!=L"")
					{
						line.length =std::stof(intervalLengthvalue)* factor;
					}
				}
			}
		}
		return;
	}

	void S100_LineStyle::GetPen(pugi::xml_node node)
	{
		if (node==nullptr)
		{
			return;
		}

		auto widthNode = node.first_attribute();
		auto widthName= widthNode.name();

		if (!strcmp(widthName, "width"))
		{
			auto widthNameValue =pugi::as_wide(widthNode.value());

			if (widthNameValue!=L"")
			{
				line.pen.width =std::stof(widthNameValue);
			}
		}

		auto colorNode = node.first_child();
		auto colornodeName = colorNode.name();

		if (!strcmp(colornodeName,"color")) 
		{
			line.pen.col =pugi::as_wide(colorNode.child_value());
		}
	}
	void S100_LineStyle::GetPen(MSXML2::IXMLDOMNodePtr pNode)
	{
		if (!pNode)
			return;

		MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
		MSXML2::IXMLDOMNodePtr pAttrNP;
		VARIANT value;

		pAttrNP = pAttr->getNamedItem(L"width");
		pAttrNP->get_nodeValue(&value);
		VariantChangeType(&value, &value, 0, VT_R4);
		line.pen.width = V_R4(&value);

		pNode = pNode->GetfirstChild();
		if (pNode)
			line.pen.col = pNode->Gettext();

	}

	void S100_LineStyle::GetDash(pugi::xml_node node)
	{
		if (node == nullptr)
		{
			return;
		}

		S100_LineDash dash;
		for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName,"start"))
			{
				auto startValue =pugi::as_wide(instruction.child_value());
				if (startValue!=L"")
				{
					dash.start = (std::stof(startValue))* factor;
				}
			}

			else if (!strcmp(instructionName, "length"))
			{
				auto lengthValue=pugi::as_wide(instruction.child_value());
				if (lengthValue!=L"") 
				{
					dash.length = (std::stof(lengthValue))*factor;
				}
			}
		}
		line.dash.push_back(dash);
	}
	void S100_LineStyle::GetDash(MSXML2::IXMLDOMNodeListPtr pNodeList)
	{
		if (!pNodeList)
			return;

		S100_LineDash dash;

		for (int i = 0; i < pNodeList->Getlength(); i++)
		{
			MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

			if (!pNode)
				continue;
			std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

			if (nodeName.compare(L"start") == 0)
			{
				dash.start = (float)_wtof(pNode->GetfirstChild()->Gettext())* factor;

			}

			else if (nodeName.compare(L"length") == 0)
			{

				dash.length = (float)_wtof(pNode->GetfirstChild()->Gettext())* factor;
			}
		}

		line.dash.push_back(dash);
	}

	void S100_LineStyle::GetSymbol(pugi::xml_node node)
	{
		if (node == nullptr)
		{
			return;
		}

		S100_LineSymbol sym;
		pugi::xml_attribute referenceNode = node.first_attribute();
		auto referenceName = referenceNode.name();
		if (!strcmp(referenceName, "reference"))
		{
			sym.reference =pugi::as_wide(referenceNode.value());
		}


		pugi::xml_node positionNode = node.first_child();
		auto positionName = positionNode.name();

		if (!strcmp(positionName, "position"))
		{
			auto positinoValue= pugi::as_wide(positionNode.child_value());
			if (positinoValue!=L"")
			{
				sym.position = std::stof(positinoValue)*factor;
			}
		}

		line.sym.push_back(sym);
	}
	void S100_LineStyle::GetSymbol(MSXML2::IXMLDOMNodePtr pNode)
	{
		if (!pNode)
			return;

		MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
		MSXML2::IXMLDOMNodePtr pAttrNP;
		VARIANT value;

		S100_LineSymbol sym;

		pAttrNP = pAttr->getNamedItem(L"reference");
		pAttrNP->get_nodeValue(&value);
		sym.reference = std::wstring(value.bstrVal);

		pNode = pNode->GetfirstChild();
		sym.position = (float)_wtof(pNode->GetfirstChild()->Gettext()) * factor;

		line.sym.push_back(sym);
	}
}