#pragma once

#ifndef __GradientColorRepository_H__
#define __GradientColorRepository_H__

#include "Helper.h"


namespace tinyxml2
{
	class XMLDocument;
	class XMLElement;
}

BEGIN_NAME_SPACE(tGis, Core)

class GradientColorRepositoryImpl;
class GradientColor;

class TGIS_CORE_API GradientColorRepository
{
public:
	static GradientColorRepository* _instance;
	static GradientColorRepository* INSTANCE();

public:
	~GradientColorRepository();

private:
	GradientColorRepository();
	GradientColorRepository(const GradientColorRepository &) = delete;
	GradientColorRepository &operator=(const GradientColorRepository &) = delete;

public:
	size_t GetGradientColorCount();
	GradientColor* GetGradientColor(size_t);

protected:
	void AddGradientColor(GradientColor* color);

protected:
	void FromXml(tinyxml2::XMLDocument* doc);

	void ToXml(tinyxml2::XMLDocument* doc);

private:
	GradientColorRepositoryImpl* _impl_;
};


END_NAME_SPACE(tGis, Core)

#endif
