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

QStandardItem * QLayerWidget::CreateLayerItem(ILayer * layer, ILayerProvider * provider)
{
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
	udLayerProvider.setValue<ILayerProviderPtr>(provider);
	pItem->setData(udLayerProvider, LayerProviderRole);

	return pItem;
}

void QLayerWidget::SetMap(IMapPtr map)
{
	_map = map;
}

IMapPtr QLayerWidget::GetMap()
{
	return _map;
}

void QLayerWidget::AddLayer(IMapPtr map, ILayerPtr layer)
{
	if (_map != map)
		return;

	QStandardItem* pItem = CreateLayerItem(layer, layer->GetProvider());
	_model->insertRow(0, pItem);
	emit LayerSelectionChanged(_map, _selectedLayer, _selectedLayerProvider);
}

void QLayerWidget::RemoveLayer(IMapPtr map, ILayerPtr layer)
{
	if (_map != map)
		return;

	int rowCount = _model->rowCount();
	QStandardItem * parent = _model->invisibleRootItem();
	for (int i = 0; i < rowCount; i++)
	{
		QStandardItem * item = parent->child(i);
		if (item != nullptr)
		{
			ILayerPtr itemLayer = item->data(LayerPtrRole).value<ILayerPtr>();
			if (itemLayer == layer)
			{
				parent->removeRow(i);
				break;
			}
		}
	}
}

void QLayerWidget::ClearLayer(IMapPtr map)
{
	if (_map != map)
		return;

	_model->setRowCount(0);
}

ILayerPtr QLayerWidget::GetSelectedLayer()
{
	return _selectedLayer;
}

void QLayerWidget::SelectedLayerPropertyUI()
{
	if (_selectedLayer != nullptr)
	{
		_selectedLayerProvider->UI_LayerProperty(_selectedLayer);
	}
}

void QLayerWidget::RemoveSelectedLayer()
{
	if (_selectedLayer != nullptr)
	{
		ILayer* layer = _selectedLayer;
		_map->RemoveLayer(_selectedLayer);
		layer->GetProvider()->ReleaseLayer(layer);
	}
}

void QLayerWidget::RemoveAllLayers()
{
	_map->ClearLayers();
}

void QLayerWidget::SetSelectedLayerVisible(bool visible)
{	
	if (_selectedLayer != nullptr)
	{
		bool checked = _selectedLayer->GetVisible();
		if (visible != checked)
		{
			_selectedItem->setCheckState(visible ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

			_selectedLayer->SetVisible(visible);
			emit LayerVisibleChanged(_map, _selectedLayer, _selectedLayerProvider);
		}
	}
}

bool QLayerWidget::CanMoveSelectedLayerUp()
{
	return _selectedLayerIndex > -1 
		   && _selectedLayerIndex < _map->GetLayerCount()-1;
}

bool QLayerWidget::CanMoveSelectedLayerDown()
{
	return _selectedLayerIndex > 0;
}

void QLayerWidget::MoveSelectedLayerUp()
{
	if (_selectedLayer != nullptr)
	{
		QStandardItem* selectedItem = _selectedItem;
		ILayer* selectedLayer = _selectedLayer;
		ILayerProvider* selectedLayerProvider = _selectedLayerProvider;
		int selectedLayerIndex = _selectedLayerIndex;

		_selectedLayer = nullptr;
		_selectedLayerProvider = nullptr;
		_selectedLayerIndex = -1;
		_selectedItem = nullptr;

		_map->MoveLayer(selectedLayerIndex, selectedLayerIndex + 1);
		QStandardItem* pItem = CreateLayerItem(selectedLayer, selectedLayerProvider);
		int row = selectedItem->row();
		_model->insertRow(row-1, pItem);
		_model->removeRow(selectedItem->row());
	}
}

void QLayerWidget::MoveSelectedLayerDown()
{
	if (_selectedLayer != nullptr)
	{
		QStandardItem* selectedItem = _selectedItem;
		ILayer* selectedLayer = _selectedLayer;
		ILayerProvider* selectedLayerProvider = _selectedLayerProvider;
		int selectedLayerIndex = _selectedLayerIndex;

		_selectedLayer = nullptr;
		_selectedLayerProvider = nullptr;
		_selectedLayerIndex = -1;
		_selectedItem = nullptr;

		_map->MoveLayer(selectedLayerIndex, selectedLayerIndex - 1);
		QStandardItem* pItem = CreateLayerItem(selectedLayer, selectedLayerProvider);
		int row = selectedItem->row();
		_model->insertRow(row + 2, pItem);
		_model->removeRow(selectedItem->row());
	}
}

void QLayerWidget::MoveSelectedLayerTop()
{
	if (_selectedLayer != nullptr)
	{
		QStandardItem* selectedItem = _selectedItem;
		ILayer* selectedLayer = _selectedLayer;
		ILayerProvider* selectedLayerProvider = _selectedLayerProvider;
		int selectedLayerIndex = _selectedLayerIndex;

		_selectedLayer = nullptr;
		_selectedLayerProvider = nullptr;
		_selectedLayerIndex = -1;
		_selectedItem = nullptr;

		_map->MoveLayer(selectedLayerIndex, _map->GetLayerCount() - 1);
		QStandardItem* pItem = CreateLayerItem(selectedLayer, selectedLayerProvider);
		_model->insertRow(0, pItem);
		_model->removeRow(selectedItem->row());		
	}
}

void QLayerWidget::MoveSelectedLayerBottom()
{
	if (_selectedLayer != nullptr)
	{
		QStandardItem* selectedItem = _selectedItem;
		ILayer* selectedLayer = _selectedLayer;
		ILayerProvider* selectedLayerProvider = _selectedLayerProvider;
		int selectedLayerIndex = _selectedLayerIndex;

		_selectedLayer = nullptr;
		_selectedLayerProvider = nullptr;
		_selectedLayerIndex = -1;
		_selectedItem = nullptr;

		_map->MoveLayer(selectedLayerIndex, 0);
		QStandardItem* pItem = CreateLayerItem(selectedLayer, selectedLayerProvider);
		_model->appendRow(pItem);
		_model->removeRow(selectedItem->row());		
	}
}

void QLayerWidget::selectionChanged(const QItemSelection & sel, const QItemSelection & deselected)
{
	QModelIndexList selected = sel.indexes();
	if (selected.size() > 0)
	{
		_selectedItem = _model->itemFromIndex(selected[0]);
		_selectedLayer = _selectedItem->data(LayerPtrRole).value<ILayerPtr>();
		_selectedLayerProvider = selected[0].data(LayerProviderRole).value<ILayerProviderPtr>();
		_selectedLayerIndex = _map->GetLayerIndex(_selectedLayer);
	}
	else
	{
		_selectedLayer = nullptr;
		_selectedLayerProvider = nullptr;
		_selectedLayerIndex = -1;
		_selectedItem = nullptr;
	}

	emit LayerSelectionChanged(_map, _selectedLayer, _selectedLayerProvider);
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
