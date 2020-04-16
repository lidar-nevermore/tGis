#include "wxLayerWidget.h"

#define _TOOL_PNG(file_name) \
wxBitmap(wxString(TGisApplication::INSTANCE()->GetExeDir()) \
+ wxString("/wxGui_res/wxLayerWidget/tool/"##file_name##".png"), \
wxBITMAP_TYPE_PNG )

#define _TREE_PNG(file_name) \
wxBitmap(wxString(TGisApplication::INSTANCE()->GetExeDir()) \
+ wxString("/wxGui_res/wxLayerWidget/tree/"##file_name##".png"), \
wxBITMAP_TYPE_PNG )

BEGIN_NAME_SPACE(tGis, Gui)

class layerTreeItemData : public wxTreeItemData
{
public:
	layerTreeItemData()
	{
		_layer = nullptr;
	}

	ILayer* _layer;
};


const int raster_rgb = 0;
const int raster_grayscale = 1;
const int raster_color_ramp = 2;
const int raster_pseudo = 3;
const int raster_unique = 4;
const int vector_simple = 5;
const int vector_label = 6;
const int vector_range = 7;
const int vector_unique = 8;

wxLayerWidget::wxLayerWidget( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	_map = nullptr;

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	_toolLayerVisible = _toolBar->AddTool( wxID_ANY, wxEmptyString, _TOOL_PNG("LayerVisible"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );
	_toolLayerAttrib = _toolBar->AddTool( wxID_ANY, wxEmptyString, _TOOL_PNG("LayerAttribute"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );
	_toolLayerUp = _toolBar->AddTool(wxID_ANY, wxEmptyString, _TOOL_PNG("LayerUp"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolLayerDown = _toolBar->AddTool(wxID_ANY, wxEmptyString, _TOOL_PNG("LayerDown"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolLayerTop = _toolBar->AddTool(wxID_ANY, wxEmptyString, _TOOL_PNG("LayerTop"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolLayerBottom = _toolBar->AddTool(wxID_ANY, wxEmptyString, _TOOL_PNG("LayerBottom"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolRemoveLayer = _toolBar->AddTool(wxID_ANY, wxEmptyString, _TOOL_PNG("RemoveLayer"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolRemoveAllLayers = _toolBar->AddTool(wxID_ANY, wxEmptyString, _TOOL_PNG("RemoveAllLayers"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

	_toolBar->Realize();

	bSizer1->Add( _toolBar, 0, wxEXPAND, 5 );

	_treeCtrl = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE| wxTR_HIDE_ROOT);
	bSizer1->Add( _treeCtrl, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	_imgList = new wxImageList(16, 16, true, 25);
	_imgList->Add(_TREE_PNG("raster_rgb"));
	_imgList->Add(_TREE_PNG("raster_grayscale"));
	_imgList->Add(_TREE_PNG("raster_color_ramp"));
	_imgList->Add(_TREE_PNG("raster_pseudo"));
	_imgList->Add(_TREE_PNG("raster_unique"));
	_imgList->Add(_TREE_PNG("vector_simple"));
	_imgList->Add(_TREE_PNG("vector_label"));
	_imgList->Add(_TREE_PNG("vector_range"));
	_imgList->Add(_TREE_PNG("vector_unique"));

	_treeCtrl->AssignImageList(_imgList);

	_treeCtrl->AddRoot(wxT("Layer"), 0);
}

wxLayerWidget::~wxLayerWidget()
{
}

void wxLayerWidget::SetMap(IMap * map)
{
	if (map == nullptr && _map != nullptr)
	{
		wxTreeItemId root = _treeCtrl->GetRootItem();
		_treeCtrl->DeleteChildren(root);

		_map->LayerAddedEvent.Remove(this, &wxLayerWidget::LayerAdded);
		_map->LayerClearedEvent.Remove(this, &wxLayerWidget::LayerCleared);
		_map->LayerRemovedEvent.Remove(this, &wxLayerWidget::LayerRemoved);
	}
	_map = map;
	if (_map != nullptr)
	{
		size_t layerCount = _map->GetLayerCount();
		for (size_t i = 0; i < layerCount; i++)
			AddLayerNode(_map->GetLayer(i));

		_map->LayerAddedEvent.Add(this, &wxLayerWidget::LayerAdded);
		_map->LayerClearedEvent.Add(this, &wxLayerWidget::LayerCleared);
		_map->LayerRemovedEvent.Add(this, &wxLayerWidget::LayerRemoved);		
	}
}

void wxLayerWidget::LayerAdded(IMapPtr, ILayerPtr layer)
{
	AddLayerNode(layer);
}

void wxLayerWidget::LayerRemoved(IMapPtr, ILayerPtr layer)
{
	RemoveLayerNode(layer);
}

void wxLayerWidget::LayerCleared(IMapPtr)
{
	wxTreeItemId root = _treeCtrl->GetRootItem();
	_treeCtrl->DeleteChildren(root);
}

inline void wxLayerWidget::AddLayerNode(ILayer * layer)
{
	layerTreeItemData* layerData = new layerTreeItemData();
	layerData->_layer = layer;
	wxString label = layer->GetName();
	ILayerRender* render = layer->GetRender();
	wxTreeItemId root = _treeCtrl->GetRootItem();
	if (render->IsTypeOf(RasterRgbLayerRender::S_GetType()))
		_treeCtrl->AppendItem(root, label, raster_rgb, raster_rgb, layerData);
	else if (render->IsTypeOf(RasterColorRampLayerRender::S_GetType()))
		_treeCtrl->AppendItem(root, label, raster_color_ramp, raster_color_ramp, layerData);
	else if (render->IsTypeOf(RasterGrayScaleLayerRender::S_GetType()))
		_treeCtrl->AppendItem(root, label, raster_grayscale, raster_grayscale, layerData);
	else //if (render->IsTypeOf(VectorSimpleLayerRender::S_GetType()))
		_treeCtrl->AppendItem(root, label, vector_simple, vector_simple, layerData);
}

inline void wxLayerWidget::RemoveLayerNode(ILayer * layer)
{
	wxTreeItemId root = _treeCtrl->GetRootItem();
	wxTreeItemIdValue layerCookie;
	wxTreeItemId layerNodeId = _treeCtrl->GetFirstChild(root, layerCookie);
	while (layerNodeId.IsOk())
	{
		layerTreeItemData* layerData = (layerTreeItemData*)_treeCtrl->GetItemData(layerNodeId);
		if (layerData->_layer == layer)
		{
			_treeCtrl->Delete(layerNodeId);
			break;
		}
		layerNodeId = _treeCtrl->GetNextChild(root, layerCookie);
	}
}

void wxLayerWidget::ClearLayerNode()
{
	wxTreeItemId root = _treeCtrl->GetRootItem();
	_treeCtrl->DeleteChildren(root);
}

END_NAME_SPACE(tGis, Gui)
