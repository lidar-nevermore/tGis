#include "tGis_wxApp.h"
#include "MainFrame.h"

BEGIN_NAME_SPACE(tGis, App)

IMPLEMENT_APP(tGisWxApp)

bool tGisWxApp::OnInit()
{
	wxInitAllImageHandlers();
	MapWidget::SetMaxSize(1920, 1280);
	_mainFrame  = new MainFrame();
	SetTopWindow(_mainFrame);
	_mainFrame->Show(true);

	return true;
}

wxFrame * tGisWxApp::GetMainFrame()
{
	return _mainFrame;
}

END_NAME_SPACE(tGis, App)
