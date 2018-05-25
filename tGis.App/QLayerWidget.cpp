#include "QLayerWidget.h"
#include "QDir"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVariant>

#include "tOrganizer.h"

#include "IconRes.h"
#include "tGisMetaType.h"

using namespace tGis::Core;

QLayerWidget::QLayerWidget(QWidget *parent)
	:QTreeView(parent)
{
	model = new QStandardItemModel();
	rootNode = model->invisibleRootItem();
	setModel(model);
}


QLayerWidget::~QLayerWidget()
{
}

void QLayerWidget::LayerAdded(IMapPtr map, ILayerPtr layer, ILayerProviderPtr layerProvider)
{
	QStandardItem* pItem = new QStandardItem();
	QString dsName = QString::fromLocal8Bit(layer->GetName());
	pItem->setText(dsName);
	pItem->setEditable(false);
	const QIcon* icon = IconRes::INSTANCE.GetIcon(layer->GetType());
	if (icon != nullptr)
	{
		pItem->setIcon(*icon);
	}

	QVariant udLayer;
	udLayer.setValue<ILayerPtr>(layer);
	pItem->setData(udLayer, LayerPtrRole);

	QVariant udLayerProvider;
	udLayerProvider.setValue<ILayerProviderPtr>(layerProvider);
	pItem->setData(udLayerProvider, LayerProviderRole);

	rootNode->appendRow(pItem);
}

