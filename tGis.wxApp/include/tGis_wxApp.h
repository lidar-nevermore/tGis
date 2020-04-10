#pragma once

#ifndef __TGIS_APP_H__
#define __TGIS_APP_H__

#include <wx/wx.h>

#include "tGis_wxAppCfg.h"
#include "tGis_Core.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, App)

class tGisWxApp : public wxApp
{
public:
	virtual bool OnInit();
};

DECLARE_APP(tGisWxApp)

END_NAME_SPACE(tGis, App)

#endif
