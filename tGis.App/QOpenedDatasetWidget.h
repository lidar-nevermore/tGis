#pragma once
#include <QTreeView>
#include <QList>

class QStandardItem;
class QStandardItemModel;

#include "tOrganizer.h"
#include "Event.h"
#include "QtHelper.h"

using namespace tGis::Core;

class QOpenedDatasetWidget : public QTreeView
{
	Q_OBJECT
public:
	explicit QOpenedDatasetWidget(QWidget *parent = 0);
	~QOpenedDatasetWidget();

signals:
	void SelectionChanged(IDataSourcePtr, IDatasetPtr, IDataSourceProviderPtr);

private:
	QStandardItemModel* _model;
	IDataSourcePtr _selectedDataSource;
	IDatasetPtr _selectedDataset;
	IDataSourceProviderPtr _selectedDataSourceProvider;

	QStandardItem* _selectedItem;

protected:
	virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

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
	struct QDatasetOpenEventHandler : public DatasetEventHandler
	{
		QOpenedDatasetWidget* _widget;
		QStandardItem* _node;
		void operator()(IDataSourceProviderPtr& dsp, IDatasetPtr& dataset);
		void operator()(const IDataSourceProviderPtr& dsp, const IDatasetPtr& dataset);
	};

	struct QDatasetCloseEventHandler : public DatasetEventHandler
	{
		QOpenedDatasetWidget* _widget;
		QStandardItem* _node;
		void operator()(IDataSourceProviderPtr& dsp, IDatasetPtr& dataset);
		void operator()(const IDataSourceProviderPtr& dsp, const IDatasetPtr& dataset);
	};

	QList<QDatasetOpenEventHandler*> _QDatasetOpenEventHandlers;
	QList<QDatasetCloseEventHandler*> _QDatasetCloseEventHandlers;

private:
	void AddDatasetNode(QStandardItem* parent, IDataSourceProvider* dsp, IDataset* dt);
	void RemoveDatasetNode(QStandardItem* parent, IDataSourceProvider* dsp, IDataset* dt);

private slots:
	void NodeDoubleClicked(const QModelIndex &index);
	void ExpandedOrCollapsed(const QModelIndex &index);
};

