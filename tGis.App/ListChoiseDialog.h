#pragma once

#include <QDialog>

#include "ui_ListChoiseDialog.h"

class ListChoiseDialog : public QDialog
{
	Q_OBJECT
public:
	explicit ListChoiseDialog(QWidget *parent = 0);
	~ListChoiseDialog();

public:
	QListWidget* GetQListWidget();

private slots:
	void on_listWidget_itemSelectionChanged();

private:
	Ui::ListChoiseDialog ui;
};

