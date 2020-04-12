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

wxDataSourceWidget::wxDataSourceWidget( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	_toolBar->Realize();

	bSizer1->Add( _toolBar, 0, wxEXPAND, 5 );

	_treeCtrl = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT | wxNO_BORDER);
	bSizer1->Add( _treeCtrl, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	
	wxImageList* imglist = new wxImageList(16, 16, true, 9);
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
	_treeCtrl->AppendItem(root, wxT("Opened Dataset"), 0);
	dsTreeItemData* mdsData = new dsTreeItemData();
	mdsData->_ds = &MemoryDataSource::INSTANCE();
	_treeCtrl->AppendItem(root, wxT("Memory"), 1, 1, mdsData);
	wxTreeItemId fileSystemId = _treeCtrl->AppendItem(root, wxT("FileSystem"), 2);


#ifdef _WINDOWS_
	char disk[4] = { 'C',':','\\',0 };
	for (char c = 65; c < 91; c++)
	{
		disk[0] = c;
		if (wxDir::Exists(disk))
		{
			FileSystemDataSource* fds = new FileSystemDataSource(disk);
			dsTreeItemData* fdsData = new dsTreeItemData(true);
			fdsData->_ds = fds;
			_treeCtrl->AppendItem(fileSystemId, disk, 3, 3, fdsData);
		}
	}

#endif // _WINDOWS_

}

wxDataSourceWidget::~wxDataSourceWidget()
{
}

END_NAME_SPACE(tGis, Gui)
