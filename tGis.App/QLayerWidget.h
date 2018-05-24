#pragma once
#include <QTreeView>

class QStandardItem;

#include "tOrganizer.h"

namespace tg = tGis::Core;

class QLayerWidget : public QTreeView
{
	Q_OBJECT
public:
	explicit QLayerWidget(QWidget *parent = 0);
	~QLayerWidget();

public:// slots:
	void LayerAdded(IMap*, ILayer*);
};

