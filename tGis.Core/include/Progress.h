#pragma once

#ifndef __PROGRESS_H__
#define __PROGRESS_H__

#include "Helper.h"
#include "Event.h"


BEGIN_NAME_SPACE(tGis, Core)

struct TGIS_CORE_API Progress
{
	Progress()
		:Value(0)
		, Max(100)
		, Message(nullptr)
	{
	}

	Progress(int value, int max = 100, char* msg = nullptr)
		:Value(value)
		, Max(max)
		, Message(msg)
	{
	}

	int Value;
	const int Max;
	char* Message;
};

typedef IEventHandler<Progress> ProgressEventHandler;
typedef Event<Progress> ProgressEvent;

template struct TGIS_CORE_API IEventHandler<Progress>;
template class TGIS_CORE_API Event<Progress>;

END_NAME_SPACE(tGis, Core)


#endif
