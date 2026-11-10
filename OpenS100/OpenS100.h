#pragma once

#ifndef __AFXWIN_H__
#endif

#include "resource.h" 
#include "DockablePaneLayerManager.h"
#include "DockablePaneCurrentSelection.h"
#include "DockablePaneEditWindow.h"
#include "DockablePaneRelation.h"

#include "../GISLibrary/GISLibrary.h"

#include <memory>

namespace GISLibrary
{
	class SystemDatabaseSession;
}

class COpenS100View;
class COpenS100App : public CWinAppEx
{
public:
	COpenS100App() noexcept;
	virtual ~COpenS100App();

public:
	ULONG_PTR gdiplusToken = NULL; // GDI

	COpenS100View* pView = nullptr;

	CDockablePaneLayerManager m_pDockablePaneLayerManager; //Layer Manager
	CDockablePaneCurrentSelection m_DockablePaneCurrentSelection; //Current Selection
	CDockablePaneRelation m_DockablePaneRelation; //Relation
	CDockablePaneEditWindow m_DockablePaneEditWindow; //

	BOOL  m_bHiColorIcons;

	CGISLibraryApp* gisLib = new CGISLibraryApp();
	CGISLibraryApp* gisLib2;// = new CGISLibraryApp();

	// The installed S-100 data (catalogues, datasets, support files); null when it could not be
	// opened. Its chart layer draws the installed datasets.
	std::unique_ptr<GISLibrary::SystemDatabaseSession> systemDatabase;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	void LoadSettings();

public:
	// Initial Map Positioning
	void InitStartingLocation();
	void MapRefresh();
	void SaveSettings();
};

extern COpenS100App theApp;