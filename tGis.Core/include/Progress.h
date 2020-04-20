#pragma once

#ifndef __PROGRESS_H__
#define __PROGRESS_H__

#include "Helper.h"
#include "Event.h"


BEGIN_NAME_SPACE(tGis, Core)

struct TGIS_CORE_API Progress
{
	Progress(int totalValue, 
		int stepValue, 
		int maxValue = 100, 
		const char* msg = nullptr,
		const char* stepInfo = nullptr)
		:TotalValue(totalValue)
		, StepValue(stepValue)
		, MaxValue(maxValue)
		, Message(msg)
		, StepInfo(stepInfo)
	{
	}

	const int TotalValue;
	const int StepValue;
	const int MaxValue;
	const char* Message;
	const char* StepInfo;

	TGIS_DECLARE_NO_COPY_CLASS(Progress);
};

typedef IEventHandler<const Progress&> ProgressEventHandler;
typedef Event<const Progress&> ProgressEvent;

template struct TGIS_CORE_API IEventHandler<const Progress&>;
template class TGIS_CORE_API Event<const Progress&>;

END_NAME_SPACE(tGis, Core)


#endif
