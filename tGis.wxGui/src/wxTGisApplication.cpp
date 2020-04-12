#include "wxTGisApplication.h"


BEGIN_NAME_SPACE(tGis, Gui)

wxTGisApplication::wxTGisApplication(wxFrame* mainFrame)
{
	_mainFrame = mainFrame;
}


wxTGisApplication::~wxTGisApplication()
{
}

END_NAME_SPACE(tGis, Gui)