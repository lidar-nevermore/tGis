#include "elr_mpl.h"
#include "Helper.h"
#include "ITGisObject.h"

BEGIN_NAME_SPACE(tGis, Core)

struct MemoryPoolInitializer
{
	MemoryPoolInitializer() 
	{
		elr_mpl_init();
	}

	~MemoryPoolInitializer()
	{
		elr_mpl_finalize();
	}
};

MemoryPoolInitializer memoryPoolInitializer;

END_NAME_SPACE(tGis, Core)


