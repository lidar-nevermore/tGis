#pragma once

#include <QMap>
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
	const QIcon* GetIcon(const QString& type);

private:
	void ParseConfigObject(const QJsonObject& configObject, const QString& appPath);

private:
	bool _initialed;
	QMap<QString, QIcon*> _mapIcon;

};

