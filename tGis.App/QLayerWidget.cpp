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
	setSelectionMode(SingleSelection);
	model = new QStandardItemModel();
	rootNode = model->invisibleRootItem();
	setModel(model);
}


QLayerWidget::~QLayerWidget()
{
}

void QLayerWidget::SetMap(IMapPtr map)
{
	_map = map;
}

IMapPtr QLayerWidget::GetMap()
{
	return _map;
}

ILayerPtr QLayerWidget::GetSelectedLayer()
{
	QModelIndexList selected = selectedIndexes();
	if (selected.size() > 0)
	{
		return selected[0].data(LayerPtrRole).value<ILayerPtr>();
	}
	return nullptr;
}

void QLayerWidget::RemoveSelectedLayer()
{
	QModelIndexList selected = selectedIndexes();
	if (selected.size() > 0)
	{
		ILayer* layer = selected[0].data(LayerPtrRole).value<ILayerPtr>();
		ILayerProvider* provider = selected[0].data(LayerProviderRole).value<ILayerProviderPtr>();
		_map->RemoveLayer(layer);
		provider->ReleaseLayer(layer);
		rootNode->removeRow(selected[0].row());
	}
}

void QLayerWidget::RemoveAllLayers()
{
	_map->ClearLayers();
	rootNode->setRowCount(0);
}

void QLayerWidget::selectionChanged(const QItemSelection & sel, const QItemSelection & deselected)
{
	ILayer* layer = nullptr;
	ILayerProvider* provider = nullptr;
	QModelIndexList selected = sel.indexes();
	if (selected.size() > 0)
	{
		layer = selected[0].data(LayerPtrRole).value<ILayerPtr>();
		provider = selected[0].data(LayerProviderRole).value<ILayerProviderPtr>();
	}

	emit LayerSelectionChanged(_map, layer, provider);
}

void QLayerWidget::LayerAdded(IMapPtr map, ILayerPtr layer, ILayerProviderPtr layerProvider)
{
	if (_map != map)
		return;

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

	rootNode->insertRow(0,pItem);
}

