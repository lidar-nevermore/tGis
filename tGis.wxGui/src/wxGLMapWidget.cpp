#include "wxGLMapWidget.h"

#include "wx/wx.h"

BEGIN_NAME_SPACE(tGis, Gui)

wxBEGIN_EVENT_TABLE(wxGLMapWidget, wxGLCanvas)
    EVT_PAINT(wxGLMapWidget::OnPaint)
	EVT_SIZE(wxGLMapWidget::OnSize)
wxEND_EVENT_TABLE()

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

void wxGLMapWidget::RepaintMap()
{
	_repaint = true;
}

void wxGLMapWidget::PresentMap()
{
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

END_NAME_SPACE(tGis, Gui)
