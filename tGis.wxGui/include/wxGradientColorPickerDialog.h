#pragma once

#ifndef __wxGradientColorPickerDialog_H__
#define __wxGradientColorPickerDialog_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/gdicmn.h>
#include <wx/notebook.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

#include "tGis_wxGuiCfg.h"


using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class wxGradientColorRepositoryWidget;

class wxGradientColorPickerDialog : public wxDialog
{
public:
	wxGradientColorPickerDialog(wxWindow* parent = nullptr, wxWindowID id = wxID_ANY, const wxString& title = wxT("Gradient Color"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(569, 376), long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
	~wxGradientColorPickerDialog();

protected:
	wxNotebook* _tabGrdColor;
	wxStdDialogButtonSizer* _sdbSz;
	wxButton* _sdbSzOK;
	wxButton* _sdbSzCancel;
	wxGradientColorRepositoryWidget* _gcrWidget;

};

END_NAME_SPACE(tGis, Gui)

#endif

