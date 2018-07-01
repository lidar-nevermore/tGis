#include "QToolKitWidget.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVariant>

#include "tUtility.h"


QToolKitWidget::QToolKitWidget(QWidget *parent)
	:QTreeView(parent)
{
	int kitCount = ToolKitRepository::INSTANCE().GetToolKitCount();
	QStandardItemModel* model = new QStandardItemModel();
	QStandardItem* rootNode = model->invisibleRootItem();
	for (int i = 0; i < kitCount; i++)
	{
		ToolKit* kit = ToolKitRepository::INSTANCE().GetToolKit(i);
		QStandardItem* pItem = new QStandardItem();
		pItem->setText(QString::fromLocal8Bit(kit->GetName()));
		pItem->setEditable(false);
		QIcon icon(":/tGisApp/Images/ToolKit/toolkit.png");
		pItem->setIcon(icon);

		QVariant userData;
		userData.setValue<ToolKitPtr>(kit);
		pItem->setData(userData, DataRole);

		QVariant userDataType;
		userDataType.setValue<int>(ToolKitType);
		pItem->setData(userDataType, DataTypeRole);
		rootNode->appendRow(pItem);
	}
	setModel(model);
	expandAll();
	connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(NodeDoubleClicked(const QModelIndex&)));
	connect(this, &QToolKitWidget::expanded, this, &QToolKitWidget::ExpandedOrCollapsed);
	connect(this, &QToolKitWidget::collapsed, this, &QToolKitWidget::ExpandedOrCollapsed);
}


QToolKitWidget::~QToolKitWidget()
{
}

void QToolKitWidget::AddToolKitNode(QStandardItem * parent, ToolKit * kit)
{
	int kitCount = kit->GetToolKitCount();
	for (int i = 0; i < kitCount; i++)
	{
		ToolKit* subKit = kit->GetToolKit(i);
		QStandardItem* pItem = new QStandardItem();
		pItem->setText(QString::fromLocal8Bit(subKit->GetName()));
		pItem->setEditable(false);
		QIcon icon(":/tGisApp/Images/ToolKit/toolkit.png");
		pItem->setIcon(icon);
		QVariant userData;
		userData.setValue<ToolKitPtr>(subKit);
		pItem->setData(userData, DataRole);

		QVariant userDataType;
		userDataType.setValue<int>(ToolKitType);
		pItem->setData(userDataType, DataTypeRole);
		parent->appendRow(pItem);
	}

	int toolCount = kit->GetToolCount();
	for (int i = 0; i < toolCount; i++)
	{
		ITool* tool = kit->GetTool(i);
		QStandardItem* pItem = new QStandardItem();
		pItem->setText(QString::fromLocal8Bit(tool->GetName()));
		pItem->setEditable(false);
		QIcon icon(":/tGisApp/Images/ToolKit/tool.png");
		pItem->setIcon(icon);

		QVariant userData;
		userData.setValue<IToolPtr>(tool);
		pItem->setData(userData, DataRole);

		QVariant userDataType;
		userDataType.setValue<int>(ToolType);
		pItem->setData(userDataType, DataTypeRole);
		parent->appendRow(pItem);
	}
}

void QToolKitWidget::ExpandedOrCollapsed(const QModelIndex & index)
{
	resizeColumnToContents(0);
}

void QToolKitWidget::NodeDoubleClicked(const QModelIndex & index)
{
	QStandardItemModel* model = (QStandardItemModel*)index.model();
	QStandardItem* pItem = model->itemFromIndex(index);
	int type = index.data(DataTypeRole).toInt();

	if (type == ToolKitType)
	{
		if (pItem->rowCount() == 0)
		{
			ToolKit* kit = index.data(DataRole).value<ToolKitPtr>();
			AddToolKitNode(pItem, kit);
			expand(index);
		}
		else
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
	}
	else
	{
		ITool* tool = index.data(DataRole).value<IToolPtr>();
		tool->Execute();
	}
}



