#pragma once

#include <QMetaType>

#include "tOrganizer.h"
#include "tUtility.h"

using namespace tGis::Core;

Q_DECLARE_METATYPE(IMapPtr)
Q_DECLARE_METATYPE(ILayerPtr)
Q_DECLARE_METATYPE(ILayerProviderPtr)
Q_DECLARE_METATYPE(IDataSourceProviderPtr)
Q_DECLARE_METATYPE(IDataSourcePtr)
Q_DECLARE_METATYPE(IDatasetPtr)
Q_DECLARE_METATYPE(ITGisObjectPtr)
Q_DECLARE_METATYPE(IToolPtr)
Q_DECLARE_METATYPE(ToolKitPtr)

class QMainWindow;
QMainWindow* GetMainWindow();
