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

class TGIS_GUI_API wxLayerWidget : public wxPanel
{
public:
	wxLayerWidget(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500, 300), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
	~wxLayerWidget();

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

public:
	void SetMap(IMap* map);

private:
	IMap* _map;

private:
	virtual void LayerAdded(IMapPtr, ILayerPtr);
	virtual void LayerRemoved(IMapPtr, ILayerPtr);
	virtual void LayerCleared(IMapPtr);

private:
	void AddLayerNode(ILayer* layer);
	void RemoveLayerNode(ILayer* layer);
	void ClearLayerNode();

};

END_NAME_SPACE(tGis, Gui)

#endif
