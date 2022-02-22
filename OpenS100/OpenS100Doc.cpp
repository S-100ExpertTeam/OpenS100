#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "OpenS100.h"
#endif

#include "OpenS100Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COpenS100Doc

IMPLEMENT_DYNCREATE(COpenS100Doc, CDocument)

BEGIN_MESSAGE_MAP(COpenS100Doc, CDocument)
END_MESSAGE_MAP()



COpenS100Doc::COpenS100Doc() noexcept
{

}

COpenS100Doc::~COpenS100Doc()
{
}

BOOL COpenS100Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;


	return TRUE;
}


// COpenS100Doc serialization
void COpenS100Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

#ifdef SHARED_HANDLERS

void COpenS100Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

void COpenS100Doc::InitializeSearchContent()
{
	CString strSearchContent;

	SetSearchContent(strSearchContent);
}

void COpenS100Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS


#ifdef _DEBUG
void COpenS100Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void COpenS100Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


