#pragma once

#ifndef __OBJECTSAMPLETOOLDIALOG_H__
#define __OBJECTSAMPLETOOLDIALOG_H__

#include <QWidget>

#include "ui_ObjectSampleToolDialog.h"

#include "tEntity.h"
#include "tOrganizer.h"
#include "tUtility.h"

using namespace tGis::Core;


BEGIN_NAME_SPACE(tGis, Utility)

class ObjectSampleToolDialog : public QDialog
{
public:
	ObjectSampleToolDialog(QWidget *parent = 0);
	~ObjectSampleToolDialog();

public:
	void UpdateChoise();

private:
	Ui::ObjectSampleToolDialog ui;
};

END_NAME_SPACE(tGis, Utility)


#endif


