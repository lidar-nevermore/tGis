#include "ObjectSampleToolDialog.h"
#include "tOrganizer.h"
#include <QVariant>
#include "QtHelper.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Utility)

ObjectSampleToolDialog::ObjectSampleToolDialog(QWidget *parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Tool | Qt::WindowCloseButtonHint);
	setFixedSize(width(),height());
}


ObjectSampleToolDialog::~ObjectSampleToolDialog()
{
}

void ObjectSampleToolDialog::UpdateChoise()
{
	bool valid = false;
	ui.cboSamples->clear();
	int providerCount = DataSourceProviderRepository::INSTANCE().GetDataSourceProviderCount();
	for (int i = 0; i < providerCount; i++)
	{
		IDataSourceProvider* provider = DataSourceProviderRepository::INSTANCE().GetDataSourceProvider(i);
		int dsCount = provider->GetConnectedDataSourceCount();
		for (int j = 0; j < dsCount; j++)
		{
			IDataSource* ds = provider->GetConnectedDataSource(j);
			if (ds->IsTypeOf(ObjectSampleDataSource::S_GetType()))
			{
				QVariant userData;
				userData.setValue<IDataSourcePtr>(ds);
				ui.cboSamples->addItem(ds->GetName(),userData);
				valid = true;
			}
		}
	}

	ui.cboLayer->clear();
	IMap* map = GetCurrentMap();
	int layerCount = map->GetLayerCount();
	for (int i = 0; i < layerCount; i++)
	{
		ILayer* layer = map->GetLayer(i);
		IDataset* dataset = layer->GetDataset();
		if (dataset->IsTypeOf(MyGDALRasterDataset::S_GetType()))
		{
			QVariant userData;
			userData.setValue<ILayerPtr>(layer);
			ui.cboLayer->addItem(layer->GetName(), userData);
			valid = valid && true;
		}
	}

	ui.chkRect->setEnabled(valid);
}


END_NAME_SPACE(tGis, Utility)

