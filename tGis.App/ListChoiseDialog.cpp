#include "ListChoiseDialog.h"
#include <QPushButton>


ListChoiseDialog::ListChoiseDialog(QWidget *parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
	QPushButton* button = ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok);
	button->setEnabled(false);
}


ListChoiseDialog::~ListChoiseDialog()
{
}

QListWidget * ListChoiseDialog::GetQListWidget()
{
	return ui.listWidget;
}

void ListChoiseDialog::on_listWidget_itemSelectionChanged()
{
	QList<QListWidgetItem*> items = ui.listWidget->selectedItems();
	QPushButton* button = ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok);
	button->setEnabled(items.size() > 0);
}
