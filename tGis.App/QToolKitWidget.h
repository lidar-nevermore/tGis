#pragma once
#include <QTreeView>

#include "tOrganizer.h"
#include "tUtility.h"
#include "tGisMetaType.h"

using namespace tGis::Core;

class QStandardItem;

class QToolKitWidget : public QTreeView
{
	Q_OBJECT
public:
	explicit QToolKitWidget(QWidget *parent = 0);
	~QToolKitWidget();

private:
	static const int ToolKitType = 1;
	static const int ToolType = 2;
	//节点关联的数据的类型
	static const int DataTypeRole = Qt::UserRole + 1;
	//节点关联的数据
	static const int DataRole = Qt::UserRole + 2;

private:
	void AddToolKitNode(QStandardItem* parent, ToolKit* kit);

private slots:
    void ExpandedOrCollapsed(const QModelIndex &index);
	void NodeDoubleClicked(const QModelIndex &index);
};

