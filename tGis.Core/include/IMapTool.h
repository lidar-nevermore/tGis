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

	//响应相同消息的两个工具不兼容
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
	//用于标识本工具需要响应那些消息，一位表示一种消息
	//如果需要响应某种消息则响应的位为1
	//具体某位代表那种消息由UI模块自行定义
	uint64_t _msgFlag;
};

typedef IMapTool* IMapToolPtr;

END_NAME_SPACE(tGis, Core)


#endif
