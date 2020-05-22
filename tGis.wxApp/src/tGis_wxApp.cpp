#include "tGis_wxApp.h"
#include "MainFrame.h"
#include <stdio.h>

IMPLEMENT_APP(tGisWxApp)

bool tGisWxApp::OnInit()
{
	wxInitAllImageHandlers();

	//TODO: 获取屏幕最大尺寸
	MapWidget::SetMaxSize(1920, 1280);
	_mainFrame  = new MainFrame();

	std::string appNameFile = GetExeDir();
	appNameFile.append(TGIS_PATH_SEPARATOR_STR);
	appNameFile.append(".appname");
	FILE* fpAppName = fopen(appNameFile.c_str(), "rt");
	if (fpAppName != nullptr)
	{
		char buf[1000] = { 0 };
		fgets(buf, 1000, fpAppName);
		wxString strAppName = buf;
		_mainFrame->SetTitle(strAppName);
		fclose(fpAppName);
	}

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

