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
		pItem->setText(provider->GetCatagory());
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
}


QDataSourceWidget::~QDataSourceWidget()
{
}

void QDataSourceWidget::AddDataSourceNode(QStandardItem * item, IDataSource * ds)
{
	QStandardItem* pItem = new QStandardItem();
	pItem->setText(ds->GetName());
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

	item->appendRow(pItem);

	if (ds->IsConnected())
	{
		int subDataSourceCount = ds->GetDataSourceCount();
		for (int i = 0; i < subDataSourceCount; i++)
		{
			AddDataSourceNode(pItem, ds->GetDataSource(i));
		}

		int datasetCount = ds->GetDatasetCount();
		for (int i = 0; i < datasetCount; i++)
		{
			IDataset* dt = ds->GetDataset(i);
			QStandardItem* pDatasetItem = new QStandardItem();
			pDatasetItem->setText(dt->GetName());
			pDatasetItem->setEditable(false);
			const QIcon* icon = IconRes::INSTANCE.GetIcon(dt->GetType());
			if (icon != nullptr)
			{
				pDatasetItem->setIcon(*icon);
			}
			QVariant userData;
			userData.setValue<void*>((void*)dt);
			pItem->setData(userData, DataRole);
			QVariant userDataType;
			userDataType.setValue<int>(DatasetType);
			pItem->setData(userDataType, DataTypeRole);
		}
	}
}
