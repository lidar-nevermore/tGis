#pragma once

#ifndef __TGISAPPLICATION_H__
#define __TGISAPPLICATION_H__


#include "Helper.h"
#include "tEntity.h"
#include "tVisualize.h"

BEGIN_NAME_SPACE(tGis, Core)

class TGIS_API TGisApplication
{
private:
	static TGisApplication* _instance;
public:
	static TGisApplication& INSTANCE();

protected:
	TGisApplication();
	virtual ~TGisApplication();

public:
	virtual IMap*  GetCurrentMap() = 0;
	virtual IMapWidget*  GetCurrentMapWidget() = 0;
};


END_NAME_SPACE(tGis, Core)


#endif

