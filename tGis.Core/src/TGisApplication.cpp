#include "TGisApplication.h"


BEGIN_NAME_SPACE(tGis, Core)

TGisApplication* TGisApplication::_instance = nullptr;

TGisApplication & TGisApplication::INSTANCE()
{
	return *_instance;
}

TGisApplication::TGisApplication()
{
	_instance = this;
}

TGisApplication::~TGisApplication()
{
}

END_NAME_SPACE(tGis, Core)
