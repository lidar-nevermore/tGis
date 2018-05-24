#pragma once
#include <QTreeView>

class QStandardItem;

#include "tOrganizer.h"
#include "tGisMetaType.h"

namespace tg = tGis::Core;

class QDataSourceWidget : public QTreeView
{
	Q_OBJECT
public:
	explicit QDataSourceWidget(QWidget *parent = 0);
	~QDataSourceWidget();

public:
signals:
	void LayerAdded(IMapPtr, ILayerPtr);

private:
	static const int DataSourceProviderType = 1;
	static const int DataSourceType = 2;
	static const int DatasetType = 3;

	static const int DataTypeRole = Qt::UserRole + 1;
	static const int DataRole = Qt::UserRole + 2;

private:
	void AddDataSourceNode(QStandardItem* parent, tg::IDataSource* ds);
	void AddDataSourceChildNode(QStandardItem* node, tg::IDataSource* ds);

private slots:
	void NodeDoubleClicked(const QModelIndex &index);
};

