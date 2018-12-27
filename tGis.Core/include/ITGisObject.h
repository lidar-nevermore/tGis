#pragma once

#ifndef __I_TGISOBJECT_H__
#define __I_TGISOBJECT_H__

#include "Helper.h"
#include "tGisCoreCfg.h"

class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct TGIS_CORE_API ITGisObject
{
	ITGisObject() {};
	virtual ~ITGisObject() {};

	static bool CanTransform(const OGRSpatialReference* from, const OGRSpatialReference* to);

	virtual const char* GetType() = 0;
	virtual bool IsTypeOf(const char* type) = 0;
	virtual bool IsTypeOf(ITGisObject* object) = 0;

private:
	ITGisObject(const ITGisObject &) = delete;
	ITGisObject &operator=(const ITGisObject &) = delete;
};

typedef ITGisObject* ITGisObjectPtr;

END_NAME_SPACE(tGis, Core)


#endif
