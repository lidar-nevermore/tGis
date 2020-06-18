#include "wxGradientColorRepositoryWidget.h"
#include "wxGLMapWidget.h"
#include "tGis_Core.h"

BEGIN_NAME_SPACE(tGis, Gui)

class wxGradientColorRepoDispalyWidget : public wxGLMapWidget
{
public:
	explicit
		wxGradientColorRepoDispalyWidget(wxWindow *parent,
			wxWindowID id = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = 0,
			const wxString& name = wxGLCanvasName,
			const wxPalette& palette = wxNullPalette)
		:wxGLMapWidget(parent, id, pos, size, style, name, palette, false)
	{
		_scrollBar = nullptr;
		_map = new Map();
		Layer* layer = new Layer(nullptr);
		_render = new GradientColorRepositoryRender(layer);
		_render->SelectGradientColor(GradientColorRepository::INSTANCE()->GetGradientColor(0));
		_map->AddLayer(layer);
		SetMap(_map);
	};

	~wxGradientColorRepoDispalyWidget()
	{
		SetMap(nullptr);
		delete _map;
	}

	void OnSize(wxSizeEvent& event)
	{
		wxSize sz = GetClientSize();
		if (_render != nullptr)
		{
			_render->UpdateEnvelope(sz.x, sz.y);
			const OGREnvelope* env = _render->GetEnvelope();
			if (_scrollBar != nullptr)
				_scrollBar->SetScrollbar(0, sz.y, -env->MinY, 1);
		}
		SetSurfaceSize(sz.x, sz.y);
		GetViewPort()->IncludeEnvelope(0, 0, sz.x, -sz.y);

		event.Skip();
	}

	Map* _map;
	GradientColorRepositoryRender* _render;
	wxScrollBar* _scrollBar;
};


class wxGradientColorRepositoryWidgetImpl : public MapTool
{
public:
	wxGradientColorRepositoryWidgetImpl(wxGradientColorRepositoryWidget* owner)
	{
		_owner = owner;
		_selColor = nullptr;
	}

	~wxGradientColorRepositoryWidgetImpl()
	{
		if (_mapWidget != nullptr)
			_mapWidget->RemoveMapTool(this);
		SetMapWidget(nullptr);
	}

	wxGradientColorRepositoryWidget* _owner;
	GradientColorRepositoryRender* _render;
	GradientColor* _selColor;

protected:
	virtual void SetMapWidget(IMapWidget* mapWidget)
	{
		if (mapWidget == nullptr && _mapWidget != nullptr)
		{
			wxGLMapWidget* widget = (wxGLMapWidget*)_mapWidget;
			widget->MouseEvent.Remove<wxGradientColorRepositoryWidgetImpl>(this, &wxGradientColorRepositoryWidgetImpl::MouseDown);
		}
		_mapWidget = mapWidget;
		if (mapWidget != nullptr)
		{
			wxGLMapWidget* widget = (wxGLMapWidget*)_mapWidget;
			widget->MouseEvent.Add<wxGradientColorRepositoryWidgetImpl>(this, &wxGradientColorRepositoryWidgetImpl::MouseDown);
		}
	}

	virtual void MouseDown(wxGLMapWidget* mapWidget, wxMouseEvent* e)
	{
		if (_enabled == false)
			return;

		if (e->LeftDown())
		{
			int cliX = e->GetX();
			int cliY = e->GetY();
			_selColor = _render->SelectGradientColor(cliX, cliY);
			_mapWidget->RepaintMap();
		}
	}
};

wxGradientColorRepositoryWidget::wxGradientColorRepositoryWidget( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	_impl_ = new wxGradientColorRepositoryWidgetImpl(this);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	_gcrWidget = new wxGradientColorRepoDispalyWidget(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxGLCanvasName, wxNullPalette);
	bSizer1->Add( _gcrWidget, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	_scrollBar = new wxScrollBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
	bSizer1->Add( _scrollBar, 0, wxALL|wxEXPAND, 5 );

	((wxGradientColorRepoDispalyWidget*)_gcrWidget)->_scrollBar = _scrollBar;
	_impl_->_render = ((wxGradientColorRepoDispalyWidget*)_gcrWidget)->_render;
	_gcrWidget->AddMapTool(_impl_);

	this->SetSizer( bSizer1 );
	this->Layout();
}

wxGradientColorRepositoryWidget::~wxGradientColorRepositoryWidget()
{
	delete _impl_;
}

GradientColor * wxGradientColorRepositoryWidget::GetSelGradientColor()
{
	return _impl_->_selColor;
}

void wxGradientColorRepositoryWidget::SetSelGradientColor(GradientColor * color)
{
	_impl_->_selColor = color;
	_impl_->_render->SelectGradientColor(color);
	_gcrWidget->Refresh();
}

void wxGradientColorRepositoryWidget::_scrollBar_scroll(wxScrollEvent & event)
{
	int pos = event.GetPosition();
	GeoViewPort* vport = _gcrWidget->GetViewPort();
	double cenX, cenY;
	vport->GetSpatialCenter(&cenX, &cenY);
	wxSize sz = _gcrWidget->GetClientSize();
	vport->SetSpatialCenter(cenX, -pos - sz.y / 2.0);
	_gcrWidget->Refresh();
}

wxBEGIN_EVENT_TABLE(wxGradientColorRepositoryWidget, wxPanel)
    EVT_SCROLL(wxGradientColorRepositoryWidget::_scrollBar_scroll)
wxEND_EVENT_TABLE()

END_NAME_SPACE(tGis, Gui)
