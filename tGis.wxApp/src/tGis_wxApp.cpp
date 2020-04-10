#include "tGis_wxApp.h"
#include "MainFrame.h"

BEGIN_NAME_SPACE(tGis, App)

IMPLEMENT_APP(tGisWxApp)

bool tGisWxApp::OnInit()
{
	MapWidget::SetMaxSize(1920, 1280);

	MainFrame * mainWnd = new MainFrame();
	SetTopWindow(mainWnd);
	mainWnd->Show(true);

	return true;
}

END_NAME_SPACE(tGis, App)
