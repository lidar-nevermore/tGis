#pragma once


#ifndef __wxGridPaletteTable_H__
#define __wxGridPaletteTable_H__

#include <wx/grid.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class wxGridPaletteTableImpl;

class TGIS_GUI_API wxGridPaletteTable : public wxGridTableBase
{
public:
	wxGridPaletteTable(Palette* pal);
	virtual ~wxGridPaletteTable();

	void SetPalette(Palette* pal);
	Palette* GetPalette();

	// Í¨¹ý wxGridTableBase ¼Ì³Ð
	virtual int GetNumberRows() override;
	virtual int GetNumberCols() override;
	virtual wxString GetValue(int row, int col) override;
	virtual void SetValue(int row, int col, const wxString & value) override;
	virtual wxString GetTypeName(int row, int col) override;
	virtual long GetValueAsLong(int row, int col);
	virtual void SetValueAsLong(int row, int col, long value);

	virtual void Clear() override;
	virtual bool InsertRows(size_t pos = 0, size_t numRows = 1) override;
	virtual bool AppendRows(size_t numRows = 1) override;
	virtual bool DeleteRows(size_t pos = 0, size_t numRows = 1) override;
	virtual bool InsertCols(size_t pos = 0, size_t numCols = 1) override;
	virtual bool AppendCols(size_t numCols = 1) override;
	virtual bool DeleteCols(size_t pos = 0, size_t numCols = 1) override;

	virtual wxString GetColLabelValue(int col);
private:
	Palette* _palette;
	wxGridPaletteTableImpl* _impl_;
};

END_NAME_SPACE(tGis, Gui)

#endif

