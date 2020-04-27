#include "wxSymbolPickerWidget.h"
#include "wxSymbolLibraryWidget.h"

BEGIN_NAME_SPACE(tGis, Gui)

class wxSymbolPickerWidgetImpl
{
public:
	wxSymbolPickerWidgetImpl(wxSymbolPickerWidget* owner)
	{
		_owner = owner;
	}

	wxSymbolPickerWidget* _owner;
	vector<ISymbolLibrary*> _vecSymbolLibrary;
};

wxSymbolPickerWidget::wxSymbolPickerWidget(SymbolLibraryType defaultSymLibType, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	_selSymLib = nullptr;
	_selSym = nullptr;
	_impl_ = new wxSymbolPickerWidgetImpl(this);


	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer(0, 0);
	gbSizer1->SetFlexibleDirection(wxBOTH);
	gbSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("Symbol Lib"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText2->Wrap(-1);
	gbSizer1->Add(m_staticText2, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL | wxALIGN_CENTER_VERTICAL, 5);

	wxArrayString _choiceSymLibChoices;
	_choiceSymLib = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, _choiceSymLibChoices, 0);
	_choiceSymLib->SetSelection(0);
	gbSizer1->Add(_choiceSymLib, wxGBPosition(0, 1), wxGBSpan(1, 7), wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	_symLibWidget = new wxSymbolLibraryWidget(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE);
	bSizer2->Add(_symLibWidget, 1, wxALL | wxEXPAND, 5);

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer(wxVERTICAL);

	bSizer8->SetMinSize(wxSize(100, -1));
	_markerSymPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxFlexGridSizer* fgSizer211;
	fgSizer211 = new wxFlexGridSizer(0, 2, 0, 0);
	fgSizer211->SetFlexibleDirection(wxBOTH);
	fgSizer211->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText2111 = new wxStaticText(_markerSymPanel, wxID_ANY, wxT("Color"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText2111->Wrap(-1);
	fgSizer211->Add(m_staticText2111, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_cpMarker = new wxColourPickerCtrl(_markerSymPanel, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	fgSizer211->Add(_cpMarker, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

	m_staticText311 = new wxStaticText(_markerSymPanel, wxID_ANY, wxT("Red"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText311->Wrap(-1);
	fgSizer211->Add(m_staticText311, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtMarkerRed = new wxTextCtrl(_markerSymPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer211->Add(_txtMarkerRed, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText411 = new wxStaticText(_markerSymPanel, wxID_ANY, wxT("Green"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText411->Wrap(-1);
	fgSizer211->Add(m_staticText411, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtMarkerGreen = new wxTextCtrl(_markerSymPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer211->Add(_txtMarkerGreen, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText511 = new wxStaticText(_markerSymPanel, wxID_ANY, wxT("Blue"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText511->Wrap(-1);
	fgSizer211->Add(m_staticText511, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtMarkerBlue = new wxTextCtrl(_markerSymPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer211->Add(_txtMarkerBlue, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText611 = new wxStaticText(_markerSymPanel, wxID_ANY, wxT("Alpha"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText611->Wrap(-1);
	fgSizer211->Add(m_staticText611, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_sldMarkerAlpha = new wxSlider(_markerSymPanel, wxID_ANY, 255, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	fgSizer211->Add(_sldMarkerAlpha, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText35 = new wxStaticText(_markerSymPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	m_staticText35->Wrap(-1);
	fgSizer211->Add(m_staticText35, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtMarkerAlpha = new wxTextCtrl(_markerSymPanel, wxID_ANY, wxT("255"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer211->Add(_txtMarkerAlpha, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

	m_staticText171 = new wxStaticText(_markerSymPanel, wxID_ANY, wxT("Width"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText171->Wrap(-1);
	fgSizer211->Add(m_staticText171, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtMarkerWidth = new wxTextCtrl(_markerSymPanel, wxID_ANY, wxT("32"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer211->Add(_txtMarkerWidth, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText30 = new wxStaticText(_markerSymPanel, wxID_ANY, wxT("Height"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText30->Wrap(-1);
	fgSizer211->Add(m_staticText30, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtMarkerHeight = new wxTextCtrl(_markerSymPanel, wxID_ANY, wxT("32"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer211->Add(_txtMarkerHeight, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText312 = new wxStaticText(_markerSymPanel, wxID_ANY, wxT("Line W"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText312->Wrap(-1);
	fgSizer211->Add(m_staticText312, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtMarkerLineWidth = new wxTextCtrl(_markerSymPanel, wxID_ANY, wxT("3"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer211->Add(_txtMarkerLineWidth, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText32 = new wxStaticText(_markerSymPanel, wxID_ANY, wxT("X Off"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText32->Wrap(-1);
	fgSizer211->Add(m_staticText32, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_spMarkerXOff = new wxSpinCtrl(_markerSymPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100, 100, 0);
	fgSizer211->Add(_spMarkerXOff, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText33 = new wxStaticText(_markerSymPanel, wxID_ANY, wxT("Y Off"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText33->Wrap(-1);
	fgSizer211->Add(m_staticText33, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_spMarkerYOff = new wxSpinCtrl(_markerSymPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100, 100, 0);
	fgSizer211->Add(_spMarkerYOff, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);


	_markerSymPanel->SetSizer(fgSizer211);
	_markerSymPanel->Layout();
	fgSizer211->Fit(_markerSymPanel);
	bSizer8->Add(_markerSymPanel, 1, wxEXPAND | wxALL, 5);

	_lineSymPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxFlexGridSizer* fgSizer2111;
	fgSizer2111 = new wxFlexGridSizer(0, 2, 0, 0);
	fgSizer2111->SetFlexibleDirection(wxBOTH);
	fgSizer2111->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText21111 = new wxStaticText(_lineSymPanel, wxID_ANY, wxT("Color"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText21111->Wrap(-1);
	fgSizer2111->Add(m_staticText21111, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_cpLine = new wxColourPickerCtrl(_lineSymPanel, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	fgSizer2111->Add(_cpLine, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

	m_staticText3111 = new wxStaticText(_lineSymPanel, wxID_ANY, wxT("Red"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText3111->Wrap(-1);
	fgSizer2111->Add(m_staticText3111, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtLineRed = new wxTextCtrl(_lineSymPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer2111->Add(_txtLineRed, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText4111 = new wxStaticText(_lineSymPanel, wxID_ANY, wxT("Green"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText4111->Wrap(-1);
	fgSizer2111->Add(m_staticText4111, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtLineGreen = new wxTextCtrl(_lineSymPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer2111->Add(_txtLineGreen, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText5111 = new wxStaticText(_lineSymPanel, wxID_ANY, wxT("Blue"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText5111->Wrap(-1);
	fgSizer2111->Add(m_staticText5111, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtLineBlue = new wxTextCtrl(_lineSymPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer2111->Add(_txtLineBlue, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText6111 = new wxStaticText(_lineSymPanel, wxID_ANY, wxT("Alpha"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText6111->Wrap(-1);
	fgSizer2111->Add(m_staticText6111, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_sldLineAlpha = new wxSlider(_lineSymPanel, wxID_ANY, 255, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	fgSizer2111->Add(_sldLineAlpha, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText351 = new wxStaticText(_lineSymPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	m_staticText351->Wrap(-1);
	fgSizer2111->Add(m_staticText351, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtLineAlpha = new wxTextCtrl(_lineSymPanel, wxID_ANY, wxT("255"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer2111->Add(_txtLineAlpha, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

	m_staticText1711 = new wxStaticText(_lineSymPanel, wxID_ANY, wxT("Width"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1711->Wrap(-1);
	fgSizer2111->Add(m_staticText1711, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtLineWidth = new wxTextCtrl(_lineSymPanel, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer2111->Add(_txtLineWidth, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);


	_lineSymPanel->SetSizer(fgSizer2111);
	_lineSymPanel->Layout();
	fgSizer2111->Fit(_lineSymPanel);
	bSizer8->Add(_lineSymPanel, 1, wxEXPAND | wxALL, 5);

	_fillSymPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxFlexGridSizer* fgSizer2112;
	fgSizer2112 = new wxFlexGridSizer(0, 2, 0, 0);
	fgSizer2112->SetFlexibleDirection(wxBOTH);
	fgSizer2112->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText21112 = new wxStaticText(_fillSymPanel, wxID_ANY, wxT("Color"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText21112->Wrap(-1);
	fgSizer2112->Add(m_staticText21112, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_cpFill = new wxColourPickerCtrl(_fillSymPanel, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	fgSizer2112->Add(_cpFill, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

	m_staticText3112 = new wxStaticText(_fillSymPanel, wxID_ANY, wxT("Red"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText3112->Wrap(-1);
	fgSizer2112->Add(m_staticText3112, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtFillRed = new wxTextCtrl(_fillSymPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer2112->Add(_txtFillRed, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText4112 = new wxStaticText(_fillSymPanel, wxID_ANY, wxT("Green"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText4112->Wrap(-1);
	fgSizer2112->Add(m_staticText4112, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtFillGreen = new wxTextCtrl(_fillSymPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer2112->Add(_txtFillGreen, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText5112 = new wxStaticText(_fillSymPanel, wxID_ANY, wxT("Blue"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText5112->Wrap(-1);
	fgSizer2112->Add(m_staticText5112, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtFillBlue = new wxTextCtrl(_fillSymPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer2112->Add(_txtFillBlue, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText6112 = new wxStaticText(_fillSymPanel, wxID_ANY, wxT("Alpha"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText6112->Wrap(-1);
	fgSizer2112->Add(m_staticText6112, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_sldFillAlpha = new wxSlider(_fillSymPanel, wxID_ANY, 255, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	fgSizer2112->Add(_sldFillAlpha, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

	m_staticText352 = new wxStaticText(_fillSymPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	m_staticText352->Wrap(-1);
	fgSizer2112->Add(m_staticText352, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	_txtFillAlpha = new wxTextCtrl(_fillSymPanel, wxID_ANY, wxT("255"), wxDefaultPosition, wxDefaultSize, 0);
	fgSizer2112->Add(_txtFillAlpha, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);


	_fillSymPanel->SetSizer(fgSizer2112);
	_fillSymPanel->Layout();
	fgSizer2112->Fit(_fillSymPanel);
	bSizer8->Add(_fillSymPanel, 1, wxEXPAND | wxALL, 5);


	bSizer2->Add(bSizer8, 0, wxEXPAND, 5);


	gbSizer1->Add(bSizer2, wxGBPosition(1, 0), wxGBSpan(1, 8), wxEXPAND, 5);


	gbSizer1->AddGrowableCol(2);
	gbSizer1->AddGrowableCol(7);
	gbSizer1->AddGrowableRow(1);

	this->SetSizer(gbSizer1);
	this->Layout();

	_markerSymPanel->Hide();
	_lineSymPanel->Hide();
	_fillSymPanel->Hide();
	if (defaultSymLibType == SymbolLibraryType::Marker)
		_markerSymPanel->Show();
	else if (defaultSymLibType == SymbolLibraryType::Line)
		_lineSymPanel->Show();
	else
		_fillSymPanel->Show();

	Bind(wxEVT_SLIDER, &wxSymbolPickerWidget::_sldAlpha_scroll, this, _sldLineAlpha->GetId());
	Bind(wxEVT_SLIDER, &wxSymbolPickerWidget::_sldAlpha_scroll, this, _sldFillAlpha->GetId());
	Bind(wxEVT_SLIDER, &wxSymbolPickerWidget::_sldAlpha_scroll, this, _sldMarkerAlpha->GetId());

	Bind(wxEVT_COLOURPICKER_CHANGED, &wxSymbolPickerWidget::_colourPicker_changed, this, _cpLine->GetId());
	Bind(wxEVT_COLOURPICKER_CHANGED, &wxSymbolPickerWidget::_colourPicker_changed, this, _cpFill->GetId());
	Bind(wxEVT_COLOURPICKER_CHANGED, &wxSymbolPickerWidget::_colourPicker_changed, this, _cpMarker->GetId());

}

wxSymbolPickerWidget::~wxSymbolPickerWidget()
{
	Unbind(wxEVT_SLIDER, &wxSymbolPickerWidget::_sldAlpha_scroll, this, _sldLineAlpha->GetId());
	Unbind(wxEVT_SLIDER, &wxSymbolPickerWidget::_sldAlpha_scroll, this, _sldFillAlpha->GetId());
	Unbind(wxEVT_SLIDER, &wxSymbolPickerWidget::_sldAlpha_scroll, this, _sldMarkerAlpha->GetId());

	Unbind(wxEVT_COLOURPICKER_CHANGED, &wxSymbolPickerWidget::_colourPicker_changed, this, _cpLine->GetId());
	Unbind(wxEVT_COLOURPICKER_CHANGED, &wxSymbolPickerWidget::_colourPicker_changed, this, _cpFill->GetId());
	Unbind(wxEVT_COLOURPICKER_CHANGED, &wxSymbolPickerWidget::_colourPicker_changed, this, _cpMarker->GetId());


	if (_selSym != nullptr)
		_selSym->Release();
	delete _impl_;
}

void wxSymbolPickerWidget::AddSymbolLibrary(ISymbolLibrary * symLib)
{
	_impl_->_vecSymbolLibrary.push_back(symLib);
	int itemIndex = _choiceSymLib->Append(wxString(symLib->GetName()), symLib);
	if (itemIndex == 0)
	{
		_choiceSymLib->SetSelection(itemIndex);
		_symLibWidget->SetSymbolLibrary(symLib);
		_selSymLib = symLib;

		unsigned char red = (unsigned char)238;
		unsigned char green = (unsigned char)177;
		unsigned char blue = (unsigned char)17;
		unsigned char alpha = (unsigned char)255;

		if (symLib->GetType() == SymbolType::Fill)
		{
			red = (unsigned char)32;
			green = (unsigned char)95;
			blue = (unsigned char)223;
		}
		else if (symLib->GetType() == SymbolType::Marker)
		{
			red = (unsigned char)0;
			green = (unsigned char)0;
			blue = (unsigned char)0;
		}

		_txtMarkerRed->SetValue(wxString::Format(wxT("%d"), red));
		_txtMarkerGreen->SetValue(wxString::Format(wxT("%d"), green));
		_txtMarkerBlue->SetValue(wxString::Format(wxT("%d"), blue));
		_txtMarkerAlpha->SetValue(wxString::Format(wxT("%d"), alpha));
		_cpMarker->SetColour(wxColor(red, green, blue));

		_txtLineRed->SetValue(wxString::Format(wxT("%d"), red));
		_txtLineGreen->SetValue(wxString::Format(wxT("%d"), green));
		_txtLineBlue->SetValue(wxString::Format(wxT("%d"), blue));
		_txtLineAlpha->SetValue(wxString::Format(wxT("%d"), alpha));
		_cpLine->SetColour(wxColor(red, green, blue));

		_txtFillRed->SetValue(wxString::Format(wxT("%d"), red));
		_txtFillGreen->SetValue(wxString::Format(wxT("%d"), green));
		_txtFillBlue->SetValue(wxString::Format(wxT("%d"), blue));
		_txtFillAlpha->SetValue(wxString::Format(wxT("%d"), alpha));
		_cpFill->SetColour(wxColor(red, green, blue));
	}
}

ISymbolLibrary * wxSymbolPickerWidget::GetSelSymbolLibrary()
{
	return _selSymLib;
}

ISymbol * wxSymbolPickerWidget::GetSymbol()
{
	if (_selSym == nullptr
		|| _selSym->GetSymbolLibrary() != _selSymLib
		|| _selSym->GetId() != _symLibWidget->GetSelSymbolId())
	{
		if (_selSym != nullptr)
			_selSym->Release();

		_selSym = _selSymLib->GetSymbol(_symLibWidget->GetSelSymbolId());
	}

	if (_selSym->GetType() == SymbolType::Fill)
	{
		wxString redStr = _txtFillRed->GetValue();
		wxString greenStr = _txtFillGreen->GetValue();
		wxString blueStr = _txtFillBlue->GetValue();
		wxString alphaStr = _txtFillAlpha->GetValue();

		long lred;
		long lgreen;
		long lblue;
		long lalpha;
		redStr.ToLong(&lred);
		greenStr.ToLong(&lgreen);
		blueStr.ToLong(&lblue);
		alphaStr.ToLong(&lalpha);

		unsigned char red = (unsigned char)lred;
		unsigned char green = (unsigned char)lgreen;
		unsigned char blue = (unsigned char)lblue;
		unsigned char alpha = (unsigned char)lalpha;
		
		IFillSymbol* fillSym = (IFillSymbol*)_selSym;

		fillSym->SetColor(red, green, blue, alpha);
		
	}
	else if (_selSym->GetType() == SymbolType::Line)
	{
		wxString redStr = _txtLineRed->GetValue();
		wxString greenStr = _txtLineGreen->GetValue();
		wxString blueStr = _txtLineBlue->GetValue();
		wxString alphaStr = _txtLineAlpha->GetValue();

		long lred;
		long lgreen;
		long lblue;
		long lalpha;
		redStr.ToLong(&lred);
		greenStr.ToLong(&lgreen);
		blueStr.ToLong(&lblue);
		alphaStr.ToLong(&lalpha);

		unsigned char red = (unsigned char)lred;
		unsigned char green = (unsigned char)lgreen;
		unsigned char blue = (unsigned char)lblue;
		unsigned char alpha = (unsigned char)lalpha;

		wxString widthStr = _txtLineWidth->GetValue();
		long lwidth;
		widthStr.ToLong(&lwidth);

		ILineSymbol* lineSym = (ILineSymbol*)_selSym;

		lineSym->SetColor(red, green, blue, alpha);
		lineSym->SetWidth((int)lwidth);
	}
	else
	{
		wxString redStr = _txtMarkerRed->GetValue();
		wxString greenStr = _txtMarkerGreen->GetValue();
		wxString blueStr = _txtMarkerBlue->GetValue();
		wxString alphaStr = _txtMarkerAlpha->GetValue();

		long lred;
		long lgreen;
		long lblue;
		long lalpha;
		redStr.ToLong(&lred);
		greenStr.ToLong(&lgreen);
		blueStr.ToLong(&lblue);
		alphaStr.ToLong(&lalpha);

		unsigned char red = (unsigned char)lred;
		unsigned char green = (unsigned char)lgreen;
		unsigned char blue = (unsigned char)lblue;
		unsigned char alpha = (unsigned char)lalpha;

		wxString widthStr = _txtMarkerWidth->GetValue();
		long lwidth;
		widthStr.ToLong(&lwidth);

		wxString heightStr = _txtMarkerHeight->GetValue();
		long lheight;
		heightStr.ToLong(&lheight);

		wxString lineWidthStr = _txtMarkerLineWidth->GetValue();
		long llineWidth;
		widthStr.ToLong(&llineWidth);

		int xOff = _spMarkerXOff->GetValue();
		int yOff = _spMarkerYOff->GetValue();

		IMarkerSymbol* markerSym = (IMarkerSymbol*)_selSym;

		markerSym->SetColor(red, green, blue, alpha);
		markerSym->SetWidth((int)lwidth);
		markerSym->SetHeight((int)lheight);
		markerSym->SetLineWidth((int)llineWidth);
		markerSym->SetXOffset(xOff);
		markerSym->SetYOffset(yOff);
	}
	return _selSym;
}

void wxSymbolPickerWidget::SetSymbol(ISymbol * sym)
{
	if (sym == nullptr)
		return;

	const ISymbolLibrary* symLib = sym->GetSymbolLibrary();
	SymbolType symType = sym->GetType();

	ISymbolLibrary* symLibFind = nullptr;
	int itemIndex = 0;
	for (auto it = _impl_->_vecSymbolLibrary.begin(); it != _impl_->_vecSymbolLibrary.end(); it++)
	{
		ISymbolLibrary* symLibIn = *it;
		if (symLib == symLibIn 
			|| (symLib == nullptr && symLibIn->GetType() == symType))
		{
			symLibFind = symLibIn;
			break;
		}
		itemIndex++;
	}

	if (symLibFind == nullptr)
		return;

	_selSymLib = symLibFind;

	if (_selSym != sym)
	{
		if (_selSym != nullptr)
			_selSym->Release();
		_selSym = sym;
		if (_selSym != nullptr)
			_selSym->Reference();
	}

	_choiceSymLib->SetSelection(itemIndex);
	_symLibWidget->SetSymbolLibrary(symLibFind);
	_symLibWidget->SetSelSymbolId(_selSym->GetId());

	if (_selSym->GetType() == SymbolType::Fill)
	{
		IFillSymbol* fillSym = (IFillSymbol*)_selSym;
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha;

		fillSym->GetColor(&red, &green, &blue, &alpha);

		_txtMarkerRed->SetValue(wxString::Format(wxT("%d"), red));
		_txtMarkerGreen->SetValue(wxString::Format(wxT("%d"), green));
		_txtMarkerBlue->SetValue(wxString::Format(wxT("%d"), blue));
		_txtMarkerAlpha->SetValue(wxString::Format(wxT("%d"), alpha));
		_cpMarker->SetColour(wxColor(red, green, blue));

		_txtLineRed->SetValue(wxString::Format(wxT("%d"), red));
		_txtLineGreen->SetValue(wxString::Format(wxT("%d"), green));
		_txtLineBlue->SetValue(wxString::Format(wxT("%d"), blue));
		_txtLineAlpha->SetValue(wxString::Format(wxT("%d"), alpha));
		_cpLine->SetColour(wxColor(red, green, blue));

		_txtFillRed->SetValue(wxString::Format(wxT("%d"), red));
		_txtFillGreen->SetValue(wxString::Format(wxT("%d"), green));
		_txtFillBlue->SetValue(wxString::Format(wxT("%d"), blue));
		_txtFillAlpha->SetValue(wxString::Format(wxT("%d"), alpha));
		_cpFill->SetColour(wxColor(red, green, blue));
	}
	else if (_selSym->GetType() == SymbolType::Line)
	{
		ILineSymbol* lineSym = (ILineSymbol*)_selSym;

		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha;

		lineSym->GetColor(&red, &green, &blue, &alpha);

		_txtMarkerRed->SetValue(wxString::Format(wxT("%d"), red));
		_txtMarkerGreen->SetValue(wxString::Format(wxT("%d"), green));
		_txtMarkerBlue->SetValue(wxString::Format(wxT("%d"), blue));
		_txtMarkerAlpha->SetValue(wxString::Format(wxT("%d"), alpha));
		_cpMarker->SetColour(wxColor(red, green, blue));

		_txtLineRed->SetValue(wxString::Format(wxT("%d"), red));
		_txtLineGreen->SetValue(wxString::Format(wxT("%d"), green));
		_txtLineBlue->SetValue(wxString::Format(wxT("%d"), blue));
		_txtLineAlpha->SetValue(wxString::Format(wxT("%d"), alpha));
		_cpLine->SetColour(wxColor(red, green, blue));

		_txtFillRed->SetValue(wxString::Format(wxT("%d"), red));
		_txtFillGreen->SetValue(wxString::Format(wxT("%d"), green));
		_txtFillBlue->SetValue(wxString::Format(wxT("%d"), blue));
		_txtFillAlpha->SetValue(wxString::Format(wxT("%d"), alpha));
		_cpFill->SetColour(wxColor(red, green, blue));

		int width = lineSym->GetWidth();
		_txtLineWidth->SetValue(wxString::Format(wxT("%d"), width));
	}
	else
	{
		IMarkerSymbol* markerSym = (IMarkerSymbol*)_selSym;

		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha;

		markerSym->GetColor(&red, &green, &blue, &alpha);

		_txtMarkerRed->SetValue(wxString::Format(wxT("%d"), red));
		_txtMarkerGreen->SetValue(wxString::Format(wxT("%d"), green));
		_txtMarkerBlue->SetValue(wxString::Format(wxT("%d"), blue));
		_txtMarkerAlpha->SetValue(wxString::Format(wxT("%d"), alpha));
		_cpMarker->SetColour(wxColor(red, green, blue));

		_txtLineRed->SetValue(wxString::Format(wxT("%d"), red));
		_txtLineGreen->SetValue(wxString::Format(wxT("%d"), green));
		_txtLineBlue->SetValue(wxString::Format(wxT("%d"), blue));
		_txtLineAlpha->SetValue(wxString::Format(wxT("%d"), alpha));
		_cpLine->SetColour(wxColor(red, green, blue));

		_txtFillRed->SetValue(wxString::Format(wxT("%d"), red));
		_txtFillGreen->SetValue(wxString::Format(wxT("%d"), green));
		_txtFillBlue->SetValue(wxString::Format(wxT("%d"), blue));
		_txtFillAlpha->SetValue(wxString::Format(wxT("%d"), alpha));
		_cpFill->SetColour(wxColor(red, green, blue));

		int width = markerSym->GetWidth();
		_txtMarkerWidth->SetValue(wxString::Format(wxT("%d"), width));
		int height = markerSym->GetHeight();
		_txtMarkerHeight->SetValue(wxString::Format(wxT("%d"), height));
		int lineWidth = markerSym->GetLineWidth();
		_txtMarkerLineWidth->SetValue(wxString::Format(wxT("%d"), lineWidth));
		int xOff = markerSym->GetXOffset();
		_spMarkerXOff->SetValue(xOff);
		int yOff = markerSym->GetYOffset();
		_spMarkerYOff->SetValue(yOff);
	}
}

void wxSymbolPickerWidget::_choiceSymLib_selected(wxCommandEvent & event)
{
	_selSymLib = nullptr;
	int sel = event.GetSelection();
	if (sel < 0)
		return;
	_selSymLib = (ISymbolLibrary*)_choiceSymLib->GetClientData(sel);
	_symLibWidget->SetSymbolLibrary(_selSymLib);
	if (_selSymLib->GetType() == SymbolLibraryType::Marker)
	{
		_markerSymPanel->Show();
		_lineSymPanel->Hide();
		_fillSymPanel->Hide();
	}
	else if (_selSymLib->GetType() == SymbolLibraryType::Line)
	{
		_markerSymPanel->Hide();
		_lineSymPanel->Show();
		_fillSymPanel->Hide();
	}
	else
	{
		_markerSymPanel->Hide();
		_lineSymPanel->Hide();
		_fillSymPanel->Show();
	}
}

void wxSymbolPickerWidget::_sldAlpha_scroll(wxCommandEvent & event)
{
	_txtLineAlpha->SetValue(wxString::Format(wxT("%d"), event.GetInt()));
	_txtFillAlpha->SetValue(wxString::Format(wxT("%d"), event.GetInt()));
	_txtMarkerAlpha->SetValue(wxString::Format(wxT("%d"), event.GetInt()));
}

void wxSymbolPickerWidget::_colourPicker_changed(wxColourPickerEvent & event)
{
	wxColor color = event.GetColour();
	_txtLineRed->SetValue(wxString::Format(wxT("%d"), color.Red()));
	_txtLineGreen->SetValue(wxString::Format(wxT("%d"), color.Green()));
	_txtLineBlue->SetValue(wxString::Format(wxT("%d"), color.Blue()));

	_txtFillRed->SetValue(wxString::Format(wxT("%d"), color.Red()));
	_txtFillGreen->SetValue(wxString::Format(wxT("%d"), color.Green()));
	_txtFillBlue->SetValue(wxString::Format(wxT("%d"), color.Blue()));

	_txtMarkerRed->SetValue(wxString::Format(wxT("%d"), color.Red()));
	_txtMarkerGreen->SetValue(wxString::Format(wxT("%d"), color.Green()));
	_txtMarkerBlue->SetValue(wxString::Format(wxT("%d"), color.Blue()));
}


END_NAME_SPACE(tGis, Gui)
