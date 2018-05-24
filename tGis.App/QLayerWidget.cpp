#include "QLayerWidget.h"
#include "QDir"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVariant>

#include "tOrganizer.h"

#include "IconRes.h"

using namespace tGis::Core;

QLayerWidget::QLayerWidget(QWidget *parent)
	:QTreeView(parent)
{

}


QLayerWidget::~QLayerWidget()
{
}

void QLayerWidget::LayerAdded(IMap *, ILayer *)
{
}
