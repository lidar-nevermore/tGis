#pragma once


#ifndef __wxDataViewColorRenderer_H__
#define __wxDataViewColorRenderer_H__

#include <wx/dataview.h>
#include <wx/clrpicker.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API wxDataViewColorRenderer : public wxDataViewCustomRenderer
{
public:
	wxDataViewColorRenderer(wxDataViewCellMode mode = wxDATAVIEW_CELL_EDITABLE,
		int alignment = wxDVR_DEFAULT_ALIGNMENT);

	virtual bool SetValue(const wxVariant & value) override;

	virtual bool GetValue(wxVariant & value) const override;

	virtual bool Render(wxRect cell, wxDC * dc, int state) override;

	virtual wxSize GetSize() const override;

private:
	wxColour _clr;
};

END_NAME_SPACE(tGis, Gui)

#endif

