#include "ObjectSampleLabelDialog.h"


BEGIN_NAME_SPACE(tGis, Utility)

ObjectSampleLabelDialog::ObjectSampleLabelDialog(QWidget *parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}


ObjectSampleLabelDialog::~ObjectSampleLabelDialog()
{
}


END_NAME_SPACE(tGis, Utility)


