#include "ObjectSampleDialog.h"
#include "ObjectSampleLabelDialog.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVariant>
#include "QMessageBox"
#include "qevent.h"

#include "QtHelper.h"

BEGIN_NAME_SPACE(tGis, Utility)

ObjectSampleDialog::ObjectSampleDialog(QWidget *parent)
	:QDialog(parent)
	, _MapWidgetLoadedEventHandler(this,&ObjectSampleDialog::OnMapWidgetLoaded)
{
	ui.setupUi(this);
	_model = new QStandardItemModel();
	ui.lstClass->setModel(_model);
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	ui.mapWidget->SetMap(&_map);
	ui.mapWidget->LoadedEvent += &_MapWidgetLoadedEventHandler;
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(ui.btnNewClass, &QPushButton::clicked, this, &ObjectSampleDialog::on_btnNewClass_clicked);
}


ObjectSampleDialog::~ObjectSampleDialog()
{
}

void ObjectSampleDialog::SetObjectSampleDataSource(ObjectSampleDataSource * samples)
{
	int osmCount = samples->GetObjectSampleMetadataCount();
	for (int i = 0; i < osmCount; i++)
	{
		ObjectSampleMetadata * osm = samples->GetObjectSampleMetadata(i);
		QStandardItem* pItem = CreateObjectSampleItem(osm);
		_model->appendRow(pItem);
	}
}

QStandardItem * ObjectSampleDialog::CreateObjectSampleItem(ObjectSampleMetadata * osm)
{
	QStandardItem* pItem = new QStandardItem();
	QString osmName = QString::fromUtf8(osm->Name);
	pItem->setText(osmName);
	pItem->setEditable(false);
	pItem->setCheckable(false);

	QVariant udOsm;
	udOsm.setValue<>(osm);
	pItem->setData(udOsm, DataRole);

	return pItem;
}

void ObjectSampleDialog::OnMapWidgetLoaded(IMapWidget * mapWidget, int width, int height)
{
	_map.AddLayer(_layer);
	const OGREnvelope* envelope = _layer->GetEnvelope();
	ui.mapWidget->GetGeoSurface()->IncludeEnvelope(envelope);
	IGeoSurface* surf = ui.mapWidget->GetGeoSurface();
	surf->Spatial2Surface(_sampleAreaLeft, _sampleAreaTop, &_sampleRect._left, &_sampleRect._top);
	surf->Spatial2Surface(_sampleAreaRight, _sampleAreaBottom, &_sampleRect._right, &_sampleRect._bottom);
	ui.mapWidget->GetOverlayLayer()->AddOverlayObject(&_sampleRect);
}

void ObjectSampleDialog::on_btnNewClass_clicked(bool checked)
{
	ObjectSampleLabelDialog dlg((QWidget*)this);
	if (dlg.exec() == QDialog::Accepted)
	{
		QString name = dlg.GetClassName();

		int label = dlg.GetClassLabel();
		ObjectSampleMetadata* osm = _samples->GetObjectSampleMetadataByLabel(label);
		if (osm != nullptr)
		{
			QMessageBox::information((QWidget*)this,
				QStringLiteral("Warning"),
				QStringLiteral("所取的类别标签值已经存在！"),
				QMessageBox::Yes, QMessageBox::Yes);
			return;
		}

		QByteArray bytes = name.toUtf8();
		if (bytes.length() > ObjectSampleNameLength) {
			QMessageBox::information((QWidget*)this,
				QStringLiteral("Warning"),
				QStringLiteral("所指定的类别名称过长！"),
				QMessageBox::Yes, QMessageBox::Yes);
			return;
		}

		ObjectSampleMetadata osmn;
		memset(&osmn, 0, sizeof(ObjectSampleMetadata));
		osmn.Label = label;
		int i = 0;
		for (QByteArray::iterator it = bytes.begin(); it != bytes.end(); it++)
		{
			osmn.Name[i] = (*it);
			i++;
		}

		osm = _samples->AddObjectSampleMetadata(&osmn);
		QStandardItem* pItem = CreateObjectSampleItem(osm);
		_model->appendRow(pItem);
	}
}



END_NAME_SPACE(tGis, Utility)

