#pragma once

#ifndef __MYGDALRASTERDATASET_H__
#define __MYGDALRASTERDATASET_H__

#include "Helper.h"

#include "MyGDALDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"

BEGIN_NAME_SPACE(tGis, Core)

class DataSource;

class TGIS_CORE_API MyGDALRasterDataset : public MyGDALDataset
{
public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	MyGDALRasterDataset();	
	virtual ~MyGDALRasterDataset();

protected:
	MyGDALRasterDataset(DataSource* ds, const char* path, GDALAccess eAccess = GA_Update, bool delayOpen = true, bool autoClose = true);

public:
	virtual void Open();
	virtual void Close();

protected:
	//padfTransform[1] is the x position increment of each pixel column, padfTransform[2] is the x position increment of each pixel row.
	//padfTransform[4] is the y position increment of each pixel column, padfTransform[5] is the y position increment of each pixel row.
	//In a north up image, padfTransform[1] is the pixel width, and padfTransform[5] is the pixel height. 
	//The upper left corner of the upper left pixel is at position (padfTransform[0],padfTransform[3]).
	double _geoTransform[6];

public:
	using MyGDALDataset::GetGDALDataset;
	using MyGDALDataset::SetAutoClose;
	using MyGDALDataset::GetAutoClose;

	void Attach(GDALDataset* dataset, bool autoClose = false, double noDataValue = NAN);

	void Detach();

	//Spatial position of the upper left corner of the pixel.
	void Pixel2Spatial(int pixX,int pixY,double *projX, double *projY);

	void Spatial2Pixel(double projX,double projY,double *pixX, double *pixY);

	const double* GetGeoTransform();

	bool IsNorthUp();

  private:
    CPL_DISALLOW_COPY_ASSIGN(MyGDALRasterDataset)
};



inline double MyGDALGetPixelValue(GDALDataType dt, void* pix)
{
	double pixValue = 0.0;
	switch (dt)
	{
	case GDT_Byte:
		pixValue = *((unsigned char*)pix);
		break;
	case GDT_UInt16:
		pixValue = *((unsigned short*)pix);
		break;
	case GDT_Int16:
		pixValue = *((short*)pix);
		break;
	case GDT_UInt32:
		pixValue = *((unsigned int*)pix);
		break;
	case GDT_Int32:
		pixValue = *((int*)pix);
		break;
	case GDT_Float32:
		pixValue = *((float*)pix);
		break;
	case GDT_Float64:
		pixValue = *((double*)pix);
		break;
	}

	return pixValue;
}

//inline void MyGDALSetPixelValue(GDALDataType dt,double v, void* pix)
//{
//	int dataBytes = GDALGetDataTypeSizeBytes(dt);
//	unsigned char byteV = 0;
//	unsigned short ushortV = 0;
//	short shortV = 0;
//	unsigned int uintV = 0;
//	int intV = 0;
//	float floatV = 0;
//
//	switch(dt)
//	{
//	case GDT_Byte:
//		byteV = (unsigned char)v;
//		memcpy(pix,&byteV,dataBytes);
//		break;
//	case GDT_UInt16:
//		ushortV = (unsigned short)v;
//		memcpy(pix,&ushortV,dataBytes);
//		break;
//	case GDT_Int16:
//		shortV = (short)v;
//		memcpy(pix,&shortV,dataBytes);
//		break;
//	case GDT_UInt32:
//		uintV = (unsigned int)v;
//		memcpy(pix,&uintV,dataBytes);
//		break;
//	case GDT_Int32:
//		intV = (int)v;
//		memcpy(pix,&intV,dataBytes);
//		break;
//	case GDT_Float32:
//		floatV = (float)v;
//		memcpy(pix,&floatV,dataBytes);
//		break;
//	case GDT_Float64:
//		memcpy(pix,&v,dataBytes);
//		break;
//	}
//}

inline void MyGDALSetPixelValue(GDALDataType dt, double v, void* pix)
{
	switch (dt)
	{
	case GDT_Byte:
		*((unsigned char*)pix) = (unsigned char)v;
		break;
	case GDT_UInt16:
		*((unsigned short*)pix) = (unsigned short)v;
		break;
	case GDT_Int16:
		*((short*)pix) = (short)v;
		break;
	case GDT_UInt32:
		*((unsigned int*)pix) = (unsigned int)v;
		break;
	case GDT_Int32:
		*((int*)pix) = (int)v;
		break;
	case GDT_Float32:
		*((float*)pix) = (float)v;
		break;
	case GDT_Float64:
		*((double*)pix) = v;
		break;
	}
}


END_NAME_SPACE(tGis, Core)


#endif


