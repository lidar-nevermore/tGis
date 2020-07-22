#pragma once

#ifndef __wxRaster3dViewDialog_H__
#define __wxRaster3dViewDialog_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/treectrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/slider.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include "wx/glcanvas.h"
#include <wx/choice.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;


BEGIN_NAME_SPACE(tGis, Gui)

class wxRaster3dViewDialogImpl;

class TGIS_GUI_API wxRaster3dViewDialog : public wxDialog
{
public:
	wxRaster3dViewDialog(wxWindow* parent = nullptr, 
		wxWindowID id = wxID_ANY, 
		const wxString& title = wxT("Raster 3d View"), 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxSize(1417, 844), 
		long style = wxDEFAULT_DIALOG_STYLE | wxMAXIMIZE_BOX | wxRESIZE_BORDER);
	~wxRaster3dViewDialog();

public:
	void SetRaster(MyGDALRasterDataset* raster, IVertex2dList* polygon);

private:
	wxRaster3dViewDialogImpl* _impl_;

private:
	wxChoice* _chBand;
	wxButton* _btnYawPlus;
	wxButton* _btnYawMinus;
	wxButton* _btnRollPlus;
	wxButton* _btnRollMinus;
	wxButton* _btnPitchPlus;
	wxButton* _btnPitchMinus;
	wxButton* _btnZoomIn;
	wxButton* _btnZoomOut;
	wxButton* _btnGradColor;
	wxCheckBox* _chkGradColor;
	wxCheckBox* _chkRefLevel;
	wxCheckBox* _chkBoundary;
	wxStaticText* m_staticText1;
	wxSlider* _sldZStretch;
	wxTextCtrl* _txtZStretch;
	wxStaticText* m_staticText2;
	wxSlider* _sldRefLevel;
	wxTextCtrl* _txtRefLevel;
	wxStaticText* m_staticText3;
	wxTextCtrl* _txtVolumeAbove;
	wxStaticText* m_staticText4;
	wxTextCtrl* _txtVolumeBelow;

private:
	void _chBand_Choice(wxCommandEvent& event);
	void _btnYawPlus_Clicked(wxCommandEvent& event);
	void _btnYawMinus_Clicked(wxCommandEvent& event);
	void _btnRollPlus_Clicked(wxCommandEvent& event);
	void _btnRollMinus_Clicked(wxCommandEvent& event);
	void _btnPitchPlus_Clicked(wxCommandEvent& event);
	void _btnPitchMinus_Clicked(wxCommandEvent& event);
	void _btnZoomIn_Clicked(wxCommandEvent& event);
	void _btnZoomOut_Clicked(wxCommandEvent& event);
	void _btnGradColor_Clicked(wxCommandEvent& event);
	void _chkGradColor_Check(wxCommandEvent& event);
	void _chkRefLevel_Check(wxCommandEvent& event);
	void _chkBoundary_Check(wxCommandEvent& event);
	void _sldZStretch_Scroll(wxCommandEvent& event);
	void _sldRefLevel_scroll(wxCommandEvent& event);
	void _txtZStretch_TextEnter(wxCommandEvent& event);
	void _txtRefLevel_TextEnter(wxCommandEvent& event);
};


END_NAME_SPACE(tGis, Gui)

#endif

