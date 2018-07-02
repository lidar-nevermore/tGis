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
	, _AfterDatasetOpenEventHandler(this, &QDataSourceWidget::AfterDatasetOpen)
	, _BeforeDatasetCloseEventHandler(this, &QDataSourceWidget::BeforeDatasetClose)
	, _AfterDataSourceConnectEventHandler(this, &QDataSourceWidget::AfterDataSourceConnect)
	, _BeforeDataSourceDisconnectEventHandler(this, &QDataSourceWidget::BeforeDataSourceDisconnect)
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
		userData.setValue<ITGisObjectPtr>(provider);
		pItem->setData(userData, DataRole);

		QVariant userDataType;
		userDataType.setValue<int>(DataSourceProviderType);
		pItem->setData(userDataType, DataTypeRole);
		rootNode->appendRow(pItem);

		QVariant udDataSourceProvider;
		udDataSourceProvider.setValue<IDataSourceProviderPtr>(provider);
		pItem->setData(udDataSourceProvider, DataSourceProviderRole);

		provider->AfterDatasetOpenEvent += &_AfterDatasetOpenEventHandler;
		provider->BeforeDatasetCloseEvent += &_BeforeDatasetCloseEventHandler;
		provider->AfterDataSourceConnectEvent += &_AfterDataSourceConnectEventHandler;
		provider->BeforeDataSourceDisconnectEvent += &_BeforeDataSourceDisconnectEventHandler;

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
	int providerCount = DataSourceProviderRepository::INSTANCE().GetDataSourceProviderCount();
	for (int i = 0; i < providerCount; i++)
	{
		IDataSourceProvider* provider = DataSourceProviderRepository::INSTANCE().GetDataSourceProvider(i);
		provider->AfterDatasetOpenEvent -= &_AfterDatasetOpenEventHandler;
		provider->BeforeDatasetCloseEvent -= &_BeforeDatasetCloseEventHandler;
		provider->AfterDataSourceConnectEvent -= &_AfterDataSourceConnectEventHandler;
		provider->BeforeDataSourceDisconnectEvent -= &_BeforeDataSourceDisconnectEventHandler;
	}
	for (QList<IDataSourcePtr>::iterator it = _rootDataSource.begin(); it != _rootDataSource.end(); it++)
	{
		IDataSourcePtr ds = *it;
		IDataSourceProviderPtr dsp = ds->GetProvider();
		dsp->ReleaseDataSource(ds);
	}
}

void QDataSourceWidget::RefreshSelectedDataSource()
{
	_selectedItem->setRowCount(0);
	_selectedDataSource->Disconnect();	
	_selectedDataSource->Connect();
	AddDataSourceChildNode(_selectedItem, _selectedDataSource, _selectedDataSourceProvider);
}

void QDataSourceWidget::AfterDatasetOpen(IDataSourceProvider * provider, IDataset * dt)
{
	AfterDatasetOpenEvent(provider, dt);
}

void QDataSourceWidget::BeforeDatasetClose(IDataSourceProvider * provider, IDataset * dt)
{
	FindNode(_model->invisibleRootItem(), provider, dt, 
		[](QStandardItem* item)->void
	    {
	    	IDataset * dto = (IDatasetPtr)item->data(DataRole).value<ITGisObjectPtr>();
	    	const QIcon* icon = IconRes::INSTANCE.GetIcon(dto->GetType(), "Default");
	    	if (icon != nullptr)
	    	{
	    		item->setIcon(*icon);
	    	}
	    }
		);
	BeforeDatasetCloseEvent(provider, dt);
}

void QDataSourceWidget::FindNode(QStandardItem* parent, IDataSourceProvider * provider, ITGisObject * object,const std::function<void(QStandardItem*)>& nodeFoundEvent)
{
	int rowCount = parent->rowCount();
	for (int i = 0; i < rowCount; i++)
	{
		QStandardItem * item = parent->child(i);
		if (item != nullptr)
		{
			ITGisObject* go = item->data(DataRole).value<ITGisObjectPtr>();
			IDataSourceProvider* prd = item->data(DataSourceProviderRole).value<IDataSourceProviderPtr>();
			if (go == object)
			{
				nodeFoundEvent(item);
			}
			if (provider->IsTypeOf(prd))
			{
				//可能因析构导致的数据集关闭事件，此时object已部分析构，不可以调用其成员方法判断类型
				FileSystemDataSource* dso = dynamic_cast<FileSystemDataSource*>(object);
				if (provider->IsTypeOf(FileSystemDataSourceProvider::S_GetType()) 
					&& go->IsTypeOf(FileSystemDataSource::S_GetType())
					&& dso != nullptr)
				{
					FileSystemDataSource* ds = (FileSystemDataSource*)go;
					
					QString dsPath = QString::fromLocal8Bit(ds->GetCreationString());
					QString dsoPath = QString::fromLocal8Bit(dso->GetCreationString());
					if (dsoPath.contains(dsPath))
					{
						FindNode(item, provider, object, nodeFoundEvent);
					}
				}
				else
				{
					FindNode(item, provider, object, nodeFoundEvent);
				}
			}
		}
	}
}

void QDataSourceWidget::AfterDataSourceConnect(IDataSourceProvider * provider, IDataSource * ds)
{
	AfterDataSourceConnectEvent(provider, ds);
}

void QDataSourceWidget::BeforeDataSourceDisconnect(IDataSourceProvider * provider, IDataSource * ds)
{
	//FindNode(_model->invisibleRootItem(), provider, ds, 
	//	[](QStandardItem* item)->void
	//    {
	//	    IDataSource * dso = (IDataSourcePtr)item->data(DataRole).value<ITGisObjectPtr>();
	//    	const QIcon* icon = IconRes::INSTANCE.GetIcon(dso->GetType(), "Default");
	//    	if (icon != nullptr)
	//    	{
	//    		item->setIcon(*icon);
	//    	}
	//    }
	//	);
	BeforeDataSourceDisconnectEvent(provider, ds);
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
	udData.setValue<ITGisObjectPtr>(ds);
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
			udData.setValue<ITGisObjectPtr>(dt);
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

	QMessageBox::information((QWidget*)QtHelper::INSTANCE.GetMainWindow(),
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
		return true;
	}

	QMessageBox::information((QWidget*)QtHelper::INSTANCE.GetMainWindow(),
		QStringLiteral("Warning"),
		QStringLiteral("数据源连接失败！"),
		QMessageBox::Yes, QMessageBox::Yes);

	return false;
}

void QDataSourceWidget::selectionChanged(const QItemSelection & sel, const QItemSelection & deselected)
{
	QModelIndexList selected = sel.indexes();
	if (selected.size() > 0)
	{
		_selectedItem = _model->itemFromIndex(selected[0]);
		int type = _selectedItem->data(DataTypeRole).toInt();
		if (type == DataSourceType)
		{
			_selectedDataSource = (IDataSourcePtr)_selectedItem->data(DataRole).value<ITGisObjectPtr>();
			_selectedDataset = nullptr;
			_selectedDataSourceProvider = _selectedItem->data(DataSourceProviderRole).value<IDataSourceProviderPtr>();
		}
		else if (type == DatasetType)
		{
			_selectedDataSource = nullptr;
			_selectedDataset = (IDatasetPtr)_selectedItem->data(DataRole).value<ITGisObjectPtr>();
			_selectedDataSourceProvider = _selectedItem->data(DataSourceProviderRole).value<IDataSourceProviderPtr>();
		}
		else
		{
			_selectedDataSource = nullptr;
			_selectedDataset = nullptr;
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
		IDataSource* ds = (IDataSourcePtr)index.data(DataRole).value<ITGisObjectPtr>();
		bool connected = ds->IsConnected();
		ConnectDataSource(pItem, ds, dsp);
		if (connected && isExpanded(index))
		{
			collapse(index);
		}
		else
		{
			expand(index);
		}
	}
	else if (type == DatasetType)
	{
		IDataset* dt = (IDatasetPtr)index.data(DataRole).value<ITGisObjectPtr>();

		if (!OpenDataset(pItem, dt))
			return;

		int layerProviderCount = LayerProviderRepository::INSTANCE().GetLayerProviderCountSupportDataset(dt);
		ILayerProviderPtr* providers = new ILayerProviderPtr[layerProviderCount];
		LayerProviderRepository::INSTANCE().GetLayerProviderSupportDataset(dt, layerProviderCount, providers);

		ListChoiseDialog choise((QWidget*)QtHelper::INSTANCE.GetMainWindow());
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
					QMessageBox::information((QWidget*)QtHelper::INSTANCE.GetMainWindow(),
						QStringLiteral("Warning"),
						QStringLiteral("投影不一致，其无法转换。图层添加失败！"),
						QMessageBox::Yes, QMessageBox::Yes);
				}
			}
		}
	}
}
