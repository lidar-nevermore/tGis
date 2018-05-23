#pragma once
#include <QTreeView>

class QStandardItem;

#include "tOrganizer.h"

namespace tg = tGis::Core;

class QDataSourceWidget : public QTreeView
{
	Q_OBJECT
public:
	explicit QDataSourceWidget(QWidget *parent = 0);
	~QDataSourceWidget();

private:
	static const int DataSourceProviderType = 1;
	static const int DataSourceType = 2;
	static const int DatasetType = 3;

	static const int DataTypeRole = Qt::UserRole + 1;
	static const int DataRole = Qt::UserRole + 2;

private:
	void AddDataSourceNode(QStandardItem* item, tg::IDataSource* ds);
};

