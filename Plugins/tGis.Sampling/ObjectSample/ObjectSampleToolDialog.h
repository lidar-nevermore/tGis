#pragma once

#ifndef __OBJECTSAMPLETOOLDIALOG_H__
#define __OBJECTSAMPLETOOLDIALOG_H__

#include <QWidget>

#include "ui_ObjectSampleToolDialog.h"

#include "tEntity.h"
#include "tOrganizer.h"
#include "tUtility.h"
#include "tVisualize.h"
#include "TakeObjectSampleTool.h"

using namespace tGis::Core;




class ObjectSampleToolDialog : public QDialog
{
public:
	ObjectSampleToolDialog(IMapWidget* mapWidget, QWidget *parent = 0);
	~ObjectSampleToolDialog();

private:
	IMapWidget* _mapWidget;
	ObjectSampleDataSource* _selectedSamples;
	ILayer* _selectedLayer;
	TakeObjectSampleTool _takeObjectSampleTool;

private:
	void MapToolAddedOrChanged(IMapWidget*, IMapTool*);
	void LayerAdded(IMapPtr, ILayerPtr);
	void LayerRemoved(IMapPtr, ILayerPtr);
	void LayerCleared(IMapPtr);
	void BeforeMapWidgetDestory(IMapWidget*);

	void AfterDataSourceConnect(IDataSourceProvider*, IDataSource*);
	void BeforeDataSourceDisconnect(IDataSourceProvider*, IDataSource*);

public:
	void UpdateChoise();

private:
	void closeEvent(QCloseEvent *event);

private slots :
	void on_chkRect_toggled(bool checked);
	void on_cboSamples_currentIndexChanged(int index);
	void on_cboLayer_currentIndexChanged(int index);

private:
	Ui::ObjectSampleToolDialog ui;
};




#endif


