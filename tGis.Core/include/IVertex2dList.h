#pragma once

#ifndef __IVertex2dList_H__
#define __IVertex2dList_H__

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct TGIS_CORE_API IVertex2dList
{
	IVertex2dList() {};
	virtual ~IVertex2dList() {};

	virtual size_t GetVertexCount() = 0;
	virtual void GetVertex(size_t pos, double* x, double* y) = 0;
	virtual void AddVertex(double x, double y) = 0;
	virtual void SetVertex(size_t pos, double x, double y) = 0;
	virtual void RemoveVertex(size_t pos) = 0;
	virtual void ClearVertex() = 0;
};

END_NAME_SPACE(tGis, Core)


#endif
