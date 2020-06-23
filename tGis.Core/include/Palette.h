#pragma once

#ifndef __Palette_H__
#define __Palette_H__

#include "Helper.h"
#include "IReferenceObject.h"
#include "Progress.h"

BEGIN_NAME_SPACE(tGis, Core)

class PaletteImpl;
class MyGDALRasterDataset;
class GradientColor;

class TGIS_CORE_API Palette : public IReferenceObject
{
public:
	Palette();
	~Palette();

public:
	//使用随机颜色初始化调色板，所需entry数量小于5000个时可以保证每个entry颜色不同
	static Palette* CreatePalette(MyGDALRasterDataset* dt, int band, ProgressEventHandler * progressHanler = nullptr);
	//使用渐变颜色初始化调色板
	static Palette* CreatePalette(MyGDALRasterDataset* dt, int band, GradientColor* color, ProgressEventHandler * progressHanler = nullptr);
	//随机创建颜色
	static void GenerateColor(unsigned char *r, unsigned char *g, unsigned char *b);

public:
	Palette* Clone();

	size_t GetColorCount();
	void ForEachColor(void(__stdcall *pfunc)(int* entry, unsigned char *r, unsigned char *g, unsigned char *b, void* ud), void* ud);
	void SetColor(int entry, unsigned char r, unsigned char g, unsigned char b);
	void RemoveColor(int entry);
	void GetColor(int entry, unsigned char *r, unsigned char *g, unsigned char *b);
	bool ExistsColor(int entry);
	void ClearColor();

private:
	PaletteImpl* _impl_;
};


END_NAME_SPACE(tGis, Core)

#endif
