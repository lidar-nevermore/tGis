#pragma once

#ifndef __wxDatasetInfoDialog_H__
#define __wxDatasetInfoDialog_H__

#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>


#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;


BEGIN_NAME_SPACE(tGis, Gui)

class wxDatasetInfoWidget;

class TGIS_GUI_API wxDatasetInfoDialog : public wxDialog
{
public:
	wxDatasetInfoDialog(IDataset* dataset, 
		wxWindow* parent = nullptr, 
		wxWindowID id = wxID_ANY, 
		const wxString& title = wxT("Dataset Info"), 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxSize(461, 538), 
		long style = wxDEFAULT_DIALOG_STYLE);

	~wxDatasetInfoDialog();

private:
	wxDatasetInfoWidget* _txtDtInfo;
	wxStdDialogButtonSizer* _sdbSz;
	wxButton* _sdbSzOK;
};

END_NAME_SPACE(tGis, Gui)

#endif

