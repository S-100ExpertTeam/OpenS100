#pragma once
class COpenS100Doc : public CDocument
{
protected:
	COpenS100Doc() noexcept;
	DECLARE_DYNCREATE(COpenS100Doc)

public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	virtual ~COpenS100Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	//A helper function that sets search content for a search processor.
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
