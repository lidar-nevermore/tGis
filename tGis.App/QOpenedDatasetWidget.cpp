#include "QOpenedDatasetWidget.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVariant>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>

#include "tOrganizer.h"

#include "IconRes.h"
#include "ListChoiseDialog.h"


QOpenedDatasetWidget::QOpenedDatasetWidget(QWidget *parent)
	:QTreeView(parent)
{
	int providerCount = DataSourceProviderRepository::INSTANCE().GetDataSourceProviderCount();

	_model = new QStandardItemModel();
	QStandardItem* rootNode = _model->invisibleRootItem();
	for (int i = 0; i < providerCount; i++)
	{
		IDataSourceProvider* provider = DataSourceProviderRepository::INSTANCE().GetDataSourceProvider(i);

		QStandardItem* pItem = new QStandardItem();
		pItem->setText(QString::fromLocal8Bit(provider->GetName()));
		pItem->setEditable(false);
		const QIcon* icon = IconRes::INSTANCE.GetIcon(provider->GetType());
		if (icon != nullptr)
		{
			pItem->setIcon(*icon);
		}
		QVariant userData;
		userData.setValue<void*>((void*)provider);
		pItem->setData(userData, DataRole);
		QVariant userDataType;
		userDataType.setValue<int>(DataSourceProviderType);
		pItem->setData(userDataType, DataTypeRole);
		rootNode->appendRow(pItem);

		QDatasetOpenEventHandler* doeh = new QDatasetOpenEventHandler();
		doeh->_widget = this;
		doeh->_node = pItem;
		_QDatasetOpenEventHandlers.append(doeh);
		provider->AfterDatasetOpenEvent += doeh;

		QDatasetCloseEventHandler* dceh = new QDatasetCloseEventHandler();
		dceh->_widget = this;
		dceh->_node = pItem;
		_QDatasetCloseEventHandlers.append(dceh);
		provider->BeforeDatasetCloseEvent += dceh;
	}

	setModel(_model);
	expandAll();
	connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(NodeDoubleClicked(const QModelIndex&)));
	connect(this, &QOpenedDatasetWidget::expanded, this, &QOpenedDatasetWidget::ExpandedOrCollapsed);
	connect(this, &QOpenedDatasetWidget::collapsed, this, &QOpenedDatasetWidget::ExpandedOrCollapsed);
}


QOpenedDatasetWidget::~QOpenedDatasetWidget()
{
	for (QList<QDatasetOpenEventHandler*>::iterator it = _QDatasetOpenEventHandlers.begin(); it != _QDatasetOpenEventHandlers.end(); it++)
	{
		delete *it;
	}

	for (QList<QDatasetCloseEventHandler*>::iterator it = _QDatasetCloseEventHandlers.begin(); it != _QDatasetCloseEventHandlers.end(); it++)
	{
		delete *it;
	}
}

void QOpenedDatasetWidget::selectionChanged(const QItemSelection & sel, const QItemSelection & deselected)
{
	QModelIndexList selected = sel.indexes();
	if (selected.size() > 0)
	{
		_selectedItem = _model->itemFromIndex(selected[0]);
		int type = _selectedItem->data(DataTypeRole).toInt();
		if (type == DataSourceType)
		{
			_selectedDataSource = _selectedItem->data(DataRole).value<IDataSourcePtr>();
			_selectedDataset = nullptr;
			_selectedDataSourceProvider = _selectedItem->data(DataSourceProviderRole).value<IDataSourceProviderPtr>();
		}
		else if (type == DatasetType)
		{
			_selectedDataSource = nullptr;
			_selectedDataset = _selectedItem->data(DataRole).value<IDatasetPtr>();
			_selectedDataSourceProvider = _selectedItem->data(DataSourceProviderRole).value<IDataSourceProviderPtr>();
		}
	}
	else
	{
		_selectedDataSource = nullptr;
		_selectedDataset = nullptr;
		_selectedDataSourceProvider = nullptr;
		_selectedItem = nullptr;
	}

	emit SelectionChanged(_selectedDataSource, _selectedDataset, _selectedDataSourceProvider);
}

void QOpenedDatasetWidget::AddDatasetNode(QStandardItem * parent, IDataSourceProvider* dsp, IDataset * dt)
{
	QStandardItem* pDatasetItem = new QStandardItem();
	pDatasetItem->setText(QString::fromLocal8Bit(dt->GetName()));
	pDatasetItem->setEditable(false);
	const QIcon* icon = IconRes::INSTANCE.GetIcon(dt->GetType(), "Opened");
	if (icon != nullptr)
	{
		pDatasetItem->setIcon(*icon);
	}
	QVariant udData;
	udData.setValue<IDatasetPtr>(dt);
	pDatasetItem->setData(udData, DataRole);

	QVariant udDataType;
	udDataType.setValue<int>(DatasetType);
	pDatasetItem->setData(udDataType, DataTypeRole);

	QVariant udDataSourceProvider;
	udDataSourceProvider.setValue<IDataSourceProviderPtr>(dsp);
	pDatasetItem->setData(udDataSourceProvider, DataSourceProviderRole);

	parent->appendRow(pDatasetItem);
}

void QOpenedDatasetWidget::RemoveDatasetNode(QStandardItem * parent, IDataSourceProvider* dsp, IDataset * dt)
{
	int rowCount = parent->rowCount();
	for (int i = 0; i < rowCount; i++)
	{
		QStandardItem * item = parent->child(i);
		if (item != nullptr)
		{
			IDataset* itemdt = item->data(DataRole).value<IDatasetPtr>();
			if (itemdt == dt)
			{
				parent->removeRow(i);
				break;
			}
		}		
	}
}

void QOpenedDatasetWidget::ExpandedOrCollapsed(const QModelIndex & index)
{
	resizeColumnToContents(0);
}

void QOpenedDatasetWidget::NodeDoubleClicked(const QModelIndex & index)
{
	QStandardItemModel* model = (QStandardItemModel*)index.model();
	QStandardItem* pItem = model->itemFromIndex(index);
	int type = index.data(DataTypeRole).toInt();

	if (type == DataSourceProviderType)
	{
		if (isExpanded(index))
		{
			collapse(index);
		}
		else
		{
			expand(index);
		}
	}

	if (type == DatasetType)
	{
		IDataset* dt = index.data(DataRole).value<IDatasetPtr>();

		int layerProviderCount = LayerProviderRepository::INSTANCE().GetLayerProviderCountSupportDataset(dt);
		ILayerProviderPtr* providers = new ILayerProviderPtr[layerProviderCount];
		LayerProviderRepository::INSTANCE().GetLayerProviderSupportDataset(dt, layerProviderCount, providers);

		ListChoiseDialog choise((QWidget*)GetMainWindow());
		QListWidget* list = choise.GetQListWidget();

		for (int i = 0; i < layerProviderCount; i++)
		{
			QListWidgetItem *item = new QListWidgetItem(QString::fromLocal8Bit(providers[i]->GetName()));
			QVariant udLayerProvider;
			udLayerProvider.setValue<ILayerProviderPtr>(providers[i]);
			item->setData(DataRole, udLayerProvider);
			list->addItem(item);
		}

		if (QDialog::Accepted == choise.exec())
		{
			QListWidgetItem *item = list->currentItem();
			ILayerProviderPtr provider = item->data(DataRole).value<ILayerProviderPtr>();
			ILayer* layer = provider->UI_CreateLayer(dt);

			if (layer != nullptr)
			{
				IMap* map = GetCurrentMap();
				IMapWidget* mapWidget = GetCurrentMapWidget();
				IGeoSurface* geoSurface = mapWidget->GetGeoSurface();

				if (map->AddLayer(layer) > -1)
				{
					int layerCount = map->GetLayerCount();
					if (layerCount == 0)
					{
						geoSurface->SetSpatialReference(layer->GetSpatialReference());
					}
					const OGREnvelope* envelope = layer->GetEnvelope();

					geoSurface->IncludeEnvelope(envelope);

					mapWidget->RepaintMap();
				}
				else
				{
					QMessageBox::information((QWidget*)GetMainWindow(),
						QStringLiteral("Warning"),
						QStringLiteral("投影不一致，其无法转换。图层添加失败！"),
						QMessageBox::Yes, QMessageBox::Yes);
				}
			}
		}
	}
}


void QOpenedDatasetWidget::QDatasetOpenEventHandler::operator()(IDataSourceProviderPtr& dsp, IDatasetPtr & dataset)
{
	_widget->AddDatasetNode(_node, dsp, dataset);
}

void QOpenedDatasetWidget::QDatasetCloseEventHandler::operator()(IDataSourceProviderPtr& dsp, IDatasetPtr & dataset)
{
	_widget->RemoveDatasetNode(_node, dsp, dataset);
}
