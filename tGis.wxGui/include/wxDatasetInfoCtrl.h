#pragma once

#ifndef __wxDatasetInfoCtrl_H__
#define __wxDatasetInfoCtrl_H__


#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/textctrl.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;


BEGIN_NAME_SPACE(tGis, Gui)

class wxDatasetInfoCtrlImpl;

class TGIS_GUI_API wxDatasetInfoCtrl : public wxTextCtrl
{
public:
	wxDatasetInfoCtrl(IDataset* dataset,
		wxWindow *parent, wxWindowID id,
		const wxString& value = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxTextCtrlNameStr);

	~wxDatasetInfoCtrl();

private:
	wxDatasetInfoCtrlImpl* _impl_;

private:
	void OnSize(wxSizeEvent& event);
	void OnDatasetInfo(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};

END_NAME_SPACE(tGis, Gui)

#endif
