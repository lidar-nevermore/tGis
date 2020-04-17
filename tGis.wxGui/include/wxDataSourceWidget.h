#pragma once

#ifndef __WX_DATASOURCEWIDGET_H__
#define __WX_DATASOURCEWIDGET_H__

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

class TGIS_GUI_API wxDataSourceWidget : public wxPanel
{
public:
	wxDataSourceWidget(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500, 300), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
	~wxDataSourceWidget();

public:
	IDataSource* GetSelDataSource() { return _selDs; }
	IDataset* GetSelDataset() { return _selDt; }

public:
	Event<IDataset*> AfterDatasetActivatedEvent;
	Event<IDataSource*> AfterDataSourceActivatedEvent;

protected:
	wxToolBar* _toolBar;
	wxToolBarToolBase* _toolConnDs;
	wxToolBarToolBase* _toolDsInfo;
	wxToolBarToolBase* _toolRefreshDs;
	wxToolBarToolBase* _toolDisConnDs;
	wxToolBarToolBase* _toolDtInfo;
	wxToolBarToolBase* _toolSaveDtAs;
	wxToolBarToolBase* _toolCloseDt;
	wxTreeCtrl* _treeCtrl;
	wxImageList* _imgList;

private:
	wxTreeItemId _selId;
	IDataSource* _selDs;
	IDataset* _selDt;

private:
	wxTreeItemId _openedDtItemId;
	wxTreeItemId _memDsItemId;
	wxTreeItemId _fileSysItemId;

private:
	static void OnEachDataSource(IDataSource* ds, void* ud);
	static void OnEachDataset(IDataset* dt, void* ud);
	wxTreeItemId AddDataSourceNode(wxTreeItemId &parent, IDataSource* ds, bool autoDelete);
	void AddDataSourceSubNode(wxTreeItemId &parent, IDataSource* ds);
	void AddDatasetNode(wxTreeItemId &parent, IDataset* dt);
	void RemoveOpenedDatasetNode(IDataset* dt);

private:
	void OnNodeActivated(wxTreeEvent& event);
	void OnNodeSelChanged(wxTreeEvent& event);
	void _toolCloseDt_Clicked(wxCommandEvent& event);

};

END_NAME_SPACE(tGis, Gui)

#endif
