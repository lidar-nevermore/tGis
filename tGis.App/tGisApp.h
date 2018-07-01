#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_tGisApp.h"

#include "tVisualize.h"

#include "Event.h"

#include "MapPanTool.h"
#include "MapZoomTool.h"
#include "DrawRectTool.h"
#include "RectZoomTool.h"

using namespace tGis::Core;
using namespace tGis::Utility;

class QLayerWidget;

class tGisApp : public QMainWindow
{
	Q_OBJECT

public:
	tGisApp(QWidget *parent = Q_NULLPTR);
	~tGisApp();

private:
	Map _map;
	MapPanTool _mapPanTool;
	MapZoomTool _mapZoomTool;
	RectZoomTool _rectZoomTool;

private:
	MemberEventHandler<tGisApp, IDataSourceProvider*, IDataset*> _AfterDatasetOpenEventHandler;
	MemberEventHandler<tGisApp, IDataSourceProvider*, IDataset*> _BeforeDatasetCloseEventHandler;
	void AfterDatasetOpen(IDataSourceProvider*, IDataset*);
	void BeforeDatasetClose(IDataSourceProvider*, IDataset*);

	void AfterDataSourceConnect(IDataSourceProvider*, IDataSource*);
	void BeforeDataSourceDisconnect(IDataSourceProvider*, IDataSource*);

	void LayerAdded(IMapPtr, ILayerPtr);
	void LayerRemoved(IMapPtr, ILayerPtr);
	void LayerCleared(IMapPtr);

private:
	tGis::Core::ObjectSampleDataSource* _selectedObjectSampleDataSource;
	RasterLayer* _selectedRasterLayer;
	IDataSource* _selectedDataSource;
	IDataset* _selectedDataset;

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
	void on_rasterSubAreaAction_toggled(bool checked);
	void on_showGridAction_toggled(bool checked);
	void on_closeDatasetAction_triggered(bool checked);
	void on_refreshDataSourceAction_triggered(bool checked);

	void on_layerWidget_LayerSelectionChanged(IMapPtr map, ILayerPtr layer, ILayerProviderPtr provider);
	void on_layerWidget_LayerVisibleChanged(IMapPtr map, ILayerPtr layer, ILayerProviderPtr provider);
	void on_dataSourceWidget_SelectionChanged(IDataSourcePtr ds, IDatasetPtr dt, IDataSourceProviderPtr provider);
	void on_openedDatasetWidget_SelectionChanged(IDataSourcePtr ds, IDatasetPtr dt, IDataSourceProviderPtr provider);

private:
	Ui::tGisAppClass ui;
};
