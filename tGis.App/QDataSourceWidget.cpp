#include "QDataSourceWidget.h"
#include "QDir"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVariant>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>

#include "tOrganizer.h"

#include "IconRes.h"
#include "ListChoiseDialog.h"

using namespace tGis::Core;

QDataSourceWidget::QDataSourceWidget(QWidget *parent)
	:QTreeView(parent)
{
	int providerCount = DataSourceProviderRepository::INSTANCE().GetDataSourceProviderCount();

	QStandardItemModel* _model = new QStandardItemModel();
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

		if (provider->IsTypeOf(&FileSystemDataSourceProvider::INSTANCE()))
		{
			QFileInfoList roots = QDir::drives();
			for (QFileInfoList::iterator it = roots.begin(); it != roots.end(); it++)
			{
				QString path = QDir::toNativeSeparators(it->absolutePath());
				IDataSourcePtr ds = FileSystemDataSourceProvider::INSTANCE().CreateDataSource(path.toStdString().c_str());
				_rootDataSource.append(ds);
				AddDataSourceNode(pItem, ds, provider);
			}
		}
	}

	setModel(_model);
	expandAll();
	connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(NodeDoubleClicked(const QModelIndex&)));
	connect(this, &QDataSourceWidget::expanded, this, &QDataSourceWidget::ExpandedOrCollapsed);
	connect(this, &QDataSourceWidget::collapsed, this, &QDataSourceWidget::ExpandedOrCollapsed);
}


QDataSourceWidget::~QDataSourceWidget()
{
	for (QList<IDataSourcePtr>::iterator it = _rootDataSource.begin(); it != _rootDataSource.end(); it++)
	{
		IDataSourcePtr ds = *it;
		IDataSourceProviderPtr dsp = ds->GetProvider();
		dsp->ReleaseDataSource(ds);
	}
}

void QDataSourceWidget::AddDataSourceNode(QStandardItem * parent, IDataSource * ds,IDataSourceProvider* dsp)
{
	QStandardItem* pItem = new QStandardItem();
	QString dsName = QString::fromLocal8Bit(ds->GetName());
	pItem->setText(dsName);
	pItem->setEditable(false);
	const QIcon* icon = IconRes::INSTANCE.GetIcon(ds->GetType(),(ds->IsConnected()?"Connected":"Default"));
	if (icon != nullptr)
	{
		pItem->setIcon(*icon);
	}
	QVariant udData;
	udData.setValue<IDataSourcePtr>(ds);
	pItem->setData(udData, DataRole);

	QVariant udDataType;
	udDataType.setValue<int>(DataSourceType);
	pItem->setData(udDataType, DataTypeRole);

	QVariant udDataSourceProvider;
	udDataSourceProvider.setValue<IDataSourceProviderPtr>(dsp);
	pItem->setData(udDataSourceProvider, DataSourceProviderRole);

	parent->appendRow(pItem);

	AddDataSourceChildNode(pItem, ds, dsp);
}

bool QDataSourceWidget::AddDataSourceChildNode(QStandardItem * node, IDataSource * ds, IDataSourceProvider* dsp)
{
	node->setRowCount(0);
	if (ds->IsConnected())
	{
		int subDataSourceCount = ds->GetDataSourceCount();
		for (int i = 0; i < subDataSourceCount; i++)
		{
			AddDataSourceNode(node, ds->GetDataSource(i), dsp);
		}

		int datasetCount = ds->GetDatasetCount();
		for (int i = 0; i < datasetCount; i++)
		{
			IDataset* dt = ds->GetDataset(i);
			QStandardItem* pDatasetItem = new QStandardItem();
			pDatasetItem->setText(QString::fromLocal8Bit(dt->GetName()));
			pDatasetItem->setEditable(false);
			const QIcon* icon = IconRes::INSTANCE.GetIcon(dt->GetType(), (dt->IsOpened() ? "Opened" : "Default"));
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

			node->appendRow(pDatasetItem);
		}

		return true;
	}

	return false;
}

bool QDataSourceWidget::OpenDataset(QStandardItem* pItem, IDataset* dt)
{
	if (dt->IsOpened())
		return true;

	dt->Open();

	if (dt->IsOpened())
	{
		const QIcon* icon = IconRes::INSTANCE.GetIcon(dt->GetType(),"Opened");
		if (icon != nullptr)
		{
			pItem->setIcon(*icon);
		}
		return true;
	}

	QMessageBox::information((QWidget*)GetMainWindow(),
		QStringLiteral("Warning"),
		QStringLiteral("数据集打开失败！"),
		QMessageBox::Yes, QMessageBox::Yes);

	return false;
}

bool QDataSourceWidget::ConnectDataSource(QStandardItem* pItem, IDataSource* ds, IDataSourceProvider* dsp)
{
	if (ds->IsConnected())
		return true;

	ds->Connect();
	if (ds->IsConnected())
	{
		AddDataSourceChildNode(pItem, ds, dsp);
		const QIcon* icon = IconRes::INSTANCE.GetIcon(ds->GetType(), "Connected");
		if (icon != nullptr)
		{
			pItem->setIcon(*icon);
		}
		setExpanded(pItem->index(), true);
		return true;
	}

	QMessageBox::information((QWidget*)GetMainWindow(),
		QStringLiteral("Warning"),
		QStringLiteral("数据源连接失败！"),
		QMessageBox::Yes, QMessageBox::Yes);

	return false;
}

void QDataSourceWidget::ExpandedOrCollapsed(const QModelIndex & index)
{
	resizeColumnToContents(0);
}

void QDataSourceWidget::NodeDoubleClicked(const QModelIndex & index)
{
	QStandardItemModel* model = (QStandardItemModel*)index.model();
	QStandardItem* pItem = model->itemFromIndex(index);
	int type = index.data(DataTypeRole).toInt();
	
	if (type == DataSourceType)
	{
		IDataSourceProvider* dsp = index.data(DataSourceProviderRole).value<IDataSourceProviderPtr>();
		IDataSource* ds = index.data(DataRole).value<IDataSourcePtr>();
		ConnectDataSource(pItem, ds, dsp);
	}
	else if (type == DatasetType)
	{
		IDataset* dt = index.data(DataRole).value<IDatasetPtr>();

		if (!OpenDataset(pItem, dt))
			return;

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
					emit LayerAdded(map, layer, provider);
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
