#pragma once

#ifndef __wxDatasetInfoWidget_H__
#define __wxDatasetInfoWidget_H__


#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/textctrl.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;


BEGIN_NAME_SPACE(tGis, Gui)

class wxDatasetInfoWidgetImpl;

class TGIS_GUI_API wxDatasetInfoWidget : public wxTextCtrl
{
public:
	wxDatasetInfoWidget(IDataset* dataset,
		wxWindow *parent, wxWindowID id,
		const wxString& value = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxTextCtrlNameStr);

	~wxDatasetInfoWidget();

private:
	wxDatasetInfoWidgetImpl* _impl_;

private:
	void OnSize(wxSizeEvent& event);
	void OnDatasetInfo(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};

END_NAME_SPACE(tGis, Gui)

#endif
