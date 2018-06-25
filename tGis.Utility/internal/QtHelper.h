#pragma once

#ifndef __QTHELPER_H__
#define __QTHELPER_H__

#include "tEntity.h"

#include <QMetaType>
#include <QObject>

class QMainWindow;

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Utility)

Q_DECLARE_METATYPE(ObjectSampleMetadataPtr)

QMainWindow* GetMainWindow();

class QtHelper : public QObject
{
	Q_OBJECT
public:
	explicit QtHelper(QObject *parent = nullptr);
	~QtHelper();
};

END_NAME_SPACE(tGis, Utility)


#endif

