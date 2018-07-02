#pragma once

#ifndef __TAKEOBJECTSAMPLETOOL_H__
#define __TAKEOBJECTSAMPLETOOL_H__


#include "tVisualize.h"

#include "DrawRectTool.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Utility)

class TakeObjectSampleTool : public DrawRectTool
{
public:
	TakeObjectSampleTool();
	~TakeObjectSampleTool();

private:
	ILayer* _layer;
	MyGDALRasterDataset* _dataset;
	ObjectSampleDataSource* _samples;

public:
	void SetRasterLayer(ILayer* layer);
	void SetObjectSampleDataSource(ObjectSampleDataSource* samples);

public:
	virtual void MouseUp(void*);
};


END_NAME_SPACE(tGis, Utility)


#endif

