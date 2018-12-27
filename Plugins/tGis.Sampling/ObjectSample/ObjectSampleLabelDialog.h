#pragma once

#ifndef __OBJECTSAMPLELABELDIALOG_H__
#define __OBJECTSAMPLELABELDIALOG_H__

#include <QDialog>

#include "ui_ObjectSampleLabelDialog.h"
#include "QMapWidget.h"

#include "tEntity.h"
#include "tOrganizer.h"
#include "Event.h"

using namespace tGis::Core;



class ObjectSampleLabelDialog : public QDialog
{
public:
	ObjectSampleLabelDialog(QWidget *parent = 0);
	~ObjectSampleLabelDialog();

public:
	inline QString GetClassName()
	{
		return ui.leClassName->text();
	}

	inline int GetClassLabel() 
	{
		QString label = ui.leClassLabel->text();
		return label.toInt();
	}

private:
	Ui::ObjectSampleLabelDialog ui;
};





#endif



