
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "MFCMapWidget.h"

using namespace tGis::wGui;

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

private:
	IDataset* _dataset;

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	MFCMapWidget    m_wndView;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnOpenImg();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCloseImg();
	afx_msg void OnWholeView();
};


