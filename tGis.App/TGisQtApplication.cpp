#include "TGisQtApplication.h"



TGisQtApplication::TGisQtApplication()
{
}


TGisQtApplication::~TGisQtApplication()
{
}

IMap * TGisQtApplication::GetCurrentMap()
{
	return _curMap;
}

IMapWidget * TGisQtApplication::GetCurrentMapWidget()
{
	return _curMapWidget;
}
