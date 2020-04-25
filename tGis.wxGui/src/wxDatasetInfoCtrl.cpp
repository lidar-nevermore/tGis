#include "wxDatasetInfoCtrl.h"

#include "gdalinfo.inl"
#include "ogrinfo.inl"

wxDECLARE_EVENT(wxEVT_DATASET_INFO_COMPLETED, wxCommandEvent);

wxDEFINE_EVENT(wxEVT_DATASET_INFO_COMPLETED, wxCommandEvent);

BEGIN_NAME_SPACE(tGis, Gui)

class wxDatasetInfoCtrlImpl
{
public:
	wxDatasetInfoCtrlImpl(wxDatasetInfoCtrl* owner, IDataset* dataset)
	{
		_owner = owner;
		_dataset = dataset;
	}

	wxDatasetInfoCtrl* _owner;
	IDataset* _dataset;
	wxString _info;
	bool _infoShowing;
};


wxDatasetInfoCtrl::wxDatasetInfoCtrl(IDataset * dataset, wxWindow * parent, wxWindowID id, const wxString & value, const wxPoint & pos, const wxSize & size, long style, const wxValidator & validator, const wxString & name)
	: wxTextCtrl(parent, id, value, pos, size, style, validator, name)
{
	_impl_ = new wxDatasetInfoCtrlImpl(this, dataset);
	_impl_->_infoShowing = false;

	Bind(wxEVT_SIZE, &wxDatasetInfoCtrl::OnSize, this, this->GetId());
}

wxDatasetInfoCtrl::~wxDatasetInfoCtrl()
{
	Unbind(wxEVT_SIZE, &wxDatasetInfoCtrl::OnSize, this, this->GetId());
	delete _impl_;
}

void wxDatasetInfoCtrl::OnDatasetInfo(wxCommandEvent & event)
{
	SetValue(_impl_->_info);

	_impl_->_infoShowing = true;
}

class MyThread : public wxThread
{
public:
	MyThread(wxDatasetInfoCtrl *w, wxDatasetInfoCtrlImpl* wimpl)
	{
		_handler = w;
		_handler_impl_ = wimpl;
	};
	virtual wxThread::ExitCode Entry()
	{
		GDALDataset* dataset = (dynamic_cast<MyGDALDataset*>(_handler_impl_->_dataset))->GetGDALDataset();

		if (_handler_impl_->_dataset->IsTypeOf(MyGDALRasterDataset::S_GetType()))
		{
			GDALInfoOptions opt;

			opt.bComputeMinMax = FALSE;
			opt.bReportHistograms = FALSE;
			opt.bReportProj4 = TRUE;
			opt.bStats = FALSE;
			opt.bApproxStats = TRUE;
			opt.bSample = FALSE;
			opt.bComputeChecksum = FALSE;
			opt.bShowGCPs = TRUE;
			opt.bShowMetadata = TRUE;
			opt.bShowRAT = TRUE;
			opt.bShowColorTable = TRUE;
			opt.bListMDD = FALSE;
			opt.bShowFileList = TRUE;
			opt.papszExtraMDDomains = nullptr;
			opt.bStdoutOutput = FALSE;

			GDALInfo(dataset, &opt, _handler_impl_->_info);
		}
		else
		{
			OGRInfo(dataset, _handler_impl_->_info);
		}
		// signal the event handler that this thread is going to be destroyed
		// NOTE: here we assume that using the m_pHandler pointer is safe,
		//       (in this case this is assured by the MyFrame destructor)
		wxQueueEvent(_handler, new wxCommandEvent(wxEVT_DATASET_INFO_COMPLETED));
		return (wxThread::ExitCode)0;     // success
	}

	wxDatasetInfoCtrl* _handler;
	wxDatasetInfoCtrlImpl* _handler_impl_;
};

void wxDatasetInfoCtrl::OnSize(wxSizeEvent & event)
{
	event.Skip();

	if (_impl_->_dataset == nullptr)
		return;

	if (_impl_->_infoShowing)
		return;

	MyThread* pThread = new MyThread(this, _impl_);
	if (pThread->Run() != wxTHREAD_NO_ERROR)
	{
		wxLogError("Can't create the thread!");
		delete pThread;
	}
}

wxBEGIN_EVENT_TABLE(wxDatasetInfoCtrl, wxTextCtrl)
    EVT_COMMAND(wxID_ANY, wxEVT_DATASET_INFO_COMPLETED, wxDatasetInfoCtrl::OnDatasetInfo)
wxEND_EVENT_TABLE()


END_NAME_SPACE(tGis, Gui)

