#pragma once

#ifndef __WX_LAYERRENDERPANEL_H__
#define __WX_LAYERRENDERPANEL_H__


#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/panel.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;

struct ILayerRenderCtrl;
class RgbLayerRenderCtrl;
class GrayScaleLayerRenderCtrl;

BEGIN_NAME_SPACE(tGis, Gui)

class wxLayerRenderPanelImpl;

class TGIS_GUI_API wxLayerRenderPanel : public wxPanel
{
public:
	wxLayerRenderPanel(ILayer* layer, wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(638, 613), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
	~wxLayerRenderPanel();

public:
	void UpdateLayerRender();

private:
	ILayer* _layer;
	IDataset* _dataset;

private:
	bool SelectRenderCtrl(ILayerRenderCtrl* renderCtrl, bool selected);

protected:
	wxChoice* _choiceRander;
	ILayerRenderCtrl* _selLayerRenderCtrl;

	RgbLayerRenderCtrl* _rgbRenderCtrl;
	GrayScaleLayerRenderCtrl* _grayScaleRenderCtrl;

private:
	void _choiceRander_selected(wxCommandEvent& event);

private:
	wxLayerRenderPanelImpl* _impl_;
};

END_NAME_SPACE(tGis, Gui)

#endif
