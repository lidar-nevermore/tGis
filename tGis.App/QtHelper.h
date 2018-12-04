#pragma once

#ifndef __QTHELPER_H__
#define __QTHELPER_H__

#include "tEntity.h"
#include "tOrganizer.h"
#include "tUtility.h"
#include "tVisualize.h"

#include <QMetaType>
#include <QObject>

using namespace tGis::Core;


class QMainWindow;

Q_DECLARE_METATYPE(IMapPtr)
Q_DECLARE_METATYPE(ILayerPtr)
Q_DECLARE_METATYPE(ILayerProviderPtr)
Q_DECLARE_METATYPE(IDataSourceProviderPtr)
Q_DECLARE_METATYPE(IDataSourcePtr)
Q_DECLARE_METATYPE(IDatasetPtr)
Q_DECLARE_METATYPE(ITGisObjectPtr)
Q_DECLARE_METATYPE(IToolPtr)
Q_DECLARE_METATYPE(ToolKitPtr)



class QtHelper : public QObject
{
	Q_OBJECT
public:
	static QtHelper INSTANCE;

private:
	explicit QtHelper(QObject *parent = nullptr);
private:
	~QtHelper();

private:
	QMainWindow* _mainWindow;

public:
	QMainWindow* GetMainWindow();
};


#endif


