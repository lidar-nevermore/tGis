#pragma once

#ifndef __GradientColor_H__
#define __GradientColor_H__

#include "Helper.h"
#include "IInclusionObject.h"

namespace tinyxml2
{
	class XMLDocument;
	class XMLElement;
}

BEGIN_NAME_SPACE(tGis, Core)

class GradientColorImpl;

class TGIS_CORE_API GradientColor : public IInclusionObject
{
	friend class GradientColorRepository;
public:
	GradientColor();
	~GradientColor();

public:
	void AddKeyColor(unsigned char r, unsigned char g, unsigned char b, double pos);
	size_t GetKeyColorCount();
	void GetKeyColor(size_t idx, unsigned char *r, unsigned char *g, unsigned char *b, double *pos);
	void SetKeyColor(size_t idx, unsigned char r, unsigned char g, unsigned char b, double pos);

protected:
	void FromXml(tinyxml2::XMLElement* xelem);

	void ToXml(tinyxml2::XMLElement* xelem);

public:
	void GetColor(unsigned char *r, unsigned char *g, unsigned char *b, double pos);
	void GetColor(double *r, double *g, double *b, double pos);

private:
	GradientColorImpl* _impl_;
};


END_NAME_SPACE(tGis, Core)

#endif
