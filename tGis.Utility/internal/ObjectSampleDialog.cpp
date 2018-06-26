#include "ObjectSampleDialog.h"
#include "ObjectSampleLabelDialog.h"
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

	_osm = nullptr;
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	ui.mapWidget->SetMap(&_map);
	ui.mapWidget->LoadedEvent += &_MapWidgetLoadedEventHandler;
	QPushButton* button = ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok);
	button->setEnabled(false);
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(ui.btnNewClass, &QPushButton::clicked, this, &ObjectSampleDialog::on_btnNewClass_clicked);
	connect(ui.lstClass, &QListWidget::itemSelectionChanged, this, &ObjectSampleDialog::on_lstClass_itemSelectionChanged);
}


ObjectSampleDialog::~ObjectSampleDialog()
{
}

void ObjectSampleDialog::SetObjectSampleDataSource(ObjectSampleDataSource * samples)
{
	_samples = samples;
	int osmCount = samples->GetObjectSampleMetadataCount();
	for (int i = 0; i < osmCount; i++)
	{
		ObjectSampleMetadata * osm = samples->GetObjectSampleMetadata(i);
		QListWidgetItem* pItem = CreateObjectSampleItem(osm);
		ui.lstClass->addItem(pItem);
	}
}

QListWidgetItem * ObjectSampleDialog::CreateObjectSampleItem(ObjectSampleMetadata * osm)
{
	QListWidgetItem* pItem = new QListWidgetItem();
	QString osmName = QString::fromUtf8(osm->Name);
	pItem->setText(osmName);

	QVariant udOsm;
	udOsm.setValue<>(osm);
	pItem->setData(DataRole,udOsm);

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

void ObjectSampleDialog::on_lstClass_itemSelectionChanged()
{
	_osm = nullptr;
	QList<QListWidgetItem*> items = ui.lstClass->selectedItems();
	QPushButton* button = ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok);
	button->setEnabled(items.size() > 0);
	if (items.size() == 0)
		return;
	QListWidgetItem* item = items[0];
	_osm = item->data(DataRole).value<ObjectSampleMetadataPtr>();
	ui.leName->setText(item->text());
	ui.leLabel->setText(QString::number(_osm->Label));
	ui.leMinObjectWidth->setText(QString::number(_osm->MinObjectWidth));
	ui.leMaxObjectWidth->setText(QString::number(_osm->MaxObjectWidth));
	ui.leMinObjectHeight->setText(QString::number(_osm->MinObjectHeight));
	ui.leMaxObjectHeight->setText(QString::number(_osm->MaxObjectHeight));
	ui.leMinPixelSize->setText(QString::number(_osm->MinPixelSize));
	ui.leMaxPixelSize->setText(QString::number(_osm->MaxPixelSize));
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
		QListWidgetItem* pItem = CreateObjectSampleItem(osm);
		ui.lstClass->addItem(pItem);
		_samples->SaveObjectSampleMetadata();
	}
}



END_NAME_SPACE(tGis, Utility)

