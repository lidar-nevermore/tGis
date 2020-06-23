#include "wxGridPaletteTable.h"


BEGIN_NAME_SPACE(tGis, Gui)

class wxGridPaletteTableImpl
{
public:
	wxGridPaletteTableImpl(wxGridPaletteTable* owner)
	{
		_owner = owner;
		_maxEntry = INT_MIN;
	}

	wxGridPaletteTable* _owner;
	vector<int> _vecEntry;
	int _maxEntry;
};

static void __stdcall ForEachColor(int* pos, unsigned char *r, unsigned char *g, unsigned char *b, void* ud)
{
	wxGridPaletteTableImpl* u = (wxGridPaletteTableImpl*)ud;
	u->_vecEntry.push_back(*pos);
	u->_maxEntry = _tgis_max(u->_maxEntry, *pos);
}

wxGridPaletteTable::wxGridPaletteTable(Palette* pal)
{
	_impl_ = new wxGridPaletteTableImpl(this);
	_palette = pal;
	if (_palette != nullptr)
	{
		_palette->Reference();
		_palette->ForEachColor(ForEachColor, _impl_);
	}
	else
	{
		_palette = new Palette();
		_impl_->_maxEntry = 0;
	}
}

wxGridPaletteTable::~wxGridPaletteTable()
{
	if (_palette != nullptr)
		_palette->Release();

	delete _impl_;
}

void wxGridPaletteTable::SetPalette(Palette * pal)
{
	if (_palette != nullptr)
		_palette->Release();
	_palette = pal;
	_palette->Reference();
}

Palette * wxGridPaletteTable::GetPalette()
{
	return _palette;
}

int wxGridPaletteTable::GetNumberRows()
{
	return _impl_->_vecEntry.size();
}

int wxGridPaletteTable::GetNumberCols()
{
	return 2;
}

wxString wxGridPaletteTable::GetValue(int row, int col)
{
	if (col == 0)
	{
		return wxString::Format(wxT("%d"), _impl_->_vecEntry.at(row));
	}
	else if (col == 1)
	{
		int pos = _impl_->_vecEntry.at(row);

		unsigned char r, g, b;
		_palette->GetColor(pos, &r, &g, &b);
		wxColour clr;
		clr.Set(r, g, b);

		return clr.GetAsString();
	}

	return wxT("");
}

void wxGridPaletteTable::SetValue(int row, int col, const wxString & value)
{
	if (col == 0)
	{
		long oe = _impl_->_vecEntry.at(row);
		long ne;
		if (value.ToLong(&ne) && ne != oe)
		{
			int ioe = (int)oe;
			int ine = (int)ne;
			unsigned char r, g, b;
			_palette->GetColor(ioe, &r, &g, &b);
			_palette->RemoveColor(ioe);
			_palette->SetColor(ine, r, g, b);
			_impl_->_vecEntry[row] = ine;
		}
	}
	else if (col == 1)
	{
		int oe = _impl_->_vecEntry.at(row);
		wxColor clr;
		if (clr.Set(value))
			_palette->SetColor(oe, clr.Red(), clr.Green(), clr.Blue());	
	}
}

wxString wxGridPaletteTable::GetTypeName(int row, int col)
{
	if (col == 0)
		return wxGridTableBase::GetTypeName(row, col);
	else
		return wxT("Color");
}

long wxGridPaletteTable::GetValueAsLong(int row, int col)
{
	int pos = _impl_->_vecEntry.at(row);

	if (col == 1)
	{
		unsigned char r, g, b;
		_palette->GetColor(pos, &r, &g, &b);
		long clr = r | g << 8 | b << 16;
		return clr;
	}

	return pos;
}

void wxGridPaletteTable::SetValueAsLong(int row, int col, long value)
{
	long oe = _impl_->_vecEntry.at(row);
	if (col == 0)
	{
		if (value != oe)
		{
			int ioe = (int)oe;
			int ine = (int)value;
			unsigned char r, g, b;
			_palette->GetColor(ioe, &r, &g, &b);
			_palette->RemoveColor(ioe);
			_palette->SetColor(ine, r, g, b);
			_impl_->_vecEntry[row] = ine;
		}
	}
	else if (col == 1)
	{
		int ioe = (int)oe;
		unsigned char r = (unsigned char)(0xFF & value);
		unsigned char g = (unsigned char)(0xFF & value>>8);
		unsigned char b = (unsigned char)(0xFF & value>>16);
		_palette->SetColor(ioe, r, g, b);
	}
}

void wxGridPaletteTable::Clear()
{
	_impl_->_vecEntry.clear();
	_palette->ClearColor();
}

bool wxGridPaletteTable::InsertRows(size_t pos, size_t numRows)
{
	return false;
}

bool wxGridPaletteTable::AppendRows(size_t numRows)
{
	for (size_t i = 0; i < numRows; i++)
	{
		_impl_->_maxEntry++;
		unsigned char r, g, b;
		Palette::GenerateColor(&r, &g, &b);
		_palette->SetColor(_impl_->_maxEntry, r, g, b);
		_impl_->_vecEntry.push_back(_impl_->_maxEntry);
	}
	return true;
}

bool wxGridPaletteTable::DeleteRows(size_t pos, size_t numRows)
{
	auto it = _impl_->_vecEntry.begin() + pos;

	for (size_t i = 0; i < numRows; i++)
	{
		int entry = *it;
		_palette->RemoveColor(entry);
		it = _impl_->_vecEntry.erase(it);
	}
	
	return true;
}

bool wxGridPaletteTable::InsertCols(size_t pos, size_t numCols)
{
	return false;
}

bool wxGridPaletteTable::AppendCols(size_t numCols)
{
	return false;
}

bool wxGridPaletteTable::DeleteCols(size_t pos, size_t numCols)
{
	return false;
}

wxString wxGridPaletteTable::GetColLabelValue(int col)
{
	if(col == 0)
		return wxT("Value");
	else
		return wxT("Color");
}


END_NAME_SPACE(tGis, Gui)

