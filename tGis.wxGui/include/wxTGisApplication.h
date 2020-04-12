#pragma once

#ifndef __WX_TGISAPPLICATION_H__
#define __WX_TGISAPPLICATION_H__

#include <wx/wx.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API wxTGisApplication : public TGisApplication
{
public:
	wxTGisApplication(wxFrame* mainFrame);
	virtual ~wxTGisApplication();

public:
	wxWindow* GetMainFrame() { return _mainFrame; }

private:
	wxWindow* _mainFrame;
};

END_NAME_SPACE(tGis, Gui)

#endif
