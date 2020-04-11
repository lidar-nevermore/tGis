#include "wxGLMapWidget.h"

#include "wx/wx.h"

BEGIN_NAME_SPACE(tGis, Gui)

wxGLMapWidget::wxGLMapWidget(wxWindow *parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name,
	const wxPalette& palette)
	:wxGLCanvas(parent,id,nullptr,pos,size,style,name, palette)
	, _thisGeoSurface(this)
{
	_repaint = false;
	_geoSurface = &_thisGeoSurface;
	_br = _backgroundR / 255.0f;
	_bg = _backgroundG / 255.0f;
	_bb = _backgroundB / 255.0f;
}

wxGLMapWidget::~wxGLMapWidget()
{
}

void wxGLMapWidget::LayerAdded(IMapPtr map, ILayerPtr layer)
{
	MapWidget::LayerAdded(map, layer);
	Refresh();
}

void wxGLMapWidget::LayerRemoved(IMapPtr map, ILayerPtr layer)
{
	MapWidget::LayerRemoved(map, layer);
	Refresh();
}

void wxGLMapWidget::LayerCleared(IMapPtr map)
{
	MapWidget::LayerCleared(map);
	Refresh();
}

void wxGLMapWidget::SetBackgroundColor(unsigned char R, unsigned char G, unsigned char B)
{
	_backgroundR = R;
	_backgroundG = G;
	_backgroundB = B;
	_br = _backgroundR / 255.0f;
	_bg = _backgroundG / 255.0f;
	_bb = _backgroundB / 255.0f;
}

void wxGLMapWidget::RepaintMap()
{
	_repaint = true;
	Refresh();
}

void wxGLMapWidget::PresentMap()
{
	Refresh();
}

void wxGLMapWidget::Client2Screen(int cliX, int cliY, int * scrX, int * scrY)
{
}

void wxGLMapWidget::Screen2Client(int scrX, int scrY, int * cliX, int * cliY)
{
}

void wxGLMapWidget::OnPaint(wxPaintEvent & event)
{
	wxPaintDC dc(this);

	//if (_repaint == true)
	//{
	//	_repaint = false;
	//	MapWidget::RepaintMap();
	//}
	//else
	//{
	//	MapWidget::PresentMap();
	//}

	MapWidget::RepaintMap();

	SwapBuffers();
}

void wxGLMapWidget::OnSize(wxSizeEvent & event)
{
	wxSize sz = GetClientSize();
	SetSurfaceSize(sz.x, sz.y);
	RepaintMap();
}

void wxGLMapWidget::OnMouseEvent(wxMouseEvent & event)
{
	MouseEvent(this, &event);
}

void wxGLMapWidget::OnWheelEvent(wxMouseEvent & event)
{
	WheelEvent(this, &event);
}

wxBEGIN_EVENT_TABLE(wxGLMapWidget, wxGLCanvas)
    EVT_PAINT(wxGLMapWidget::OnPaint)
	EVT_SIZE(wxGLMapWidget::OnSize)
	EVT_MOUSE_EVENTS(wxGLMapWidget::OnMouseEvent)
	EVT_MOUSEWHEEL(wxGLMapWidget::OnWheelEvent)
wxEND_EVENT_TABLE()

END_NAME_SPACE(tGis, Gui)
