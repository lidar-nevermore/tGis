#pragma once

#ifndef __I_MAPTOOL_H__
#define __I_MAPTOOL_H__

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IMapWidget;

struct TGIS_CORE_API IMapTool
{
	friend class MapWidget;

	virtual IMapWidget* GetMapWidget() = 0;

	//��Ӧ��ͬ��Ϣ���������߲�����
	virtual bool IsCompatible(IMapTool* tool)
	{
		if ((tool->_msgFlag & _msgFlag) == (uint64_t)0)
			return true;
		return false;
	};

	virtual void SetEnabled(bool enabled) = 0;
	virtual bool GetEnabled() = 0;

	IMapTool() {};
	virtual ~IMapTool() {};

protected:
	virtual void SetMapWidget(IMapWidget* mapWidget) = 0;

private:
	IMapTool(const IMapTool &) = delete;
	IMapTool &operator=(const IMapTool &) = delete;

protected:
	//���ڱ�ʶ��������Ҫ��Ӧ��Щ��Ϣ��һλ��ʾһ����Ϣ
	//�����Ҫ��Ӧĳ����Ϣ����Ӧ��λΪ1
	//����ĳλ����������Ϣ��UIģ�����ж���
	uint64_t _msgFlag;
};

typedef IMapTool* IMapToolPtr;

END_NAME_SPACE(tGis, Core)


#endif
