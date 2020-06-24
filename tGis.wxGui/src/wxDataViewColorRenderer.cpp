
#include <wx/dc.h>
#include <wx/clrpicker.h>

#include "wxDataViewColorRenderer.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

wxDataViewColorRenderer::wxDataViewColorRenderer(wxDataViewCellMode mode, int alignment)
	:wxDataViewCustomRenderer("colour", mode, alignment)
{

}

bool wxDataViewColorRenderer::SetValue(const wxVariant & value)
{
	long cv = value.GetLong();
	unsigned char r = (unsigned char)(0xFF & cv);
	unsigned char g = (unsigned char)(0xFF & cv >> 8);
	unsigned char b = (unsigned char)(0xFF & cv >> 16);
	_clr.Set(r, g, b);
	//_clr << value;
	return true;
}

bool wxDataViewColorRenderer::GetValue(wxVariant & value) const
{
	value = (long)(_clr.GetPixel());
	return true;
}

bool wxDataViewColorRenderer::Render(wxRect cell, wxDC * dc, int state)
{
	dc->SetBrush(_clr);
	dc->SetPen(*wxTRANSPARENT_PEN);
	wxRect rc = cell;
	rc.Deflate(3);
	dc->DrawRectangle(rc);

	return true;
}

wxSize wxDataViewColorRenderer::GetSize() const
{
	return wxSize(-1, 23);
}


END_NAME_SPACE(tGis, Gui)
