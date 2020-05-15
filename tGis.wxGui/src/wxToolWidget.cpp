#include <wx/wx.h>
#include "wxToolWidget.h"
#include "StandaloneToolDialog.h"
#include "wxTGisApplication.h"

#define _TOOL_PNG(file_name) \
wxBitmap(wxString(tGisApplication::INSTANCE()->GetExeDir()) \
+ wxString("/wxGui_res/wxToolWidget/tool/"##file_name##".png"), \
wxBITMAP_TYPE_PNG )

#define _TREE_PNG(file_name) \
wxBitmap(wxString(tGisApplication::INSTANCE()->GetExeDir()) \
+ wxString("/wxGui_res/wxToolWidget/tree/"##file_name##".png"), \
wxBITMAP_TYPE_PNG )

BEGIN_NAME_SPACE(tGis, Gui)

class kitTreeItemData : public wxTreeItemData
{
public:
	kitTreeItemData()
	{
		_kit = nullptr;
		_tool = nullptr;
		_expanded = false;
	}

	~kitTreeItemData()
	{
	}

	bool _expanded;
	ToolKit* _kit;
	ITool* _tool;
};


const int tool_img = 0;
const int toolkit_img = 1;

wxToolWidget::wxToolWidget( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	_toolAddTool = _toolBar->AddTool(wxID_ANY, wxEmptyString, _TOOL_PNG("AddTool"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolRemoveTool = _toolBar->AddTool(wxID_ANY, wxEmptyString, _TOOL_PNG("RemoveTool"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);


	_srchCtrl = new wxSearchCtrl(_toolBar, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
#ifndef __WXMAC__
	_srchCtrl->ShowSearchButton(true);
#endif
	_srchCtrl->ShowCancelButton(true);
	_toolSrch  = _toolBar->AddControl(_srchCtrl);
	//_toolEndFind = _toolBar->AddTool(wxID_ANY, wxEmptyString, _TOOL_PNG("EndFind"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

	_toolBar->Realize();

	bSizer1->Add( _toolBar, 0, wxEXPAND, 5 );

	_treeCtrl = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT);
	bSizer1->Add( _treeCtrl, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	_selKit = nullptr;
	_selTool = nullptr;

	_imgList = new wxImageList(16, 16, true, 2);
	_imgList->Add(_TREE_PNG("tool"));
	_imgList->Add(_TREE_PNG("toolkit"));
	_treeCtrl->AssignImageList(_imgList);

	wxTreeItemId root = _treeCtrl->AddRoot(wxT("toolkits"), 0);

	size_t kitCount = ToolKitRepository::INSTANCE()->GetToolKitCount();
	for (size_t i = 0; i < kitCount; i++)
	{
		ToolKit* kit = ToolKitRepository::INSTANCE()->GetToolKit(i);
		wxString label = kit->GetName();
		kitTreeItemData* kitData = new kitTreeItemData();
		kitData->_kit = kit;
		_treeCtrl->AppendItem(root, label, toolkit_img, toolkit_img, kitData);
	}

	Bind(wxEVT_TREE_ITEM_ACTIVATED, &wxToolWidget::OnNodeActivated, this);
	Bind(wxEVT_TREE_SEL_CHANGED, &wxToolWidget::OnNodeSelChanged, this);

	Bind(wxEVT_TOOL, &wxToolWidget::_toolAddTool_Clicked, this, _toolAddTool->GetId());
	Bind(wxEVT_TOOL, &wxToolWidget::_toolRemoveTool_Clicked, this, _toolRemoveTool->GetId());

}

wxToolWidget::~wxToolWidget()
{
	Unbind(wxEVT_TOOL, &wxToolWidget::_toolAddTool_Clicked, this, _toolAddTool->GetId());
	Unbind(wxEVT_TOOL, &wxToolWidget::_toolRemoveTool_Clicked, this, _toolRemoveTool->GetId());

	Unbind(wxEVT_TREE_ITEM_ACTIVATED, &wxToolWidget::OnNodeActivated, this);
	Unbind(wxEVT_TREE_SEL_CHANGED, &wxToolWidget::OnNodeSelChanged, this);

}

void wxToolWidget::AddToolKitSubNode(wxTreeItemId & parent, ToolKit * toolKit)
{
	size_t kitCount = toolKit->GetToolKitCount();
	for (size_t i = 0; i < kitCount; i++)
	{
		ToolKit* kit = toolKit->GetToolKit(i);
		wxString label = kit->GetName();
		kitTreeItemData* kitData = new kitTreeItemData();
		kitData->_kit = kit;
		_treeCtrl->AppendItem(parent, label, toolkit_img, toolkit_img, kitData);
	}

	size_t toolCount = toolKit->GetToolCount();
	for (size_t i = 0; i < toolCount; i++)
	{
		ITool* tool = toolKit->GetTool(i);
		wxString label = tool->GetName();
		kitTreeItemData* kitData = new kitTreeItemData();
		kitData->_tool = tool;
		_treeCtrl->AppendItem(parent, label, tool_img, tool_img, kitData);
	}
}

void wxToolWidget::OnNodeActivated(wxTreeEvent & event)
{
	wxTreeItemId selId = event.GetItem();
	kitTreeItemData* selData = (kitTreeItemData*)_treeCtrl->GetItemData(selId);
	if (selData->_tool != nullptr)
	{
		selData->_tool->Execute();
	}
	else
	{
		if (false == selData->_expanded)
		{
			AddToolKitSubNode(selId, selData->_kit);
			_treeCtrl->Expand(selId);
			selData->_expanded = true;
		}
		else
		{
			if (_treeCtrl->IsExpanded(selId))
			{
				_treeCtrl->Collapse(selId);
			}
			else
			{
				_treeCtrl->Expand(selId);
			}
		}
	}
}

void wxToolWidget::OnNodeSelChanged(wxTreeEvent & event)
{
	_toolBar->EnableTool(_toolRemoveTool->GetId(), false);
	_selId = event.GetItem();
	_selKit = nullptr;
	_selTool = nullptr;

	if (_selId.IsOk() == false)
		return;

	kitTreeItemData* selData = (kitTreeItemData*)_treeCtrl->GetItemData(_selId);

	if (selData == nullptr)
		return;

	_selKit = selData->_kit;
	_selTool = selData->_tool;

	if(_selTool != nullptr && _selTool->IsTypeOf(StandaloneTool::S_GetType()))
		_toolBar->EnableTool(_toolRemoveTool->GetId(), true);
}

void wxToolWidget::_toolAddTool_Clicked(wxCommandEvent & event)
{
	ToolKit* drParentKit = _selKit;
	if (_selKit == nullptr && _selTool != nullptr)
		drParentKit = _selTool->GetParent();

	StandaloneToolDialog dlg(nullptr, drParentKit);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString strBelong = dlg.GetBelong();
		wxArrayString arrStrBelong = wxSplit(strBelong, wxT('/'));

		ToolKit* toFillKit = nullptr;
		ToolKitSet* kitSet = ToolKitRepository::INSTANCE();
		wxTreeItemId kitSetId = _treeCtrl->GetRootItem();
		bool needNewKit = false;

		for (auto it = arrStrBelong.begin(); it != arrStrBelong.end(); it++)
		{
			wxString& strKit = *it;

			if (strKit.IsEmpty())
				continue;

			ToolKit* kit = needNewKit ? nullptr : kitSet->GetToolKit(strKit.c_str());
			if (kit == nullptr)
			{
				needNewKit = true;
				kit = new ToolKit(strKit.c_str());
				kitSet->AddToolKit(kit);
				if (kitSetId.IsOk())
				{
					kitTreeItemData* parentData = (kitTreeItemData*)_treeCtrl->GetItemData(kitSetId);
					//parentData == nullptr 说明是root
					if (parentData == nullptr || parentData->_expanded)
					{
						kitTreeItemData* kitData = new kitTreeItemData();
						kitData->_kit = kit;
						kitSetId = _treeCtrl->AppendItem(kitSetId, strKit, toolkit_img, toolkit_img, kitData);
					}
					else
						kitSetId = wxTreeItemId();
				}
			}
			else
			{
				if (kitSetId.IsOk())
				{
					bool found = false;
					wxTreeItemIdValue dsCookie;
					wxTreeItemId dsNodeId = _treeCtrl->GetFirstChild(kitSetId, dsCookie);
					while (dsNodeId.IsOk())
					{
						kitTreeItemData* kitData = (kitTreeItemData*)_treeCtrl->GetItemData(dsNodeId);
						if (kitData->_kit == kit)
						{
							found = true;
							kitSetId = dsNodeId;
							break;
						}
						dsNodeId = _treeCtrl->GetNextChild(kitSetId, dsCookie);
					}

					if(false == found)
						kitSetId = wxTreeItemId();
				}
			}

			toFillKit = kit;
			kitSet = kit;
		}

		StandaloneTool* tool = new StandaloneTool(dlg.GetStandaloneTool());
		toFillKit->AddTool(tool);

		if (kitSetId.IsOk())
		{
			kitTreeItemData* parentData = (kitTreeItemData*)_treeCtrl->GetItemData(kitSetId);
			if (parentData->_expanded)
			{
				kitTreeItemData* kitData = new kitTreeItemData();
				kitData->_tool = tool;
				kitSetId = _treeCtrl->AppendItem(kitSetId, wxString(tool->GetName()), tool_img, tool_img, kitData);
			}
		}

		ToolKitRepository::INSTANCE()->SaveStandaloneTool();
	}
}

void wxToolWidget::_toolRemoveTool_Clicked(wxCommandEvent & event)
{
	ToolKit* kit = _selTool->GetParent();
	kit->RemoveTool(_selTool);
	_selTool = nullptr;
	_treeCtrl->Delete(_selId);

	//wxTreeCtrl有bug删除item之后选择新节点但是不触发事件
	//wxTreeCtrl还有另外的bug，删除item之后，GetChildrenCount返回的值不变
	wxTreeEvent treeEvent;
	treeEvent.SetItem(_treeCtrl->GetSelection());
	OnNodeSelChanged(treeEvent);

	ToolKitRepository::INSTANCE()->SaveStandaloneTool();
}

END_NAME_SPACE(tGis, Gui)

