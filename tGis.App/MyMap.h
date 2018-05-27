#pragma once

#include "tEntity.h"

using namespace tGis::Core;

//请确保添加至该地图的图层都是使用对应的Provider的CreateLayer方法创建出来的
class MyMap : public Map
{
public:
	MyMap();
	~MyMap();
};

