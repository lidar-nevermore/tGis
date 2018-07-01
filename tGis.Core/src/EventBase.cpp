#include "EventBase.h"
#include "elr_mpl.h"


BEGIN_NAME_SPACE(tGis, Core)

class EventSystemInitializer
{
public:
	static EventSystemInitializer INSTANCE;

public:
	EventSystemInitializer()
	{
		elr_mpl_init();
	}

	~EventSystemInitializer()
	{
		elr_mpl_finalize();
	}
};

EventSystemInitializer EventSystemInitializer::INSTANCE;

EventSystemInitializer*  EventBase::initializer = &EventSystemInitializer::INSTANCE;


END_NAME_SPACE(tGis, Core)
