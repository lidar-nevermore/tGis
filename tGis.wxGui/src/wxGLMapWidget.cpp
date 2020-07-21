#include "wxGLMapWidget.h"

#include "wx/wx.h"

BEGIN_NAME_SPACE(tGis, Gui)

static const int wxGLAttribList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };

wxGLMapWidget::wxGLMapWidget(wxWindow *parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name,
	const wxPalette& palette,
	bool extraBuffer)
	:wxGLCanvas(parent, id, wxGLAttribList, pos, size, style, name, palette)
	, _thisGeoSurface(this, extraBuffer)
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

void wxGLMapWidget::LayerAdded(IMapPtr map, ILayerPtr layer, size_t pos)
{
	MapWidget::LayerAdded(map, layer, pos);
	const OGREnvelope* envelope = layer->GetEnvelope();
	_viewPort.IncludeEnvelope(envelope);
	RepaintMap();
	Refresh();
}

void wxGLMapWidget::LayerRemoved(IMapPtr map, ILayerPtr layer, size_t pos)
{
	MapWidget::LayerRemoved(map, layer, pos);
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
	if(!_thisGeoSurface._extraBuffer)
		_repaint = true;
	Refresh();
}

void wxGLMapWidget::Client2Screen(int cliX, int cliY, int * scrX, int * scrY)
{
	*scrX = cliX;
	*scrY = cliY;
	ClientToScreen(scrX, scrY);
}

void wxGLMapWidget::Screen2Client(int scrX, int scrY, int * cliX, int * cliY)
{
	*cliX = scrX;
	*cliY = scrY;
	ScreenToClient(cliX, cliY);
}

void wxGLMapWidget::OnPaint(wxPaintEvent & event)
{
	wxPaintDC dc(this);

	if (_repaint == true)
	{
		_repaint = false;
		MapWidget::RepaintMap();
	}
	else
	{
		if (!_thisGeoSurface._extraBuffer)
			MapWidget::RepaintMap();
		else
			MapWidget::PresentMap();
	}

	SwapBuffers();
}

void wxGLMapWidget::OnSize(wxSizeEvent & event)
{
	wxSize sz = GetClientSize();
	SetSurfaceSize(sz.x, sz.y);
	RepaintMap();
	event.Skip();
}

void wxGLMapWidget::OnMouseEvent(wxMouseEvent & event)
{
	MouseEvent(this, &event);
	event.Skip();
}

void wxGLMapWidget::OnWheelEvent(wxMouseEvent & event)
{
	WheelEvent(this, &event);
	event.Skip();
}

void wxGLMapWidget::OnKeyDownEvent(wxKeyEvent & event)
{
	KeyDownEvent(this, &event);
	event.Skip();
}

void wxGLMapWidget::OnKeyUpEvent(wxKeyEvent & event)
{
	KeyUpEvent(this, &event);
	event.Skip();
}

wxBEGIN_EVENT_TABLE(wxGLMapWidget, wxGLCanvas)
    EVT_PAINT(wxGLMapWidget::OnPaint)
	EVT_SIZE(wxGLMapWidget::OnSize)
	EVT_MOUSE_EVENTS(wxGLMapWidget::OnMouseEvent)
	EVT_MOUSEWHEEL(wxGLMapWidget::OnWheelEvent)
	EVT_KEY_DOWN(wxGLMapWidget::OnKeyDownEvent)
	EVT_KEY_UP(wxGLMapWidget::OnKeyUpEvent)
wxEND_EVENT_TABLE()

END_NAME_SPACE(tGis, Gui)
