#include "PaletteColorDialog.h"

PaletteColorDialog::PaletteColorDialog( wxWindow* parent )
:
PaletteColorDialogBase( parent )
{

}

void PaletteColorDialog::SetPaletteColor(int entry, unsigned char r, unsigned char g, unsigned char b)
{
	_txtValue->SetValue(wxString::Format(wxT("%d"), entry));
	wxColour clr;
	clr.Set(r, g, b);
	_cpColor->SetColour(clr);
}

bool PaletteColorDialog::GetPaletteColor(int * entry, unsigned char * r, unsigned char * g, unsigned char * b)
{	
	wxString entryStr = _txtValue->GetValue();
	bool flag = !entryStr.IsEmpty();
	long entryL;
	flag = flag && entryStr.ToLong(&entryL);
	flag = flag && entryL < INT_MAX;

	*entry = (int)entryL;

	wxColor clr = _cpColor->GetColour();
	*r = clr.Red();
	*g = clr.Green();
	*b = clr.Blue();

	return flag;
}

void PaletteColorDialog::SetKeyColor(double pos, unsigned char r, unsigned char g, unsigned char b)
{
	_txtValue->SetValue(wxString::Format(wxT("%.3f"), pos));
	wxColour clr;
	clr.Set(r, g, b);
	_cpColor->SetColour(clr);
}

bool PaletteColorDialog::GetKeyColor(double * pos, unsigned char * r, unsigned char * g, unsigned char * b)
{
	wxString posStr = _txtValue->GetValue();
	bool flag = !posStr.IsEmpty();
	flag = flag && posStr.ToDouble(pos);

	wxColor clr = _cpColor->GetColour();
	*r = clr.Red();
	*g = clr.Green();
	*b = clr.Blue();

	return flag;
}
