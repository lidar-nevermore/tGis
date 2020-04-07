#pragma once

#ifndef __WXGL_MAPWIDGET_H__
#define __WXGL_MAPWIDGET_H__

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, wxGui)

class TGIS_GUI_API wxGLMapWidget : public MapWidget
{
public:
	wxGLMapWidget();
	virtual ~wxGLMapWidget();

	// Í¨¹ý MapWidget ¼Ì³Ð
	virtual void Client2Screen(int cliX, int cliY, int * scrX, int * scrY) override;
	virtual void Screen2Client(int scrX, int scrY, int * cliX, int * cliY) override;
};

END_NAME_SPACE(tGis, wxGui)

#endif
