#include "wxGradientColorWidget.h"
#include "tGis_Core.h"

BEGIN_NAME_SPACE(tGis, Gui)

wxGradientColorWidget::wxGradientColorWidget(wxWindow *parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name,
	const wxPalette& palette)
	:wxGLMapWidget(parent, id, pos, size, style, name, palette, false)
{
	_color = GradientColorRepository::INSTANCE()->GetGradientColor(0);
	_map = new Map();
	Layer* layer = new Layer(nullptr);
	_render = new GradientColorRender(layer);
	_render->SetGradientColor(_color);
	_map->AddLayer(layer);
	SetMap(_map);
}

wxGradientColorWidget::~wxGradientColorWidget()
{
	SetMap(nullptr);
	if (_color != nullptr)
		_color->Release();
	delete _map;
}

void wxGradientColorWidget::SetGradientColor(GradientColor * color)
{
	if (_color != nullptr)
		_color->Release();
	_color = color;
	if (_color != nullptr)
		_color->Reference();
	_render->SetGradientColor(_color);
	RepaintMap();
}

GradientColor * wxGradientColorWidget::GetGradientColor()
{
	return _color;
}

void wxGradientColorWidget::OnSize(wxSizeEvent & event)
{
	wxSize sz = GetClientSize();
	_render->SetHeight(sz.y);
	wxGLMapWidget::OnSize(event);
}


END_NAME_SPACE(tGis, Gui)

