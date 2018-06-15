#pragma once

#include <QMap>
#include <QList>
#include <QPair>
#include <QIcon>
#include <QJsonObject>

class IconRes
{
public:
	static IconRes INSTANCE;

private:
	IconRes();
	~IconRes();

public:
	const QIcon* GetIcon(const QString& type, const QString& status = "Default");

private:
	void ParseConfigObject(const QJsonObject& configObject, const QString& appPath);

private:
	bool _initialed;
	QMap<QString, QList<QPair<QString, QIcon*>>> _mapIcon;

};

