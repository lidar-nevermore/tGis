#pragma once

#ifndef __WX_DATASOURCEWIDGET_H__
#define __WX_DATASOURCEWIDGET_H__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/toolbar.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>


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
	Event<IDataSource*, IDataset*> DataSelChangedEvent;

public:
	//添加外部创建的数据集dt，
	//如果dt未打开内部则会调用打开函数，如果打开函数调用识别则返回false
	bool AddOuterDataset(IDataset* dt);

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
	static void __stdcall OnEachDataSource(IDataSource* ds, void* ud);
	static void __stdcall OnEachDataset(IDataset* dt, void* ud);
	wxTreeItemId AddDataSourceNode(wxTreeItemId &parent, IDataSource* ds, bool autoDelete);
	void AddDataSourceSubNode(wxTreeItemId &parent, IDataSource* ds);
	wxTreeItemId AddDatasetNode(wxTreeItemId &parent, IDataset* dt, wxTreeItemId* itemId = nullptr);
	void UpdateDatasetNode(wxTreeItemId itemId, IDataset* dt);
	void RemoveOpenedDatasetNode(IDataset* dt);

private:
	void OnNodeActivated(wxTreeEvent& event);
	void OnNodeSelChanged(wxTreeEvent& event);
	void _toolCloseDt_Clicked(wxCommandEvent& event);
	void _toolDtInfo_Clicked(wxCommandEvent& event);
	void _toolRefreshDs_Clicked(wxCommandEvent& event);
};

END_NAME_SPACE(tGis, Gui)

#endif
