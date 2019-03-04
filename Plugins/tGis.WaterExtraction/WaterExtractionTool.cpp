#include "WaterExtractionTool.h"
#include "WaterExtrationDialog.h"
#include "QtHelper.h"

WaterExtractionTool WaterExtractionTool::INSTANCE;

WaterExtractionTool::WaterExtractionTool()
{
	MyGDALDataset::GDALInit();
}


WaterExtractionTool::~WaterExtractionTool()
{
}

const char * WaterExtractionTool::GetName()
{
	return "Water Extraction";
}

void WaterExtractionTool::Execute()
{
	WaterExtrationDialog dlg((QWidget*)QtHelper::INSTANCE.GetMainWindow());
	if (dlg.exec() == QDialog::Accepted)
	{

	}
}
