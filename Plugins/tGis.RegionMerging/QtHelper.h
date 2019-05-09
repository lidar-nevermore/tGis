#pragma once

#ifndef __QTHELPER_H__
#define __QTHELPER_H__

#include "tEntity.h"

#include <QMetaType>
#include <QObject>


using namespace tGis::Core;

class QMainWindow;


class QtHelper : public QObject
{
	Q_OBJECT
public:
	static QtHelper INSTANCE;

private:
	explicit QtHelper(QObject *parent = nullptr);
	~QtHelper();

private:
	QMainWindow* _mainWindow;

public:
	QMainWindow* GetMainWindow();
};




#endif

