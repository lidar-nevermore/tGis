#pragma once

#include "tOrganizer.h"

using namespace tGis::Core;

class TGisQtApplication : public TGisApplication
{
public:
	TGisQtApplication();
	~TGisQtApplication();

public:
	IMap* _curMap;
	IMapWidget* _curMapWidget;

public:
	IMap*  GetCurrentMap() override;
	IMapWidget*  GetCurrentMapWidget() override;
};

