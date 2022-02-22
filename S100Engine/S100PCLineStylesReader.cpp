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

					if (nullptr != pLineStyle)
					{
						pLineStyle->name = findFileData.cFileName;
						pLineStyles->mapLineStyle.insert({ findFileData.cFileName, pLineStyle });
					}
				}
			}
		} while (FindNextFile(hFind, &findFileData));


		FindClose(hFind);

		return true;

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