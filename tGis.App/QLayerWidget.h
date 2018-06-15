#pragma once
#include <QListView>
#include <QMetaType>

class QStandardItem;
class QStandardItemModel;

#include "tOrganizer.h"
#include "tGisMetaType.h"


using namespace tGis::Core;


class QLayerWidget : public QListView
{
	Q_OBJECT
public:
	explicit QLayerWidget(QWidget *parent = 0);
	~QLayerWidget();

signals:
	void LayerSelectionChanged(IMapPtr, ILayerPtr, ILayerProviderPtr);
signals:
	void LayerVisibleChanged(IMapPtr, ILayerPtr, ILayerProviderPtr);

private:
	static const int LayerPtrRole = Qt::UserRole + 1;
	static const int LayerProviderRole = Qt::UserRole + 2;

private:
	QStandardItemModel* _model;
	IMapPtr _map;
	IMapWidgetPtr _mapWidget;

public:
	void SetMap(IMapPtr map);
	IMapPtr GetMap();

	void SetMapWidget(IMapWidgetPtr mapWidget);
	IMapWidgetPtr GetMapWidget();

	ILayerPtr GetSelectedLayer();

	void SelectedLayerPropertyUI();
	void RemoveSelectedLayer();
	void RemoveAllLayers();
	void SetSelectedLayerVisible(bool visible);

protected:
	virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

public slots:
	void LayerAdded(IMapPtr, ILayerPtr, ILayerProviderPtr);
	void LayerClicked(const QModelIndex &index);
};


