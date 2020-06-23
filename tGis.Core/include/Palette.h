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
	//ʹ�������ɫ��ʼ����ɫ�壬����entry����С��5000��ʱ���Ա�֤ÿ��entry��ɫ��ͬ
	static Palette* CreatePalette(MyGDALRasterDataset* dt, int band, ProgressEventHandler * progressHanler = nullptr);
	//ʹ�ý�����ɫ��ʼ����ɫ��
	static Palette* CreatePalette(MyGDALRasterDataset* dt, int band, GradientColor* color, ProgressEventHandler * progressHanler = nullptr);
	//���������ɫ
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
