#pragma once

#ifndef __wxSpatialReferenceDialog_H__
#define __wxSpatialReferenceDialog_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;

class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API wxSpatialReferenceDialog : public wxDialog
{
public:
	wxSpatialReferenceDialog(wxWindow* parent = nullptr,
		wxWindowID id = wxID_ANY, 
		const wxString& title = wxT("SpatialReference"), 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxSize(485, 550), 
		long style = wxDEFAULT_DIALOG_STYLE);
	~wxSpatialReferenceDialog();

protected:
	wxTextCtrl* _txtSpatialReference;
	wxStdDialogButtonSizer* _sdbSz;
	wxButton* _sdbSzOK;

public:
	void SetSpatialReference(const OGRSpatialReference* spatialRef);

};

END_NAME_SPACE(tGis, Gui)

#endif

