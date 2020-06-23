#include "wxGridCellColorEditor.h"


BEGIN_NAME_SPACE(tGis, Gui)

wxGridCellColorEditor::wxGridCellColorEditor()
{

}


wxGridCellColorEditor::~wxGridCellColorEditor()
{
}

void wxGridCellColorEditor::Create(wxWindow * parent, wxWindowID id, wxEvtHandler * evtHandler)
{
	_cp = new wxColourPickerCtrl(parent, id);
	SetControl(_cp);
	wxGridCellEditor::Create(parent, id, evtHandler);
}

void wxGridCellColorEditor::BeginEdit(int row, int col, wxGrid * grid)
{
	long cv = grid->GetTable()->GetValueAsLong(row, col);
	unsigned char r = (unsigned char)(0xFF & cv);
	unsigned char g = (unsigned char)(0xFF & cv >> 8);
	unsigned char b = (unsigned char)(0xFF & cv >> 16);
	wxColour clr;
	clr.Set(r, g, b);
	_cp->SetColour(clr);
	_cp->SetFocus();
}

bool wxGridCellColorEditor::EndEdit(int row, int col, const wxGrid * grid, const wxString & oldval, wxString * newval)
{
	const wxColour clr = _cp->GetColour();

	if (_color == clr)
	{
		return false;
	}

	_color = clr;

	if (newval)
	{
		*newval = _color.GetAsString();
	}

	return true;
}

void wxGridCellColorEditor::ApplyEdit(int row, int col, wxGrid * grid)
{
	grid->GetTable()->SetValueAsLong(row, col, _color.GetPixel());
}

void wxGridCellColorEditor::Reset()
{
}

wxGridCellEditor * wxGridCellColorEditor::Clone() const
{
	return new wxGridCellColorEditor();
}

wxString wxGridCellColorEditor::GetValue() const
{
	return _color.GetAsString();
}

END_NAME_SPACE(tGis, Gui)

