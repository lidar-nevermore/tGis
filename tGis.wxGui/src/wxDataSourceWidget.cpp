#include "wxDataSourceWidget.h"
#include <wx/dir.h>

namespace xpm
{
#include "dataset_open.xpm"
#include "mem.xpm"
#include "disk.xpm"
#include "folder.xpm"
#include "folder_open.xpm"
#include "raster.xpm"
#include "raster_open.xpm"
#include "vector.xpm"
#include "vector_open.xpm"
}

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
	_toolBar->Realize();

	bSizer1->Add( _toolBar, 0, wxEXPAND, 5 );

	_treeCtrl = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT | wxNO_BORDER);
	bSizer1->Add( _treeCtrl, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	
	wxImageList* imglist = new wxImageList(16, 16, true, 25);
	imglist->Add(wxBitmap(xpm::dataset_open));
	imglist->Add(wxBitmap(xpm::mem));
	imglist->Add(wxBitmap(xpm::disk));
	imglist->Add(wxBitmap(xpm::folder));
	imglist->Add(wxBitmap(xpm::folder_open));
	imglist->Add(wxBitmap(xpm::raster));
	imglist->Add(wxBitmap(xpm::raster_open));
	imglist->Add(wxBitmap(xpm::vector));
	imglist->Add(wxBitmap(xpm::vector_open));

	_treeCtrl->AssignImageList(imglist);

	wxTreeItemId root = _treeCtrl->AddRoot(wxT("DataSource"), 0);
	_treeCtrl->AppendItem(root, wxT("Opened Dataset"), dataset_open_img);
	AddDataSourceNode(root, &MemoryDataSource::INSTANCE());
	wxTreeItemId fileSystemId = _treeCtrl->AppendItem(root, wxT("FileSystem"), disk_img);


#ifdef _WINDOWS_
	char disk[4] = { 'C',':','\\',0 };
	for (char c = 65; c < 91; c++)
	{
		disk[0] = c;
		if (wxDir::Exists(disk))
		{
			FileSystemDataSource* fds = new FileSystemDataSource(disk);
			AddDataSourceNode(fileSystemId, fds);
		}
	}

#endif // _WINDOWS_
	_treeCtrl->Expand(fileSystemId);

	Bind(wxEVT_TREE_ITEM_ACTIVATED, &wxDataSourceWidget::OnNodeActivated, this);
	Bind(wxEVT_TREE_SEL_CHANGED, &wxDataSourceWidget::OnNodeSelChanged, this);
}

wxDataSourceWidget::~wxDataSourceWidget()
{
	Unbind(wxEVT_TREE_ITEM_ACTIVATED, &wxDataSourceWidget::OnNodeActivated, this);
	Unbind(wxEVT_TREE_SEL_CHANGED, &wxDataSourceWidget::OnNodeSelChanged, this);
}

void wxDataSourceWidget::AddDataSourceNode(wxTreeItemId &parent, IDataSource * ds)
{
	dsTreeItemData* mdsData = new dsTreeItemData();
	mdsData->_ds = ds;
	wxString label = ds->GetName();
	if(ds->IsTypeOf(MemoryDataSource::S_GetType()))
		_treeCtrl->AppendItem(parent, label, mem_img, mem_img, mdsData);
	else //if (ds->IsTypeOf(FileSystemDataSource::S_GetType()))
		_treeCtrl->AppendItem(parent, label, folder_img, folder_img, mdsData);
}

void wxDataSourceWidget::OnEachDataSource(IDataSource* ds, void* ud)
{
	wxDataSourceWidget* pDsWidget = (wxDataSourceWidget*)((void**)ud)[0];
	wxTreeItemId* parentItemId = (wxTreeItemId*)((void**)ud)[1];
	pDsWidget->AddDataSourceNode(*parentItemId, ds);
}

void wxDataSourceWidget::OnEachDataset(IDataset* dt, void* ud)
{
	wxDataSourceWidget* pDsWidget = (wxDataSourceWidget*)((void**)ud)[0];
	wxTreeItemId* parentItemId = (wxTreeItemId*)((void**)ud)[1];
	dsTreeItemData* mdsData = new dsTreeItemData();
	mdsData->_dt = dt;
	wxString label = wxString::From8BitData(dt->GetName());
	if(dt->IsTypeOf(MyGDALRasterDataset::S_GetType()))
		pDsWidget->_treeCtrl->AppendItem(*parentItemId, label, raster_img, raster_img, mdsData);
	else //if (dt->IsTypeOf(MyGDALVectorDataset::S_GetType()))
		pDsWidget->_treeCtrl->AppendItem(*parentItemId, label, vector_img, vector_img, mdsData);
}

void wxDataSourceWidget::AddDataSourceSubNode(wxTreeItemId &parent, IDataSource * ds)
{
	void* ud[2] = { this, &parent };
	ds->ForEachDataSource(wxDataSourceWidget::OnEachDataSource, (void*)ud);
	ds->ForEachDataset(wxDataSourceWidget::OnEachDataset, (void*)ud);
}

void wxDataSourceWidget::OnNodeActivated(wxTreeEvent & event)
{
	wxTreeItemId selId = event.GetItem();
	dsTreeItemData* selData = (dsTreeItemData*)_treeCtrl->GetItemData(selId);
	if (selData != nullptr)
	{
		if (selData->_ds != nullptr && selData->_ds->IsConnected() == false)
		{
			selData->_ds->Connect();
			if (selData->_ds->IsConnected())
			{
				AddDataSourceSubNode(selId, selData->_ds);
				//TODO: 当前只有文件系统类型数据源
				_treeCtrl->SetItemImage(selId, folder_open_img);
				_treeCtrl->SetItemImage(selId, folder_open_img, wxTreeItemIcon_Selected);
				_treeCtrl->Expand(selId);
			}
		}
	}
}

void wxDataSourceWidget::OnNodeSelChanged(wxTreeEvent & event)
{
	wxTreeItemId selId = event.GetItem();
	dsTreeItemData* selData = (dsTreeItemData*)_treeCtrl->GetItemData(selId);
	if (selData != nullptr)
	{
		_selDs = selData->_ds;
		_selDt = selData->_dt;
	}
	else
	{
		_selDs = nullptr;
		_selDt = nullptr;
	}
}

END_NAME_SPACE(tGis, Gui)
