#include "stdafx.h"
#include "S100_Transform.h"

S100_Transform::S100_Transform()
{
	isTransfer = false;
}

S100_Transform::~S100_Transform()
{
	if (!isTransfer)
	{
		for (auto itor = templates.begin(); itor != templates.end(); itor++)
			delete *itor;
	}

	for (auto itor = includes.begin(); itor != includes.end(); itor++)
		delete *itor;
}

bool S100_Transform::ReadRuleFile(std::wstring path)
{
	MSXML2::IXMLDOMDocument2Ptr pDoc;							// XML Document
	MSXML2::IXMLDOMNamedNodeMapPtr pNodeMap;					// for attribute
	MSXML2::IXMLDOMNodeListPtr pNodeInfo;						// find node List
	MSXML2::IXMLDOMNodePtr pNode;								

	if (!path.empty())
	{
		pDoc.CreateInstance(__uuidof(DOMDocument));  
		VARIANT_BOOL ret = pDoc->load((_variant_t)path.c_str());
		if (!ret)
			return false;
		
		MSXML2::IXMLDOMNodeListPtr pChild = pDoc->childNodes;

		for (int i = 0; i < pChild->Getlength(); i++)
		{
			MSXML2::IXMLDOMNodePtr pNode = pChild->Getitem(i);

			std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();
			if (nodeName.compare(L"xsl:transform") == 0)
			{
				GetContents(pNode->childNodes);
			}
		}
	}

	ExcuteIncludeRuleFile();

	return true;
}

void S100_Transform::GetContents(MSXML2::IXMLDOMNodeListPtr pNodeList)
{
	if (!pNodeList)
		return;

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"xsl:template") == 0)
		{
			S100_Template *cont = new S100_Template();
			templates.push_back(cont);

			cont->GetContents(pNode);
		}
		else 
		if (nodeName.compare(L"xsl:include") == 0)
		{
			S100_Include *cont = new S100_Include();
			includes.push_back(cont);

			cont->GetContents(pNode);
		}
		else if (nodeName.compare(L"xsl:output") == 0)
		{
		}
		else if (nodeName.compare(L"#comment") == 0)
		{
		}
		else if (nodeName.compare(L"xsl:decimal-format") == 0)
		{

		}
	}
}

void S100_Transform::ExcuteIncludeRuleFile()
{
	for (auto itor = includes.begin(); itor != includes.end(); itor++)
	{
		S100_Include *cont = *itor;
		S100_Transform* transform = new S100_Transform();
		
		transform->ReadRuleFile(cont->GetHerf());

		transform->CopyTo(this);
		delete transform;
	}

	for (auto itor = includes.begin(); itor != includes.end(); itor++)
		delete *itor;
	includes.clear();
}

void S100_Transform::CopyTo(S100_Transform* toObject)
{
	toObject->templates.insert(toObject->templates.end(), templates.begin(), templates.end());
	isTransfer = true;
}

void S100_Transform::SetIsTransfer(bool value)
{
	isTransfer = value;
}

bool S100_Transform::GetIsTransfer() 
{
	return isTransfer;
}

void S100_Transform::SetTemplates(S100_Template* value) 
{
	templates.push_back(value);
}

void S100_Transform::SetTemplates(std::vector<S100_Template*> value) 
{
	 templates = value;
}

void S100_Transform::SetInclude(S100_Include* value) 
{
	includes.push_back(value);
}

void S100_Transform::SetIncludes(std::vector<S100_Include*> value) 
{
	includes = value;
}

S100_Template* S100_Transform::GetTemplate(int index)
{
	return templates.at(index);
}
std::vector<S100_Template*> S100_Transform::GetTemplates() 
{
	return templates;
}

S100_Include* S100_Transform::GetInclude(int index)
{
	return includes.at(index);
}

std::vector<S100_Include*> S100_Transform::GetIncludes()
{
	return includes;
}
