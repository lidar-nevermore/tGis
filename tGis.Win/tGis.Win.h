
// tGis.Win.h : tGis.Win Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CtGisWinApp:
// �йش����ʵ�֣������ tGis.Win.cpp
//

class CtGisWinApp : public CWinApp
{
public:
	CtGisWinApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CtGisWinApp theApp;
