
// MainFrm.cpp : CMainFrame ���ʵ��
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
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

	// ����һ����ͼ��ռ�ÿ�ܵĹ�����
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("δ�ܴ�����ͼ����\n");
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame ���

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


// CMainFrame ��Ϣ�������

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// ������ǰ�Ƶ���ͼ����
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ����ͼ��һ�γ��Ը�����
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// ����ִ��Ĭ�ϴ���
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::OnOpenImg()
{
	// TODO: �ڴ���������������
	CString     strOpenFilter = _T("ң��Ӱ��(*.img;*.tiff;*.tif;*.pix)|*.img;*.tiff;*.tif;*.pix|����Ӱ��(*.jpg;*.jpeg;*.bmp;*.png)|*.jpg;*.jpeg;*.bmp;*.png|All Files(*.*)|*.*||");
	CFileDialog openFileDlg(TRUE,
		_T(""),
		NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		strOpenFilter);
	if (openFileDlg.DoModal() == IDOK)
	{
		//���´���Ϊ������	
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
			MessageBox(_T("������Ч��դ���ļ���"));
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
	// TODO: �ڴ���������������
	if (_dataset != nullptr)
	{
		m_wndView.GetMap()->ClearLayers();
		delete _dataset;
		_dataset = nullptr;
	}
}


void CMainFrame::OnWholeView()
{
	// TODO: �ڴ���������������
	if (_dataset != nullptr)
	{
		m_wndView.GetViewPort()->IncludeEnvelope(m_wndView.GetMap()->GetEnvelope());
		m_wndView.RepaintMap();
	}
}
