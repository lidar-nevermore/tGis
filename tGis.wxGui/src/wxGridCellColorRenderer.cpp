#include "wxGridCellColorRenderer.h"
#include "wx/dc.h"

BEGIN_NAME_SPACE(tGis, Gui)

wxGridCellColorRenderer::wxGridCellColorRenderer()
{
}


wxGridCellColorRenderer::~wxGridCellColorRenderer()
{
}

void wxGridCellColorRenderer::Draw(wxGrid & grid, wxGridCellAttr & attr, wxDC & dc, const wxRect & rect, int row, int col, bool isSelected)
{
	wxGridCellRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
	wxGridTableBase *table = grid.GetTable();
	long cv = table->GetValueAsLong(row, col);
	unsigned char r = (unsigned char)(0xFF & cv);
	unsigned char g = (unsigned char)(0xFF & cv >> 8);
	unsigned char b = (unsigned char)(0xFF & cv >> 16);
	wxColour clr;
	clr.Set(r, g, b);
	dc.SetBrush(clr);
	dc.SetPen(*wxTRANSPARENT_PEN);
	wxRect rc = rect;
	rc.Deflate(3);
	dc.DrawRectangle(rc);
}

wxSize wxGridCellColorRenderer::GetBestSize(wxGrid & grid, wxGridCellAttr & attr, wxDC & dc, int row, int col)
{
	wxSize sz = grid.GetCellSize(wxGridCellCoords(row, col));
	return sz;
}

wxGridCellRenderer * wxGridCellColorRenderer::Clone() const
{
	return new wxGridCellColorRenderer();
}

END_NAME_SPACE(tGis, Gui)
