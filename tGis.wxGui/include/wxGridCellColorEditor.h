#pragma once

#ifndef __wxGridCellColorEditor_H__
#define __wxGridCellColorEditor_H__

#include <wx/grid.h>
#include <wx/clrpicker.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class wxGridCellColorEditor : public wxGridCellEditor
{
public:
	wxGridCellColorEditor();
	virtual ~wxGridCellColorEditor();

	// Í¨¹ý wxGridCellEditor ¼Ì³Ð
	virtual void Create(wxWindow * parent, wxWindowID id, wxEvtHandler * evtHandler) override;
	virtual void BeginEdit(int row, int col, wxGrid * grid) override;
	virtual bool EndEdit(int row, int col, const wxGrid * grid, const wxString & oldval, wxString * newval) override;
	virtual void ApplyEdit(int row, int col, wxGrid * grid) override;
	virtual void Reset() override;
	virtual wxGridCellEditor * Clone() const override;
	virtual wxString GetValue() const override;

private:
	wxColour _color;
	wxColourPickerCtrl* _cp;
};

END_NAME_SPACE(tGis, Gui)

#endif

