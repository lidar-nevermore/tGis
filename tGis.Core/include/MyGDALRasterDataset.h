#pragma once

#ifndef __I_MYGDALDATASET_H__
#define __I_MYGDALDATASET_H__

#include "Helper.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include "IDataset.h"
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

inline double MyGDALGetPixelValue(GDALDataType dt, void* pix)
{
	double pixValue = 0.0;
	switch(dt)
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

inline void MyGDALSetPixelValue(GDALDataType dt,double v, void* pix)
{
	switch(dt)
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

class TGISCORE_API MyGDALRasterDataset : public IDataset
{
public:
	const char* GetType();
	const char* GetName();
	const char* GetOpenString();

	bool IsOpened();
	void Open();
	void Close();
	IDataSource* GetDataSource();
	const OGRSpatialReference* GetSpatialReference();

private:
	static const char* const _type;
	OGRSpatialReference* _spatialRef;

protected:
	string _openStr;
	string _name;
	IDataSource* _dataSource;

private:
	class GDALInit
	{
	public:
		GDALInit();
	};

	static GDALInit _init;
private:
	GDALDataset* _dataset;
	bool _autoClose;
	GDALAccess _eAccess;

	double _geoTransform[6];
	OGREnvelope _envelope;

public:
	MyGDALRasterDataset();
   	MyGDALRasterDataset(const char* path, bool delayOpen = true, GDALAccess eAccess = GA_Update);
	virtual ~MyGDALRasterDataset();

public:
	static int GetSupportedFileFormatCount();
	static const vector<string>& GetSupportedFileFormatExt(int);
	static const char* GetSupportedFileFormatName(int);
	static bool GetSupportedFileFormatCreatable(int);
	static bool IsSupportedFileFormatExt(const char*);

public:

	GDALDataset* GetGDALDataset();

	void Attach(const char* file,GDALAccess eAccess,bool autoClose = true);

	void Attach(const char* file, GDALAccess eAccess, double noDataVale, bool autoClose = true);

	void AttachHDF(const char* file,GDALAccess eAccess,const int subdataset,bool autoClose = true);

	void Attach(GDALDataset* dataset,bool autoClose = false);

	void Attach(GDALDataset* dataset,double noDataVale,bool autoClose = false);

	void Detach();

	void SetAutoClose(bool autoClose);

	bool GetAutoClose();

	//Spatial position of the upper left corner of the pixel.
	void Pixel2Spatial(int pixX,int pixY,double *projX, double *projY);

	void Spatial2Pixel(double projX,double projY,double *pixX, double *pixY);

	const OGREnvelope* GetEnvelope();

	const double* GetGeoTransform();

	bool IsNorthUp();

  private:
    CPL_DISALLOW_COPY_ASSIGN(MyGDALRasterDataset)
};



END_NAME_SPACE(tGis, Core)


#endif


