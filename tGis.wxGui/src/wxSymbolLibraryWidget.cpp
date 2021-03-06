
#include "wxSymbolLibraryWidget.h"
#include "wxGLMapWidget.h"
#include "tGis_Core.h"

BEGIN_NAME_SPACE(tGis, Gui)

class wxSymbolDispalyWidget : public wxGLMapWidget
{
public:
    explicit 
    wxSymbolDispalyWidget(wxWindow *parent,
		wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxGLCanvasName,
        const wxPalette& palette = wxNullPalette)
		:wxGLMapWidget(parent, id, pos, size, style, name, palette, false)
	{
		_render = nullptr;
	};

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

	SymbolLibraryRender* _render;
	wxScrollBar* _scrollBar;
};

class wxSymbolLibraryWidgetImpl : public MapTool
{
public:
	wxSymbolLibraryWidgetImpl(wxSymbolLibraryWidget* owner)
	{
		_owner = owner;
		_enabled = true;
		_msgFlag = 0;
	}

	~wxSymbolLibraryWidgetImpl()
	{
		if (_mapWidget != nullptr)
			_mapWidget->RemoveMapTool(this);
		SetMapWidget(nullptr);
	}

	wxSymbolLibraryWidget* _owner;
	SymbolLibraryRender* _render;
	int _selSymId;

protected:
	virtual void SetMapWidget(IMapWidget* mapWidget)
	{
		if (mapWidget == nullptr && _mapWidget != nullptr)
		{
			wxGLMapWidget* widget = (wxGLMapWidget*)_mapWidget;
			widget->MouseEvent.Remove<wxSymbolLibraryWidgetImpl>(this, &wxSymbolLibraryWidgetImpl::MouseDown);
		}
		_mapWidget = mapWidget;
		if (mapWidget != nullptr)
		{
			wxGLMapWidget* widget = (wxGLMapWidget*)_mapWidget;
			widget->MouseEvent.Add<wxSymbolLibraryWidgetImpl>(this, &wxSymbolLibraryWidgetImpl::MouseDown);
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
			_selSymId = _render->SelectSymbol(cliX, cliY);
			_mapWidget->RepaintMap();
		}
	}
};

wxSymbolLibraryWidget::wxSymbolLibraryWidget( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	_impl_ = new wxSymbolLibraryWidgetImpl(this);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );

	_symWidget = new wxSymbolDispalyWidget( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxGLCanvasName, wxNullPalette);
	bSizer2->Add( _symWidget, 1, wxEXPAND | wxALL, 5 );

	_scrollBar = new wxScrollBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
	bSizer2->Add( _scrollBar, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();

	((wxSymbolDispalyWidget*)_symWidget)->_scrollBar = _scrollBar;
	_map = new Map();
	Layer* layer = new Layer(nullptr);
	_render = new SymbolLibraryRender(layer);
	_impl_->_selSymId = 0;
	_render->SelectSymbol(0);
	((wxSymbolDispalyWidget*)_symWidget)->_render = _render;
	_map->AddLayer(layer);
	_symWidget->SetMap(_map);

	_impl_->_render = _render;
	_symWidget->AddMapTool(_impl_);
}

wxSymbolLibraryWidget::~wxSymbolLibraryWidget()
{
	_symWidget->SetMap(nullptr);
	delete _map;
	delete _impl_;
}

void wxSymbolLibraryWidget::SetSymbolLibrary(ISymbolLibrary * symLib)
{
	_render->SetSymbolLibrary(symLib);
	_symWidget->Refresh();
}

int wxSymbolLibraryWidget::GetSelSymbolId()
{
	return _impl_->_selSymId;
}

void wxSymbolLibraryWidget::SetSelSymbolId(int symId)
{
	_impl_->_selSymId = _render->SelectSymbol(symId);
	_symWidget->Refresh();
}

void wxSymbolLibraryWidget::_scrollBar_scroll(wxScrollEvent & event)
{
	int pos = event.GetPosition();
	GeoViewPort* vport = _symWidget->GetViewPort();
	double cenX, cenY;
	vport->GetSpatialCenter(&cenX, &cenY);
	wxSize sz = _symWidget->GetClientSize();
	vport->SetSpatialCenter(cenX, -pos - sz.y / 2.0);
	_symWidget->Refresh();
}

wxBEGIN_EVENT_TABLE(wxSymbolLibraryWidget, wxPanel)
    EVT_SCROLL(wxSymbolLibraryWidget::_scrollBar_scroll)
wxEND_EVENT_TABLE()

END_NAME_SPACE(tGis, Gui)
