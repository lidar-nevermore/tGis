#pragma once

#ifndef __wxGradientColorWidget_H__
#define __wxGradientColorWidget_H__

#include "tGis_wxGuiCfg.h"

#include "wxGLMapWidget.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API wxGradientColorWidget : public wxGLMapWidget
{
public:
	explicit // avoid implicitly converting a wxWindow* to wxGLCanvas
		wxGradientColorWidget(wxWindow *parent,
			wxWindowID id = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = 0,
			const wxString& name = wxGLCanvasName,
			const wxPalette& palette = wxNullPalette);
	~wxGradientColorWidget();

public:
	void SetGradientColor(GradientColor* color);
	GradientColor* GetGradientColor();

private:
	Map* _map;
	GradientColor* _color;
	GradientColorRender* _render;
};

END_NAME_SPACE(tGis, Gui)

#endif

