#include "stdafx.h"
#include "SVG.h"
#include "CommonDefinition.h"
#include "CSSFill.h"
#include "CSSStroke.h"
#include "CSSStrokeWidth.h"
#include "CSSStrokeLinecap.h"
#include "CSSStrokeLinejoin.h"
#include "CSSDisplay.h"
#include "CSSOpacity.h"
#include "UnitOfCSS.h"
#include "Path.h"
#include "Circle.h"
#include "OrgPath.h"
#include "DArc.h"
#include "DPoint.h"
#include "CSS.h"
#include "Description.h"
#include "IHO_Description.h"
#include "OrgCircle.h"
#include "DrawingPathUnit.h"
#include "DRAW_BRUSH_STRUCT.h"

#include "..\\LibMFCUtil\\LibMFCUtil.h"
#include "..\\LibMFCUtil\\StringTokenizer.h"

#include <locale>
#include <codecvt>
#include <vector>

#define RADIAN_TO_DEGREE(x) (180.0f * x / 3.141592f) 

namespace S100_SVG_D2D1_DLL
{
	SVG::SVG()
	{

	}

	SVG::~SVG()
	{
		std::list<DrawingPattern*>::iterator itor;
		for (itor = m_drawingObjList.begin(); itor != m_drawingObjList.end(); itor++)
		{
			if ((*itor)->type == 1)
			{
				Path* p = (Path*)(*itor);
				delete p;
			}
			else if ((*itor)->type == 2)
			{
				Circle* c = (Circle*)(*itor);
				delete c;
			}
		}

		std::list<OrgDrawingPattern*>::iterator orgitor;
		for (orgitor = m_orgDrawingObjList.begin(); orgitor != m_orgDrawingObjList.end(); orgitor++)
		{
			if ((*orgitor)->type == 2 ||
				(*orgitor)->type == 3)
			{
				OrgPath* p = (OrgPath*)(*orgitor);
				delete p;
			}
			else if ((*orgitor)->type == 4)
			{
				OrgCircle* c = (OrgCircle*)(*orgitor);
				delete c;
			}
		}
	}

	bool SVG::ReadSVGFileByPugiXml(std::wstring path)
	{
		if (!path.empty())
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(path.c_str());
			pugi::xml_node& node = doc.child("svg");

			m_filepath = path;

			GetSVGInfo(node);
		}

		OrganizeSymbol();
		return true;
	}

	void SVG::GetSVGInfo(pugi::xml_node& node)
	{
		std::unordered_map < std::string , pugi::xml_attribute > attri;
		for (pugi::xml_attribute attrinode = node.first_attribute(); attrinode; attrinode = attrinode.next_attribute())
		{
			attri[attrinode.name()]= attrinode;
		}

		auto height=attri.find("height");
		if (height!=attri.end())
		{
			m_height= pugi::as_wide(attri["height"].value());
		}
		else
		{
			m_height = L"10";
		}

		auto width= attri.find("width");
		if (width!=attri.end())
		{
			m_width = pugi::as_wide(attri["width"].value());
		}
		else
		{
			m_width = L"10";
		}

		auto viewBox = attri.find("viewBox");
		if (viewBox!=attri.end())
		{
			m_viewBox = pugi::as_wide(attri["viewBox"].value());
		}
		else 
		{
			m_viewBox = L"-5 -5 5 5";
		}

		for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			const pugi::char_t* instructionName = instruction.name();
			 if (!strcmp(instructionName, "title"))
			{
				GetTitle(instruction);
			}
			else if (!strcmp(instructionName, "desc"))
			{
				Getdesc(instruction);
			}
			else if (!strcmp(instructionName, "metadata"))
			{
				Getmetadata(instruction);

			}
			else if (!strcmp(instructionName, "rect"))
			{
				GetFigure(instruction);
			}
			else if (!strcmp(instructionName, "path"))
			{
				GetFigure(instruction);

			}
			else if (!strcmp(instructionName, "circle"))
			{
				GetFigure(instruction);
			}
			else 
			{

			}
		}
	}

	void SVG::GetTitle(pugi::xml_node& node)
	{
		if (!node)
			return;

		m_title.value = pugi::as_wide(node.child_value());

	}

	void SVG::Getdesc(pugi::xml_node& node)
	{
		if (!node)
			return;

		m_desc.value = pugi::as_wide(node.child_value());
	}

	void SVG::Getmetadata(pugi::xml_node& node)
	{
		if (!node)
			return;

		for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			const pugi::char_t* instructionName = instruction.name();
			if (!strcmp(instructionName, "iho:S100SVG"))
			{
				auto attri = instruction.attribute("xmlns:iho");
				if (attri)
				{
					m_metadata.s100svg.xmlns = pugi::as_wide(attri.value());
				}
				auto Description = instruction.child("iho:Description");
				if (Description)
				{
					auto attri = Description.attribute("iho:publisher");
					if (attri)
					{
						m_metadata.s100svg.desc.publisher = pugi::as_wide(attri.value());
					}

					attri = Description.attribute("iho:creationDate");
					if (attri)
					{
						m_metadata.s100svg.desc.creationDate = pugi::as_wide(attri.value());
					}

					attri = Description.attribute("iho:source");
					if (attri)
					{
						m_metadata.s100svg.desc.source = pugi::as_wide(attri.value());
					}

					attri = Description.attribute("iho:format");
					if (attri)
					{
						m_metadata.s100svg.desc.format = pugi::as_wide(attri.value());
					}

					attri = Description.attribute("iho:version");
					if (attri)
					{
						m_metadata.s100svg.desc.version = pugi::as_wide(attri.value());
					}
				}
			}
		}
	}

	void SVG::GetFigure(pugi::xml_node& node)
	{
		if (!node)
			return;

		auto nodename = pugi::as_wide(node.name());
		if (nodename.compare(L"rect") == 0)
		{
			auto attri = node.attribute("class");
			if (attri)
			{
				auto attriValue = pugi::as_wide(attri.value());
				if (attriValue.compare(L"symbolBox layout") == 0)
				{
					GetSymbolBoxLayout(node);
				}
				else if (attriValue.compare(L"svgBox layout") == 0)
				{
					GetSVGBoxLayout(node);
				}
				else
				{
					GetSymbol(node);
				}
			}
		}
		else if (nodename.compare(L"path") == 0)
		{
			GetSymbol(node);
		}

		else if (nodename.compare(L"circle") == 0)
		{
			auto attri = node.attribute("class");
			if (attri)
			{
				auto attriValue = pugi::as_wide(attri.value());
				if (attriValue.compare(L"pivotPoint layout") == 0)
				{
					GetPivotPointLayout(node);
				}
				else
				{
					GetSymbol(node);
				}
			}
		}
	}

	void SVG::GetSymbolBoxLayout(pugi::xml_node& node)
	{
		if (!node)
			return;


		for each (pugi::xml_attribute attri in node.attributes())
		{
			auto attriName = attri.name();
			if (!strcmp(attriName, "class"))
			{
				m_symbolLayout.className = pugi::as_wide(attri.value());
			}
			else if (!strcmp(attriName, "fill"))
			{
				m_symbolLayout.fill = pugi::as_wide(attri.value());
			}
			else if (!strcmp(attriName, "x"))
			{
				m_symbolLayout.x = pugi::as_wide(attri.value());
			}
			else if (!strcmp(attriName, "y"))
			{
				m_symbolLayout.y = pugi::as_wide(attri.value());
			}
			else if (!strcmp(attriName, "height"))
			{
				m_symbolLayout.height = pugi::as_wide(attri.value());
			}
			else if (!strcmp(attriName, "width"))
			{
				m_symbolLayout.width = pugi::as_wide(attri.value());
			}
		}
	}

	void SVG::GetSVGBoxLayout(pugi::xml_node& node)
	{
		if (!node)
			return;


		for each (pugi::xml_attribute attri in node.attributes())
		{
			auto attriName = attri.name();
			if (!strcmp(attriName, "class"))
			{
				m_SVGBoxLayout.className = pugi::as_wide(attri.value());
			}
			else if (!strcmp(attriName, "fill"))
			{
				m_SVGBoxLayout.fill = pugi::as_wide(attri.value());
			}
			else if (!strcmp(attriName, "x"))
			{
				m_SVGBoxLayout.x = pugi::as_wide(attri.value());
			}
			else if (!strcmp(attriName, "y"))
			{
				m_SVGBoxLayout.y = pugi::as_wide(attri.value());
			}
			else if (!strcmp(attriName, "height"))
			{
				m_SVGBoxLayout.height = pugi::as_wide(attri.value());
			}
			else if (!strcmp(attriName, "width"))
			{
				m_SVGBoxLayout.width = pugi::as_wide(attri.value());
			}
		}

	}

	void SVG::GetSymbol(pugi::xml_node& node)
	{
		if (!node)
			return;

		auto nodename = pugi::as_wide(node.name());
		if (0 == nodename.compare(L"path"))
		{
			Path* pPath = new Path();
			m_drawingObjList.push_back(pPath);

			for each (pugi::xml_attribute attri in node.attributes())
			{
				auto attriName = attri.name();
				if (!strcmp(attriName, "d"))
				{
					pPath->d = pugi::as_wide(attri.value());
				}
				else if (!strcmp(attriName, "class"))
				{
					pPath->className = pugi::as_wide(attri.value());
				}
				else if (!strcmp(attriName, "style"))
				{
					pPath->style = pugi::as_wide(attri.value());
				}
			}
		}
		else if (0 == nodename.compare(L"circle"))
		{
			Circle* pCircle = new Circle();
			m_drawingObjList.push_back(pCircle);

			for each (pugi::xml_attribute attri in node.attributes())
			{
				auto attriName = attri.name();
				if (!strcmp(attriName, "class"))
				{
					pCircle->className = pugi::as_wide(attri.value());
				}
				else if (!strcmp(attriName, "style"))
				{
					pCircle->style = pugi::as_wide(attri.value());
				}
				else if (!strcmp(attriName, "cx"))
				{
					pCircle->cx = pugi::as_wide(attri.value());
				}
				else if (!strcmp(attriName, "cy"))
				{
					pCircle->cy = pugi::as_wide(attri.value());
				}
				else if (!strcmp(attriName, "r"))
				{
					pCircle->r = pugi::as_wide(attri.value());
				}
			}
		}
		else if (0 == nodename.compare(L"rect"))
		{
			auto rect = new BoxLayout();

			for each (pugi::xml_attribute item in node.attributes())
			{
				auto name = item.name();
				if (0 == strcmp(name, "class"))
				{
					rect->SetClass(pugi::as_wide(item.value()));
				}
				else if (0 == strcmp(name, "style"))
				{
					rect->SetStyle(pugi::as_wide(item.value()));
				}
				else if (0 == strcmp(name, "fill"))
				{
					rect->SetFill(pugi::as_wide(item.value()));
				}
				else if (0 == strcmp(name, "x"))
				{
					rect->SetX(pugi::as_wide(item.value()));
				}
				else if (0 == strcmp(name, "y"))
				{
					rect->SetY(pugi::as_wide(item.value()));
				}
				else if (0 == strcmp(name, "height"))
				{
					rect->SetHeight(pugi::as_wide(item.value()));
				}
				else if (0 == strcmp(name, "width"))
				{
					rect->SetWidth(pugi::as_wide(item.value()));
				}
			}

			auto path = rect->ToPath();

			InsertDrawingObject(path);
			delete rect;
			rect = nullptr;
		}
	}

	void SVG::GetPivotPointLayout(pugi::xml_node& node)
	{
		if (!node)
			return;

		auto nodename = pugi::as_wide(node.name());

		for each (pugi::xml_attribute attri in node.attributes())
		{
			auto attriName = attri.name();
			if (!strcmp(attriName, "class"))
			{
				m_pivotPointLayout.className = pugi::as_wide(attri.value());
			}
			else if (!strcmp(attriName, "fill"))
			{
				m_pivotPointLayout.fill = pugi::as_wide(attri.value());
			}
			else if (!strcmp(attriName, "cx"))
			{
				m_pivotPointLayout.cx = pugi::as_wide(attri.value());
			}
			else if (!strcmp(attriName, "cy"))
			{
				m_pivotPointLayout.cy = pugi::as_wide(attri.value());
			}
			else if (!strcmp(attriName, "r"))
			{
				m_pivotPointLayout.r = pugi::as_wide(attri.value());
			}

		}
	}


	Symbol SVG::GetSymbol(ID2D1Factory* m_pDirect2dFactory, std::wstring paletteName, S100_ColorProfile *colorProfile)
	{
		float fx = 0;
		float fy = 0;
		float fradius = 0;
		OrgPath* path = NULL;
		OrgCircle* circle = NULL;
		std::string d, com, coordi;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::list<CSSStyle*>::iterator cssitor;

		int size = 0;
		DPoint *p = NULL;
		DArc *a = NULL;

		Symbol sym;

		for (auto i = m_orgDrawingObjList.begin(); i != m_orgDrawingObjList.end(); i++)
		{
			OrgDrawingPattern* pattern = *i;
			std::list<DRAW_BRUSH_STRUCT> brushes;
			float PEN_WIDTH = 1;
			int PEN_COLOR = 1;
			int PEN_STYLE = PS_SOLID;
			int EDGE_TYPE = 1;
			int CONNECT_TYPE = 1;
			bool DISPLAY = true;
			float OPACITY = 0;
			
			SymbolPath tempSym;

			float mx = 0;
			float my =  0;
			switch (pattern->type)
			{
			case 2:
			case 3:
				tempSym.geom = NULL;
				tempSym.width = 0;
				path = (OrgPath*)pattern;
				if (OPACITY > 0)
				{
					p = (DPoint*)path->dUnit[0];
					mx = p->x;
					my = p->y;
					float lx = mx, ly = my;

					//
					std::vector<D2D1_POINT_2F> tempVec;
					D2D1_POINT_2F point;
					tempVec.push_back(point);
					tempVec[tempVec.size() - 1].x = p->x;
					tempVec[tempVec.size() - 1].y = p->y;
					
					size = (int)path->dUnit.size();
					for (int i = 1; i < size; i++)
					{
						if (path->dUnit[i]->type == 1) // Draw Point
						{
							p = (DPoint*)path->dUnit[i];
							mx = p->x;
							my = p->y;

							lx = mx;
							ly = my;

							D2D1_POINT_2F point;
							tempVec.push_back(point);
							tempVec[tempVec.size() - 1].x = p->x;
							tempVec[tempVec.size() - 1].y = p->y;
						}
						else if (path->dUnit[i]->type == 2) // Draw Arc
						{
							a = (DArc*)path->dUnit[i];
						}
					}
					D2D1_POINT_2F* var = new D2D1_POINT_2F[tempVec.size()];

					for (unsigned i = 0; i < tempVec.size(); i++)
					{
						var[i] = tempVec[i];
					}

					m_pDirect2dFactory->CreatePathGeometry(&tempSym.geom);

					ID2D1GeometrySink* arrowSink;
					tempSym.geom->Open(&arrowSink);

					arrowSink->SetFillMode(D2D1_FILL_MODE_WINDING);
					arrowSink->BeginFigure(
						var[0],
						D2D1_FIGURE_BEGIN_FILLED);

					arrowSink->AddLines(var, (UINT32)(_msize(var) / sizeof(*var)));

					arrowSink->AddLine(tempVec[tempVec.size() - 1]);
					arrowSink->EndFigure(D2D1_FIGURE_END_OPEN);
					arrowSink->Close();
					SafeRelease(&arrowSink);
					delete var;

					for (auto bi = brushes.begin(); bi != brushes.end(); bi++)
					{
						SymColor symCol;
						symCol.c = RGB(((*bi).BRUSH_COLOR >> 16) & 0xff, ((*bi).BRUSH_COLOR >> 8) & 0xff, ((*bi).BRUSH_COLOR >> 0) & 0xff);
						symCol.opacity = OPACITY;
						symCol.fill = true;
						tempSym.color = symCol;
					}
				}
				else
				{
					p = (DPoint*)path->dUnit[0];

					if (false)
					{
						mx = p->x;
						my = p->y;
						RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, 0);

						size = (int)path->dUnit.size();
						for (int i = 1; i < size; i++)
						{
							if (path->dUnit[i]->type == 1) // Draw Point
							{
								p = (DPoint*)path->dUnit[i];
								mx = p->x;
								my = p->y;
								RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, 0);
							}
							else if (path->dUnit[i]->type == 2) // Draw Arc
							{
								a = (DArc*)path->dUnit[i];
								if (a->dwRadius == 0)
								{
									mx = a->sx;
									my = a->sy;
									RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, 0);
								}
								else
								{
									mx = a->sx;
									my = a->sy;
									RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, 0);
								}
							}
						}
					}
					else
					{
						//The drawing part.
						std::vector<D2D1_POINT_2F> tempVec;
						D2D1_POINT_2F point;
						tempVec.push_back(point);
						tempVec[tempVec.size() - 1].x = p->x;
						tempVec[tempVec.size() - 1].y = p->y;

						size = (int)path->dUnit.size();
						for (int i = 1; i < size; i++)
						{
							if (path->dUnit[i]->type == 1) // Draw Point
							{
								p = (DPoint*)path->dUnit[i];
								D2D1_POINT_2F point;
								tempVec.push_back(point);
								tempVec[tempVec.size() - 1].x = p->x;
								tempVec[tempVec.size() - 1].y = p->y;
							}
							else if (path->dUnit[i]->type == 2) // Draw Arc
							{
								a = (DArc*)path->dUnit[i];
							}
						}


						//
						D2D1_POINT_2F* var = new D2D1_POINT_2F[tempVec.size()];

						for (unsigned i = 0; i < tempVec.size(); i++) 
						{
							var[i] = tempVec[i];
						}

						m_pDirect2dFactory->CreatePathGeometry(&tempSym.geom);

						ID2D1GeometrySink* arrowSink;
						tempSym.geom->Open(&arrowSink);

						arrowSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
						arrowSink->BeginFigure(
							var[0],
							D2D1_FIGURE_BEGIN_FILLED);

						arrowSink->AddLines(var, (UINT32)(_msize(var) / sizeof(*var)));

						arrowSink->AddLine(tempVec[tempVec.size() - 1]);
						arrowSink->EndFigure(D2D1_FIGURE_END_OPEN);
						arrowSink->Close();
						SafeRelease(&arrowSink);
						delete var;
					}

					for (auto bi = brushes.begin(); bi != brushes.end(); bi++)
					{
						if ((*bi).FILL_POLYGON)
						{
							SymColor symCol;
							symCol.c = RGB((((*bi).BRUSH_COLOR >> 16) & 0xff), (((*bi).BRUSH_COLOR >> 8) & 0xff), (((*bi).BRUSH_COLOR >> 0) & 0xff));
							symCol.fill = true;
							symCol.opacity = 1;
							tempSym.color = symCol;

						}
						else
						{
							SymColor symCol;
							symCol.c = RGB(((PEN_COLOR >> 16) & 0xff), ((PEN_COLOR >> 8) & 0xff), ((PEN_COLOR >> 0) & 0xff));
							symCol.fill = false;
							symCol.opacity = 1;
							tempSym.color = symCol;
						}
					}

				}

				sym.symPath.push_back(tempSym);
				break;
			case 4:
				circle = (OrgCircle*)pattern;
				fradius = circle->radius;
				if (false)
				{
					mx = circle->center_x;
					my = circle->center_y;
					RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, 0);
					fx = mx;
					fy = my;
				}
				else
				{
					fx = (circle->center_x);
					fy = (circle->center_y);
				}

				for (auto bi = brushes.begin(); bi != brushes.end(); bi++)
				{
					if ((*bi).FILL_POLYGON)
					{
						SymbolCircle tempCir;
						m_pDirect2dFactory->CreateEllipseGeometry(
							D2D1::Ellipse(D2D1::Point2F(fx, fy), fradius, fradius),
							&tempCir.geom
						);

						SymColor symCol;
						symCol.c = RGB((((*bi).BRUSH_COLOR >> 16) & 0xff), (((*bi).BRUSH_COLOR >> 8) & 0xff), (((*bi).BRUSH_COLOR >> 0) & 0xff));
						symCol.fill = true;
						symCol.opacity = 1;
						tempCir.color = symCol;

						sym.symCircle.push_back(tempCir);
					}
					else
					{
						SymbolCircle tempCir;
						m_pDirect2dFactory->CreateEllipseGeometry
						(
							D2D1::Ellipse(D2D1::Point2F(fx, fy), fradius, fradius),
							&tempCir.geom
						);

						SymColor symCol;
						symCol.c = RGB(((PEN_COLOR >> 16) & 0xff), ((PEN_COLOR >> 8) & 0xff), ((PEN_COLOR >> 0) & 0xff));
						symCol.fill = false;
						symCol.opacity = 1;
						tempCir.color = symCol;

						sym.symCircle.push_back(tempCir);
					}
				}



				break;
			}
		}

		sym.height = m_OrgHeight;
		sym.width = m_OrgWidth;
		return sym;
	}

	void SVG::DrawSVGInfomationToWindow(HDC hdc, int cx, int cy)
	{
		CRect r;
		r.left = 10; r.right = 400;
		r.top = 10; r.bottom = 30;

		HPEN hPen = CreatePen(PS_DOT, 2, RGB(255, 0, 0));
		HPEN hPen2 = CreatePen(PS_DOT, 2, RGB(0, 255, 0));

		SelectObject(hdc, (HGDIOBJ)NULL_BRUSH);

		HPEN hOldPen = (HPEN)SelectObject(hdc, (HGDIOBJ)hPen);

		DrawText(hdc, m_title.value.c_str(), -1, &r, DT_LEFT);
		r.top = r.bottom;	r.bottom += 20;
		DrawText(hdc, m_desc.value.c_str(), -1, &r, DT_LEFT);

		double x = _ttof(m_SVGBoxLayout.x.c_str());
		double y = _ttof(m_SVGBoxLayout.y.c_str());
		double radius = 0;
		double height = _ttof(m_SVGBoxLayout.height.c_str());
		double width = _ttof(m_SVGBoxLayout.width.c_str());

		int ix = (int)(x * factor) + cx;
		int iy = (int)(y * factor) + cy;
		int iradius = 0;
		int iheight = (int)(height * factor);
		int iwidth = (int)(width * factor);
		Rectangle(hdc, ix, iy, ix + iwidth, iy + iheight);

		SelectObject(hdc, (HGDIOBJ)hPen2);
		x = _ttof(m_symbolLayout.x.c_str());
		y = _ttof(m_symbolLayout.y.c_str());
		height = _ttof(m_symbolLayout.height.c_str());
		width = _ttof(m_symbolLayout.width.c_str());

		ix = (int)(x * factor) + cx;
		iy = (int)(y * factor) + cy;
		iheight = (int)(height * factor);
		iwidth = (int)(width * factor);

		Rectangle(hdc, ix, iy, ix + iwidth, iy + iheight);

		////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////
		SelectObject(hdc, (HGDIOBJ)hOldPen);

		DeleteObject(hPen);
		DeleteObject(hPen2);


		std::list<DrawingPattern*>::iterator itor;
		Path* path = NULL;
		Circle* circle = NULL;
		size_t beg, end;
		std::string d, com, coordi;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		StringTokenizer *pstringTokenizer1, *pstringTokenizer2;
		for (itor = m_drawingObjList.begin(); itor != m_drawingObjList.end(); itor++)
		{
			DrawingPattern* pattern = *itor;

			switch (pattern->type)
			{
			case 1:
				path = (Path*)pattern;

				beg = path->d.find_first_not_of(L"\a\b\f\n\r\t\v ");
				end = path->d.find_last_not_of(L"\a\b\f\n\r\t\v ");

				d = converter.to_bytes(std::wstring(path->d, beg, end - beg + 1));
				pstringTokenizer1 = new StringTokenizer(d, " ");

				while (pstringTokenizer1->hasMoreTokens())
				{
					com = pstringTokenizer1->nextToken();
					if (pstringTokenizer1->hasMoreTokens())
					{
						coordi = pstringTokenizer1->nextToken();

						pstringTokenizer2 = new StringTokenizer(coordi, ",");

						if (pstringTokenizer2->hasMoreTokens())
						{
							x = atof(pstringTokenizer2->nextToken().c_str());
						}

						if (pstringTokenizer2->hasMoreTokens())
						{
							y = atof(pstringTokenizer2->nextToken().c_str());
						}

						ix = (int)(x * factor) + cx;
						iy = (int)(y * factor) + cy;

						if (com.compare("M") == 0)
						{
							MoveToEx(hdc, ix, iy, NULL);
						}
						else if (com.compare("L") == 0)
						{
							LineTo(hdc, ix, iy);
						}
						else if (com.compare("Z") == 0 || com.compare("z") == 0)
						{
							LineTo(hdc, ix, iy);
						}
						delete pstringTokenizer2;
					}
				}
				delete pstringTokenizer1;
				break;
			case 2:
				circle = (Circle*)pattern;

				x = _wtof(circle->cx.c_str());
				y = _wtof(circle->cy.c_str());
				radius = _wtof(circle->r.c_str());

				ix = (int)(x * factor) + cx;
				iy = (int)(y * factor) + cy;
				iradius = (int)(radius * factor);
				iradius = iradius > 1 ? iradius : 1;

				Ellipse(hdc, ix - iradius, iy + iradius, ix + iradius, iy - iradius);
				break;
			}
		}
	}

	void SVG::DrawSVGOrganizedInfomationToWindow()
	{

	}

	void SVG::DrawSVGOrganizedInfomationToWindow(HDC hdc, int cx, int cy, int rotation, float scaleFactor)
	{

		int ix = 0;
		int iy = 0;
		int iradius = 0;
		////////////////////////////////////////////////////////////////////////////////////////////
		std::list<OrgDrawingPattern*>::iterator itor;
		OrgPath* path = NULL;
		OrgCircle* circle = NULL;
		std::string d, com, coordi;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::list<CSSStyle*>::iterator cssitor;

		HPEN hOldPen;
		HPEN pen;
		HBRUSH brush;
		HBRUSH pOldBrush;
		LOGPEN lp;
		LOGBRUSH lb;
		int size = 0;
		DPoint *p = NULL;
		DArc *a = NULL;

		for (itor = m_orgDrawingObjList.begin(); itor != m_orgDrawingObjList.end(); itor++)
		{
			OrgDrawingPattern* pattern = *itor;

			std::list<DRAW_BRUSH_STRUCT> brushes;
			int PEN_WIDTH = 1;
			int PEN_COLOR = 1;
			int PEN_STYLE = PS_SOLID;
			int EDGE_TYPE = 1;
			int CONNECT_TYPE = 1;
			bool DISPLAY = true;
			double OPACITY = 0;

			int mx = 0;
			int my = 0;
			switch (pattern->type)
			{
			case 2:
			case 3:
				path = (OrgPath*)pattern;

				if (OPACITY > 0)
				{
					Gdiplus::GraphicsPath gPath;
					p = (DPoint*)path->dUnit[0];
					mx = (int)(p->x * scaleFactor);
					my = (int)(p->y * scaleFactor);
					RotatePoint(mx, my, (int)m_pivotPoint.x, (int)m_pivotPoint.y, (double)rotation);
					int lx = mx + cx, ly = my + cy;

					size = (int)path->dUnit.size();
					for (int i = 1; i < size; i++)
					{
						if (path->dUnit[i]->type == 1) // Draw Point
						{
							p = (DPoint*)path->dUnit[i];
							mx = (int)(p->x * scaleFactor);
							my = (int)(p->y * scaleFactor);
							RotatePoint(mx, my, (int)m_pivotPoint.x, (int)m_pivotPoint.y, rotation);
							gPath.AddLine(lx, ly, mx + cx, my + cy);

							lx = mx + cx;
							ly = my + cy;
						}
						else if (path->dUnit[i]->type == 2) // Draw Arc
						{
							a = (DArc*)path->dUnit[i];

							mx = (int)(a->sx * scaleFactor);
							my = (int)(a->sy * scaleFactor);
							RotatePoint(mx, my, (int)m_pivotPoint.x, (int)m_pivotPoint.y, rotation);

							if (a->dwRadius == 0)
								gPath.AddLine(lx, ly, mx + cx, my + cy);
							else
								gPath.AddArc(mx + cx, my + cy, (int)(a->dwRadius * 2) + 1, (int)(a->dwRadius * 2) + 1, a->eStartAngle, a->eSweepAngle);
						}
						for (auto bi = brushes.begin(); bi != brushes.end(); bi++)
						{
							Gdiplus::Graphics g(hdc);
							Gdiplus::Color color((int)(255.0f * OPACITY), ((*bi).BRUSH_COLOR >> 16) & 0xff, ((*bi).BRUSH_COLOR >> 8) & 0xff, ((*bi).BRUSH_COLOR >> 0) & 0xff);
							Gdiplus::SolidBrush solidBrush(color);
							g.FillPath(&solidBrush, &gPath);

						}
					}
				}
				else
				{
					BeginPath(hdc);
					p = (DPoint*)path->dUnit[0];

					if (rotation > 0)
					{
						mx = (int)(p->x * scaleFactor);
						my = (int)(p->y * scaleFactor);
						RotatePoint(mx, my, (int)m_pivotPoint.x, (int)m_pivotPoint.y, rotation);
						::MoveToEx(hdc, mx + cx, my + cy, NULL);
					
						size = (int)path->dUnit.size();
						for (int i = 1; i < size; i++)
						{
							if (path->dUnit[i]->type == 1) // Draw Point
							{
								p = (DPoint*)path->dUnit[i];
								mx = (int)(p->x * scaleFactor);
								my = (int)(p->y * scaleFactor);
								RotatePoint(mx, my, (int)m_pivotPoint.x, (int)m_pivotPoint.y, rotation);
								::LineTo(hdc, mx + cx, my + cy);
							}
							else if (path->dUnit[i]->type == 2) // Draw Arc
							{
								a = (DArc*)path->dUnit[i];
								if (a->dwRadius == 0)
								{
									mx = (int)(a->sx * scaleFactor);
									my = (int)(a->sy * scaleFactor);
									RotatePoint(mx, my, (int)m_pivotPoint.x, (int)m_pivotPoint.y, rotation);
									::LineTo(hdc, mx + cx, my + cy);
								}
								else
								{
									mx = (int)(a->sx * scaleFactor);
									my = (int)(a->sy * scaleFactor);
									RotatePoint(mx, my, (int)m_pivotPoint.x, (int)m_pivotPoint.y, rotation);
									::AngleArc(hdc, mx + cx, my + cy, (DWORD)(a->dwRadius * scaleFactor + 1), a->eStartAngle + rotation, a->eSweepAngle);
								}
							}
						}
						EndPath(hdc);
					}
					else
					{
						::MoveToEx(hdc, (int)(p->x * scaleFactor + cx), (int)(p->y * scaleFactor + cy), NULL);

						size = (int)path->dUnit.size();
						for (int i = 1; i < size; i++)
						{
							if (path->dUnit[i]->type == 1) // Draw Point
							{
								p = (DPoint*)path->dUnit[i];
								::LineTo(hdc, (int)(p->x * scaleFactor + cx), (int)(p->y * scaleFactor + cy));
							}
							else if (path->dUnit[i]->type == 2) // Draw Arc
							{
								a = (DArc*)path->dUnit[i];
								if (a->dwRadius == 0)
									::LineTo(hdc, (int)(a->sx * scaleFactor + cx), (int)(a->sy * scaleFactor + cy));
								else
									::AngleArc(hdc, (int)(a->sx * scaleFactor + cx), (int)(a->sy * scaleFactor + cy), (DWORD)(a->dwRadius * scaleFactor + 1), a->eStartAngle, a->eSweepAngle);
							}
						}
						EndPath(hdc);
					}

					for (auto bi = brushes.begin(); bi != brushes.end(); bi++)
					{
						if ((*bi).FILL_POLYGON)
						{
							brush = CreateSolidBrush(RGB((((*bi).BRUSH_COLOR >> 16) & 0xff), (((*bi).BRUSH_COLOR >> 8) & 0xff), (((*bi).BRUSH_COLOR >> 0) & 0xff)));
							pOldBrush = (HBRUSH)SelectObject(hdc, (HGDIOBJ)brush);
							::FillPath(hdc);
							SelectObject(hdc, (HGDIOBJ)pOldBrush);
							DeleteObject(brush);
						}
						else
						{
							pen = CreatePen(PEN_STYLE, PEN_WIDTH, RGB(((PEN_COLOR >> 16) & 0xff), ((PEN_COLOR >> 8) & 0xff), ((PEN_COLOR >> 0) & 0xff)));
							hOldPen = (HPEN)SelectObject(hdc, (HGDIOBJ)pen);
							::StrokePath(hdc);
							SelectObject(hdc, (HGDIOBJ)hOldPen);
							DeleteObject(pen);
						}
					}
				}
				break;
			case 4:
				circle = (OrgCircle*)pattern;
				iradius = (int)(circle->radius * scaleFactor) > 1 ? (int)(circle->radius * scaleFactor) : 1;

				if (rotation > 0)
				{
					mx = (int)(circle->center_x * scaleFactor);
					my = (int)(circle->center_y * scaleFactor);
					RotatePoint(mx, my, (int)m_pivotPoint.x, (int)m_pivotPoint.y, rotation);
					ix = mx + cx;
					iy = my + cy;
				}
				else
				{
					ix = (int)(circle->center_x * scaleFactor) + cx;
					iy = (int)(circle->center_y * scaleFactor) + cy;
				}
				for (auto bi = brushes.begin(); bi != brushes.end(); bi++)
				{
					if ((*bi).FILL_POLYGON)
					{
						lp.lopnStyle = PS_NULL;
						pen = ::CreatePenIndirect(&lp);
						hOldPen = (HPEN)SelectObject(hdc, pen);
						brush = CreateSolidBrush(RGB((((*bi).BRUSH_COLOR >> 16) & 0xff), (((*bi).BRUSH_COLOR >> 8) & 0xff), (((*bi).BRUSH_COLOR >> 0) & 0xff)));
						pOldBrush = (HBRUSH)SelectObject(hdc, (HGDIOBJ)brush);

						Ellipse(hdc, ix - iradius, iy + iradius, ix + iradius + 1, iy - iradius + 1);
						SelectObject(hdc, (HGDIOBJ)hOldPen);
						SelectObject(hdc, (HGDIOBJ)pOldBrush);
						DeleteObject(pen);
						DeleteObject(brush);
					}
					else
					{
						lb.lbStyle = BS_NULL;
						brush = ::CreateBrushIndirect(&lb);
						pOldBrush = (HBRUSH)SelectObject(hdc, (HGDIOBJ)brush);
						pen = CreatePen(PEN_STYLE, PEN_WIDTH, RGB(((PEN_COLOR >> 16) & 0xff), ((PEN_COLOR >> 8) & 0xff), ((PEN_COLOR >> 0) & 0xff)));
						hOldPen = (HPEN)SelectObject(hdc, (HGDIOBJ)pen);

						Ellipse(hdc, ix - iradius, iy + iradius, ix + iradius + 1, iy - iradius + 1);
						SelectObject(hdc, (HGDIOBJ)hOldPen);
						SelectObject(hdc, (HGDIOBJ)pOldBrush);
						DeleteObject(pen);
						DeleteObject(brush);
					}
				}

				break;
			}
		}
	}
	void SVG::DrawSVGOrganizedInfomationToWindow(Gdiplus::Graphics *bitmapG, Gdiplus::PointF originPoint, Gdiplus::PointF symbolSize, float position, double rotation)
	{
		float ix = 0;
		float iy = 0;
		float iradius = 0;
		////////////////////////////////////////////////////////////////////////////////////////////
		std::list<OrgDrawingPattern*>::iterator itor;
		OrgPath* path = NULL;
		OrgCircle* circle = NULL;
		std::string d, com, coordi;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::list<CSSStyle*>::iterator cssitor;

		int size = 0;
		DPoint *p = NULL;
		DArc *a = NULL;

		for (itor = m_orgDrawingObjList.begin(); itor != m_orgDrawingObjList.end(); itor++)
		{
			OrgDrawingPattern* pattern = *itor;

			std::list<DRAW_BRUSH_STRUCT> brushes;
			int PEN_WIDTH = 1;
			int PEN_COLOR = 1;
			int PEN_STYLE = PS_SOLID;
			int EDGE_TYPE = 1;
			int CONNECT_TYPE = 1;
			bool DISPLAY = true;
			double OPACITY = 0;

			float mx = 0;
			float my = 0;
			switch (pattern->type)
			{
			case 2:
			case 3:
				path = (OrgPath*)pattern;
				if (OPACITY > 0)
				{
					Gdiplus::GraphicsPath gPath;

					p = (DPoint*)path->dUnit[0];
					mx = (float)p->x + position;
					my = (float)p->y;
					RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, rotation);
					float lx = mx + originPoint.X, ly = my + originPoint.Y;

					size = (int)path->dUnit.size();
					for (int i = 1; i < size; i++)
					{
						if (path->dUnit[i]->type == 1) // Draw Point
						{
							p = (DPoint*)path->dUnit[i];
							mx = (float)p->x + position;
							my = (float)p->y;
							RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, rotation);
							gPath.AddLine(lx, ly, mx + originPoint.X, my + originPoint.Y);

							lx = mx + originPoint.X;
							ly = my + originPoint.Y;
						}
						else if (path->dUnit[i]->type == 2) // Draw Arc
						{
							a = (DArc*)path->dUnit[i];

							mx = (float)a->sx + position;
							my = (float)a->sy;
							RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, rotation);

							if (a->dwRadius == 0)
								gPath.AddLine(lx, ly, mx + originPoint.X, my + originPoint.Y);
							else
								gPath.AddArc(mx + originPoint.X, my + originPoint.Y, (float)a->dwRadius * 2 + 1, (float)a->dwRadius * 2 + 1, (float)a->eStartAngle, (float)a->eSweepAngle);
						}
					}

					for (auto bi = brushes.begin(); bi != brushes.end(); bi++)
					{
						Gdiplus::Color color((int)(255.0f * OPACITY), ((*bi).BRUSH_COLOR >> 16) & 0xff, ((*bi).BRUSH_COLOR >> 8) & 0xff, ((*bi).BRUSH_COLOR >> 0) & 0xff);
						Gdiplus::SolidBrush solidBrush(color);
						bitmapG->FillPath(&solidBrush, &gPath);
					}
				}
				else
				{
					Gdiplus::GraphicsPath gPath;
					p = (DPoint*)path->dUnit[0];

					mx = (float)p->x + position;
					my = (float)p->y;
					RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, rotation);
					float lx = mx + originPoint.X, ly = my + originPoint.Y;

					size = (int)path->dUnit.size();
					for (int i = 1; i < size; i++)
					{
						if (path->dUnit[i]->type == 1) // Draw Point
						{
							p = (DPoint*)path->dUnit[i];
							mx = (float)p->x + position;
							my = (float)p->y;
							RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, rotation);
							gPath.AddLine(lx, ly, mx + originPoint.X, my + originPoint.Y);

							lx = mx + originPoint.X;
							ly = my + originPoint.Y;
						}
						else if (path->dUnit[i]->type == 2) // Draw Arc
						{
							a = (DArc*)path->dUnit[i];

							mx = (float)a->sx + position;
							my = (float)a->sy;
							RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, rotation);

							if (a->dwRadius == 0)
								gPath.AddLine(lx, ly, mx + originPoint.X, my + originPoint.Y);
							else
								gPath.AddArc(mx + originPoint.X, my + originPoint.Y, (float)a->dwRadius * 2 + 1, (float)a->dwRadius * 2 + 1, (float)a->eStartAngle, (float)a->eSweepAngle);
						}
					}
					for (auto bi = brushes.begin(); bi != brushes.end(); bi++)
					{
						if ((*bi).FILL_POLYGON)
						{
							Gdiplus::Color color(((*bi).BRUSH_COLOR >> 16) & 0xff, ((*bi).BRUSH_COLOR >> 8) & 0xff, ((*bi).BRUSH_COLOR >> 0) & 0xff);
							Gdiplus::SolidBrush solidBrush(color);
							bitmapG->FillPath(&solidBrush, &gPath);
						}
						else
						{
							Gdiplus::Color color((PEN_COLOR >> 16) & 0xff, (PEN_COLOR >> 8) & 0xff, (PEN_COLOR >> 0) & 0xff);
							Gdiplus::Pen pen(color);
							bitmapG->DrawPath(&pen, &gPath);
						}
					}
				}
				break;
			case 4:
				circle = (OrgCircle*)pattern;
				iradius = (float)(circle->radius);
				iradius = iradius > 1 ? iradius : 1;
				float tx, ty;

				double angleVer = rotation * DEG2RAD;

				double cosVerValue = cos(angleVer);
				double sinVerValue = sin(angleVer);
				tx = originPoint.X + (Gdiplus::REAL)(position  * cosVerValue) - (iradius);
				ty = originPoint.Y + (Gdiplus::REAL)(position  * sinVerValue) - (iradius);

				for (auto bi = brushes.begin(); bi != brushes.end(); bi++)
				{
					if ((*bi).FILL_POLYGON)
					{
						Gdiplus::Color color(((*bi).BRUSH_COLOR >> 16) & 0xff, ((*bi).BRUSH_COLOR >> 8) & 0xff, ((*bi).BRUSH_COLOR >> 0) & 0xff);
						Gdiplus::SolidBrush solidBrush(color);
						bitmapG->FillEllipse(&solidBrush, tx, ty, iradius * 2, iradius * 2);
					}
					else
					{
						Gdiplus::Color color((PEN_COLOR >> 16) & 0xff, (PEN_COLOR >> 8) & 0xff, (PEN_COLOR >> 0) & 0xff);
						Gdiplus::Pen pen(color);
						bitmapG->DrawEllipse(&pen, tx, ty, iradius * 2, iradius * 2);
					}
				}
				break;
			}
		}
	}
	void SVG::DrawSVGOrganizedInfomationToWindow(Gdiplus::Graphics *bitmapG, Gdiplus::PointF originPoint, double rotation)
	{
		float ix = 0;
		float iy = 0;
		float iradius = 0;
		////////////////////////////////////////////////////////////////////////////////////////////
		std::list<OrgDrawingPattern*>::iterator itor;
		OrgPath* path = NULL;
		OrgCircle* circle = NULL;
		std::string d, com, coordi;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::list<CSSStyle*>::iterator cssitor;

		int size = 0;
		DPoint *p = NULL;
		DArc *a = NULL;

		float symbolLayout_X = _wtoi(m_symbolLayout.x.c_str())* factor;
		float symbolLayout_Y = _wtoi(m_symbolLayout.y.c_str())* factor;

		for (itor = m_orgDrawingObjList.begin(); itor != m_orgDrawingObjList.end(); itor++)
		{
			OrgDrawingPattern* pattern = *itor;

			bool FILL_POLYGON = false;
			int BRUSH_COLOR = 0;
			int PEN_WIDTH = 1;
			int PEN_COLOR = 1;
			int PEN_STYLE = PS_SOLID;
			int EDGE_TYPE = 1;
			int CONNECT_TYPE = 1;
			bool DISPLAY = true;
			double OPACITY = 0;

			float mx = 0;
			float my = 0;


			switch (pattern->type)
			{
			case 2:
			case 3:
				path = (OrgPath*)pattern;

				if (OPACITY > 0)
				{
					Gdiplus::Color color((int)(255.0f * OPACITY), (BRUSH_COLOR >> 16) & 0xff, (BRUSH_COLOR >> 8) & 0xff, (BRUSH_COLOR >> 0) & 0xff);
					Gdiplus::SolidBrush solidBrush(color);
					Gdiplus::Pen gPen(&solidBrush, (Gdiplus::REAL)PEN_WIDTH);
					Gdiplus::GraphicsPath gPath;

					p = (DPoint*)path->dUnit[0];
					mx = (float)p->x - symbolLayout_X;
					my = (float)p->y - symbolLayout_Y;
					RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, rotation);
					float lx = mx + originPoint.X, ly = my + originPoint.Y;

					size = (int)path->dUnit.size();
					for (int i = 1; i < size; i++)
					{
						if (path->dUnit[i]->type == 1) // Draw Point
						{
							p = (DPoint*)path->dUnit[i];
							mx = (float)p->x - symbolLayout_X;
							my = (float)p->y - symbolLayout_Y;
							RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, rotation);
							gPath.AddLine(lx, ly, mx + originPoint.X, my + originPoint.Y);

							lx = mx + originPoint.X;
							ly = my + originPoint.Y;
						}
						else if (path->dUnit[i]->type == 2) // Draw Arc
						{
							a = (DArc*)path->dUnit[i];

							mx = (float)a->sx - symbolLayout_X;
							my = (float)a->sy - symbolLayout_Y;
							RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, rotation);

							if (a->dwRadius == 0)
								gPath.AddLine(lx, ly, mx + originPoint.X, my + originPoint.Y);
							else
								gPath.AddArc(mx + originPoint.X, my + originPoint.Y, (float)a->dwRadius * 2 + 1, (float)a->dwRadius * 2 + 1, (float)a->eStartAngle, (float)a->eSweepAngle);
						}
					}
					
					bitmapG->DrawPath(&gPen, &gPath);
				}
				else
				{
					Gdiplus::GraphicsPath gPath;
					p = (DPoint*)path->dUnit[0];

					mx = (float)p->x - symbolLayout_X;
					my = (float)p->y - symbolLayout_Y;
					RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, rotation);
					float lx = mx + originPoint.X, ly = my + originPoint.Y;

					size = (int)path->dUnit.size();
					for (int i = 1; i < size; i++)
					{
						if (path->dUnit[i]->type == 1) // Draw Point
						{
							p = (DPoint*)path->dUnit[i];
							mx = (float)p->x - symbolLayout_X;
							my = (float)p->y - symbolLayout_Y;
							RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, rotation);
							gPath.AddLine(lx, ly, mx + originPoint.X, my + originPoint.Y);
							lx = mx + originPoint.X;
							ly = my + originPoint.Y;
						}
						else if (path->dUnit[i]->type == 2) // Draw Arc
						{
							a = (DArc*)path->dUnit[i];

							mx = (float)a->sx - symbolLayout_X;
							my = (float)a->sy - symbolLayout_Y;
							RotatePoint(mx, my, m_pivotPoint.x, m_pivotPoint.y, rotation);

							if (a->dwRadius == 0)
								gPath.AddLine(lx, ly, mx + originPoint.X, my + originPoint.Y);
							else
								gPath.AddArc(mx + originPoint.X, my + originPoint.Y, (float)a->dwRadius * 2 + 1, (float)a->dwRadius * 2 + 1, (float)a->eStartAngle, (float)a->eSweepAngle);
						}
					}

					if (FILL_POLYGON)
					{
						Gdiplus::Color color((BRUSH_COLOR >> 16) & 0xff, (BRUSH_COLOR >> 8) & 0xff, (BRUSH_COLOR >> 0) & 0xff);
						Gdiplus::SolidBrush solidBrush(color);
						bitmapG->FillPath(&solidBrush, &gPath);
					}
					else
					{
						Gdiplus::Color color((PEN_COLOR >> 16) & 0xff, (PEN_COLOR >> 8) & 0xff, (PEN_COLOR >> 0) & 0xff);
						Gdiplus::Pen pen(color, (Gdiplus::REAL)PEN_WIDTH);

						bitmapG->DrawPath(&pen, &gPath);
					}
				}
				break;
			case 4:
				circle = (OrgCircle*)pattern;
				iradius = (float)(circle->radius * factor * 2);
				iradius = iradius > 1 ? iradius : 1;
				float tx = iradius / 2, ty = iradius / 2;


				RotatePoint(tx, ty, 0, 0, rotation - 90);
				ix += originPoint.X + tx;
				iy += originPoint.Y + ty;

				Gdiplus::Color color((BRUSH_COLOR >> 16) & 0xff, (BRUSH_COLOR >> 8) & 0xff, (BRUSH_COLOR >> 0) & 0xff);
				if (FILL_POLYGON)
				{
					Gdiplus::SolidBrush solidBrush(color);
					bitmapG->FillEllipse(&solidBrush, ix, iy, iradius + 1, iradius + 1);
				}
				else
				{
					Gdiplus::Pen pen(color);
					bitmapG->DrawEllipse(&pen, ix, iy, iradius + 1, iradius + 1);
				}
				break;
			}
		}
	}

	void SVG::DrawSVGSimpleLine(Gdiplus::Graphics *bitmapG, Gdiplus::PointF _p1, Gdiplus::PointF _p2)
	{
		std::list<OrgDrawingPattern*>::iterator itor;
		std::list<CSSStyle*>::iterator cssitor;

		for (itor = m_orgDrawingObjList.begin(); itor != m_orgDrawingObjList.end(); itor++)
		{
			OrgDrawingPattern* pattern = *itor;

			bool FILL_POLYGON = false;
			int BRUSH_COLOR = 0;
			int PEN_WIDTH = 1;
			int PEN_COLOR = 1;
			int PEN_STYLE = PS_SOLID;
			int EDGE_TYPE = 1;
			int CONNECT_TYPE = 1;
			bool DISPLAY = true;
			double OPACITY = 0;

			float mx = 0;
			float my = 0;
			if (OPACITY == 0)
				OPACITY = 1;
			Gdiplus::Color color((int)(255.0f * OPACITY), (PEN_COLOR >> 16) & 0xff, (PEN_COLOR >> 8) & 0xff, (PEN_COLOR >> 0) & 0xff);
			Gdiplus::Pen pen(color, (Gdiplus::REAL)PEN_WIDTH);
			bitmapG->DrawLine(&pen, _p1, _p2);
		}
	}

	void SVG::OrganizeSymbol()
	{
		float x, y, radius;
		float rx, ry, dx, dy;
		int x_axis_ratation, large_arc_flag, sweep_flag;
		Path* path = NULL;
		Circle* circle = NULL;
		size_t beg, end;
		std::string d, com;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		StringTokenizer *pstringTokenizer1;

		std::vector<DrawingPathUnit*> vectorUnits;
		std::list<CSSStyle*> styles;

		OrgPath *orgPath = NULL;
		OrgCircle *orgCircle = NULL;

		DPoint *dPoint = NULL;
		DArc *dArc = NULL;

		float lx, ly;
		std::string tok;
		char headChar;
		std::list<std::string> tokens;
		std::list<std::string>::iterator tokitor;

		m_pivotPoint.x = (float)(_wtof(m_pivotPointLayout.cx.c_str()) * factor);
		m_pivotPoint.y = (float)(_wtof(m_pivotPointLayout.cy.c_str()) * factor);

		m_OrgWidth = (float)(_wtof(m_width.c_str()) * factor);
		m_OrgHeight = (float)(_wtof(m_height.c_str()) * factor);

		for (auto itor = m_drawingObjList.begin(); itor != m_drawingObjList.end(); itor++)
		{
			DrawingPattern* pattern = *itor;

			vectorUnits.clear();
			styles.clear();

			switch (pattern->type)
			{
			case 1: // Path
				styles.clear();

				path = (Path*)pattern;

				if (path->style.size() > 0)
				{
					std::list<CSSStyle*> retStyle = GetStyle(1, path->style);
					styles.insert(styles.begin(), retStyle.begin(), retStyle.end());
				}

				if (path->className.size() > 0)
				{
					std::list<CSSStyle*> retStyle = GetStyle(2, path->className);
					styles.insert(styles.begin(), retStyle.begin(), retStyle.end());
				}

				beg = path->d.find_first_not_of(L"\a\b\f\n\r\t\v ");
				end = path->d.find_last_not_of(L"\a\b\f\n\r\t\v ");

				d = converter.to_bytes(std::wstring(path->d, beg, end - beg + 1));
				pstringTokenizer1 = new StringTokenizer(d, " ,");

				tokens.clear();

				while (pstringTokenizer1->hasMoreTokens())
				{
					tok = pstringTokenizer1->nextToken();
					if (tok.size() <= 0)
						continue;

					headChar = tok.c_str()[0];
					if (tok.substr(0, 5).compare("class") == 0)
					{

					}
					else if (headChar <= 'z' &&
						headChar >= 'A' &&
						tok.size() > 1)
					{
						tokens.push_back(tok.substr(0, 1));
						tokens.push_back(tok.substr(1, tok.size() - 1));
					}
					else
					{
						tokens.push_back(tok);
					}
				}
				delete pstringTokenizer1;

				for (tokitor = tokens.begin(); tokitor != tokens.end(); tokitor++)
				{
					com = *tokitor;

					if (com.compare("M") == 0 ||
						com.compare("L") == 0)
					{
						tokitor++;
						if (tokitor != tokens.end()) {
							x = (float)atof((*tokitor).c_str());
						}
						else {
							continue;
						}

						tokitor++;
						if (tokitor != tokens.end()) {
							y = (float)atof((*tokitor).c_str());
						}
						else {
							continue;
						}
						dPoint = new DPoint();
						dPoint->x = (x * factor);
						dPoint->y = (y * factor);
						vectorUnits.push_back(dPoint);

						lx = x;
						ly = y;
					}
					else if (com.compare("Z") == 0 || com.compare("z") == 0)
					{
						DPoint* pClosePoint = new DPoint();

						dPoint = (DPoint*)vectorUnits[0];
						pClosePoint->x = dPoint->x;
						pClosePoint->y = dPoint->y;
						vectorUnits.push_back(pClosePoint);

						orgPath = new OrgPath();
						orgPath->orgValue = path;
						orgPath->fill = true;
						MoveVectorPoint(orgPath->dUnit, vectorUnits);
						orgPath->type = 3;

						m_orgDrawingObjList.push_back(orgPath);
					}
					else if (com.compare("A") == 0)
					{
						tokitor++;
						if (tokitor != tokens.end()) {
							rx = (float)atof((*tokitor).c_str());
						}
						else { continue; }

						tokitor++;
						if (tokitor != tokens.end()) {
							ry = (float)atof((*tokitor).c_str());
						}
						else { continue; }

						tokitor++;
						if (tokitor != tokens.end()) {
							x_axis_ratation = atoi((*tokitor).c_str());
						}
						else { continue; }

						tokitor++;
						if (tokitor != tokens.end()) {
							large_arc_flag = atoi((*tokitor).c_str());
						}
						else { continue; }

						tokitor++;
						if (tokitor != tokens.end()) {
							sweep_flag = atoi((*tokitor).c_str());
						}
						else { continue; }

						tokitor++;
						if (tokitor != tokens.end()) {
							dx = (float)atof((*tokitor).c_str());
						}
						else { continue; }

						tokitor++;
						if (tokitor != tokens.end()) {
							dy = (float)atof((*tokitor).c_str());
						}
						else { continue; }

						dArc = new DArc();

						if (dy != ly && dx != lx)
						{
							dArc->sx = (dx * factor);
							dArc->sy = (dy * factor);
							dArc->dwRadius = 0;
							dArc->eStartAngle = 0;
							dArc->eSweepAngle = 0;
						}
						else
						{
							int startAngle = 0;
							if (dy == ly)
								startAngle = (int)(acos(dx - lx) * 180 / 3.141592f);
							else if (dx == lx)
								startAngle = (int)(acos(dy - ly) * 180 / 3.141592f + 90);

							dArc->sx = ((lx + (dx - lx) / 2) * factor);
							dArc->sy = ((ly + (dy - ly) / 2) * factor);
							dArc->dwRadius = rx * factor;
							dArc->eStartAngle = (float)startAngle;

							float direction = sweep_flag == 1 ? -1.0f : 1.0f;

							dArc->eSweepAngle = (float)direction * 180;
						}
						vectorUnits.push_back(dArc);
					}
				}

				if (vectorUnits.size() > 0)
				{
					orgPath = new OrgPath();
					orgPath->orgValue = path;
					orgPath->fill = false;
					MoveVectorPoint(orgPath->dUnit, vectorUnits);
					orgPath->type = 2;

					m_orgDrawingObjList.push_back(orgPath);
				}


				break;
			case 2:
				circle = (Circle*)pattern;

				if (circle->style.size() > 0)
				{
					std::list<CSSStyle*> retStyle = GetStyle(1, circle->style);
					styles.insert(styles.begin(), retStyle.begin(), retStyle.end());
				}

				if (circle->className.size() > 0)
				{
					std::list<CSSStyle*> retStyle = GetStyle(2, circle->className);
					styles.insert(styles.begin(), retStyle.begin(), retStyle.end());
				}

				OrgCircle *orgCircle = new OrgCircle();
				x = (float)_wtof(circle->cx.c_str());
				y = (float)_wtof(circle->cy.c_str());
				radius = (float)_wtof(circle->r.c_str());

				orgCircle->center_x = (x * factor);
				orgCircle->center_y = (y * factor);
				orgCircle->radius = (radius * factor);
				orgCircle->type = 4;

				m_orgDrawingObjList.push_back(orgCircle);
				break;
			}
		}
	}

	void SVG::MoveVectorPoint(std::vector<DrawingPathUnit*> &pOrgVector, std::vector<DrawingPathUnit*> &pSourceVector)
	{
		std::vector<DrawingPathUnit*>::iterator itor;
		DPoint *dPoint = NULL;
		DArc *dArc = NULL;
		int i = 0;
		for (itor = pSourceVector.begin(); itor != pSourceVector.end(); itor++)
		{
			pOrgVector.push_back(*itor);
		}
		pSourceVector.clear();
	}

	std::list<CSSStyle*> SVG::GetStyle(int type, std::wstring _strStyle)
	{
		std::list<CSSStyle*> styles;

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		StringTokenizer *pstringTokenizer1, *pstringTokenizer2;

		std::string tokDeclaration;
		std::string tokProperty, tokValue;
		std::string strStyle = converter.to_bytes(_strStyle);
		std::string strStyleUnit;
		std::wstring wstrProperty, wstrValue, wstrStyleUnit;
		switch (type)
		{
		case 1: // style
			pstringTokenizer1 = new StringTokenizer(strStyle, ";");

			while (pstringTokenizer1->hasMoreTokens())
			{
				tokDeclaration = pstringTokenizer1->nextToken();
				pstringTokenizer2 = new StringTokenizer(tokDeclaration, ":");

				if (pstringTokenizer2->hasMoreTokens())
				{
					tokProperty = pstringTokenizer2->nextToken();

					if (pstringTokenizer2->hasMoreTokens())
					{
						tokValue = pstringTokenizer2->nextToken();
					}
					else
					{
						delete pstringTokenizer1;
						delete pstringTokenizer2;
						continue;
					}
				}
				delete pstringTokenizer2;

				wstrProperty = converter.from_bytes(tokProperty);
				wstrValue = converter.from_bytes(tokValue);
			}
			delete pstringTokenizer1;
			break;
		case 2: // class
			pstringTokenizer1 = new StringTokenizer(strStyle, " ");
			while (pstringTokenizer1->hasMoreTokens())
			{
				strStyleUnit = pstringTokenizer1->nextToken();
				wstrStyleUnit = converter.from_bytes(strStyleUnit);

				std::unordered_map<std::wstring, UnitOfCSS*>::iterator cuitor;
			}
			delete pstringTokenizer1;
			break;
		}
		return styles;
	}

	void SVG::RotatePoint(POINT &p, const POINT pivotPoint, const double ROTATION)
	{
		double pi = acos(-1.0);
		double DEGRAD = pi / 180;

		double cosV = cos(ROTATION * DEGRAD);
		double sinV = sin(ROTATION * DEGRAD);

		p.x = (LONG)(p.x * cosV - p.y * sinV);
		p.y = (LONG)(p.x * sinV + p.y * cosV);
	}

	void SVG::RotatePoint(int &x, int &y, const int pvX, const int pvY, const double ROTATION)
	{
		Gdiplus::GpMatrix matrix;
		Gdiplus::GpPointF gp((Gdiplus::REAL)x, (Gdiplus::REAL)y);
		Gdiplus::GpPointF cp((Gdiplus::REAL)pvX, (Gdiplus::REAL)pvY);
		matrix.RotateAt((Gdiplus::REAL)ROTATION, cp);
		matrix.TransformPoints(&gp);
		x = (int)gp.X;
		y = (int)gp.Y;
	}

	void SVG::RotatePoint(Gdiplus::PointF &p, const POINT pivotPoint, const double ROTATION)
	{
		double pi = acos(-1.0);
		double DEGRAD = pi / 180;

		double cosV = cos(ROTATION * DEGRAD);
		double sinV = sin(ROTATION * DEGRAD);

		p.X = (float)(p.X * cosV - p.Y * sinV);
		p.Y = (float)(p.X * sinV + p.Y * cosV);
	}

	void SVG::RotatePoint(float &x, float &y, const int pvX, const int pvY, const double ROTATION)
	{
		Gdiplus::GpMatrix matrix;
		Gdiplus::GpPointF gp((Gdiplus::REAL)x, (Gdiplus::REAL)y);
		Gdiplus::GpPointF cp((Gdiplus::REAL)pvX, (Gdiplus::REAL)pvY);
		matrix.RotateAt((Gdiplus::REAL)ROTATION, cp);
		matrix.TransformPoints(&gp);
		x = (float)gp.X;
		y = (float)gp.Y;
	}

	void SVG::RotatePoint(float &x, float &y, const float pvX, const float pvY, const double ROTATION)
	{
		Gdiplus::GpMatrix matrix;
		Gdiplus::GpPointF gp((Gdiplus::REAL)x, (Gdiplus::REAL)y);
		Gdiplus::GpPointF cp((Gdiplus::REAL)pvX, (Gdiplus::REAL)pvY);
		matrix.RotateAt((Gdiplus::REAL)ROTATION, cp);
		matrix.TransformPoints(&gp);
		x = (float)gp.X;
		y = (float)gp.Y;

	}

	void SVG::InsertDrawingObject(DrawingPattern *item)
	{
		m_drawingObjList.push_back(item);
	}
}