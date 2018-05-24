#pragma once
#include <QTreeView>
#include <QMetaType>

class QStandardItem;

#include "tOrganizer.h"


using namespace tGis::Core;


class QLayerWidget : public QTreeView
{
	Q_OBJECT
public:
	explicit QLayerWidget(QWidget *parent = 0);
	~QLayerWidget();

public:// slots:
	void LayerAdded(IMapPtr, ILayerPtr);
};


