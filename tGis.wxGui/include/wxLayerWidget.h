#pragma once

#ifndef __WX_LAYERWIDGET_H__
#define __WX_LAYERWIDGET_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/toolbar.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class wxLayerWidgetImpl;

class TGIS_GUI_API wxLayerWidget : public wxPanel
{
public:
	wxLayerWidget(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500, 300), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
	~wxLayerWidget();

public:
	void SetMap(IMap* map, IMapWidget* mapWidget);
	ILayer* GetSelLayer() { return _selLayer; }

public:
	Event<IMap*, ILayer*, size_t> LayerSelChangedEvent;

protected:
	wxToolBar* _toolBar;
	wxToolBarToolBase* _toolLayerVisible;
	wxToolBarToolBase* _toolLayerAttrib;
	wxToolBarToolBase* _toolLayerUp;
	wxToolBarToolBase* _toolLayerDown;
	wxToolBarToolBase* _toolLayerTop;
	wxToolBarToolBase* _toolLayerBottom;
	wxToolBarToolBase* _toolRemoveLayer;
	wxToolBarToolBase* _toolRemoveAllLayers;

	wxTreeCtrl* _treeCtrl;
	wxImageList* _imgList;

private:
	IMap* _map;
	IMapWidget* _mapWidget;
	wxTreeItemId _selId;
	ILayer* _selLayer;
	wxLayerWidgetImpl* _impl_;

private:
	virtual void LayerAdded(IMapPtr, ILayerPtr, size_t);
	virtual void LayerRemoved(IMapPtr, ILayerPtr, size_t);
	virtual void LayerCleared(IMapPtr);

private:
	wxTreeItemId AddLayerNode(ILayer* layer, size_t pos);
	void RemoveLayerNode(ILayer* layer, size_t pos);
	//void RemoveLayerNode(IDataset* dt);
	void OnDatasetClose(IDataset* dt);
	size_t UpdateLayerTool();

private:
	void OnNodeSelChanged(wxTreeEvent& event);
	void _toolLayerVisible_Clicked(wxCommandEvent& event);
	void _toolRemoveLayer_Clicked(wxCommandEvent& event);
	void _toolRemoveAllLayers_Clicked(wxCommandEvent& event);
	void _toolLayerAttrib_Clicked(wxCommandEvent& event);
	void _toolLayerUp_Clicked(wxCommandEvent& event);
	void _toolLayerDown_Clicked(wxCommandEvent& event);
	void _toolLayerTop_Clicked(wxCommandEvent& event);
	void _toolLayerBottom_Clicked(wxCommandEvent& event);	
};

END_NAME_SPACE(tGis, Gui)

#endif
