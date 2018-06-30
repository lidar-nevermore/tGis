#pragma once
#include <QTreeView>
#include <QList>

class QStandardItem;
class QStandardItemModel;


#include "tOrganizer.h"
#include "Event.h"
#include "tGisMetaType.h"

using namespace tGis::Core;

class QDataSourceWidget : public QTreeView
{
	Q_OBJECT
public:
	explicit QDataSourceWidget(QWidget *parent = 0);
	~QDataSourceWidget();

public:
	void RefreshSelectedDataSource();

private:
	MemberEventHandler<QDataSourceWidget, IDataSourceProvider*, IDataset*> _AfterDatasetOpenEventHandler;
	MemberEventHandler<QDataSourceWidget, IDataSourceProvider*, IDataset*> _BeforeDatasetCloseEventHandler;
	void AfterDatasetOpen(IDataSourceProvider*, IDataset*);
	void BeforeDatasetClose(IDataSourceProvider*, IDataset*);

	void FindNode(QStandardItem* parent, IDataSourceProvider*, ITGisObject*,const std::function<void(QStandardItem*)>&);

	MemberEventHandler<QDataSourceWidget, IDataSourceProvider*, IDataSource*> _AfterDataSourceConnectEventHandler;
	MemberEventHandler<QDataSourceWidget, IDataSourceProvider*, IDataSource*> _BeforeDataSourceDisconnectEventHandler;
	void AfterDataSourceConnect(IDataSourceProvider*, IDataSource*);
	void BeforeDataSourceDisconnect(IDataSourceProvider*, IDataSource*);

public:
	Event<IDataSourceProvider*, IDataset*> AfterDatasetOpenEvent;

	Event<IDataSourceProvider*, IDataset*> BeforeDatasetCloseEvent;

	Event<IDataSourceProvider*, IDataSource*> AfterDataSourceConnectEvent;

	Event<IDataSourceProvider*, IDataSource*> BeforeDataSourceDisconnectEvent;

signals:
	void SelectionChanged(IDataSourcePtr, IDatasetPtr, IDataSourceProviderPtr);

private:
	QList<IDataSourcePtr> _rootDataSource;

private:
	//数据类型的定义
	static const int DataSourceProviderType = 1;
	static const int DataSourceType = 2;
	static const int DatasetType = 3;
	//节点关联的数据的类型
	static const int DataTypeRole = Qt::UserRole + 1;
	//节点关联的数据
	static const int DataRole = Qt::UserRole + 2;
	//节点关联的数据的Provider
	static const int DataSourceProviderRole = Qt::UserRole + 3;
	//节点关联的数据的状态
	static const int DataStatusRole = Qt::UserRole + 4;

private:
	void AddDataSourceNode(QStandardItem* parent, IDataSource* ds, IDataSourceProvider* dsp);
	bool AddDataSourceChildNode(QStandardItem* node, IDataSource* ds, IDataSourceProvider* dsp);
	bool OpenDataset(QStandardItem* pItem, IDataset* dt);
	bool ConnectDataSource(QStandardItem* pItem, IDataSource* ds, IDataSourceProvider* dsp);

private:
	QStandardItemModel* _model;
	IDataSourcePtr _selectedDataSource;
	IDatasetPtr _selectedDataset;
	IDataSourceProviderPtr _selectedDataSourceProvider;

	QStandardItem* _selectedItem;
protected:
	virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private slots:
	void NodeDoubleClicked(const QModelIndex &index);
	void ExpandedOrCollapsed(const QModelIndex &index);
};

