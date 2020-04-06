
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "tGis.Win.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_OPEN_IMG, &CMainFrame::OnOpenImg)
//	ON_WM_SIZE()
ON_COMMAND(ID_CLOSE_IMG, &CMainFrame::OnCloseImg)
ON_COMMAND(ID_WHOLE_VIEW, &CMainFrame::OnWholeView)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	_dataset = nullptr;
}

CMainFrame::~CMainFrame()
{
	OnCloseImg();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 创建一个视图以占用框架的工作区
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 否则，执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::OnOpenImg()
{
	// TODO: 在此添加命令处理程序代码
	CString     strOpenFilter = _T("遥感影像(*.img;*.tiff;*.tif;*.pix)|*.img;*.tiff;*.tif;*.pix|常规影像(*.jpg;*.jpeg;*.bmp;*.png)|*.jpg;*.jpeg;*.bmp;*.png|All Files(*.*)|*.*||");
	CFileDialog openFileDlg(TRUE,
		_T(""),
		NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		strOpenFilter);
	if (openFileDlg.DoModal() == IDOK)
	{
		//以下代码为打开数据	
		CString imgPath = openFileDlg.GetPathName();
#pragma push_macro("new")
#undef  new

		_dataset = new MyGDALFileRasterDataset(nullptr, (char*)imgPath.GetBuffer(), GA_ReadOnly, false);
		Layer* layer = new Layer(_dataset);
		int bandCount = ((MyGDALFileRasterDataset*)_dataset)->GetGDALDataset()->GetRasterCount();
		if (bandCount == 0)
		{
			delete layer;
			delete _dataset;
			_dataset = nullptr;
			MessageBox(_T("不是有效的栅格文件！"));
			return;
		}
		ILayerRender* layerRender = nullptr;
		if(bandCount < 3)
			layerRender = new RasterGrayScaleLayerRender(layer,1);
		else
			layerRender = new RasterRgbLayerRender(layer, 1, 2, 3);
#pragma pop_macro("new")
		m_wndView.GetMap()->AddLayer(layer);
	}
}


void CMainFrame::OnCloseImg()
{
	// TODO: 在此添加命令处理程序代码
	if (_dataset != nullptr)
	{
		m_wndView.GetMap()->ClearLayers();
		delete _dataset;
		_dataset = nullptr;
	}
}


void CMainFrame::OnWholeView()
{
	// TODO: 在此添加命令处理程序代码
	if (_dataset != nullptr)
	{
		m_wndView.GetViewPort()->IncludeEnvelope(m_wndView.GetMap()->GetEnvelope());
		m_wndView.RepaintMap();
	}
}
