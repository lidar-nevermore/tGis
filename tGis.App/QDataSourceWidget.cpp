#include "QDataSourceWidget.h"
#include "QDir"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVariant>

#include "tOrganizer.h"

#include "IconRes.h"

using namespace tGis::Core;

QDataSourceWidget::QDataSourceWidget(QWidget *parent)
	:QTreeView(parent)
{
	QFileInfoList roots = QDir::drives();
	for (QFileInfoList::iterator it = roots.begin(); it != roots.end(); it++)
	{
		QString path = QDir::toNativeSeparators(it->absolutePath());
		FileSystemDataSourceProvider::INSTANCE.CreateDataSource(path.toStdString().c_str());
	}

	int providerCount = DataSourceProviderRepository::INSTANCE.GetDataSourceProviderCount();

	QStandardItemModel* model = new QStandardItemModel();
	QStandardItem* rootNode = model->invisibleRootItem();
	for (int i = 0; i < providerCount; i++)
	{
		IDataSourceProvider* provider = DataSourceProviderRepository::INSTANCE.GetDataSourceProvider(i);
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
		pItem->setData(userData,DataRole);
		QVariant userDataType;
		userDataType.setValue<int>(DataSourceProviderType);
		pItem->setData(userDataType, DataTypeRole);
		rootNode->appendRow(pItem);

		int datasourceCount = provider->GetDataSourceCount();
		for (int i = 0; i < datasourceCount; i++)
		{
			AddDataSourceNode(pItem, provider->GetDataSource(i));
		}
	}

	setModel(model);
	expandAll();

	connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(NodeDoubleClicked(const QModelIndex&)));
}


QDataSourceWidget::~QDataSourceWidget()
{
}

void QDataSourceWidget::AddDataSourceNode(QStandardItem * parent, IDataSource * ds)
{
	QStandardItem* pItem = new QStandardItem();
	QString dsName = QString::fromLocal8Bit(ds->GetName());
	pItem->setText(dsName);
	pItem->setEditable(false);
	const QIcon* icon = IconRes::INSTANCE.GetIcon(ds->GetType());
	if (icon != nullptr)
	{
		pItem->setIcon(*icon);
	}
	QVariant userData;
	userData.setValue<void*>((void*)ds);
	pItem->setData(userData, DataRole);
	QVariant userDataType;
	userDataType.setValue<int>(DataSourceType);
	pItem->setData(userDataType, DataTypeRole);

	parent->appendRow(pItem);

	AddDataSourceChildNode(pItem, ds);
}

void QDataSourceWidget::AddDataSourceChildNode(QStandardItem * node, tg::IDataSource * ds)
{
	if (ds->IsConnected())
	{
		int subDataSourceCount = ds->GetDataSourceCount();
		for (int i = 0; i < subDataSourceCount; i++)
		{
			AddDataSourceNode(node, ds->GetDataSource(i));
		}

		int datasetCount = ds->GetDatasetCount();
		for (int i = 0; i < datasetCount; i++)
		{
			IDataset* dt = ds->GetDataset(i);
			QStandardItem* pDatasetItem = new QStandardItem();
			pDatasetItem->setText(QString::fromLocal8Bit(dt->GetName()));
			pDatasetItem->setEditable(false);
			const QIcon* icon = IconRes::INSTANCE.GetIcon(dt->GetType());
			if (icon != nullptr)
			{
				pDatasetItem->setIcon(*icon);
			}
			QVariant userData;
			userData.setValue<void*>((void*)dt);
			pDatasetItem->setData(userData, DataRole);
			QVariant userDataType;
			userDataType.setValue<int>(DatasetType);
			pDatasetItem->setData(userDataType, DataTypeRole);

			node->appendRow(pDatasetItem);
		}
	}
}

void QDataSourceWidget::NodeDoubleClicked(const QModelIndex & index)
{
	QStandardItemModel* model = (QStandardItemModel*)index.model();
	QStandardItem* pItem = model->itemFromIndex(index);
	int type = index.data(DataTypeRole).toInt();

	if (type == DataSourceType)
	{
		IDataSource* ds = (IDataSource*)(index.data(DataRole).value<void*>());
		if (!ds->IsConnected())
		{
			ds->Connect();
			AddDataSourceChildNode(pItem, ds);
			expand(index);
		}
	}
	else if (type == DatasetType)
	{
		//TODO: ÃÌº”œ‘ æ
		IDataset* dt = (IDataset*)(index.data(DataRole).value<void*>());
		dt->Open();

		if (!dt->IsOpened())
			return;

		int layerProviderCount = LayerProviderRepository::INSTANCE.GetLayerProviderCountSupportDataset(dt->GetType());
		ILayerProviderPtr* providers = new ILayerProviderPtr[layerProviderCount];
		LayerProviderRepository::INSTANCE.GetLayerProviderSupportDataset(dt->GetType(), layerProviderCount, providers);
		ILayer* layer = providers[0]->UI_CreateLayer(dt);
		
		IMap* map = GetCurrentMap();
		IMapWidget* mapWidget = GetCurrentMapWidget();
		IGeoSurface* geoSurface = mapWidget->GetGeoSurface();

		int layerCount = map->GetLayerCount();
		map->AddLayer(layer);

		if (layerCount == 0)
		{
			const OGREnvelope* envelope = layer->GetEnvelope();
			geoSurface->SetSpatialReference(layer->GetSpatialReference());
			geoSurface->IncludeEnvelope(envelope);
		}
		
		mapWidget->RepaintMap();
	}
}
