#include "wxDataSourceWidget.h"
#include <wx/dir.h>

#define _TOOL_PNG(file_name) \
wxBitmap(wxString(tGisApplication::INSTANCE()->GetExeDir()) \
+ wxString("/wxGui_res/wxDataSourceWidget/tool/"##file_name##".png"), \
wxBITMAP_TYPE_PNG )

#define _TREE_PNG(file_name) \
wxBitmap(wxString(tGisApplication::INSTANCE()->GetExeDir()) \
+ wxString("/wxGui_res/wxDataSourceWidget/tree/"##file_name##".png"), \
wxBITMAP_TYPE_PNG )

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class dsTreeItemData : public wxTreeItemData
{
public:
	dsTreeItemData(bool autoDelete = false)
	{
		_autoDelete = autoDelete;
		_ds = nullptr;
		_dt = nullptr;
	}

	~dsTreeItemData()
	{
		if (_autoDelete)
		{
			if (_ds != nullptr)
				delete _ds;
			if (_dt != nullptr)
				delete _dt;
		}
	}

	IDataSource* _ds;
	IDataset* _dt;
	wxTreeItemId _openedItemId;
	wxTreeItemId _itemId;
private:
	bool _autoDelete;
};

const int dataset_open_img = 0;
const int mem_img = 1;
const int disk_img = 2;
const int folder_img = 3;
const int folder_open_img = 4;
const int raster_img = 5;
const int raster_open_img = 6;
const int vector_img = 7;
const int vector_open_img = 8;

wxDataSourceWidget::wxDataSourceWidget( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) 
	: wxPanel( parent, id, pos, size, style, name )
{
	_selDs = nullptr;
	_selDt = nullptr;

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	_toolConnDs = _toolBar->AddTool(wxID_ANY, wxT("Connect FileSystem DataSource"), _TOOL_PNG("ConnectFileSystemDs"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolDsInfo = _toolBar->AddTool(wxID_ANY, wxT("DataSource Info"), _TOOL_PNG("DataSourceInfo"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolRefreshDs = _toolBar->AddTool(wxID_ANY, wxT("Refresh DataSource"), _TOOL_PNG("RefreshDataSource"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolDisConnDs = _toolBar->AddTool(wxID_ANY, wxT("Disconnect DataSource"), _TOOL_PNG("DisconnectDataSource"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolDtInfo = _toolBar->AddTool(wxID_ANY, wxT("Dataset Info"), _TOOL_PNG("DatasetInfo"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolSaveDtAs = _toolBar->AddTool(wxID_ANY, wxT("Save Dataset As"), _TOOL_PNG("SaveDatasetAs"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolCloseDt = _toolBar->AddTool(wxID_ANY, wxT("Close  Dataset"), _TOOL_PNG("CloseDataset"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

	_toolBar->Realize();

	bSizer1->Add( _toolBar, 0, wxEXPAND, 5 );

	_treeCtrl = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT );
	bSizer1->Add( _treeCtrl, 1, wxALL|wxEXPAND, 5 );

	this->SetSizer( bSizer1 );
	this->Layout();
	
	_imgList = new wxImageList(16, 16, true, 25);
	_imgList->Add(_TREE_PNG("dataset_open"));
	_imgList->Add(_TREE_PNG("mem"));
	_imgList->Add(_TREE_PNG("disk"));
	_imgList->Add(_TREE_PNG("folder"));
	_imgList->Add(_TREE_PNG("folder_open"));
	_imgList->Add(_TREE_PNG("raster"));
	_imgList->Add(_TREE_PNG("raster_open"));
	_imgList->Add(_TREE_PNG("vector"));
	_imgList->Add(_TREE_PNG("vector_open"));

	_treeCtrl->AssignImageList(_imgList);

	wxTreeItemId root = _treeCtrl->AddRoot(wxT("DataSource"), 0);
	_openedDtItemId = _treeCtrl->AppendItem(root, wxT("Opened Dataset"), dataset_open_img);
	_memDsItemId = AddDataSourceNode(root, MemoryDataSource::INSTANCE(), false);
	_fileSysItemId = _treeCtrl->AppendItem(root, wxT("FileSystem"), disk_img);


#ifdef _WINDOWS_
	char disk[4] = { 'C',':','\\',0 };
	for (char c = 65; c < 91; c++)
	{
		disk[0] = c;
		if (wxDir::Exists(disk))
		{
			FileSystemDataSource* fds = new FileSystemDataSource(disk);
			AddDataSourceNode(_fileSysItemId, fds, true);
		}
	}

#endif // _WINDOWS_
	_treeCtrl->Expand(_fileSysItemId);

	_toolBar->EnableTool(_toolDsInfo->GetId(), false);
	_toolBar->EnableTool(_toolRefreshDs->GetId(), false);
	_toolBar->EnableTool(_toolDisConnDs->GetId(), false);
	_toolBar->EnableTool(_toolDtInfo->GetId(), false);
	_toolBar->EnableTool(_toolSaveDtAs->GetId(), false);
	_toolBar->EnableTool(_toolCloseDt->GetId(), false);

	DataSourceRepository::INSTANCE()->BeforeDatasetCloseEvent.Add(this, &wxDataSourceWidget::RemoveOpenedDatasetNode);
	
	Bind(wxEVT_TREE_ITEM_ACTIVATED, &wxDataSourceWidget::OnNodeActivated, this);
	Bind(wxEVT_TREE_SEL_CHANGED, &wxDataSourceWidget::OnNodeSelChanged, this);
	
	Bind(wxEVT_COMMAND_TOOL_CLICKED, &wxDataSourceWidget::_toolCloseDt_Clicked, this, _toolCloseDt->GetId());

}

wxDataSourceWidget::~wxDataSourceWidget()
{
	Unbind(wxEVT_TREE_ITEM_ACTIVATED, &wxDataSourceWidget::OnNodeActivated, this);
	Unbind(wxEVT_TREE_SEL_CHANGED, &wxDataSourceWidget::OnNodeSelChanged, this);

	Unbind(wxEVT_COMMAND_TOOL_CLICKED, &wxDataSourceWidget::_toolCloseDt_Clicked, this, _toolCloseDt->GetId());

	//delete _imgList;

	DataSourceRepository::INSTANCE()->BeforeDatasetCloseEvent.Remove(this, &wxDataSourceWidget::RemoveOpenedDatasetNode);
}

wxTreeItemId wxDataSourceWidget::AddDataSourceNode(wxTreeItemId &parent, IDataSource * ds, bool autoDelete)
{
	dsTreeItemData* mdsData = new dsTreeItemData(autoDelete);
	mdsData->_ds = ds;
	wxString label = ds->GetName();
	wxTreeItemId itemId;
	//TODO: 更多数据源类型支持
	if(ds->IsTypeOf(MemoryDataSource::S_GetType()))
		itemId = _treeCtrl->AppendItem(parent, label, mem_img, mem_img, mdsData);
	else //if (ds->IsTypeOf(FileSystemDataSource::S_GetType()))
		itemId = _treeCtrl->AppendItem(parent, label, folder_img, folder_img, mdsData);
    
	mdsData->_itemId = itemId;

	return itemId;
}

void wxDataSourceWidget::OnEachDataSource(IDataSource* ds, void* ud)
{
	wxDataSourceWidget* pDsWidget = (wxDataSourceWidget*)((void**)ud)[0];
	wxTreeItemId* parentItemId = (wxTreeItemId*)((void**)ud)[1];
	pDsWidget->AddDataSourceNode(*parentItemId, ds, false);
}

void wxDataSourceWidget::OnEachDataset(IDataset* dt, void* ud)
{
	wxDataSourceWidget* pDsWidget = (wxDataSourceWidget*)((void**)ud)[0];
	wxTreeItemId* parentItemId = (wxTreeItemId*)((void**)ud)[1];
	pDsWidget->AddDatasetNode(*parentItemId, dt);
}

void wxDataSourceWidget::AddDataSourceSubNode(wxTreeItemId &parent, IDataSource * ds)
{
	void* ud[2] = { this, &parent };
	ds->ForEachDataSource(wxDataSourceWidget::OnEachDataSource, (void*)ud);
	ds->ForEachDataset(wxDataSourceWidget::OnEachDataset, (void*)ud);
}

wxTreeItemId wxDataSourceWidget::AddDatasetNode(wxTreeItemId & parent, IDataset * dt, wxTreeItemId* itemId_)
{
	dsTreeItemData* mdsData = new dsTreeItemData();
	mdsData->_dt = dt;
	wxString label = dt->GetName();
	wxTreeItemId itemId;
	//TODO: 更多数据集类型支持
	if (dt->IsTypeOf(MyGDALRasterDataset::S_GetType()))
	{
		if(dt->IsOpened())
			itemId = _treeCtrl->AppendItem(parent, label, raster_open_img, raster_open_img, mdsData);
		else
			itemId = _treeCtrl->AppendItem(parent, label, raster_img, raster_img, mdsData);
	}
	else //if (dt->IsTypeOf(MyGDALVectorDataset::S_GetType()))
	{
		if (dt->IsOpened())
			itemId = _treeCtrl->AppendItem(parent, label, vector_open_img, vector_open_img, mdsData);
		else
			itemId = _treeCtrl->AppendItem(parent, label, vector_img, vector_img, mdsData);
	}

	if (itemId_ != nullptr)
	{
		mdsData->_openedItemId = itemId;
		mdsData->_itemId = *itemId_;
	}
	else
		mdsData->_itemId = itemId;

	return itemId;
}

void wxDataSourceWidget::UpdateDatasetNode(wxTreeItemId itemId, IDataset * dt)
{
	//TODO: 更多数据集类型支持
	if (dt->IsOpened())
	{
		if (dt->IsTypeOf(MyGDALRasterDataset::S_GetType()))
		{
			_treeCtrl->SetItemImage(itemId, raster_open_img);
			_treeCtrl->SetItemImage(itemId, raster_open_img, wxTreeItemIcon_Selected);
		}
		else
		{
			_treeCtrl->SetItemImage(itemId, vector_open_img);
			_treeCtrl->SetItemImage(itemId, vector_open_img, wxTreeItemIcon_Selected);
		}
	}
	else
	{
		if (dt->IsTypeOf(MyGDALRasterDataset::S_GetType()))
		{
			_treeCtrl->SetItemImage(itemId, raster_img);
			_treeCtrl->SetItemImage(itemId, raster_img, wxTreeItemIcon_Selected);
		}
		else
		{
			_treeCtrl->SetItemImage(itemId, vector_img);
			_treeCtrl->SetItemImage(itemId, vector_img, wxTreeItemIcon_Selected);
		}
	}
}

void wxDataSourceWidget::RemoveOpenedDatasetNode(IDataset * dt)
{
	wxTreeItemIdValue dsCookie;
	wxTreeItemId dsNodeId = _treeCtrl->GetFirstChild(_openedDtItemId, dsCookie);
	while (dsNodeId.IsOk())
	{
		dsTreeItemData* dsData = (dsTreeItemData*)_treeCtrl->GetItemData(dsNodeId);
		if (dsData->_dt == dt)
		{
			UpdateDatasetNode(dsData->_itemId, dsData->_dt);			
			//if (dsData->_itemId == _selId)
			{
				_toolBar->EnableTool(_toolDtInfo->GetId(), false);
				_toolBar->EnableTool(_toolSaveDtAs->GetId(), false);
				_toolBar->EnableTool(_toolCloseDt->GetId(), false);
			}
			_treeCtrl->Delete(dsNodeId);
			break;
		}
		dsNodeId = _treeCtrl->GetNextChild(_openedDtItemId, dsCookie);
	}
}

void wxDataSourceWidget::OnNodeActivated(wxTreeEvent & event)
{
	wxTreeItemId selId = event.GetItem();
	dsTreeItemData* selData = (dsTreeItemData*)_treeCtrl->GetItemData(selId);
	if (selData != nullptr)
	{
		if (selData->_ds != nullptr)
		{
			bool isConnected = selData->_ds->IsConnected();
			selData->_ds->Connect();
			if (isConnected == false && selData->_ds->IsConnected())
			{
				AddDataSourceSubNode(selId, selData->_ds);

				//TODO: 当前只有文件系统类型数据源
				_treeCtrl->SetItemImage(selId, folder_open_img);
				_treeCtrl->SetItemImage(selId, folder_open_img, wxTreeItemIcon_Selected);
				
				_treeCtrl->Expand(selId);
			}

			AfterDataSourceActivatedEvent(selData->_ds);
		}
		else if (selData->_dt != nullptr)
		{
			bool isOpen = selData->_dt->IsOpened();
			selData->_dt->Open();
			if (isOpen == false && selData->_dt->IsOpened())
			{
				AddDatasetNode(_openedDtItemId, selData->_dt, &selId);
				UpdateDatasetNode(selId, selData->_dt);
			}

			AfterDatasetActivatedEvent(selData->_dt);
		}
	}

	if (_treeCtrl->IsSelected(selId))
		OnNodeSelChanged(event);
}

void wxDataSourceWidget::OnNodeSelChanged(wxTreeEvent & event)
{
	_selDs = nullptr;
	_selDt = nullptr;

	_toolBar->EnableTool(_toolDsInfo->GetId(), false);
	_toolBar->EnableTool(_toolRefreshDs->GetId(), false);
	_toolBar->EnableTool(_toolDisConnDs->GetId(), false);
	_toolBar->EnableTool(_toolDtInfo->GetId(), false);
	_toolBar->EnableTool(_toolSaveDtAs->GetId(), false);
	_toolBar->EnableTool(_toolCloseDt->GetId(), false);

	_selId = event.GetItem();

	if (_selId.IsOk() == false)
		return;
	
	dsTreeItemData* selData = (dsTreeItemData*)_treeCtrl->GetItemData(_selId);
	if (selData != nullptr)
	{
		_selDs = selData->_ds;
		_selDt = selData->_dt;
		if (_selDs != nullptr)
		{
			if (_selDs->IsConnected())
			{
				_toolBar->EnableTool(_toolDsInfo->GetId(), true);
				_toolBar->EnableTool(_toolRefreshDs->GetId(), true);
				_toolBar->EnableTool(_toolDisConnDs->GetId(), true);
			}
		}
		else
		{
			if (_selDt->IsOpened())
			{
				_toolBar->EnableTool(_toolDtInfo->GetId(), true);
				_toolBar->EnableTool(_toolSaveDtAs->GetId(), true);
				_toolBar->EnableTool(_toolCloseDt->GetId(), true);
			}
		}
	}
}

void wxDataSourceWidget::_toolCloseDt_Clicked(wxCommandEvent & event)
{
	_selDt->Close();
}

END_NAME_SPACE(tGis, Gui)
