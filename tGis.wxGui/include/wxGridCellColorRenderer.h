#pragma once


#ifndef __wxGridCellColorRenderer_H__
#define __wxGridCellColorRenderer_H__

#include <wx/grid.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API wxGridCellColorRenderer : public wxGridCellRenderer
{
public:
	wxGridCellColorRenderer();
	virtual ~wxGridCellColorRenderer();

	// Í¨¹ý wxGridCellRenderer ¼Ì³Ð
	virtual void Draw(wxGrid & grid, wxGridCellAttr & attr, wxDC & dc, const wxRect & rect, int row, int col, bool isSelected) override;
	virtual wxSize GetBestSize(wxGrid & grid, wxGridCellAttr & attr, wxDC & dc, int row, int col) override;
	virtual wxGridCellRenderer * Clone() const override;
};

END_NAME_SPACE(tGis, Gui)

#endif

