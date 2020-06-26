#pragma once

#ifndef __TGIS_GUI_CFG_H__
#define __TGIS_GUI_CFG_H__

#include "Helper.h"
#include "tGis_Core.h"


/**
*                    _ooOoo_
*                   o8888888o
*                   88" . "88
*                   (| -_- |)
*                    O\ = /O
*                ____/`---'\____
*              .   ' \\| |// `.
*               / \\||| : |||// \
*             / _||||| -:- |||||- \
*               | | \\\ - /// | |
*             | \_| ''\---/'' | |
*              \ .-\__ `-` ___/-. /
*           ___`. .' /--.--\ `. . __
*        ."" '< `.___\_<|>_/___.' >'"".
*       | | : `- \`.;`\ _ /`;.`/ - ` : | |
*         \ \ `-. \_ __\ /__ _/ .-` / /
* ======`-.____`-.___\_____/___.-`____.-'======
*                    `=---='
*
* .............................................
*          佛祖保佑wxWidget的BUG别害人
*/

#ifdef TGIS_GUI_EXPORTS
  #define TGIS_GUI_API TGIS_SYMBOL_EXPORT
#else
  #define TGIS_GUI_API TGIS_SYMBOL_IMPORT
#endif

#endif
