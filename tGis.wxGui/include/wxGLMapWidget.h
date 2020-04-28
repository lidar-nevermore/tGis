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
	//���ǿ���ȷ��������Ҫ���ƵĶ�����������ⲿ��ȡ�����򲻿�ʹextraBuffer = false
    explicit // avoid implicitly converting a wxWindow* to wxGLCanvas
    wxGLMapWidget(wxWindow *parent,
		wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxGLCanvasName,
        const wxPalette& palette = wxNullPalette,
		bool extraBuffer = true);

	virtual ~wxGLMapWidget();

public:
	virtual void SetBackgroundColor(unsigned char R, unsigned char G, unsigned char B);
	virtual void RepaintMap();
	virtual void PresentMap();

	// ͨ�� MapWidget �̳�
	virtual void Client2Screen(int cliX, int cliY, int * scrX, int * scrY) override;
	virtual void Screen2Client(int scrX, int scrY, int * cliX, int * cliY) override;

public:
	Event<wxGLMapWidget*, wxMouseEvent*> MouseEvent;
	Event<wxGLMapWidget*, wxMouseEvent*> WheelEvent;

private:
	bool _repaint;
	GLfloat _br;
	GLfloat _bg;
	GLfloat _bb;

protected:
	wxGLGeoSurface _thisGeoSurface;

protected:
	void LayerAdded(IMapPtr, ILayerPtr, size_t) override;
	void LayerRemoved(IMapPtr, ILayerPtr, size_t) override;
	void LayerCleared(IMapPtr) override;

private:
	virtual void OnPaint(wxPaintEvent& event);
	virtual void OnSize(wxSizeEvent& event);
	virtual void OnMouseEvent(wxMouseEvent& event);
	virtual void OnWheelEvent(wxMouseEvent& event);

	wxDECLARE_EVENT_TABLE();
};

END_NAME_SPACE(tGis, Gui)

#endif
