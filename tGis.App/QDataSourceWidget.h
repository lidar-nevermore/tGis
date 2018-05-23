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
	static const int DATASOURCEPROVIDER = 1;
	static const int DATASOURCE = 2;
	static const int DATASET = 3;

	static const int DATATYPEROLE = 6;
	static const int DATAROLE = 7;

private:
	void AddDataSourceNode(QStandardItem* item, tg::IDataSource* ds);
};

