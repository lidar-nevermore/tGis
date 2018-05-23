#include "IconRes.h"

#include "QCoreApplication"
#include "QFile"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


IconRes IconRes::INSTANCE;

IconRes::IconRes()
{
	_initialed = false;
}


IconRes::~IconRes()
{
}

const QIcon* IconRes::GetIcon(const QString & type)
{
	if (!_initialed)
	{
		_initialed = true;
		QString appPath = QCoreApplication::applicationDirPath();
		QString configPath(appPath);
		configPath.append("/Config/icon.config.json");

		QFile configFile(configPath);
		configFile.open(QIODevice::ReadOnly);
		QByteArray configData = configFile.readAll();
		QJsonDocument configDoc = QJsonDocument::fromJson(configData);
		QJsonObject configObject = configDoc.object();

		ParseConfigObject(configObject, appPath);

		QJsonObject::const_iterator includeObjectPos = configObject.find(QString("Include"));
		if (includeObjectPos != configObject.end())
		{
			QJsonArray arr = includeObjectPos.value().toArray();
			for (QJsonArray::iterator it = arr.begin(); it != arr.end(); it++)
			{
				QString incConfigPath = it->toString();
				incConfigPath.replace(QString("{StartupDir}"), appPath);
				QFile incConfigFile(incConfigPath);
				incConfigFile.open(QIODevice::ReadOnly);
				QByteArray incConfigData = incConfigFile.readAll();
				QJsonDocument incConfigDoc = QJsonDocument::fromJson(incConfigData);
				QJsonObject incConfigObject = incConfigDoc.object();
				ParseConfigObject(incConfigObject, appPath);
			}
		}
	}
	QMap<QString, QIcon*>::const_iterator pos = _mapIcon.find(type);
	if (pos != _mapIcon.end())
	{
		return (*pos);
	}

	return nullptr;
}

void IconRes::ParseConfigObject(const QJsonObject & configObject, const QString & appPath)
{
	QJsonObject::const_iterator iconArrayPos = configObject.find(QString("Icon"));
	if (iconArrayPos != configObject.end())
	{
		QJsonArray arr = iconArrayPos.value().toArray();
		for (QJsonArray::iterator it = arr.begin(); it != arr.end(); it++)
		{
			QJsonObject iconObject = it->toObject();
			QString type = iconObject.take(QString("Type")).toString();
			QString iconPath = iconObject.take(QString("Icon")).toString();
			iconPath.replace(QString("{StartupDir}"), appPath);
			QMap<QString, QIcon*>::const_iterator pos = _mapIcon.find(type);
			if (pos == _mapIcon.end())
			{
				QIcon* icon = new QIcon(iconPath);
				_mapIcon.insert(type, icon);
			}
		}
	}
}
