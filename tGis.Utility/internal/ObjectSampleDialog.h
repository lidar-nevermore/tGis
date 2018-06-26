#pragma once

#ifndef __OBJECTSAMPLEDIALOG_H__
#define __OBJECTSAMPLEDIALOG_H__

#include <QDialog>
#include <QListWidgetItem>

#include "ui_ObjectSampleDialog.h"
#include "QMapWidget.h"

#include "tEntity.h"
#include "tOrganizer.h"
#include "Event.h"


class QStandardItem;
class QStandardItemModel;

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Utility)

class ObjectSampleDialog : public QDialog
{
public:
	ObjectSampleDialog(QWidget *parent = 0);
	~ObjectSampleDialog();

public:

	inline void SetLayer(ILayer* layer)
	{
		_layer = layer;
	}

	inline void SetSampleArea(double left, double top, double right, double bottom)
	{
		_sampleAreaLeft = left;
		_sampleAreaTop = top;
		_sampleAreaRight = right;
		_sampleAreaBottom = bottom;
	}

	void SetObjectSampleDataSource(ObjectSampleDataSource* samples);

	inline ObjectSampleMetadata* GetObjectSampleMetadata()
	{
		return _osm;
	}

private:
	//节点关联的数据
	static const int DataRole = Qt::UserRole + 1;
	QListWidgetItem* CreateObjectSampleItem(ObjectSampleMetadata* osm);

private:
	ObjectSampleDataSource* _samples;
	ObjectSampleMetadata* _osm;
	Map _map;
	ILayer* _layer;
	double _sampleAreaLeft;
	double _sampleAreaTop;
	double _sampleAreaRight;
	double _sampleAreaBottom;
	OverlayRect _sampleRect;
	EventHandler<ObjectSampleDialog, IMapWidget*, int, int> _MapWidgetLoadedEventHandler;

private:
	void OnMapWidgetLoaded(IMapWidget*, int, int);

private slots:
    void on_btnNewClass_clicked(bool checked);
	void on_lstClass_itemSelectionChanged();

private:
	Ui::ObjectSampleDialog ui;
};

END_NAME_SPACE(tGis, Utility)


#endif



