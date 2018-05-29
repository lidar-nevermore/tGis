#pragma once
#include <QTreeView>
#include <QMetaType>

class QStandardItem;
class QStandardItemModel;

#include "tOrganizer.h"


using namespace tGis::Core;


class QLayerWidget : public QTreeView
{
	Q_OBJECT
public:
	explicit QLayerWidget(QWidget *parent = 0);
	~QLayerWidget();

private:
	static const int LayerPtrRole = Qt::UserRole + 1;
	static const int LayerProviderRole = Qt::UserRole + 2;

private:
	QStandardItemModel* model;
	QStandardItem* rootNode;
	IMapPtr _map;

public:
	void SetMap(IMapPtr map);
	IMapPtr GetMap();

	ILayerPtr GetSelectedLayer();
	void RemoveSelectedLayer();
	void RemoveAllLayers();

protected:
	virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
Q_SIGNALS:
	void LayerSelectionChanged(IMapPtr, ILayerPtr, ILayerProviderPtr);
public slots:
	void LayerAdded(IMapPtr, ILayerPtr, ILayerProviderPtr);
};


