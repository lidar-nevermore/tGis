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
	delete _map;
}

void wxGradientColorWidget::SetGradientColor(GradientColor * color)
{
	_color = color;
	_render->SetGradientColor(_color);
	RepaintMap();
}

GradientColor * wxGradientColorWidget::GetGradientColor()
{
	return _color;
}


END_NAME_SPACE(tGis, Gui)

