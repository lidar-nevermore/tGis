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
	if (_widget == nullptr)
	{
		_widget = new ObjectSampleToolDialog((QWidget*)QtHelper::INSTANCE.GetMainWindow());
	}
	int x;
	int y;
	IMapWidget* mapWidget = GetCurrentMapWidget();
	mapWidget->GetGeoSurface()->GetViewPos(&x, &y);
	_widget->move(x,y);
	_widget->UpdateChoise();
	_widget->show();
}

END_NAME_SPACE(tGis, Utility)
