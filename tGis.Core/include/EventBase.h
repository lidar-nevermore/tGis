#pragma once

#ifndef __EVENTBASE_H__
#define __EVENTBASE_H__

#include "Helper.h"
#include "tGis_CoreCfg.h"


BEGIN_NAME_SPACE(tGis, Core)


class EventSystemInitializer;

class TGIS_CORE_API EventBase
{
public:
	EventBase() {};
	virtual ~EventBase() {};

private:
	static EventSystemInitializer* initializer;
};

END_NAME_SPACE(tGis, Core)


#endif
