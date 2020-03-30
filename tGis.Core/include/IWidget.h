#pragma once

#ifndef __I_WIDGET_H__
#define __I_WIDGET_H__

#include "Helper.h"
#include "tGisCoreCfg.h"
#include "Event.h"


BEGIN_NAME_SPACE(tGis, Core)


struct TGIS_CORE_API IWidget
{
	virtual void Client2Screen(int cliX, int cliY, int *scrX, int *scrY) = 0;
	virtual void Screen2Client(int scrX, int scrY, int *cliX, int *cliY) = 0;

	IWidget() {};
	virtual ~IWidget() {};
private:
	IWidget(const IWidget &) = delete;
	IWidget &operator=(const IWidget &) = delete;
};

typedef IWidget* IWidgetPtr;

END_NAME_SPACE(tGis, Core)


#endif
