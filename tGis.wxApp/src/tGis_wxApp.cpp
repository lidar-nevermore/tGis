#include "tGis_wxApp.h"
#include "MainFrame.h"


IMPLEMENT_APP(tGisWxApp)

bool tGisWxApp::OnInit()
{
	wxInitAllImageHandlers();
	//TODO: ��ȡ��Ļ���ߴ�
	MapWidget::SetMaxSize(1920, 1280);
	_mainFrame  = new MainFrame();
	SetTopWindow(_mainFrame);
	_mapWidget = _mainFrame->_mapWidget;
	_mainFrame->Show(true);

	return true;
}

wxFrame * tGisWxApp::GetMainFrame()
{
	return _mainFrame;
}

wxGLMapWidget * tGisWxApp::GetCurMapWidget()
{
	return _mapWidget;
}

