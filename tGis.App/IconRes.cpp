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

const QIcon* IconRes::GetIcon(const QString & type, const QString& status)
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
	QMap<QString, QList<QPair<QString, QIcon*>>>::const_iterator pos = _mapIcon.find(type);
	if (pos != _mapIcon.end())
	{
		const QList<QPair<QString, QIcon*>> & icons = *pos;
		const QIcon* ret = nullptr;

		for (QList<QPair<QString, QIcon*>>::const_iterator it = icons.begin(); it != icons.end(); it++)
		{
			const QPair<QString, QIcon*> & icon = *it;
			if (icon.first == "Default")
				ret = icon.second;
			if (icon.first == status)
			{
				ret = icon.second;
				break;
			}
		}

		return ret;
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
			QJsonObject iconGroup = iconObject.take(QString("Icon")).toObject();
			QStringList keys = iconGroup.keys();

			for (QStringList::iterator it2 = keys.begin(); it2 != keys.end(); it2++)
			{
				QString iconPath = iconGroup.take(*it2).toString();
				iconPath.replace(QString("{StartupDir}"), appPath);
				QMap<QString, QList<QPair<QString, QIcon*>>>::iterator pos = _mapIcon.find(type);
				if (pos == _mapIcon.end())
				{
					QList<QPair<QString, QIcon*>> icons;
					QIcon* icon = new QIcon(iconPath);
					icons.append(QPair<QString, QIcon*>(*it2, icon));
					_mapIcon.insert(type, icons);
				}
				else
				{
					QList<QPair<QString, QIcon*>>& icons = *pos;
					QIcon* icon = new QIcon(iconPath);
					icons.append(QPair<QString, QIcon*>(*it2, icon));
				}
			}

		}
	}
}
