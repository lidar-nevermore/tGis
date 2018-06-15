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
	:QListView(parent)
{
	_model = new QStandardItemModel();
	setModel(_model);
	connect(this, &QListView::clicked, this, &QLayerWidget::LayerClicked);
	connect(this, &QListView::pressed, this, &QLayerWidget::LayerClicked);
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

void QLayerWidget::SetMapWidget(IMapWidgetPtr mapWidget)
{
	_mapWidget = mapWidget;
}

IMapWidgetPtr QLayerWidget::GetMapWidget()
{
	return _mapWidget;
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

void QLayerWidget::SelectedLayerPropertyUI()
{
	QModelIndexList selected = selectedIndexes();
	if (selected.size() > 0)
	{
		ILayer* layer = selected[0].data(LayerPtrRole).value<ILayerPtr>();
		ILayerProvider* provider = selected[0].data(LayerProviderRole).value<ILayerProviderPtr>();
		provider->UI_LayerProperty(layer);
	}
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
		_model->removeRow(selected[0].row());
	}
}

void QLayerWidget::RemoveAllLayers()
{
	_map->ClearLayers();
	_model->setRowCount(0);
}

void QLayerWidget::SetSelectedLayerVisible(bool visible)
{
	QModelIndexList selected = selectedIndexes();
	if (selected.size() > 0)
	{
		QStandardItem* pItem = _model->itemFromIndex(selected[0]);
		ILayer* layer = pItem->data(LayerPtrRole).value<ILayerPtr>();
		ILayerProvider* provider = pItem->data(LayerProviderRole).value<ILayerProviderPtr>();
		bool checked = pItem->checkState() == Qt::CheckState::Checked;
		if (visible != checked)
		{
			pItem->setCheckState(visible ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
			layer->SetVisible(visible);
			emit LayerVisibleChanged(_map, layer, provider);
		}
	}
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

void QLayerWidget::LayerClicked(const QModelIndex & index)
{
	QStandardItem* pItem = _model->itemFromIndex(index);
	if (pItem != nullptr)
	{
		ILayer* layer = pItem->data(LayerPtrRole).value<ILayerPtr>();
		ILayerProvider* provider = pItem->data(LayerProviderRole).value<ILayerProviderPtr>();
		bool visible = layer->GetVisible();
		bool checked = pItem->checkState() == Qt::CheckState::Checked;
		if (visible != checked)
		{
			layer->SetVisible(checked);
			emit LayerVisibleChanged(_map, layer, provider);
		}
	}
}

void QLayerWidget::LayerAdded(IMapPtr map, ILayerPtr layer, ILayerProviderPtr layerProvider)
{
	if (_map != map)
		return;

	QStandardItem* pItem = new QStandardItem();
	QString dsName = QString::fromLocal8Bit(layer->GetName());
	pItem->setText(dsName);
	pItem->setEditable(false);
	pItem->setCheckable(true);
	pItem->setCheckState(Qt::CheckState::Checked);
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

	_model->insertRow(0,pItem);
}

