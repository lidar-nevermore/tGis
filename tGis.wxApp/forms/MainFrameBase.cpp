///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MainFrameBase.h"

///////////////////////////////////////////////////////////////////////////

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	_menuBar = new wxMenuBar( 0 );
	_menuProject = new wxMenu();
	wxMenuItem* _miNewProject;
	_miNewProject = new wxMenuItem( _menuProject, wxID_ANY, wxString( wxT("新建项目") ) , wxEmptyString, wxITEM_NORMAL );
	_menuProject->Append( _miNewProject );

	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( _menuProject, wxID_ANY, wxString( wxT("打开项目") ) , wxEmptyString, wxITEM_NORMAL );
	_menuProject->Append( m_menuItem4 );

	wxMenuItem* m_menuItem14;
	m_menuItem14 = new wxMenuItem( _menuProject, wxID_ANY, wxString( wxT("项目属性") ) , wxEmptyString, wxITEM_NORMAL );
	_menuProject->Append( m_menuItem14 );

	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( _menuProject, wxID_ANY, wxString( wxT("保存项目") ) , wxEmptyString, wxITEM_NORMAL );
	_menuProject->Append( m_menuItem5 );

	wxMenuItem* m_menuItem6;
	m_menuItem6 = new wxMenuItem( _menuProject, wxID_ANY, wxString( wxT("另存项目") ) , wxEmptyString, wxITEM_NORMAL );
	_menuProject->Append( m_menuItem6 );

	wxMenuItem* m_menuItem13;
	m_menuItem13 = new wxMenuItem( _menuProject, wxID_ANY, wxString( wxT("关闭项目") ) , wxEmptyString, wxITEM_NORMAL );
	_menuProject->Append( m_menuItem13 );

	_menuProject->AppendSeparator();

	wxMenuItem* m_menuItem17;
	m_menuItem17 = new wxMenuItem( _menuProject, wxID_ANY, wxString( wxT("退出") ) , wxEmptyString, wxITEM_NORMAL );
	_menuProject->Append( m_menuItem17 );

	_menuBar->Append( _menuProject, wxT("Project") );

	_menuDataSource = new wxMenu();
	wxMenuItem* m_menuItem18;
	m_menuItem18 = new wxMenuItem( _menuDataSource, wxID_ANY, wxString( wxT("添加文件系统数据源") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDataSource->Append( m_menuItem18 );

	wxMenuItem* m_menuItem19;
	m_menuItem19 = new wxMenuItem( _menuDataSource, wxID_ANY, wxString( wxT("添加PostGIS数据源") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDataSource->Append( m_menuItem19 );

	wxMenuItem* m_menuItem20;
	m_menuItem20 = new wxMenuItem( _menuDataSource, wxID_ANY, wxString( wxT("数据源信息") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDataSource->Append( m_menuItem20 );

	wxMenuItem* m_menuItem24;
	m_menuItem24 = new wxMenuItem( _menuDataSource, wxID_ANY, wxString( wxT("刷新数据源") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDataSource->Append( m_menuItem24 );

	wxMenuItem* m_menuItem21;
	m_menuItem21 = new wxMenuItem( _menuDataSource, wxID_ANY, wxString( wxT("断开数据源") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDataSource->Append( m_menuItem21 );

	_menuDataSource->AppendSeparator();

	wxMenuItem* _miOpenRasterDataset;
	_miOpenRasterDataset = new wxMenuItem( _menuDataSource, wxID_ANY, wxString( wxT("打开栅格数据集") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDataSource->Append( _miOpenRasterDataset );

	wxMenuItem* _miOpenVectorDataset;
	_miOpenVectorDataset = new wxMenuItem( _menuDataSource, wxID_ANY, wxString( wxT("打开矢量数据集") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDataSource->Append( _miOpenVectorDataset );

	wxMenuItem* m_menuItem30;
	m_menuItem30 = new wxMenuItem( _menuDataSource, wxID_ANY, wxString( wxT("新建内存栅格数据集") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDataSource->Append( m_menuItem30 );

	wxMenuItem* m_menuItem32;
	m_menuItem32 = new wxMenuItem( _menuDataSource, wxID_ANY, wxString( wxT("新建聚合栅格数据集") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDataSource->Append( m_menuItem32 );

	wxMenuItem* m_menuItem23;
	m_menuItem23 = new wxMenuItem( _menuDataSource, wxID_ANY, wxString( wxT("关闭数据集") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDataSource->Append( m_menuItem23 );

	_menuBar->Append( _menuDataSource, wxT("DataSource") );

	_menuMap = new wxMenu();
	wxMenuItem* _miZoomIn;
	_miZoomIn = new wxMenuItem( _menuMap, wxID_ANY, wxString( wxT("放大") ) , wxEmptyString, wxITEM_NORMAL );
	_menuMap->Append( _miZoomIn );

	wxMenuItem* _miZoomOut;
	_miZoomOut = new wxMenuItem( _menuMap, wxID_ANY, wxString( wxT("缩小") ) , wxEmptyString, wxITEM_NORMAL );
	_menuMap->Append( _miZoomOut );

	wxMenuItem* m_menuItem381;
	m_menuItem381 = new wxMenuItem( _menuMap, wxID_ANY, wxString( wxT("自由缩放") ) , wxEmptyString, wxITEM_NORMAL );
	_menuMap->Append( m_menuItem381 );

	wxMenuItem* m_menuItem401;
	m_menuItem401 = new wxMenuItem( _menuMap, wxID_ANY, wxString( wxT("漫游") ) , wxEmptyString, wxITEM_NORMAL );
	_menuMap->Append( m_menuItem401 );

	wxMenuItem* m_menuItem391;
	m_menuItem391 = new wxMenuItem( _menuMap, wxID_ANY, wxString( wxT("框选放大") ) , wxEmptyString, wxITEM_NORMAL );
	_menuMap->Append( m_menuItem391 );

	wxMenuItem* m_menuItem421;
	m_menuItem421 = new wxMenuItem( _menuMap, wxID_ANY, wxString( wxT("图层范围") ) , wxEmptyString, wxITEM_NORMAL );
	_menuMap->Append( m_menuItem421 );

	wxMenuItem* m_menuItem41;
	m_menuItem41 = new wxMenuItem( _menuMap, wxID_ANY, wxString( wxT("全图范围") ) , wxEmptyString, wxITEM_NORMAL );
	_menuMap->Append( m_menuItem41 );

	_menuMap->AppendSeparator();

	wxMenuItem* _menuItem1;
	_menuItem1 = new wxMenuItem( _menuMap, wxID_ANY, wxString( wxT("显示联动窗口") ) , wxEmptyString, wxITEM_NORMAL );
	_menuMap->Append( _menuItem1 );

	wxMenuItem* _menuItem2;
	_menuItem2 = new wxMenuItem( _menuMap, wxID_ANY, wxString( wxT("关闭联动窗口") ) , wxEmptyString, wxITEM_NORMAL );
	_menuMap->Append( _menuItem2 );

	wxMenuItem* _menuItem;
	_menuItem = new wxMenuItem( _menuMap, wxID_ANY, wxString( wxT("图层卷帘") ) , wxEmptyString, wxITEM_NORMAL );
	_menuMap->Append( _menuItem );

	_menuMap->AppendSeparator();

	wxMenuItem* m_menuItem51;
	m_menuItem51 = new wxMenuItem( _menuMap, wxID_ANY, wxString( wxT("地图空间参考") ) , wxEmptyString, wxITEM_NORMAL );
	_menuMap->Append( m_menuItem51 );

	wxMenuItem* m_menuItem26;
	m_menuItem26 = new wxMenuItem( _menuMap, wxID_ANY, wxString( wxT("添加图层") ) , wxEmptyString, wxITEM_NORMAL );
	_menuMap->Append( m_menuItem26 );

	wxMenuItem* m_menuItem27;
	m_menuItem27 = new wxMenuItem( _menuMap, wxID_ANY, wxString( wxT("移除图层") ) , wxEmptyString, wxITEM_NORMAL );
	_menuMap->Append( m_menuItem27 );

	wxMenuItem* m_menuItem28;
	m_menuItem28 = new wxMenuItem( _menuMap, wxID_ANY, wxString( wxT("移除所有图层") ) , wxEmptyString, wxITEM_NORMAL );
	_menuMap->Append( m_menuItem28 );

	_menuBar->Append( _menuMap, wxT("Map") );

	_menuDisplay = new wxMenu();
	wxMenuItem* m_menuItem29;
	m_menuItem29 = new wxMenuItem( _menuDisplay, wxID_ANY, wxString( wxT("最大最小值拉伸") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDisplay->Append( m_menuItem29 );

	wxMenuItem* m_menuItem33;
	m_menuItem33 = new wxMenuItem( _menuDisplay, wxID_ANY, wxString( wxT("均值+/-标准差拉伸") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDisplay->Append( m_menuItem33 );

	wxMenuItem* m_menuItem34;
	m_menuItem34 = new wxMenuItem( _menuDisplay, wxID_ANY, wxString( wxT("设定范围拉伸") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDisplay->Append( m_menuItem34 );

	_menuDisplay->AppendSeparator();

	wxMenuItem* m_menuItem35;
	m_menuItem35 = new wxMenuItem( _menuDisplay, wxID_ANY, wxString( wxT("直方图均衡化") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDisplay->Append( m_menuItem35 );

	wxMenuItem* m_menuItem36;
	m_menuItem36 = new wxMenuItem( _menuDisplay, wxID_ANY, wxString( wxT("亮度对比度调整") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDisplay->Append( m_menuItem36 );

	wxMenuItem* m_menuItem37;
	m_menuItem37 = new wxMenuItem( _menuDisplay, wxID_ANY, wxString( wxT("套用现有LUT") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDisplay->Append( m_menuItem37 );

	wxMenuItem* m_menuItem40;
	m_menuItem40 = new wxMenuItem( _menuDisplay, wxID_ANY, wxString( wxT("LUT交互调整") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDisplay->Append( m_menuItem40 );

	wxMenuItem* m_menuItem38;
	m_menuItem38 = new wxMenuItem( _menuDisplay, wxID_ANY, wxString( wxT("还原显示增强") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDisplay->Append( m_menuItem38 );

	_menuDisplay->AppendSeparator();

	wxMenuItem* m_menuItem39;
	m_menuItem39 = new wxMenuItem( _menuDisplay, wxID_ANY, wxString( wxT("查看LUT") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDisplay->Append( m_menuItem39 );

	wxMenuItem* m_menuItem42;
	m_menuItem42 = new wxMenuItem( _menuDisplay, wxID_ANY, wxString( wxT("保存LUT") ) , wxEmptyString, wxITEM_NORMAL );
	_menuDisplay->Append( m_menuItem42 );

	_menuBar->Append( _menuDisplay, wxT("Display") );

	_menuSettings = new wxMenu();
	wxMenuItem* m_menuItem341;
	m_menuItem341 = new wxMenuItem( _menuSettings, wxID_ANY, wxString( wxT("关于") ) , wxEmptyString, wxITEM_NORMAL );
	_menuSettings->Append( m_menuItem341 );

	_menuBar->Append( _menuSettings, wxT("Settings") );

	this->SetMenuBar( _menuBar );

	_toolBar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	_toolBar->Realize();

	_statusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	_menuProject->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnNewProject ), this, _miNewProject->GetId());
	_menuDataSource->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnOpenRasterDataset ), this, _miOpenRasterDataset->GetId());
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events

}
