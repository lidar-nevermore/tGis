#include "TakeObjectSampleTool.h"



BEGIN_NAME_SPACE(tGis, Utility)

TakeObjectSampleTool::TakeObjectSampleTool()
{
}


TakeObjectSampleTool::~TakeObjectSampleTool()
{
}

void TakeObjectSampleTool::SetRasterDataset(MyGDALRasterDataset * dataset)
{
	_dataset = dataset;
}

void TakeObjectSampleTool::SetObjectSampleDataSource(ObjectSampleDataSource * samples)
{
	_samples = samples;
}

void TakeObjectSampleTool::MouseUp(void *ev)
{
	DrawRectTool::MouseUp(ev);
	//TODO: 创建一个内存数据集，然后显示到加标签的对话框
}


END_NAME_SPACE(tGis, Utility)



