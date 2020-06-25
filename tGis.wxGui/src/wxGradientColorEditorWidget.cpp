#include <wx/wx.h>
#include <wx/progdlg.h>

#include "wxGradientColorEditorWidget.h"

#include "wxGradientColorWidget.h"
#include "wxDataViewColorRenderer.h"
#include "GradientColor.h"

#include "PaletteColorDialog.h"

extern "C"
{
#include "bi_list.h"
}

BEGIN_NAME_SPACE(tGis, Gui)

struct ColorNode
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	double pos;
};

class wxGradientColorEditorWidgetImpl
{
public:
	wxGradientColorEditorWidgetImpl(wxGradientColorEditorWidget* owner)
	{
		_owner = owner;
		bi_list_init(&_colors, sizeof(ColorNode), malloc, free, nullptr, nullptr);
	}

	~wxGradientColorEditorWidgetImpl()
	{
		bi_list_release(&_colors);
	}

	wxGradientColorEditorWidget* _owner;

	BI_LIST _colors;
};

wxGradientColorEditorWidget::wxGradientColorEditorWidget(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) : wxPanel(parent, id, pos, size, style, name)
{
	_impl_ = new wxGradientColorEditorWidgetImpl(this);
	_grdColor = nullptr;

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);


	bSizer2->Add(0, 0, 1, wxEXPAND, 5);

	_btnAdd = new wxButton(this, wxID_ANY, wxT("Add Key Color"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(_btnAdd, 0, wxALL, 5);

	_btnEdit = new wxButton(this, wxID_ANY, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(_btnEdit, 0, wxALL, 5);

	_btnDelete = new wxButton(this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(_btnDelete, 0, wxALL, 5);


	bSizer1->Add(bSizer2, 0, wxEXPAND, 5);

	_dvlKeyColor = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	bSizer1->Add(_dvlKeyColor, 1, wxALL | wxEXPAND, 5);

	_wxGradientColor = new wxGradientColorWidget(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	_wxGradientColor->SetMinSize(wxSize(-1, 20));
	((wxWindowBase*)_wxGradientColor)->SetMaxSize(wxSize(-1, 20));

	bSizer1->Add(_wxGradientColor, 0, wxEXPAND | wxALL, 5);

	wxDataViewColumn* col0 = _dvlKeyColor->AppendTextColumn(wxT("Positon"));
	col0->SetSortable(true);
	col0->SetWidth(115);
	wxDataViewColorRenderer* render = new wxDataViewColorRenderer();
	wxDataViewColumn* col1 = new wxDataViewColumn("Color", render, 1);
	_dvlKeyColor->AppendColumn(col1);
	col1->SetWidth(115);

	this->SetSizer(bSizer1);
	this->Layout();

	Bind(wxEVT_BUTTON, &wxGradientColorEditorWidget::_btnDelete_clicked, this, _btnDelete->GetId());
	Bind(wxEVT_BUTTON, &wxGradientColorEditorWidget::_btnEdit_clicked, this, _btnEdit->GetId());
	Bind(wxEVT_BUTTON, &wxGradientColorEditorWidget::_btnAdd_clicked, this, _btnAdd->GetId());

}

wxGradientColorEditorWidget::~wxGradientColorEditorWidget()
{
	Unbind(wxEVT_BUTTON, &wxGradientColorEditorWidget::_btnDelete_clicked, this, _btnDelete->GetId());
	Unbind(wxEVT_BUTTON, &wxGradientColorEditorWidget::_btnEdit_clicked, this, _btnEdit->GetId());
	Unbind(wxEVT_BUTTON, &wxGradientColorEditorWidget::_btnAdd_clicked, this, _btnAdd->GetId());

	if (_grdColor != nullptr)
		_grdColor->Release();
	delete _impl_;
}

void wxGradientColorEditorWidget::SetGradientColor(GradientColor * color)
{
	color->Reference();
	_wxGradientColor->SetGradientColor(color);
	if (_grdColor != nullptr)
		_grdColor->Release();
	_grdColor = color;

	size_t cc = _grdColor->GetKeyColorCount();
	for (size_t i = 0; i < cc; i++)
	{
		double pos;
		unsigned char r, g, b;
		_grdColor->GetKeyColor(i, &r, &g, &b, &pos);

		ColorNode* cn = (ColorNode*)bi_list_insert_tail(&_impl_->_colors, nullptr);
		cn->r = r;
		cn->g = g;
		cn->b = b;
		cn->pos = pos;

		wxVector<wxVariant> data;
		data.push_back(wxVariant(pos));
		long clr = r | g << 8 | b << 16;
		data.push_back(wxVariant(clr));
		_dvlKeyColor->AppendItem(data,(wxUIntPtr)(_impl_->_colors.tail));
	}
}

GradientColor * wxGradientColorEditorWidget::GetGradientColor()
{
	_grdColor->ClearKeyColor();

	void* pos = bi_list_head_pos(&_impl_->_colors);
	while (pos != nullptr)
	{
		ColorNode* cn = (ColorNode*)bi_list_at_pos(&_impl_->_colors, pos, nullptr);
		_grdColor->AddKeyColor(cn->r, cn->g, cn->b, cn->pos);
		pos = bi_list_next_pos(&_impl_->_colors, pos);
	}
	return _grdColor;
}

void wxGradientColorEditorWidget::_btnAdd_clicked(wxCommandEvent & event)
{
	PaletteColorDialog dlg(this);
	double pos = 0;
	unsigned char r, g, b;
	Palette::GenerateColor(&r, &g, &b);
	dlg.SetKeyColor(pos, r, g, b);
	if (dlg.ShowModal() == wxID_OK)
	{
		double maxPos = -DBL_MAX;
		if (_impl_->_colors.tail != nullptr)
			maxPos = ((ColorNode*)_impl_->_colors.tail->data)->pos;

		if (dlg.GetKeyColor(&pos, &r, &g, &b) && pos > maxPos)
		{
			_grdColor->AddKeyColor(r, g, b, pos);
			_wxGradientColor->Refresh();

			ColorNode* cn = (ColorNode*)bi_list_insert_tail(&_impl_->_colors, nullptr);
			cn->r = r;
			cn->g = g;
			cn->b = b;
			cn->pos = pos;

			wxVector<wxVariant> data;
			data.push_back(wxVariant(pos));
			long clr = r | g << 8 | b << 16;
			data.push_back(wxVariant(clr));
			_dvlKeyColor->AppendItem(data, (wxUIntPtr)(_impl_->_colors.tail));
		}
		else
		{
			wxMessageBox(wxT("Invalid key color position!"));
		}
	}
}

void wxGradientColorEditorWidget::_btnEdit_clicked(wxCommandEvent & event)
{
	if (_dvlKeyColor->GetSelectedItemsCount() == 1)
	{
		wxDataViewItem item = _dvlKeyColor->GetSelection();
		BI_NODE* itr = (BI_NODE*)(_dvlKeyColor->GetItemData(item));
		double pos;
		unsigned char r, g, b;
		ColorNode* cn = (ColorNode*)bi_list_at_pos(&_impl_->_colors, itr, nullptr);
		pos = cn->pos;
		r = cn->r;
		g = cn->g;
		b = cn->b;

		PaletteColorDialog dlg(this);
		dlg.SetKeyColor(pos, r, g, b);
		if (dlg.ShowModal() == wxID_OK)
		{
			bool flag = false;
			if (dlg.GetKeyColor(&pos, &r, &g, &b))
			{
				double prevPos = -DBL_MAX;
				double nextPos = DBL_MAX;

				if (itr->prev != nullptr)
					prevPos = ((ColorNode*)(itr->prev->data))->pos;
				if (itr->next != nullptr)
					nextPos = ((ColorNode*)(itr->next->data))->pos;

				if (prevPos < pos && pos < nextPos)
				{
					flag = true;
					
					cn->pos = pos;
					cn->r = r;
					cn->g = g;
					cn->b = b;

					GetGradientColor();

					_wxGradientColor->Refresh();
					int row = _dvlKeyColor->ItemToRow(item);
					_dvlKeyColor->SetValue(wxVariant(pos), row, 0);
					long clr = r | g << 8 | b << 16;
					_dvlKeyColor->SetValue(wxVariant(clr), row, 1);
				}
			}
			
			if(flag == false)
			{
				wxMessageBox(wxT("Invalid key color position!"));
			}
		}
	}
}

static int CompareInt(int* first, int *second)
{
	if (*first > *second)
		return -1;
	else if (*first == *second)
		return 0;
	else return 1;
}

void wxGradientColorEditorWidget::_btnDelete_clicked(wxCommandEvent & event)
{
	wxArrayInt rows;
	wxDataViewItemArray sels;
	_dvlKeyColor->GetSelections(sels);
	for (wxDataViewItemArray::iterator it = sels.begin(); it != sels.end(); it++)
	{
		wxDataViewItem item = *it;
		int row = _dvlKeyColor->ItemToRow(item);
		rows.push_back(row);

		BI_NODE* itr = (BI_NODE*)(_dvlKeyColor->GetItemData(item));
		bi_list_delete_at(&_impl_->_colors, itr);
	}

	GetGradientColor();
	_wxGradientColor->Refresh();

	rows.Sort(CompareInt);

	for (wxArrayInt::iterator it = rows.begin(); it != rows.end(); it++)
	{
		int row = *it;
		_dvlKeyColor->DeleteItem(row);
	}
}


END_NAME_SPACE(tGis, Gui)
