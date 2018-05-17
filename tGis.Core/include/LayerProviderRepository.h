#pragma once

#ifndef __MAPREPOSITORY_H__
#define __MAPREPOSITORY_H__

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

class TGISCORE_API LayerProviderRepository
{
public:
	static const LayerProviderRepository INSTANCE;

public:


private:
	LayerProviderRepository();
	~LayerProviderRepository();
	LayerProviderRepository(const LayerProviderRepository &) = delete;
	LayerProviderRepository &operator=(const LayerProviderRepository &) = delete;
};

END_NAME_SPACE(tGis, Core)


#endif


