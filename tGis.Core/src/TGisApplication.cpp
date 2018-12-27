#include "TGisApplication.h"
#include "PluginManager.h"

BEGIN_NAME_SPACE(tGis, Core)

TGisApplication* TGisApplication::_instance = nullptr;

Event<const char*, const char*, const char*, const char*> TGisApplication::LoadPluginEvent;

TGisApplication & TGisApplication::INSTANCE()
{
	return *_instance;
}

TGisApplication::TGisApplication()
{
	if (_instance != NULL)
		throw exception("Only allow one instance!");	
	_instance = this;
	PluginManager::INSTANCE().LoadPlugins();
}

TGisApplication::~TGisApplication()
{
}

END_NAME_SPACE(tGis, Core)
