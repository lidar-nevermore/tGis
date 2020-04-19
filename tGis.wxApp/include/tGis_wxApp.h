#pragma once

#ifndef __TGIS_APP_H__
#define __TGIS_APP_H__

#include <wx/wx.h>

#include "tGis_wxAppCfg.h"
#include "tGis_Core.h"
#include "tGis_wxGui.h"

using namespace tGis::Core;
using namespace tGis::Gui;

class MainFrame;

class tGisWxApp : public wxApp, public wxTGisApplication
{
public:
	virtual bool OnInit();

	virtual wxFrame * GetMainFrame() override;
	virtual wxGLMapWidget * GetCurMapWidget() override;

private:
	MainFrame* _mainFrame;
	wxGLMapWidget* _mapWidget;
};

DECLARE_APP(tGisWxApp)

#endif
