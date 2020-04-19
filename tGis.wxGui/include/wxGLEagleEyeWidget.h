#pragma once

#ifndef __WXGL_EAGLEEYEWIDGET_H__
#define __WXGL_EAGLEEYEWIDGET_H__

#include "tGis_wxGuiCfg.h"

#include "wxGLMapWidget.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API wxGLEagleEyeWidget : public wxGLMapWidget
{
public:
	explicit // avoid implicitly converting a wxWindow* to wxGLCanvas
	wxGLEagleEyeWidget(wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxGLCanvasName,
		const wxPalette& palette = wxNullPalette);
	~wxGLEagleEyeWidget();

public:
	virtual void SetMap(IMap*);
	virtual void SetMapWidget(IMapWidget* mapWidget);

protected:
	void LayerVisibleChanged(IMapPtr, ILayerPtr);
	void LayerAdded(IMapPtr, ILayerPtr, size_t) override;
	virtual void LayerMoved(IMapPtr, ILayerPtr, size_t, ILayerPtr, size_t);

private:
	IMapWidget* _relMapWidget;
	OverlayRect _rect;

private:
	void OnViewPortChanged(GeoViewPort* geoViewPort);
};

END_NAME_SPACE(tGis, Gui)

#endif

