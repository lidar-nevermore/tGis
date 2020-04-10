#pragma once

#ifndef __WXGL_MAPWIDGET_H__
#define __WXGL_MAPWIDGET_H__

#include "tGis_wxGuiCfg.h"

#include "wx/glcanvas.h"

#include "wxGLGeoSurface.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API wxGLMapWidget : public wxGLCanvas, public MapWidget
{
	friend class wxGLGeoSurface;
public:
    explicit // avoid implicitly converting a wxWindow* to wxGLCanvas
    wxGLMapWidget(wxWindow *parent,
		wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxGLCanvasName,
        const wxPalette& palette = wxNullPalette);

	virtual ~wxGLMapWidget();

private:
	bool _repaint;

protected:
	wxGLGeoSurface _thisGeoSurface;

protected:
	void LayerAdded(IMapPtr, ILayerPtr) override;
	void LayerRemoved(IMapPtr, ILayerPtr) override;
	void LayerCleared(IMapPtr) override;

public:
	virtual void RepaintMap();
	virtual void PresentMap();

	// Í¨¹ý MapWidget ¼Ì³Ð
	virtual void Client2Screen(int cliX, int cliY, int * scrX, int * scrY) override;
	virtual void Screen2Client(int scrX, int scrY, int * cliX, int * cliY) override;

private:
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);

	wxDECLARE_EVENT_TABLE();
};

END_NAME_SPACE(tGis, Gui)

#endif
