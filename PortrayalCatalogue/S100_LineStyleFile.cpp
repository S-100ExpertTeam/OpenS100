#include "stdafx.h"

#include "S100_LineStyleFile.h"

S100_LineStyleFile::S100_LineStyleFile()
{

}

S100_LineStyleFile::~S100_LineStyleFile()
{

}

void S100_LineStyleFile::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;

	MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
	MSXML2::IXMLDOMNodePtr pAttrNP;
	pAttrNP = pAttr->getNamedItem(L"id");
	VARIANT value;
	pAttrNP->get_nodeValue(&value);

	id = std::wstring(value.bstrVal).c_str();

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->GetchildNodes();

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;
		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"description") == 0)
		{
			description.GetContents(pNode->childNodes);
		}
		else if (nodeName.compare(L"fileName") == 0)
		{
			fileName = std::wstring(pNode->Gettext());
		}
		else if (nodeName.compare(L"fileType") == 0)
		{
			fileType = std::wstring(pNode->Gettext());
		}
		else if (nodeName.compare(L"fileFormat") == 0)
		{
			fileFormat = std::wstring(pNode->Gettext());
		}
	}
}
void S100_LineStyleFile::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	auto idvalue = node.attribute("id").value(); //Get attribute value
	if (idvalue)
	{
		id = pugi::as_wide(idvalue);
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "description"))
		{
			description.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "fileName"))
		{
			fileName = pugi::as_wide(instruction.child_value());
		}

		else if (!strcmp(instructionName, "fileType"))
		{
			fileType = pugi::as_wide(instruction.child_value());
		}

		else if (!strcmp(instructionName, "fileFormat"))
		{
			fileFormat = pugi::as_wide(instruction.child_value());
		}
	}
}

void S100_LineStyleFile::SetId(std::wstring& value)
{
	id = value;
}

void S100_LineStyleFile::SetDescription(S100_Description* value) {
	description = *value;
}

void S100_LineStyleFile::SetFileName(std::wstring& value)
{
	fileName = value;
}

void S100_LineStyleFile::SetFileType(std::wstring& value)
{
	fileType = value;
}

void S100_LineStyleFile::SetFileFormat(std::wstring& value)
{
	fileFormat = value;
}

std::wstring S100_LineStyleFile::GetId()
{
	return id;
}

S100_Description* S100_LineStyleFile::GetDescription() 
{
	return &description;
}

std::wstring S100_LineStyleFile::GetFileName() {
	return fileName;
}

std::wstring S100_LineStyleFile::GetFileType() {
	return fileType;
}


std::wstring S100_LineStyleFile::GetFileFormat() 
{
	return fileFormat;
}