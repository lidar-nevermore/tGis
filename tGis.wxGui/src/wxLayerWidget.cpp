#include <wx/wx.h>
#include "wxLayerWidget.h"
#include "wxLayerPropertyDialog.h"
#include "wxTGisApplication.h"

#define _TOOL_PNG(file_name) \
wxBitmap(wxString(tGisApplication::INSTANCE()->GetExeDir()) \
+ wxString("/wxGui_res/wxLayerWidget/tool/"##file_name##".png"), \
wxBITMAP_TYPE_PNG )

#define _TREE_PNG(file_name) \
wxBitmap(wxString(tGisApplication::INSTANCE()->GetExeDir()) \
+ wxString("/wxGui_res/wxLayerWidget/tree/"##file_name##".png"), \
wxBITMAP_TYPE_PNG )

BEGIN_NAME_SPACE(tGis, Gui)

class wxLayerWidgetImpl
{
public:
	wxLayerWidgetImpl(wxLayerWidget* owner)
	{
		_owner = owner;
	}

	wxLayerWidget* _owner;
	vector<wxTreeItemId> _vecLayerNode;
};

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
	_impl_ = new wxLayerWidgetImpl(this);
	_map = nullptr;
	_selLayer = nullptr;

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	_toolLayerVisible = _toolBar->AddTool( wxID_ANY, wxEmptyString, _TOOL_PNG("LayerVisible"), wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString, NULL );
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

	_toolBar->EnableTool(_toolLayerVisible->GetId(), false);
	_toolBar->EnableTool(_toolLayerAttrib->GetId(), false);
	_toolBar->EnableTool(_toolLayerUp->GetId(), false);
	_toolBar->EnableTool(_toolLayerDown->GetId(), false);
	_toolBar->EnableTool(_toolLayerTop->GetId(), false);
	_toolBar->EnableTool(_toolLayerBottom->GetId(), false);
	_toolBar->EnableTool(_toolRemoveLayer->GetId(), false);

	DataSourceRepository::INSTANCE()->BeforeDatasetCloseEvent.Add(this, &wxLayerWidget::OnDatasetClose);

	Bind(wxEVT_TREE_SEL_CHANGED, &wxLayerWidget::OnNodeSelChanged, this);

	Bind(wxEVT_TOOL, &wxLayerWidget::_toolLayerVisible_Clicked, this, _toolLayerVisible->GetId());
	Bind(wxEVT_TOOL, &wxLayerWidget::_toolRemoveLayer_Clicked, this, _toolRemoveLayer->GetId());
	Bind(wxEVT_TOOL, &wxLayerWidget::_toolRemoveAllLayers_Clicked, this, _toolRemoveAllLayers->GetId());
	Bind(wxEVT_TOOL, &wxLayerWidget::_toolLayerAttrib_Clicked, this, _toolLayerAttrib->GetId());
	Bind(wxEVT_TOOL, &wxLayerWidget::_toolLayerUp_Clicked, this, _toolLayerUp->GetId());
	Bind(wxEVT_TOOL, &wxLayerWidget::_toolLayerDown_Clicked, this, _toolLayerDown->GetId());
	Bind(wxEVT_TOOL, &wxLayerWidget::_toolLayerTop_Clicked, this, _toolLayerTop->GetId());
	Bind(wxEVT_TOOL, &wxLayerWidget::_toolLayerBottom_Clicked, this, _toolLayerBottom->GetId());
	//Bind(wxEVT_SET_FOCUS, &wxLayerWidget::OnTreeCtrl, this, _treeCtrl->GetId());

}

wxLayerWidget::~wxLayerWidget()
{
	Unbind(wxEVT_TREE_SEL_CHANGED, &wxLayerWidget::OnNodeSelChanged, this);

	Unbind(wxEVT_TOOL, &wxLayerWidget::_toolLayerVisible_Clicked, this, _toolLayerVisible->GetId());
	Unbind(wxEVT_TOOL, &wxLayerWidget::_toolRemoveLayer_Clicked, this, _toolRemoveLayer->GetId());
	Unbind(wxEVT_TOOL, &wxLayerWidget::_toolRemoveAllLayers_Clicked, this, _toolRemoveAllLayers->GetId());
	Unbind(wxEVT_TOOL, &wxLayerWidget::_toolLayerAttrib_Clicked, this, _toolLayerAttrib->GetId());
	Unbind(wxEVT_TOOL, &wxLayerWidget::_toolLayerUp_Clicked, this, _toolLayerUp->GetId());
	Unbind(wxEVT_TOOL, &wxLayerWidget::_toolLayerDown_Clicked, this, _toolLayerDown->GetId());
	Unbind(wxEVT_TOOL, &wxLayerWidget::_toolLayerTop_Clicked, this, _toolLayerTop->GetId());
	Unbind(wxEVT_TOOL, &wxLayerWidget::_toolLayerBottom_Clicked, this, _toolLayerBottom->GetId());

	DataSourceRepository::INSTANCE()->BeforeDatasetCloseEvent.Remove(this, &wxLayerWidget::OnDatasetClose);

	wxLayerWidget::SetMap(nullptr, nullptr);

	delete _impl_;
}

void wxLayerWidget::SetMap(IMap * map, IMapWidget* mapWidget)
{
	if (_map == map)
		return;

	if (_map != nullptr)
	{
		wxTreeItemId root = _treeCtrl->GetRootItem();
		_treeCtrl->DeleteChildren(root);

		_map->LayerAddedEvent.Remove(this, &wxLayerWidget::LayerAdded);
		_map->LayerClearedEvent.Remove(this, &wxLayerWidget::LayerCleared);
		_map->LayerRemovedEvent.Remove(this, &wxLayerWidget::LayerRemoved);
	}
	_map = map;
	_mapWidget = mapWidget;
	if (_map != nullptr)
	{
		size_t layerCount = _map->GetLayerCount();
		for (size_t i = 0; i < layerCount; i++)
			AddLayerNode(_map->GetLayer(i), i);

		_map->LayerAddedEvent.Add(this, &wxLayerWidget::LayerAdded);
		_map->LayerClearedEvent.Add(this, &wxLayerWidget::LayerCleared);
		_map->LayerRemovedEvent.Add(this, &wxLayerWidget::LayerRemoved);		
	}
}

void wxLayerWidget::LayerAdded(IMapPtr map, ILayerPtr layer, size_t pos)
{
	wxTreeItemId item = AddLayerNode(layer, pos);
	if (map->GetLayerCount() == 1)
		_treeCtrl->SelectItem(item);
	UpdateLayerTool();
}

void wxLayerWidget::LayerRemoved(IMapPtr, ILayerPtr layer, size_t pos)
{
	RemoveLayerNode(layer, pos);
	UpdateLayerTool();
}

void wxLayerWidget::LayerCleared(IMapPtr)
{
	wxTreeItemId root = _treeCtrl->GetRootItem();
	_impl_->_vecLayerNode.clear();
	_treeCtrl->DeleteChildren(root);	
}

inline wxTreeItemId wxLayerWidget::AddLayerNode(ILayer * layer, size_t pos)
{
	layerTreeItemData* layerData = new layerTreeItemData();
	layerData->_layer = layer;
	wxString label = layer->GetName();
	if (layer->GetVisible())
		label = wxT("[v]") + label;
	else
		label = wxT("[x]") + label;
	ILayerRender* render = layer->GetRender();
	wxTreeItemId root = _treeCtrl->GetRootItem();
	wxTreeItemId inserted;
	if (render->IsTypeOf(RasterRgbLayerRender::S_GetType()))
		inserted = _treeCtrl->InsertItem(root, pos, label, raster_rgb, raster_rgb, layerData);
	else if (render->IsTypeOf(RasterDualRampLayerRender::S_GetType()))
		inserted = _treeCtrl->InsertItem(root, pos, label, raster_color_ramp, raster_color_ramp, layerData);
	else if (render->IsTypeOf(RasterGrayScaleLayerRender::S_GetType()))
		inserted = _treeCtrl->InsertItem(root, pos, label, raster_grayscale, raster_grayscale, layerData);
	else //if (render->IsTypeOf(VectorSimpleLayerRender::S_GetType()))
		inserted = _treeCtrl->InsertItem(root, pos, label, vector_simple, vector_simple, layerData);

	_impl_->_vecLayerNode.insert(_impl_->_vecLayerNode.begin() + pos, inserted);

	return inserted;
}

inline void wxLayerWidget::RemoveLayerNode(ILayer * layer, size_t pos)
{
	wxTreeItemId layerNodeId = _impl_->_vecLayerNode[pos];
	_impl_->_vecLayerNode.erase(_impl_->_vecLayerNode.begin() + pos);
	_treeCtrl->Delete(layerNodeId);	

	//wxTreeCtrl有bug删除item之后选择新节点但是不触发事件
	//wxTreeCtrl还有另外的bug，删除item之后，GetChildrenCount返回的值不变
	wxTreeEvent treeEvent;
	treeEvent.SetItem(_treeCtrl->GetSelection());
	OnNodeSelChanged(treeEvent);
}

//void wxLayerWidget::RemoveLayerNode(IDataset * dt)
//{
//	for (auto it = _impl_->_vecLayerNode.begin(); it != _impl_->_vecLayerNode.end(); it++)
//	{
//		wxTreeItemId layerNodeId = *it;
//		layerTreeItemData* layerData = (layerTreeItemData*)_treeCtrl->GetItemData(layerNodeId);
//		if (layerData->_layer->GetDataset() == dt)
//		{
//			_impl_->_vecLayerNode.erase(it);
//			_treeCtrl->Delete(layerNodeId);
//			break;
//		}
//	}
//
//	//wxTreeItemId root = _treeCtrl->GetRootItem();
//	//wxTreeItemIdValue layerCookie;
//	//wxTreeItemId layerNodeId = _treeCtrl->GetFirstChild(root, layerCookie);
//	//while (layerNodeId.IsOk())
//	//{
//	//	layerTreeItemData* layerData = (layerTreeItemData*)_treeCtrl->GetItemData(layerNodeId);
//	//	if (layerData->_layer->GetDataset() == dt)
//	//	{
//	//		_treeCtrl->Delete(layerNodeId);
//	//		break;
//	//	}
//	//	layerNodeId = _treeCtrl->GetNextChild(root, layerCookie);
//	//}
//}

void wxLayerWidget::OnDatasetClose(IDataset * dt)
{
	if(_map != nullptr)
		_map->RemoveLayer(dt);
}

size_t wxLayerWidget::UpdateLayerTool()
{
	size_t selLayerPos = _map->GetLayerCount();
	if (_selLayer != nullptr)
	{
		size_t maxLayerPos = selLayerPos - 1;
		size_t selLayerPos = _map->GetLayerPos(_selLayer);

		_toolBar->EnableTool(_toolLayerUp->GetId(), selLayerPos > 0);
		_toolBar->EnableTool(_toolLayerDown->GetId(), selLayerPos < maxLayerPos);
		_toolBar->EnableTool(_toolLayerTop->GetId(), selLayerPos > 0);
		_toolBar->EnableTool(_toolLayerBottom->GetId(), selLayerPos < maxLayerPos);
	}

	return selLayerPos;
}

void wxLayerWidget::OnNodeSelChanged(wxTreeEvent & event)
{
	_selId = event.GetItem();
	_selLayer = nullptr;
	size_t selLayerPos = 0;
	//清空图层时selId居然是OK的
	if (_selId.IsOk() && _map->GetLayerCount() > 0)
	{		
		layerTreeItemData* selData = (layerTreeItemData*)_treeCtrl->GetItemData(_selId);
		_selLayer = selData->_layer;
		selLayerPos = UpdateLayerTool();
		
		_toolBar->EnableTool(_toolLayerVisible->GetId(), true);
		_toolBar->EnableTool(_toolLayerAttrib->GetId(), true);
		_toolBar->EnableTool(_toolRemoveLayer->GetId(), true);

		_toolBar->ToggleTool(_toolLayerVisible->GetId(), _selLayer->GetVisible());
	}
	else
	{
		_toolBar->EnableTool(_toolLayerVisible->GetId(), false);
		_toolBar->EnableTool(_toolLayerAttrib->GetId(), false);
		_toolBar->EnableTool(_toolLayerUp->GetId(), false);
		_toolBar->EnableTool(_toolLayerDown->GetId(), false);
		_toolBar->EnableTool(_toolLayerTop->GetId(), false);
		_toolBar->EnableTool(_toolLayerBottom->GetId(), false);
		_toolBar->EnableTool(_toolRemoveLayer->GetId(), false);
	}

	LayerSelChangedEvent(_map, _selLayer, selLayerPos);
}

void wxLayerWidget::OnTreeCtrl(wxFocusEvent& event)
{
	event.Skip();
}

void wxLayerWidget::_toolLayerVisible_Clicked(wxCommandEvent & event)
{
	if (_selLayer != nullptr)
	{
		_selLayer->SetVisible(!_selLayer->GetVisible());
		wxString label = _selLayer->GetName();
		if (_selLayer->GetVisible())
			label = wxT("[v]") + label;
		else
			label = wxT("[x]") + label;
		_treeCtrl->SetItemText(_selId, label);
		_mapWidget->RepaintMap();
	}
}

void wxLayerWidget::_toolRemoveLayer_Clicked(wxCommandEvent & event)
{
	if (_selLayer != nullptr && _map != nullptr)
	{
		_map->RemoveLayer(_selLayer);
	}
}

void wxLayerWidget::_toolRemoveAllLayers_Clicked(wxCommandEvent & event)
{
	if (_map != nullptr)
	{
		_map->ClearLayers();
	}
}

void wxLayerWidget::_toolLayerAttrib_Clicked(wxCommandEvent & event)
{
	wxLayerPropertyDialog lpDlg(_selLayer);
	if (lpDlg.ShowModal() == wxID_OK)
	{
		_map->LayerRenderChangedEvent(_map, _selLayer);
		_mapWidget->RepaintMap();
	}
}

void wxLayerWidget::_toolLayerUp_Clicked(wxCommandEvent & event)
{
	size_t selLayerPos = _map->GetLayerPos(_selLayer);
	_map->MoveLayer(selLayerPos, selLayerPos - 1);
	_mapWidget->RepaintMap();
	wxTreeItemId itemId = AddLayerNode(_selLayer, selLayerPos - 1);
	RemoveLayerNode(_selLayer, selLayerPos+1);
	_treeCtrl->SelectItem(itemId);
}

void wxLayerWidget::_toolLayerDown_Clicked(wxCommandEvent & event)
{
	size_t selLayerPos = _map->GetLayerPos(_selLayer);
	_map->MoveLayer(selLayerPos, selLayerPos + 1);
	_mapWidget->RepaintMap();
	wxTreeItemId itemId = AddLayerNode(_selLayer, selLayerPos + 2);
	RemoveLayerNode(_selLayer, selLayerPos);
	_treeCtrl->SelectItem(itemId);
}

void wxLayerWidget::_toolLayerTop_Clicked(wxCommandEvent & event)
{
	size_t selLayerPos = _map->GetLayerPos(_selLayer);
	_map->MoveLayer(selLayerPos, 0);
	_mapWidget->RepaintMap();
	wxTreeItemId itemId = AddLayerNode(_selLayer, 0);
	RemoveLayerNode(_selLayer, selLayerPos + 1);
	_treeCtrl->SelectItem(itemId);
}

void wxLayerWidget::_toolLayerBottom_Clicked(wxCommandEvent & event)
{
	size_t selLayerPos = _map->GetLayerPos(_selLayer);
	size_t layerCount = _map->GetLayerCount();
	_map->MoveLayer(selLayerPos, layerCount - 1);
	_mapWidget->RepaintMap();
	wxTreeItemId itemId = AddLayerNode(_selLayer, layerCount);
	RemoveLayerNode(_selLayer, selLayerPos);
	_treeCtrl->SelectItem(itemId);
}

END_NAME_SPACE(tGis, Gui)
