#include "stdafx.h"
#include "S100PCLineStylesReader.h"

namespace S100XMLReader
{
	S100PCLineStylesReader::S100PCLineStylesReader()
	{

	}

	S100PCLineStylesReader::~S100PCLineStylesReader()
	{

	}

	bool S100PCLineStylesReader::Open(std::wstring path, LineStylesPackage::LineStyles *pLineStyles)
	{
		WIN32_FIND_DATA findFileData;
		HANDLE hFind;

		std::wstring lineStylePath = path + L"\\*.xml";

		hFind = FindFirstFile(lineStylePath.c_str(), &findFileData);

		if (INVALID_HANDLE_VALUE == hFind)
		{
			return true;
		}

		do
		{
			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				CString filePath;
				filePath.Format(_T("%s\\%s"), path.c_str(), findFileData.cFileName);

				MSXML2::IXMLDOMDocument2Ptr pXMLDoc;
				if (!filePath.IsEmpty())
				{
					LineStylesPackage::AbstractLineStyle *pLineStyle = nullptr;

					pXMLDoc.CreateInstance(__uuidof(DOMDocument));
					pXMLDoc->load((_variant_t)filePath);
					if (pXMLDoc->selectSingleNode(L"//compositeLineStyle"))
					{
						SetLineStyle(pXMLDoc->selectSingleNode(L"//compositeLineStyle"), &pLineStyle);
					}
					else if (pXMLDoc->selectSingleNode(L"//lineStyle"))
					{
						SetLineStyle(pXMLDoc->selectSingleNode(L"//lineStyle"), &pLineStyle);
					}
					else if (pXMLDoc->selectSingleNode(L"//lineStyleReference"))
					{

					}
					else
					{
						OutputDebugString(_T("*** *** *** Invalied type of line style\n"));
					}

					if (nullptr != pLineStyle)
					{
						pLineStyle->name = findFileData.cFileName;
						pLineStyles->mapLineStyle.insert(std::map<std::wstring, LineStylesPackage::AbstractLineStyle *>::value_type(findFileData.cFileName, pLineStyle));
					}
				}
			}
			else
			{
				CString strOutput;
				strOutput.Format(_T("*** *** *** %s is directory\n"), findFileData.cFileName);
				OutputDebugString(strOutput);
			}
		} while (FindNextFile(hFind, &findFileData));

		FindClose(hFind);

		return true;
	}
	bool S100PCLineStylesReader::OpenByPugi(std::wstring path, LineStylesPackage::LineStyles *pLineStyles)
	{
		WIN32_FIND_DATA findFileData;
		HANDLE hFind;

		std::wstring lineStylePath = path + L"\\*.xml";

		hFind = FindFirstFile(lineStylePath.c_str(), &findFileData);

		if (INVALID_HANDLE_VALUE == hFind)
		{
			return true;
		}

		do
		{
			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				CString filePath;
				filePath.Format(_T("%s\\%s"), path.c_str(), findFileData.cFileName);

				if (!filePath.IsEmpty())
				{
					LineStylesPackage::AbstractLineStyle *pLineStyle = nullptr;
					pugi::xml_document doc;
					pugi::xml_parse_result result = doc.load_file(filePath);

					pugi::xml_node displayList = doc.child("lineStyle");
					if (displayList != nullptr)
					{
						SetLineStyle(displayList, &pLineStyle);
					}

					displayList = doc.child("compositeLineStyle");
					if (displayList != nullptr)
					{
						SetLineStyle(displayList, &pLineStyle);
					}

					displayList = doc.child("lineStyleReference");
					if (displayList)
					{
					}

					if (pLineStyle != nullptr)
					{

					}
				}
			}
		} while (FindNextFile(hFind, &findFileData));


		FindClose(hFind);

		return true;

	}

	bool S100PCLineStylesReader::SetLineStyle(MSXML2::IXMLDOMNodePtr _pNode, std::vector<LineStylesPackage::AbstractLineStyle *>* lineStyle)
	{
		std::wstring name = _pNode->GetnodeName();

		//Create New LineStyle
		LineStylesPackage::AbstractLineStyle* pLineStyle;
		if (name == L"compositeLineStyle")
		{
			pLineStyle = (LineStylesPackage::AbstractLineStyle*)(new LineStylesPackage::CompositeLineStyle());
		}
		else if (name == L"lineStyle")
		{
			pLineStyle = (LineStylesPackage::AbstractLineStyle*)(new LineStylesPackage::LineStyle());
		}
		lineStyle->push_back(pLineStyle);

		//Read Data
		for (int i = 0; i < _pNode->GetchildNodes()->Getlength(); i++)
		{
			MSXML2::IXMLDOMNodePtr pNode = _pNode->GetchildNodes()->Getitem(i);

			if ((pNode->nodeName) == _bstr_t(_T("intervalLength")))
			{
				((LineStylesPackage::LineStyle*)pLineStyle)->intervalLength = _wtof(_pNode->GetchildNodes()->Getitem(i)->text);
			}
			else if ((pNode->nodeName) == _bstr_t(_T("pen")))
			{
				((LineStylesPackage::LineStyle*)pLineStyle)->pen = new GraphicBasePackage::Pen();
			}
			else if ((pNode->nodeName) == _bstr_t(_T("dash")))
			{
				LineStylesPackage::Dash* pDash = new LineStylesPackage::Dash();
			}
			else if ((pNode->nodeName) == _bstr_t(_T("symbol")))
			{
				LineStylesPackage::LineSymbol* pSymbol = new LineStylesPackage::LineSymbol();

			}
			else if ((pNode->nodeName) == _bstr_t(_T("lineStyle")))
			{
				LineStylesPackage::LineStyle* pLS = new LineStylesPackage::LineStyle();
				((LineStylesPackage::CompositeLineStyle*)pLineStyle)->component.push_back((LineStylesPackage::AbstractLineStyle*)(pLS));
				MSXML2::IXMLDOMNamedNodeMapPtr pMap;
				pNode->get_attributes(&pMap);

				if (pMap->getNamedItem(_bstr_t(_T("offset"))))
				{
					pLS->offset = _wtof(pMap->getNamedItem(_bstr_t(_T("offset")))->text);
				}
			}
		}


		return TRUE;
	}
	bool S100PCLineStylesReader::SetLineStyle(pugi::xml_node node, std::vector<LineStylesPackage::AbstractLineStyle *>* lineStyle)
	{
		auto name = node.name();
		LineStylesPackage::AbstractLineStyle* pLineStyle;
		if (!strcmp(name, "compositeLineStyle"))
		{
			pLineStyle = new LineStylesPackage::CompositeLineStyle();
		}
		else if (!strcmp(name, "lineStyle"))
		{
			pLineStyle = new LineStylesPackage::LineStyle();
		}
		lineStyle->push_back(pLineStyle);

		for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName, "intervalLength"))
			{
				((LineStylesPackage::LineStyle*)pLineStyle)->intervalLength = std::stod(instruction.child_value());
			}
			else if (!strcmp(instructionName, "pen"))
			{
				((LineStylesPackage::LineStyle*)pLineStyle)->pen = new GraphicBasePackage::Pen();
			}
			else if (!strcmp(instructionName, "dash"))
			{
				LineStylesPackage::Dash* pDash = new LineStylesPackage::Dash();
			}
			else if (!strcmp(instructionName, "symbol"))
			{
				LineStylesPackage::LineSymbol* pSymbol = new LineStylesPackage::LineSymbol();
			}
			else if (!strcmp(instructionName, "lineStyle"))
			{
				LineStylesPackage::LineStyle* pLS = new LineStylesPackage::LineStyle();
				((LineStylesPackage::CompositeLineStyle*)pLineStyle)->component.push_back((LineStylesPackage::AbstractLineStyle*)(pLS));
				for (auto attri = instruction.first_attribute(); attri; attri = attri.next_attribute())
				{
					auto attriName = attri.name();

					if (!strcmp(attriName, "offset"))
					{
						pLS->offset = std::stod(attri.value());
					}

				}

			}
		}
		return true;
	}

	bool S100PCLineStylesReader::SetLineStyle(MSXML2::IXMLDOMNodePtr _pNode, LineStylesPackage::AbstractLineStyle **pLineStyle)
	{
		std::wstring name = _pNode->GetnodeName();

		//Create New LineStyle
		if (name == L"compositeLineStyle")
		{
			*pLineStyle = new LineStylesPackage::CompositeLineStyle();
		}
		else if (name == L"lineStyle")
		{
			*pLineStyle = new LineStylesPackage::LineStyle();
		}

		//Read Data
		for (int i = 0; i < _pNode->GetchildNodes()->Getlength(); i++)
		{
			MSXML2::IXMLDOMNodePtr pNode = _pNode->GetchildNodes()->Getitem(i);

			if ((pNode->nodeName) == _bstr_t(_T("intervalLength")))
			{
				((LineStylesPackage::LineStyle*)*pLineStyle)->intervalLength = _wtof(_pNode->GetchildNodes()->Getitem(i)->text);
			}
			else if ((pNode->nodeName) == _bstr_t(_T("pen")))
			{
				((LineStylesPackage::LineStyle*)*pLineStyle)->pen = new GraphicBasePackage::Pen();
				SetPen(_pNode->GetchildNodes()->Getitem(i), ((LineStylesPackage::LineStyle*)*pLineStyle)->pen);
			}
			else if ((pNode->nodeName) == _bstr_t(_T("dash")))
			{
				LineStylesPackage::Dash* pDash = new LineStylesPackage::Dash();
				((LineStylesPackage::LineStyle*)*pLineStyle)->dash.push_back(pDash);
				SetDash(_pNode->GetchildNodes()->Getitem(i), pDash);
			}
			else if ((pNode->nodeName) == _bstr_t(_T("symbol")))
			{
				LineStylesPackage::LineSymbol* pSymbol = new LineStylesPackage::LineSymbol();
				((LineStylesPackage::LineStyle*)*pLineStyle)->symbol.push_back(pSymbol);
				SetSymbol(_pNode->GetchildNodes()->Getitem(i), pSymbol);
			}
			else if ((pNode->nodeName) == _bstr_t(_T("lineStyle")))
			{
				LineStylesPackage::LineStyle* pLS = nullptr;
				SetLineStyle(pNode, (LineStylesPackage::AbstractLineStyle **)&pLS);

				MSXML2::IXMLDOMNamedNodeMapPtr pMap;
				pNode->get_attributes(&pMap);

				if (pMap->getNamedItem(_bstr_t(_T("offset"))))
				{
					pLS->offset = _wtof(pMap->getNamedItem(_bstr_t(_T("offset")))->text);
				}

				((LineStylesPackage::CompositeLineStyle *)(*pLineStyle))->component.push_back(pLS);
			}
			else
			{
				OutputDebugString(_T("*** *** *** Invalid node\n"));
			}
		}

		return true;
	}

	bool S100PCLineStylesReader::SetLineStyle(pugi::xml_node node, LineStylesPackage::AbstractLineStyle **pLineStyle)
	{
		auto name = node.name();

		if (!strcmp(name, "compositeLineStyle"))
		{
			*pLineStyle = new LineStylesPackage::CompositeLineStyle();
		}
		else if (!strcmp(name, "lineStyle"))
		{
			*pLineStyle = new LineStylesPackage::LineStyle();
		}

		for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName, "intervalLength"))
			{
				((LineStylesPackage::LineStyle*)*pLineStyle)->intervalLength = std::stod(instruction.child_value());
			}
			else if (!strcmp(instructionName, "pen"))
			{
				((LineStylesPackage::LineStyle*)*pLineStyle)->pen = new GraphicBasePackage::Pen();
				SetPen(instruction, ((LineStylesPackage::LineStyle*)*pLineStyle)->pen);
			}
			else if (!strcmp(instructionName, "dash"))
			{
				LineStylesPackage::Dash* pDash = new LineStylesPackage::Dash();
				((LineStylesPackage::LineStyle*)*pLineStyle)->dash.push_back(pDash);
				SetDash(instruction, pDash);
			}
			else if (!strcmp(instructionName, "symbol"))
			{
				LineStylesPackage::LineSymbol* pSymbol = new LineStylesPackage::LineSymbol();
				((LineStylesPackage::LineStyle*)*pLineStyle)->symbol.push_back(pSymbol);
				SetSymbol(instruction, pSymbol);
			}
			else if (!strcmp(instructionName, "lineStyle"))
			{
				LineStylesPackage::LineStyle* pLS = nullptr;
				SetLineStyle(instruction, (LineStylesPackage::AbstractLineStyle **)&pLS);


				MSXML2::IXMLDOMNamedNodeMapPtr pMap;

				for (auto attri = instruction.first_attribute(); attri; attri = attri.next_attribute())
				{
					auto attriName = attri.name();
					if (!strcmp(attriName, "offset"))
					{
						pLS->offset = std::stod(attri.value());
					}
				}

				((LineStylesPackage::CompositeLineStyle *)(*pLineStyle))->component.push_back(pLS);
			}
		}

		return true;
	}

	void S100PCLineStylesReader::SetPen(MSXML2::IXMLDOMNodePtr _pNode, GraphicBasePackage::Pen* pPen)
	{
		for (int i = 0; i < _pNode->Getattributes()->Getlength(); i++)
		{
			MSXML2::IXMLDOMNodePtr pNode = _pNode->Getattributes()->Getitem(i);

			if ((pNode->nodeName) == _bstr_t(_T("width")))
			{
				pPen->width = _wtof(pNode->text);
			}
		}

		for (int i = 0; i < _pNode->GetchildNodes()->Getlength(); i++)
		{
			MSXML2::IXMLDOMNodePtr pNode = _pNode->GetchildNodes()->Getitem(i);

			if ((pNode->nodeName) == _bstr_t(_T("color")))
			{
				pPen->color.SetToken(std::wstring(pNode->text));
			}
		}
	}

	void S100PCLineStylesReader::SetPen(pugi::xml_node node, GraphicBasePackage::Pen* pPen)
	{
		for (auto attri = node.first_attribute(); attri; attri = attri.next_attribute())
		{
			auto attriName = attri.name();
			if (!strcmp(attriName, "width"))
			{
				pPen->width = std::stod(attri.value());
			}
		}

		for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName, "color"))
			{
				pPen->color.SetToken(pugi::as_wide(instruction.child_value()));
			}
		}
	}

	void S100PCLineStylesReader::SetDash(MSXML2::IXMLDOMNodePtr _pNode, LineStylesPackage::Dash* pDash)
	{
		int max = _pNode->childNodes->Getlength();
		_pNode = _pNode->firstChild;

		for (int i = 0; i < max; i++)
		{
			if ((_pNode->nodeName) == _bstr_t(_T("start")))
			{
				pDash->start = _wtof(_pNode->text);
			}
			else if ((_pNode->nodeName) == _bstr_t(_T("length")))
			{

				pDash->length = _wtof(_pNode->text);
			}
			_pNode = _pNode->nextSibling;
		}
	}
	void S100PCLineStylesReader::SetDash(pugi::xml_node node, LineStylesPackage::Dash* pDash)
	{

		for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName, "start"))
			{
				pDash->start = std::stod(instruction.child_value());
			}
			else if (!strcmp(instructionName, "length"))
			{
				pDash->length = std::stod(instruction.child_value());
			}
		}
	}

	void S100PCLineStylesReader::SetSymbol(MSXML2::IXMLDOMNodePtr _pNode, LineStylesPackage::LineSymbol* pSymbol)
	{
		for (int i = 0; i < _pNode->Getattributes()->Getlength(); i++)
		{
			MSXML2::IXMLDOMNodePtr pNode = _pNode->Getattributes()->Getitem(i);

			if ((pNode->nodeName) == _bstr_t(_T("reference")))
			{
				pSymbol->reference = pNode->text;
			}
		}

		for (int i = 0; i < _pNode->GetchildNodes()->Getlength(); i++)
		{
			MSXML2::IXMLDOMNodePtr pNode = _pNode->GetchildNodes()->Getitem(i);

			if ((pNode->nodeName) == _bstr_t(_T("position")))
			{
				pSymbol->position = _wtof(pNode->text);
			}
		}
	}
	void S100PCLineStylesReader::SetSymbol(pugi::xml_node node, LineStylesPackage::LineSymbol* pSymbol)
	{
		for (auto attri = node.first_attribute(); attri; attri = attri.next_attribute())
		{
			auto attriName = attri.name();
			if (!strcmp(attriName, "reference"))
			{
				pSymbol->reference = pugi::as_wide(attri.value());
			}
		}

		for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName, "position"))
			{
				pSymbol->position = std::stod(instruction.child_value());
			}
		}

	}

	void S100PCLineStylesReader::SetCompositeLineStyle(MSXML2::IXMLDOMNodePtr _pNode, LineStylesPackage::LineStyle* pLineStyle)
	{
		//Read Data
		for (int i = 0; i < _pNode->GetchildNodes()->Getlength(); i++)
		{
			MSXML2::IXMLDOMNodePtr pNode = _pNode->GetchildNodes()->Getitem(i);

			if ((pNode->nodeName) == _bstr_t(_T("intervalLength")))
			{
				pLineStyle->intervalLength = _wtof(_pNode->GetchildNodes()->Getitem(i)->text);
			}
			else if ((pNode->nodeName) == _bstr_t(_T("pen")))
			{
				pLineStyle->pen = new GraphicBasePackage::Pen();
				SetPen(_pNode->GetchildNodes()->Getitem(i), ((LineStylesPackage::LineStyle*)pLineStyle)->pen);
			}
			else if ((pNode->nodeName) == _bstr_t(_T("dash")))
			{
				LineStylesPackage::Dash* pDash = new LineStylesPackage::Dash();
				pLineStyle->dash.push_back(pDash);
				SetDash(_pNode->GetchildNodes()->Getitem(i), pDash);
			}
			else if ((pNode->nodeName) == _bstr_t(_T("symbol")))
			{
				LineStylesPackage::LineSymbol* pSymbol = new LineStylesPackage::LineSymbol();
				pLineStyle->symbol.push_back(pSymbol);
				SetSymbol(_pNode->GetchildNodes()->Getitem(i), pSymbol);
			}
		}
	}
	void S100PCLineStylesReader::SetCompositeLineStyle(pugi::xml_node node, LineStylesPackage::LineStyle* pLineStyle)
	{
		for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName, "intervalLength"))
			{
				pLineStyle->intervalLength = std::stod(instruction.child_value());
			}
			else if (!strcmp(instructionName, "pen"))
			{
				pLineStyle->pen = new GraphicBasePackage::Pen();
				SetPen(instruction, ((LineStylesPackage::LineStyle*)pLineStyle)->pen);
			}
			else if (!strcmp(instructionName, "dash"))
			{
				LineStylesPackage::Dash* pDash = new LineStylesPackage::Dash();
				pLineStyle->dash.push_back(pDash);
				SetDash(instruction, pDash);
			}
			else if (!strcmp(instructionName, "symbol"))
			{
				LineStylesPackage::LineSymbol* pSymbol = new LineStylesPackage::LineSymbol();
				pLineStyle->symbol.push_back(pSymbol);
				SetSymbol(instruction, pSymbol);
			}
		}
	}
}