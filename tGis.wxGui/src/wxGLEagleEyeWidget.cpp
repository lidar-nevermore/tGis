#include "wxGLEagleEyeWidget.h"

BEGIN_NAME_SPACE(tGis, Gui)

wxGLEagleEyeWidget::wxGLEagleEyeWidget(wxWindow *parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name,
	const wxPalette& palette)
	: wxGLMapWidget(parent, id, pos, size, style, name, palette)
{
	_relMapWidget = nullptr;
	_overlayLayer.AddOverlayObject(&_rect);
}


wxGLEagleEyeWidget::~wxGLEagleEyeWidget()
{
	_overlayLayer.RemoveOverlayObject(&_rect);
	SetMapWidget(nullptr);
}

void wxGLEagleEyeWidget::SetMap(IMap *)
{
	throw std::exception("Use SetMapWidget instead!");
}

void wxGLEagleEyeWidget::SetMapWidget(IMapWidget * mapWidget)
{
	if (_relMapWidget == mapWidget)
		return;

	if (_relMapWidget != nullptr)
	{
		GeoViewPort* viewPort = _relMapWidget->GetViewPort();
		viewPort->ChangedEvent.Remove(this, &wxGLEagleEyeWidget::OnViewPortChanged);
		IMapPtr map = GetMap();
		MapWidget::SetMap(nullptr);
		if (map != nullptr)
		{
			map->LayerMovedEvent.Remove(this, &wxGLEagleEyeWidget::LayerMoved);
			map->LayerVisibleChangedEvent.Remove(this, &wxGLEagleEyeWidget::LayerVisibleChanged);
			map->LayerRenderChangedEvent.Remove(this, &wxGLEagleEyeWidget::LayerVisibleChanged);
		}
	}

	_relMapWidget = mapWidget;

	if (_relMapWidget != nullptr)
	{
		GeoViewPort* viewPort = _relMapWidget->GetViewPort();
		viewPort->ChangedEvent.Add(this, &wxGLEagleEyeWidget::OnViewPortChanged);
		IMapPtr map = _relMapWidget->GetMap();
		MapWidget::SetMap(map);
		if (map != nullptr)
		{
			map->LayerMovedEvent.Add(this, &wxGLEagleEyeWidget::LayerMoved);
			map->LayerVisibleChangedEvent.Add(this, &wxGLEagleEyeWidget::LayerVisibleChanged);
			map->LayerRenderChangedEvent.Add(this, &wxGLEagleEyeWidget::LayerVisibleChanged);
		}
	}
}

void wxGLEagleEyeWidget::LayerVisibleChanged(IMapPtr, ILayerPtr)
{
	RepaintMap();
	Refresh();
}

void wxGLEagleEyeWidget::LayerAdded(IMapPtr map, ILayerPtr layer, size_t pos)
{
	MapWidget::LayerAdded(map, layer, pos);

	const OGREnvelope* env = map->GetEnvelope();
	_viewPort.IncludeEnvelope(env);
	_viewPort.Spatial2Surface(env->MinX, env->MinY, &(_rect._left), &(_rect._top));
	_viewPort.Spatial2Surface(env->MaxX, env->MaxY, &(_rect._right), &(_rect._bottom));

	RepaintMap();
	Refresh();
}

void wxGLEagleEyeWidget::LayerMoved(IMapPtr, ILayerPtr, size_t, ILayerPtr, size_t)
{
	RepaintMap();
	Refresh();
}

void wxGLEagleEyeWidget::OnViewPortChanged(GeoViewPort * geoViewPort)
{
	int surfW, surfH;
	_geoSurface->GetSize(&surfW, &surfH);

	if (surfW == 0 || surfH == 0)
		return;

	OGREnvelope envView;
	geoViewPort->GetEnvelope(&envView);
	int left, top, right, bottom;

	_viewPort.Spatial2Surface(envView.MinX, envView.MaxY, &left, &top);
	_viewPort.Spatial2Surface(envView.MaxX, envView.MinY, &right, &bottom);

	int rcWidth = right - left;
	int rcHeight = bottom - top;
	const int _minRectSize = 7;

	//if (rcWidth >= surfW || rcHeight >= surfH)
	//{
	//	const OGREnvelope* envMap = _relMapWidget->GetMap()->GetEnvelope();
	//	_viewPort.IncludeEnvelope(envMap);
	//	_viewPort.Spatial2Surface(envView.MinX, envView.MinY, &(_rect._left), &(_rect._top));
	//	_viewPort.Spatial2Surface(envView.MaxX, envView.MaxY, &(_rect._right), &(_rect._bottom));
	//	RepaintMap();
	//}

	if (rcWidth < _minRectSize || rcHeight < _minRectSize)
	{
		int ext = 1;
		if (rcWidth < rcHeight)
			ext = (_minRectSize - rcWidth + 1) / 2;
		else
			ext = (_minRectSize - rcHeight + 1) / 2;

		_rect._left = left - ext;
		_rect._right = right + ext;
		_rect._top = top - ext;
		_rect._bottom = bottom + ext;
		PresentMap();
	}
	else
	{
		_rect._left = left;
		_rect._right = right;
		_rect._top = top;
		_rect._bottom = bottom;
		PresentMap();
	}	
}

END_NAME_SPACE(tGis, Gui)

