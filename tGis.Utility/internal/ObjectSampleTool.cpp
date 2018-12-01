#include "ObjectSampleTool.h"
#include "ObjectSampleToolDialog.h"

#include "QtHelper.h"

BEGIN_NAME_SPACE(tGis, Utility)

ObjectSampleTool ObjectSampleTool::INSTANCE;

ObjectSampleTool::ObjectSampleTool()
{
	
}


ObjectSampleTool::~ObjectSampleTool()
{
}

const char * ObjectSampleTool::GetName()
{
	return "¿ÛÑù±¾Í¼";
}

void ObjectSampleTool::Execute()
{
	IMapWidget* mapWidget = TGisApplication::INSTANCE().GetCurrentMapWidget();
	if (_widget == nullptr)
	{
		_widget = new ObjectSampleToolDialog(mapWidget,(QWidget*)QtHelper::INSTANCE.GetMainWindow());
	}
	int x;
	int y;
	
	mapWidget->GetPos(&x, &y);
	_widget->move(x,y);
	_widget->UpdateChoise();
	_widget->show();
}

END_NAME_SPACE(tGis, Utility)
