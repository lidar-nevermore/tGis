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
	//TODO: ����һ���ڴ����ݼ���Ȼ����ʾ���ӱ�ǩ�ĶԻ���
}


END_NAME_SPACE(tGis, Utility)



