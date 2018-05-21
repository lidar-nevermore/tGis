#pragma once

#ifndef __LAYER_H__
#define __LAYER_H__

#include "Helper.h"

#include "ILayer.h"

#include <string>

using namespace std;

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;

class TGISCORE_API Layer : public ILayer
{
public:
	Layer();
	virtual ~Layer();

public:
	virtual const char* GetName();
	virtual void SetName(const char* name);
	virtual bool GetVisible();
	virtual void SetVisible(bool);
	virtual float GetOpacity();
	virtual void SetOpacity(float);

protected:
	string _name;
	bool _visible;
	float _opacity;
	unsigned char _alpha;
};

END_NAME_SPACE(tGis, Core)

#endif
