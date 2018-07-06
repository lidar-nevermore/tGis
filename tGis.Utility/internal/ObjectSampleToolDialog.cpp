#include "ObjectSampleToolDialog.h"
#include "tOrganizer.h"
#include <QVariant>
#include "QtHelper.h"
#include "MapPanTool.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Utility)

ObjectSampleToolDialog::ObjectSampleToolDialog(IMapWidget* mapWidget, QWidget *parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Tool | Qt::WindowCloseButtonHint);
	setFixedSize(width(),height());
	_mapWidget = mapWidget;
	_takeObjectSampleTool.SetEnabled(false);

	IMap* map = mapWidget->GetMap();
	map->LayerAddedEvent.Add(this, &ObjectSampleToolDialog::LayerAdded);
	map->LayerRemovedEvent.Add(this, &ObjectSampleToolDialog::LayerRemoved);
	map->LayerClearedEvent.Add(this, &ObjectSampleToolDialog::LayerCleared);
	DataSourceProviderRepository::INSTANCE().AfterDataSourceConnectEvent.Add(this, &ObjectSampleToolDialog::AfterDataSourceConnect);
	DataSourceProviderRepository::INSTANCE().BeforeDataSourceDisconnectEvent.Add(this, &ObjectSampleToolDialog::BeforeDataSourceDisconnect);

	mapWidget->AddMapTool(&_takeObjectSampleTool);
	mapWidget->MapToolAddedEvent.Add(this, &ObjectSampleToolDialog::MapToolAddedOrChanged);
	mapWidget->MapToolChangedEvent.Add(this, &ObjectSampleToolDialog::MapToolAddedOrChanged);

	connect(ui.chkRect, &QCheckBox::toggled, this, &ObjectSampleToolDialog::on_chkRect_toggled);
	connect(ui.cboLayer, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ObjectSampleToolDialog::on_cboLayer_currentIndexChanged);
	connect(ui.cboSamples, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ObjectSampleToolDialog::on_cboSamples_currentIndexChanged);
}

ObjectSampleToolDialog::~ObjectSampleToolDialog()
{
}

void ObjectSampleToolDialog::MapToolAddedOrChanged(IMapWidget *mapWidget, IMapTool *mapTool)
{
	DrawRectTool* drtool = dynamic_cast<DrawRectTool* >(mapTool);
	MapPanTool* ptool = dynamic_cast<MapPanTool* >(mapTool);
	if ((drtool != nullptr || ptool != nullptr) && mapTool->GetEnabled())
	{
		if (mapTool != &_takeObjectSampleTool)
		{
			_takeObjectSampleTool.SetEnabled(false);
			ui.chkRect->setChecked(false);
		}
	}
}

void ObjectSampleToolDialog::LayerAdded(IMapPtr map, ILayerPtr layer)
{
	IDataset* dataset = layer->GetDataset();
	if (dataset->IsTypeOf(MyGDALRasterDataset::S_GetType()))
	{
		QVariant userData;
		userData.setValue<ILayerPtr>(layer);
		ui.cboLayer->addItem(layer->GetName(), userData);
	}
	if (ui.cboLayer->count() > 0)
	{
		ui.chkRect->setEnabled(true);
	}
}

void ObjectSampleToolDialog::LayerRemoved(IMapPtr map, ILayerPtr layer)
{
	IDataset* dataset = layer->GetDataset();
	if (dataset->IsTypeOf(MyGDALRasterDataset::S_GetType()))
	{
		int itemCount = ui.cboLayer->count();
		for (int i = 0; i < itemCount; i++)
		{
			ILayer* iLayer = ui.cboLayer->itemData(i).value<ILayerPtr>();
			if (iLayer == layer)
			{
				ui.cboLayer->removeItem(i);
				break;
			}
		}
	}

	if (layer == _selectedLayer)
	{
		_takeObjectSampleTool.SetEnabled(false);
	}
	
	if (ui.cboLayer->count() == 0)
	{
		ui.chkRect->setChecked(false);
		ui.chkRect->setEnabled(false);
	}
}

void ObjectSampleToolDialog::LayerCleared(IMapPtr map)
{
	ui.cboLayer->clear();
	_takeObjectSampleTool.SetEnabled(false);
	ui.chkRect->setChecked(false);
	ui.chkRect->setEnabled(false);
}

void ObjectSampleToolDialog::AfterDataSourceConnect(IDataSourceProvider * provider, IDataSource * ds)
{
	if (ds->IsTypeOf(ObjectSampleDataSource::S_GetType()))
	{
		QVariant userData;
		userData.setValue<IDataSourcePtr>(ds);
		ui.cboSamples->addItem(ds->GetName(), userData);
	}
	if (ui.cboSamples->count() > 0)
	{
		ui.chkRect->setEnabled(true);
	}
}

void ObjectSampleToolDialog::BeforeDataSourceDisconnect(IDataSourceProvider * provider, IDataSource * ds)
{
	if (ds->IsTypeOf(ObjectSampleDataSource::S_GetType()))
	{
		int itemCount = ui.cboSamples->count();
		for (int i = 0; i < itemCount; i++)
		{
			IDataSource* iDs = ui.cboSamples->itemData(i).value<IDataSourcePtr>();
			if (iDs == ds)
			{
				ui.cboSamples->removeItem(i);
				break;
			}
		}
	}

	if (ui.cboSamples->count() == 0)
	{
		ui.chkRect->setChecked(false);
		ui.chkRect->setEnabled(false);
	}
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

void ObjectSampleToolDialog::closeEvent(QCloseEvent * event)
{
	ui.chkRect->setChecked(false);
	_takeObjectSampleTool.SetEnabled(false);
}

void ObjectSampleToolDialog::on_cboSamples_currentIndexChanged(int index)
{
	_selectedSamples = (ObjectSampleDataSource*)ui.cboSamples->currentData().value<IDataSourcePtr>();
	if (_selectedSamples != nullptr)
		_takeObjectSampleTool.SetObjectSampleDataSource(_selectedSamples);
}

void ObjectSampleToolDialog::on_cboLayer_currentIndexChanged(int index)
{
	_selectedLayer = ui.cboLayer->currentData().value<ILayerPtr>();
	if(_selectedLayer != nullptr)
		_takeObjectSampleTool.SetRasterLayer(_selectedLayer);
}

void ObjectSampleToolDialog::on_chkRect_toggled(bool checked)
{
	if (checked)
	{
		_selectedSamples = (ObjectSampleDataSource*)ui.cboSamples->currentData().value<IDataSourcePtr>();
		_selectedLayer = ui.cboLayer->currentData().value<ILayerPtr>();
		_takeObjectSampleTool.SetObjectSampleDataSource(_selectedSamples);
		_takeObjectSampleTool.SetRasterLayer(_selectedLayer);
	}

	_takeObjectSampleTool.SetEnabled(checked);
}


END_NAME_SPACE(tGis, Utility)

