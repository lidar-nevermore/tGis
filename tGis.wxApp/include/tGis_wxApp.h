#pragma once

#ifndef __TGIS_APP_H__
#define __TGIS_APP_H__

#include <wx/wx.h>

#include "tGis_wxAppCfg.h"
#include "tGis_Core.h"
#include "tGis_wxGui.h"

using namespace tGis::Core;
using namespace tGis::Gui;

BEGIN_NAME_SPACE(tGis, App)

class tGisWxApp : public wxApp, public wxTGisApplication
{
public:
	virtual bool OnInit();

	// Í¨¹ý wxTGisApplication ¼Ì³Ð
	virtual wxFrame * GetMainFrame() override;

private:
	wxFrame* _mainFrame;
};

DECLARE_APP(tGisWxApp)

END_NAME_SPACE(tGis, App)

#endif
