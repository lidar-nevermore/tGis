#pragma once

#ifndef __OBJECTSAMPLEWIDGET_H__
#define __OBJECTSAMPLEWIDGET_H__

#include <QWidget>

#include "ui_ObjectSampleWidget.h"

#include "tEntity.h"
#include "tOrganizer.h"
#include "tUtility.h"

using namespace tGis::Core;


BEGIN_NAME_SPACE(tGis, Utility)

class ObjectSampleWidget : public QWidget
{
public:
	ObjectSampleWidget(QWidget *parent = 0);
	~ObjectSampleWidget();

private:
	Ui::ObjectSampleWidget ui;
};

END_NAME_SPACE(tGis, Utility)


#endif


