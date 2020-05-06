#include <wx/wx.h>
#include "StandaloneToolDialog.h"
#include "wxTGisApplication.h"

using namespace tGis::Gui;

StandaloneToolDialog::StandaloneToolDialog( wxWindow* parent, ToolKit* kit)
	: StandaloneToolDialogBase( parent )
	, _tool("")
{
	if (parent == nullptr)
		SetParent(((wxTGisApplication*)tGisApplication::INSTANCE())->GetMainFrame());

	wxString strToolBelong;
	ToolKit* parentKit = kit;
	while (nullptr != parentKit)
	{
		strToolBelong.insert(0, parentKit->GetName());
		strToolBelong.insert(0, "/");
		parentKit = parentKit->GetParent();
	}

	_txtBelong->SetValue(strToolBelong);
	_btnDelParam->Enable(false);

	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &StandaloneToolDialog::_btnAddParam_Click, this, _btnAddParam->GetId());
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &StandaloneToolDialog::_btnDelParam_Click, this, _btnDelParam->GetId());
	Bind(wxEVT_LISTBOX, &StandaloneToolDialog::_lstParam_Selection, this, _lstParam->GetId());
	Bind(wxEVT_BUTTON, &StandaloneToolDialog::_sdbSzOK_Clicked, this, _sdbSzOK->GetId());

}

StandaloneToolDialog::~StandaloneToolDialog()
{
	Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &StandaloneToolDialog::_btnAddParam_Click, this, _btnAddParam->GetId());
	Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &StandaloneToolDialog::_btnDelParam_Click, this, _btnDelParam->GetId());
	Unbind(wxEVT_LISTBOX, &StandaloneToolDialog::_lstParam_Selection, this, _lstParam->GetId());
	Unbind(wxEVT_BUTTON, &StandaloneToolDialog::_sdbSzOK_Clicked, this, _sdbSzOK->GetId());

}

void StandaloneToolDialog::_btnAddParam_Click(wxCommandEvent & event)
{
	wxString param = _txtParam->GetValue();
	if (!param.IsEmpty())
	{
		_lstParam->Append(param);
	}
}

void StandaloneToolDialog::_btnDelParam_Click(wxCommandEvent & event)
{
	wxArrayInt sels;
	_lstParam->GetSelections(sels);

	for (auto it = sels.rbegin(); it != sels.rend(); it++)
	{
		_lstParam->Delete(*it);
	}
}

void StandaloneToolDialog::_lstParam_Selection(wxCommandEvent & event)
{
	wxArrayInt sels;
	_lstParam->GetSelections(sels);
	if (sels.Count() > 0)
		_btnDelParam->Enable(true);
	else
		_btnDelParam->Enable(false);
}

void StandaloneToolDialog::_sdbSzOK_Clicked(wxCommandEvent & event)
{
	wxString strBelong = _txtBelong->GetValue();
	wxString strExeFile = _fpExeFile->GetPath();
	wxString strName = _txtName->GetValue();

	if (strBelong.IsEmpty() || strExeFile.IsEmpty() || strName.IsEmpty())
	{
		wxMessageBox(wxT("Belong and Exe File can not be empty!"));
	}
	else
	{
		_tool = StandaloneTool(strName.c_str());
		wxString tGisExe = tGisApplication::INSTANCE()->GetExeDir();
		if (strExeFile.StartsWith(tGisExe))
		{
			strExeFile.Replace(tGisExe, wxT("$(tGis)"));
		}
		_tool.SetExeFile(strExeFile.c_str());

		unsigned int paramCount = _lstParam->GetCount();
		for (unsigned int i = 0; i < paramCount; i++)
		{
			wxString strParam = _lstParam->GetString(i);
			_tool.AddParam(strParam.c_str());
		}

		event.SetId(wxID_OK);
		event.Skip();
	}
}
