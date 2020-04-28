#pragma once

#ifndef __WX_TGISAPPLICATION_H__
#define __WX_TGISAPPLICATION_H__

#include <wx/wx.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class wxGLMapWidget;

class TGIS_GUI_API wxTGisApplication : public tGisApplication
{
protected:
	wxTGisApplication();
	virtual ~wxTGisApplication();

public:
	virtual wxFrame* GetMainFrame() = 0;
	virtual wxGLMapWidget* GetCurMapWidget() = 0;
};

END_NAME_SPACE(tGis, Gui)

#endif
