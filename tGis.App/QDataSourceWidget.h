#pragma once
#include <QTreeView>

class QStandardItem;

#include "tOrganizer.h"
#include "tGisMetaType.h"

using namespace tGis::Core;

class QDataSourceWidget : public QTreeView
{
	Q_OBJECT
public:
	explicit QDataSourceWidget(QWidget *parent = 0);
	~QDataSourceWidget();

signals:
	void LayerAdded(IMapPtr, ILayerPtr, ILayerProviderPtr);

private:
	//�������͵Ķ���
	static const int DataSourceProviderType = 1;
	static const int DataSourceType = 2;
	static const int DatasetType = 3;
	//�ڵ���������ݵ�����
	static const int DataTypeRole = Qt::UserRole + 1;
	//�ڵ����������
	static const int DataRole = Qt::UserRole + 2;
	//�ڵ���������ݵ�Provider
	static const int DataSourceProviderRole = Qt::UserRole + 3;
	//�ڵ���������ݵ�״̬
	static const int DataStatusRole = Qt::UserRole + 4;

private:
	void AddDataSourceNode(QStandardItem* parent, IDataSource* ds, IDataSourceProvider* dsp);
	bool AddDataSourceChildNode(QStandardItem* node, IDataSource* ds, IDataSourceProvider* dsp);
	bool OpenDataset(QStandardItem* pItem, IDataset* dt);
	bool ConnectDataSource(QStandardItem* pItem, IDataSource* ds, IDataSourceProvider* dsp);

private slots:
	void NodeDoubleClicked(const QModelIndex &index);
};

