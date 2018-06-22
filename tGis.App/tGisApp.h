#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_tGisApp.h"

#include "tVisualize.h"

#include "MapPanTool.h"
#include "MapZoomTool.h"
#include "MyMap.h"
#include "DrawRectTool.h"
#include "RectZoomTool.h"
#include "TakeObjectSampleTool.h"

using namespace tGis::Core;
using namespace tGis::Utility;

class tGisApp : public QMainWindow
{
	Q_OBJECT

public:
	tGisApp(QWidget *parent = Q_NULLPTR);

private:
	MyMap _map;
	MapPanTool _mapPanTool;
	MapZoomTool _mapZoomTool;
	RectZoomTool _rectZoomTool;
	TakeObjectSampleTool _takeObjectSampleTool;

private slots:
	void on_zoomInAction_triggered(bool checked);
	void on_zoomOutAction_triggered(bool checked);
	void on_entireMapAction_triggered(bool checked);
	void on_refreshMapAction_triggered(bool checked);
	void on_zoomFreeAction_toggled(bool checked);
	void on_zoomRectAction_toggled(bool checked);
	void on_panAction_toggled(bool checked);
	void on_zoomLayerAction_triggered(bool checked);
	void on_zoomOriginalAction_triggered(bool checked);
	void on_removeLayerAction_triggered(bool checked);
	void on_layerVisibleAction_toggled(bool checked);
	void on_layerAttributeAction_triggered(bool checked);
	void on_removeAllLayersAction_triggered(bool checked);
	void on_layerUpAction_triggered(bool checked);
	void on_layerTopAction_triggered(bool checked);
	void on_layerDownAction_triggered(bool checked);
	void on_layerBottomAction_triggered(bool checked);
	void on_takeObjectSampleAction_toggled(bool checked);

	void on_layerWidget_LayerSelectionChanged(IMapPtr map, ILayerPtr layer, ILayerProviderPtr provider);
	void on_layerWidget_LayerVisibleChanged(IMapPtr map, ILayerPtr layer, ILayerProviderPtr provider);

private:
	Ui::tGisAppClass ui;
};
