#include <assert.h>
#include <fstream>
#include "ImageDriver.h"
#include "gdal_priv.h"

#if defined(WIN32) || defined(_WINDOWS)
#define strcasecmp _stricmp
#endif


#define BUFFERHEIGHT 100

bool ImageDriver::bsoftlock = false;

static unsigned short GDT_NUM_BYTES[] = {0, 1, 2, 2, 4, 4, 4, 8};
static unsigned short register_count = 0;



template<class T>
static bool compute_statics_buffer(T* pbuffer ,long width,long height,float null_value, double *pmin_value,double *pmax_value, double* mean, double * stddev)
{
	long data_height,block_num;

	double dMin = 1e30;
	double dMax = -1e30;

	*mean = *pmax_value = *pmin_value = *stddev = 0;

	long  num = 0;

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(*(pbuffer+i*width+j) == null_value)
			{
				continue;
			}
			else
			{
				if(*(pbuffer+i*width+j)<dMin )
					dMin = *(pbuffer+i*width+j);

				if(*(pbuffer+i*width+j)>dMax )
					dMax = *(pbuffer+i*width+j);

				*mean=*mean+*(pbuffer+i*width+j);
				num++;

			}
		}
	}

	*mean=*mean/num;
	*pmin_value=dMin;
	*pmax_value=dMax;

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(*(pbuffer+i*width+j) == null_value)
			{
				continue;
			}
			else
			{
				*stddev+=(*(pbuffer+i*width+j)-*mean)*(*(pbuffer+i*width+j)-*mean);
			}
		}
	}

	*stddev=sqrt(*stddev/num);

	return true;

}


void ImageDriver::Register()
{
	if (register_count == 0)
	{
		register_count++;
#if defined(WIN32) || defined(_WINDOWS)
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
#endif
		GDALAllRegister();
	}
	
}

ImageDriver::ImageDriver()
{
	Register();	
	
	pdataset = NULL;
	mread_only = true;
	pmin_values = pmax_values =NULL;

	m_useInvalidValue = false;
	m_invalidValue = 0.0f;
	m_local = false;
}

ImageDriver::~ImageDriver()
{
	Close();
}

bool ImageDriver::Open(const char* name, bool read_only)
{
	if (read_only)
	{
		pdataset = (GDALDataset*)GDALOpen(name, GA_ReadOnly);
	}
	else
	{
		pdataset = (GDALDataset*)GDALOpen(name, GA_Update);
	}

	path_name = name;

	if(pdataset == NULL)
		return false;

	band_num = pdataset->GetRasterCount();

	assert(band_num >= 1);

	for(int i = 0; i != band_num; ++i)
	{
		pdataset->GetRasterBand(i+1)->SetNoDataValue(0.0);
	}

	return true;
}

bool ImageDriver::Open_update(const char* name)
{

	pdataset = (GDALDataset*)GDALOpen(name, GA_Update);
	
	path_name = name;

	if(pdataset == NULL)
		return false;

	band_num = pdataset->GetRasterCount();

	assert(band_num >= 1);

	for(int i = 0; i != band_num; ++i)
	{
		pdataset->GetRasterBand(i+1)->SetNoDataValue(0.0);
	}

	return true;
}

bool ImageDriver::ReOpen()
{
	Close();

	pdataset = (GDALDataset*)GDALOpen(path_name.c_str(), GA_ReadOnly);

	if(pdataset == NULL)
		return false;

	band_num = pdataset->GetRasterCount();

	assert(band_num >= 1);
	return true;
}

bool ImageDriver::IsOpen()
{
	if(pdataset)
		return true;
	else
		return false;
}

bool ImageDriver::Create(const char* name, KQRasterInfo* fileInfo)
{
	GDALDataType datatype;
	KQBandType ldatatype=(KQBandType)fileInfo->data_type;

	if (DT_8U == ldatatype)
	{
		datatype = GDT_Byte;
	}
	else if (DT_16U == ldatatype)
	{
		datatype = GDT_UInt16;
	}
	else if (DT_16S == ldatatype)
	{
		datatype = GDT_Int16;
	}
	else if (DT_32U == ldatatype)
	{
		datatype = GDT_UInt32;
	}
	else if (DT_32S == ldatatype)
	{
		datatype =  GDT_Int32;
	}
	else if (DT_32F == ldatatype)
	{
		datatype = GDT_Float32;
	}
	else if (DT_64F == ldatatype)
	{
		datatype = GDT_Float64;
	}
	else
	{
		datatype = GDT_Unknown;
	}
	//获取文件名后缀
	std::string ext = name;

	ext = ext.substr(ext.rfind('.')+1,ext.size()-ext.rfind('.')-1);

	GDALDriver *poDriver = NULL;
	bool createtfw = false;
	if (strcasecmp(ext.c_str(),"tif")==0 || strcasecmp(ext.c_str(),"tiff")==0)
	{
		createtfw = true;
		poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
	}
	else if (strcasecmp(ext.c_str(),"img")==0)
	{
		poDriver = GetGDALDriverManager()->GetDriverByName("HFA");
	}
	else if (strcasecmp(ext.c_str(),"pix")==0)
	{
		poDriver = GetGDALDriverManager()->GetDriverByName("PCIDSK");
	}
	else if (strcasecmp(ext.c_str(),"jpg")==0)
	{
		poDriver = GetGDALDriverManager()->GetDriverByName("JPEG");
	}
	else if (strcasecmp(ext.c_str(),"bmp")==0)
	{
		poDriver = GetGDALDriverManager()->GetDriverByName("BMP");
	}
	if (!poDriver)
	{
		//ERROR_LOG("Cannot create raster dataset based on format!");
		return false;
	}

	GDALDataset *poDstDS = 0;
	char **creat_metdata = NULL;
	creat_metdata = poDriver->GetMetadata();
	
	char **pszoptions = 0;
	if(fileInfo->tiled)
	{
		//加入分块信息
        pszoptions = CSLAddString(pszoptions,"TILED=TRUE");
        char str[20];
		memset(str,0,20);
		sprintf(str,"BLOCKXSIZE=",fileInfo->tilesize);
        pszoptions = CSLAddString(pszoptions,str);
		sprintf(str,"BLOCKYSIZE=",fileInfo->tilesize);
		pszoptions = CSLAddString(pszoptions,str);

	}
	if(createtfw)
	{
       pszoptions = CSLAddString(pszoptions,"TFW=TRUE");
	}
	if (CSLFetchBoolean(creat_metdata,GDAL_DCAP_CREATE,false))
	    poDstDS = poDriver->Create(name,fileInfo->width,fileInfo->height,fileInfo->channel_num,datatype,pszoptions);

	assert(poDstDS != NULL);

	pdataset = poDstDS;

	path_name = name;

	band_num = fileInfo->channel_num;

	//for(int i = 0; i != band_num; ++i)
	//{
	//	pdataset->GetRasterBand(i+1)->SetNoDataValue(0.0);
	//}

	return true;
}

bool ImageDriver::Close()
{
	if (pdataset != NULL)
	{
		delete pdataset;
		pdataset = NULL;
	}

	if (pmin_values)
	{
		delete pmin_values;
		pmin_values = NULL;
	}
	if (pmax_values)
	{
		delete pmax_values;
		pmax_values = NULL;
	}

	return true;
}

void ImageDriver::initialize()
{
	band_num = GetBandCount();

	pmin_values = new double[band_num];
	pmax_values = new double[band_num];

	GDALRasterBand *pband = pdataset->GetRasterBand(1);
	GDALDataType datatype = pband->GetRasterDataType();

	double min_value, max_value;

	long width  = pdataset->GetRasterXSize();
	long height = pdataset->GetRasterYSize();
	long read_width = width, read_height = height;

	if(MAX(width,height)>512)
	{
		//计算需要读数据的大小
		while(true)
		{
			read_width/=2;
			read_height/=2;
			if(MAX(read_width, read_height)<=512)
				break;
		}
	}

	unsigned char* buffer = (unsigned char*)malloc(read_width*read_height*GDT_NUM_BYTES[datatype]);
	//统计各个波段的最大最小值
	for(long lband=0; lband<band_num; lband++)
	{
		switch (datatype)
		{
		case GDT_Byte:
			compute_min_max(lband+1,1,1,width,height,read_width,read_height,buffer,&min_value,&max_value);
			break;
		case GDT_UInt16:
			compute_min_max(lband+1,1,1,width,height,read_width,read_height,(unsigned short*)buffer,&min_value,&max_value);
			break;
		case GDT_Int16:
			compute_min_max(lband+1,1,1,width,height,read_width,read_height,(short*)buffer,&min_value,&max_value);
			break;
		case GDT_UInt32:
			compute_min_max(lband+1,1,1,width,height,read_width,read_height,(long*)buffer,&min_value,&max_value);
			break;
		case GDT_Int32:
			compute_min_max(lband+1,1,1,width,height,read_width,read_height,(long*)buffer,&min_value,&max_value);
			break;
		case GDT_Float32:
			compute_min_max(lband+1,1,1,width,height,read_width,read_height,(float*)buffer,&min_value,&max_value);
			break;
		case GDT_Float64:
			compute_min_max(lband+1,1,1,width,height,read_width,read_height,(double*)buffer,&min_value,&max_value);
			break;
		default:
			free(buffer);
			return;
		}

		pmin_values[lband] = min_value;
		pmax_values[lband] = max_value;
		GDALRasterBand *pband = pdataset->GetRasterBand(lband+1);
		GDALColorTable *color_table;
		color_table = pband->GetColorTable();
		if(color_table)
		{
			pmin_values[lband] = 0;
			pmax_values[lband] = 255;
		}
	}

	free(buffer);
}

bool ImageDriver::IsWritable()
{
	return mread_only;
}

long ImageDriver::GetBandCount()
{
	assert(pdataset != NULL);

	return pdataset->GetRasterCount();
}

bool ImageDriver::GetSize(long *pwidth, long *pheight)
{
	assert(pdataset != NULL);

	*pwidth  = pdataset->GetRasterXSize();
	*pheight = pdataset->GetRasterYSize();

	return true;
}

bool ImageDriver::GetExtent(KQEnvelope *pExtent) const
{
	assert(pdataset != NULL);

	double xmin,ymin,xmax,ymax;
	long width,height;

	width=pdataset->GetRasterXSize();
	height=pdataset->GetRasterYSize();

	double geo_transform[6];
	if (CE_None==pdataset->GetGeoTransform(geo_transform))
	{
		xmin = geo_transform[0] + 0*geo_transform[1] + height*geo_transform[2];
		ymin = geo_transform[3] + 0*geo_transform[4] + height*geo_transform[5];

		xmax = geo_transform[0] + width*geo_transform[1] + 0*geo_transform[2];
		ymax = geo_transform[3] + width*geo_transform[4] + 0*geo_transform[5];
	}
	else
	{
		xmin = 0;
		ymin = 0;

		xmax = width;
		ymax = height;
	}

	pExtent->init(xmin,xmax,ymin,ymax);

	return true;
}

bool ImageDriver::GetCellSize(double *xsize, double *ysize)
{
	assert(pdataset != NULL);

	KQEnvelope extent;
	long width, height;

	GetSize(&width, &height);
	GetExtent(&extent);

	*xsize = (extent.getMaxX() - extent.getMinX()) / width;
	*ysize = (extent.getMaxY() - extent.getMinY()) / height;

	return true;
}

KQBandType ImageDriver::GetBandDataType(long channel_index)
{
	assert(pdataset != NULL);

	GDALRasterBand *pBand = pdataset->GetRasterBand(channel_index);
	GDALDataType datatype = pBand->GetRasterDataType();

	KQBandType bandtype;
	switch (datatype)
	{
	case GDT_Byte:
		bandtype = DT_8U;
		break;
	case GDT_UInt16:
		bandtype = DT_16U;
		break;
	case GDT_Int16:
		bandtype = DT_16S;
		break;
	case GDT_UInt32:
		bandtype = DT_32U;
		break;
	case GDT_Int32:
		bandtype = DT_32S;
		break;
	case GDT_Float32:
		bandtype = DT_32F;
		break;
	case GDT_Float64:
		bandtype = DT_64F;
		break;
	default:
		bandtype = DT_Unknown;
		break;
	}

	return bandtype;
}

bool ImageDriver::GetBandMMValue(long channel_index, double *pmax_value, double *pmin_value)
{
	assert(pdataset!=NULL && channel_index>=1 && channel_index<=band_num);

	*pmin_value = pmin_values[channel_index-1];
	*pmax_value = pmax_values[channel_index-1];

	return true;
}

bool ImageDriver::GetBandMMValue(long channel_index, float null_value, double *pmax_value, double *pmin_value)
{
	long width,height,datatype;
	
	datatype = GetBandDataType(channel_index);
	GetSize(&width, &height);

	unsigned char* buffer = (unsigned char*)malloc(width*BUFFERHEIGHT*GDT_NUM_BYTES[datatype]);

	switch (datatype)
	{
	case DT_8U:
		compute_min_max(channel_index,null_value,buffer, pmax_value,pmin_value);
		break;
	case DT_16U:
		compute_min_max(channel_index,null_value,(unsigned short*)buffer,pmin_value, pmax_value);	
		break;
	case DT_16S:
		compute_min_max(channel_index,null_value,(short*)buffer,pmin_value, pmax_value);
		break;
	case DT_32U:
		compute_min_max(channel_index,null_value,(long*)buffer, pmin_value, pmax_value);
		break;
	case DT_32S:
		compute_min_max(channel_index,null_value,(long*)buffer, pmin_value, pmax_value);
		break;
	case DT_32F:
		compute_min_max(channel_index,null_value,(float*)buffer, pmin_value, pmax_value);
		break;
	case DT_64F:
		compute_min_max(channel_index,null_value,(double*)buffer, pmin_value, pmax_value);
		break;
	default:
		free(buffer);
		return false;
	}

	free(buffer);
	return true;
	
}

bool ImageDriver::SetExtent(KQEnvelope *pExtent)
{
	assert(pdataset != NULL);

	double geo_transform[6];
	double cell_xsize,cell_ysize;

	long width,height;
	GetSize(&width, &height);
	cell_xsize = static_cast<double>((pExtent->getMaxX() - pExtent->getMinX())/width);
	cell_ysize = static_cast<double>((pExtent->getMaxY() - pExtent->getMinY())/height);

	geo_transform[0] = pExtent->getMinX();
	geo_transform[1] = cell_xsize;
	geo_transform[2] = 0;
	geo_transform[3] = pExtent->getMaxY(); 
	geo_transform[4] = 0;
	geo_transform[5] = -cell_ysize;

	if (pdataset->SetGeoTransform(geo_transform) == CE_Failure)
	{
		return false;
	}
	return true;
}

bool ImageDriver::GeoToPixel(double x, double y, long *col,long *row, bool check)
{
	assert(pdataset != NULL);

	long width,height;
	GetSize(&width, &height);

	double geo_transform[6];
	if (CE_None == pdataset->GetGeoTransform(geo_transform))
	{
		*col = 1 + static_cast<long>(((geo_transform[5]*(x-geo_transform[0])-geo_transform[2]*
			(y-geo_transform[3]))/(geo_transform[1]*geo_transform[5]-geo_transform[4]*geo_transform[2])));
		*row = 1 + static_cast<long>(((geo_transform[4]*(x-geo_transform[0])-geo_transform[1]*
			(y-geo_transform[3]))/(geo_transform[4]*geo_transform[2]-geo_transform[1]*geo_transform[5])));

		if(check)
		{
			if(*col > width)
				*col = width;

			if(*row > height)
				*row = height;

			if(*col <= 0)
				*col = 1;

			if(*row <= 0)
				*row = 1;
		}
	}
	else
	{
		*col = static_cast<long>(x);
		*row = static_cast<long>(height -y);

		if(check)
		{
			if(*col > width)
				*col = width;

			if(*row > height)
				*row = height;

			if(*col <= 0)
				*col = 1;

			if(*row <= 0)
				*row = 1;
		}
	}

	return true;
}

bool ImageDriver::GeoToPixel(double x, double y, double *col,double *row, bool check)
{
	assert(pdataset != NULL);

	long width,height;
	GetSize(&width, &height);

	double geo_transform[6];
	if (CE_None == pdataset->GetGeoTransform(geo_transform))
	{
		*col = ((geo_transform[5]*(x-geo_transform[0])-geo_transform[2]*
			(y-geo_transform[3]))/(geo_transform[1]*geo_transform[5]-geo_transform[4]*geo_transform[2]));
		*row = ((geo_transform[4]*(x-geo_transform[0])-geo_transform[1]*
			(y-geo_transform[3]))/(geo_transform[4]*geo_transform[2]-geo_transform[1]*geo_transform[5]));

		if(check)
		{
			if(*col > width)
				*col = width;

			if(*row > height)
				*row = height;

			if(*col <= 0)
				*col = 1;

			if(*row <= 0)
				*row = 1;
		}
	}
	else
	{
		*col = x;
		*row = height -y;
	}

	return true;
}

bool ImageDriver::PixelToGeo(long col, long row, double *x, double *y)
{
	assert(pdataset != NULL);

	double geo_transform[6];
	if (CE_None == pdataset->GetGeoTransform(geo_transform))
	{
		*x = geo_transform[0]+(col - 0.5)*geo_transform[1]+(row - 0.5)*geo_transform[2];
		*y = geo_transform[3]+(col - 0.5)*geo_transform[4]+(row - 0.5)*geo_transform[5];
	}
	else
	{
		long width,height;
		GetSize(&width,&height);

		*x = (double)(col);
		*y = (double)(height-row);
	}
	return true;
}

bool ImageDriver::PixelToGeo(double col, double row, double *x, double *y)
{
	assert(pdataset != NULL);

	double geo_transform[6];
	if (CE_None == pdataset->GetGeoTransform(geo_transform))
	{
		*x = geo_transform[0] + col * geo_transform[1] + row * geo_transform[2];
		*y = geo_transform[3] + col * geo_transform[4] + row * geo_transform[5];
	}
	else
	{
		long width,height;
		GetSize(&width,&height);

		*x = (double)(col);
		*y = (double)(height-row);
	}
	return true;
}

std::string ImageDriver::GetProjection()
{
	assert(pdataset != NULL);

	return pdataset->GetProjectionRef();
}

bool ImageDriver::SetProjection(const char *psz_wkt)
{
	assert(pdataset != NULL);

	CPLErr error = pdataset->SetProjection(psz_wkt);
	
	assert(error != CE_Failure);
	/*if (error == CE_Failure)
	{
		return false;
	}*/
	return true;
}

//数据压缩
template<class T>
void normalize_data(T* raw_data, long buffer_sizex, long buffer_sizey,
		unsigned char* pbuffer, double min_value, double max_value, bool local,float invalidData)
{
	float cell_value;
	long count = buffer_sizex*buffer_sizey;

	if(local)
	{
		float minValue = 1.0e08;
		float maxValue = -1.0e08;

		for (long i=0; i<count; i++)
		{
			if(*(raw_data+i) == invalidData)
				continue;

			cell_value = float(*(raw_data+i));

			if(cell_value<minValue)
				minValue = cell_value;
			if(cell_value>maxValue)
				maxValue=cell_value;

		}

		if(minValue < maxValue)
		{
			min_value = minValue;
			max_value = maxValue;
		}
	}

	for (long i=0; i<count; i++)
	{
		cell_value = float(*raw_data);

		if(cell_value<min_value)
			cell_value = min_value;
		if(cell_value>max_value)
			cell_value=max_value;

		*pbuffer = (BYTE)((cell_value-min_value)/(float)(max_value-min_value)*255);
		++raw_data;
		++pbuffer;
	}
}	

bool ImageDriver::DataReadPyramidBandNormalize(long level, long band_index, long col, long row, 
												long width, long height,long buffer_sizex, long buffer_sizey, unsigned char *pbuffer, double& min_value,double& max_value)
{
	assert(pdataset!=NULL && band_index>=1 && band_index<=band_num);

	GDALRasterBand *pband=pdataset->GetRasterBand(band_index);
	GDALDataType datatype=pband->GetRasterDataType();
	
	unsigned char* raw_data = (unsigned char*)malloc(GDT_NUM_BYTES[datatype]*buffer_sizex*buffer_sizey);
	if(!DataReadPyramidBand(level,band_index,col,row,width,height,buffer_sizex,buffer_sizey,raw_data))
	{
		free(raw_data);
		return false;
	}

	switch(datatype)
	{
	case GDT_Byte:
		normalize_data(raw_data,	buffer_sizex,buffer_sizey,pbuffer,min_value,max_value,m_local,m_invalidValue);
		break;
	case GDT_UInt16:
		normalize_data((unsigned short*)raw_data,buffer_sizex,buffer_sizey,pbuffer,min_value,max_value,m_local,m_invalidValue);
		break;
	case GDT_Int16:
		normalize_data((short*)raw_data,buffer_sizex,buffer_sizey,pbuffer,min_value,max_value,m_local,m_invalidValue);
		break;
	case GDT_UInt32:
		normalize_data((long*)raw_data,buffer_sizex,buffer_sizey,pbuffer,min_value,max_value,m_local,m_invalidValue);
		break;
	case GDT_Int32:
		normalize_data((long*)raw_data,buffer_sizex,buffer_sizey,pbuffer,min_value,max_value,m_local,m_invalidValue);
		break;
	case GDT_Float32:
		normalize_data((float*)raw_data,buffer_sizex,buffer_sizey,pbuffer,min_value,max_value,m_local,m_invalidValue);
		break;
	case GDT_Float64:
		normalize_data((double*)raw_data,buffer_sizex,buffer_sizey,pbuffer,min_value,max_value,m_local,m_invalidValue);
		break;
	default:
		return false;
	}
	free(raw_data);
	return true;
}

bool ImageDriver::DataReadBandNormalize(long band_index, long col, long row, long width, long height, 
											  long buffer_sizex, long buffer_sizey, 
											  unsigned char *pbuffer, double& min_value,double& max_value)
{
	assert(pdataset!=NULL && band_index>=1 && band_index<=band_num);

	GDALRasterBand *pBand = pdataset->GetRasterBand(band_index);
	GDALDataType datatype = pBand->GetRasterDataType();

	unsigned char* raw_data = (unsigned char*)malloc(GDT_NUM_BYTES[datatype]*buffer_sizex*buffer_sizey);
	if(!DataReadBand(band_index,col,row,width,height,buffer_sizex,buffer_sizey,raw_data))
	{
		free(raw_data);
		return false;
	}
	switch(datatype)
	{
	case GDT_Byte:
		normalize_data(raw_data,	buffer_sizex,buffer_sizey,pbuffer,min_value,max_value,m_local,m_invalidValue);
		break;
	case GDT_UInt16:
		normalize_data((unsigned short*)raw_data,buffer_sizex,buffer_sizey,pbuffer,min_value,max_value,m_local,m_invalidValue);
		break;
	case GDT_Int16:
		normalize_data((short*)raw_data,buffer_sizex,buffer_sizey,pbuffer,min_value,max_value,m_local,m_invalidValue);
		break;
	case GDT_UInt32:
		normalize_data((long*)raw_data,buffer_sizex,buffer_sizey,pbuffer,min_value,max_value,m_local,m_invalidValue);
		break;
	case GDT_Int32:
		normalize_data((long*)raw_data,buffer_sizex,buffer_sizey,pbuffer,min_value,max_value,m_local,m_invalidValue);
		break;
	case GDT_Float32:
		normalize_data((float*)raw_data,buffer_sizex,buffer_sizey,pbuffer,min_value,max_value,m_local,m_invalidValue);
		break;
	case GDT_Float64:
		normalize_data((double*)raw_data,buffer_sizex,buffer_sizey,pbuffer,min_value,max_value,m_local,m_invalidValue);
		break;
	default:
		return false;
	}
	free(raw_data);
	return true;
}

bool ImageDriver::DataReadBand(long band_index, long col, long row, long width, long height, 
									long buffer_sizex, long buffer_sizey, void *pbuffer)
{
	assert(pdataset!=NULL && band_index>=1 && band_index<=band_num);

	GDALRasterBand *pband = pdataset->GetRasterBand(band_index);
	GDALDataType datatype = pband->GetRasterDataType();

	CPLErr error = pband->RasterIO(GF_Read,col-1,row-1,width,height,pbuffer,buffer_sizex,buffer_sizey,datatype,0,0);

	//assert(error != CE_Failure);
	if (error == CE_Failure)
	{
		return false;
	}
	return true;
}

bool ImageDriver::DataWriteBand(long band_index, long col, long row, long width, long height, void *pbuffer)
{
	if (pdataset ==NULL || band_index<1 ||  band_index > band_num)
	{
		//ERROR_LOG("the band_index has a problem!");
		return false;
	}
	assert(pdataset!=NULL && band_index>=1 && band_index<=band_num);

	GDALRasterBand *pBand = pdataset->GetRasterBand(band_index);
	GDALDataType datatype = pBand->GetRasterDataType();
	CPLErr error = pBand->RasterIO(GF_Write,col-1,row-1,width,height,pbuffer,width,height,datatype,0,0);

	assert(error != CE_Failure);
	//if (error == CE_Failure)
	//{
	//	return false;
	//}

	return true;
}

bool ImageDriver::GetPixel(long band_index, long row, long col, void *pvalue)
{
	assert(pdataset!=NULL && band_index>=1 && band_index<=band_num);
	
	GDALRasterBand *pband = pdataset->GetRasterBand(band_index);
	GDALDataType datatype = pband->GetRasterDataType();
	CPLErr error = pband->RasterIO(GF_Read, col-1, row-1, 1, 1, pvalue, 1, 1, datatype, 0, 0);

	assert(error != CE_Failure);
	/*if (error == CE_Failure)
	{
		return false;
	}*/

	return true;
}

bool ImageDriver::SetPixel(long band_index, long row, long col, void *pvalue)
{
	assert(pdataset!=NULL && band_index>=1 && band_index<=band_num);

	GDALRasterBand *pband = pdataset->GetRasterBand(band_index);
	GDALDataType datatype = pband->GetRasterDataType();
	CPLErr error = pband->RasterIO(GF_Write, col-1, row-1, 1, 1, pvalue, 1, 1, datatype, 0, 0);

	assert(error != CE_Failure);
	/*if (error == CE_Failure)
	{
		return false;
	}*/

	return true;
}

bool ImageDriver::GetChannelPalette(long channel_index, BYTE *pred, BYTE *pgreen, BYTE *pblue)
{
	assert(pdataset!=NULL && channel_index>=1 && channel_index<=band_num);

	GDALRasterBand *pband=pdataset->GetRasterBand(channel_index);
	GDALColorTable *color_table;
	GDALColorEntry color_entry;

	color_table = pband->GetColorTable();
	if(color_table == NULL)
	{
		return false;
	}
	//assert(color_table != NULL);
	/*if (!color_table)
	{
		return false;
	}*/

	long color_count = color_table->GetColorEntryCount();

	for(unsigned short i=0; i<color_count; i++)
	{
		color_table->GetColorEntryAsRGB(i, &color_entry);
		pred[i]   = static_cast<unsigned char>(color_entry.c1);
		pgreen[i] = static_cast<unsigned char>(color_entry.c2);
		pblue[i]  = static_cast<unsigned char>(color_entry.c3);
	}

	return true;
}

bool ImageDriver::SetChannelPalette(long channel_index, BYTE *pred, BYTE *pgreen, BYTE *pblue)
{
	assert(pdataset!=NULL && channel_index>=1 && channel_index<=band_num);

	GDALRasterBand *pband=pdataset->GetRasterBand(channel_index);
	GDALColorEntry color_entrys[256];
	GDALColorTable color_table;

	for(unsigned short i=0; i<256; i++)
	{
		color_entrys[i].c1 = pred[i];
		color_entrys[i].c2 = pgreen[i];
		color_entrys[i].c3 = pblue[i];
		color_entrys[i].c4 = 0;
		color_table.SetColorEntry(i, color_entrys+i);
	}

	pband->SetColorTable(&color_table);

	return true;
}
bool ImageDriver::GetBandStatisInfo(long band_index, double* min,double* max,double* mean, double* stddev)
{
	assert(pdataset!=NULL && band_index>=1 && band_index<=band_num);

	GDALRasterBand *pband=pdataset->GetRasterBand(band_index);
	pband->GetStatistics(true,true,min,max,mean,stddev);
	if(m_useInvalidValue)
	{
       GetBandStatisInfo(band_index, m_invalidValue, min, max, mean, stddev);
	}
	
	return true;
}

bool ImageDriver::GetBandStatisInfo(long band_index, float null_value, double* min,double* max,double* mean, double* stddev)
{
	long width,height,size, datatype, level = 0;
	datatype = GetBandDataType(band_index);

	if(HasPyramid())
	{
		do 
		{
			GetPyramidSize(level,&width,&height);
			size=MIN(width,height);

			if(size<2048)
				break;

			level++;
		} while (1);

		switch(datatype)
		{
		case DT_8U:
			{
				unsigned char* buffer = new unsigned char[width*height];
				DataReadPyramidBand(level,band_index,1,1,width,height,width,height,buffer);
				compute_statics_buffer(buffer,width,height,null_value,min,max,mean,stddev);
				delete []buffer;
			}
			break;
		case DT_16U:
			{
				unsigned short* buffer = new unsigned short[width*height];
				DataReadPyramidBand(level,band_index,1,1,width,height,width,height,buffer);
				compute_statics_buffer(buffer,width,height,null_value,min,max,mean,stddev);
				delete []buffer;
			}
			break;
		case DT_16S:
			{
				short *buffer = new short[width*height]; 
				DataReadPyramidBand(level,band_index,1,1,width,height,width,height,buffer);
				compute_statics_buffer(buffer,width,height,null_value,min,max,mean,stddev);
				delete []buffer;
			}
			break;
		case DT_32U:
			{
				unsigned int* buffer = new unsigned int[width*height];
				DataReadPyramidBand(level,band_index,1,1,width,height,width,height,buffer);
				compute_statics_buffer(buffer,width,height,null_value,min,max,mean,stddev);
				delete []buffer;
			}
			break;
		case DT_32F:
			{
				float *buffer = new float[width*height];
				DataReadPyramidBand(level,band_index,1,1,width,height,width,height,buffer);
				compute_statics_buffer(buffer,width,height,null_value,min,max,mean,stddev);
				delete []buffer;
			}
			break;
		case DT_64F:
			{
				double *buffer = new double[width*height];
				DataReadPyramidBand(level,band_index,1,1,width,height,width,height,buffer);
				compute_statics_buffer(buffer,width,height,null_value,min,max,mean,stddev);
				delete []buffer;
			}
			break;
		default:
			return false;
		}
	}
	else
	{
		GetSize(&width, &height);

		unsigned char* buffer = (unsigned char*)malloc(width*BUFFERHEIGHT*GDT_NUM_BYTES[datatype]);

		switch (datatype)
		{
		case DT_8U:
			compute_statis_info(band_index,null_value,buffer, min, max, mean, stddev);
			break;
		case DT_16U:
			compute_statis_info(band_index,null_value,(unsigned short*)buffer,min, max, mean, stddev);	
			break;
		case DT_16S:
			compute_statis_info(band_index,null_value,(short*)buffer,min, max, mean, stddev);
			break;
		case DT_32U:
			compute_statis_info(band_index,null_value,(long*)buffer, min, max, mean, stddev);
			break;
		case DT_32S:
			compute_statis_info(band_index,null_value,(long*)buffer, min, max, mean, stddev);
			break;
		case DT_32F:
			compute_statis_info(band_index,null_value,(float*)buffer, min, max, mean, stddev);
			break;
		case DT_64F:
			compute_statis_info(band_index,null_value,(double*)buffer, min, max, mean, stddev);
			break;
		default:
			free(buffer);
			return false;
		}

		free(buffer);
	}

	return true;
}

bool ImageDriver::GetBlockMinMax(long channel_index, long start_col, long start_row, 
	long read_width, long read_height, long buf_width, long buf_height, double *pmin_value, double *pmax_value)
{
	
	KQBandType datatype = GetBandDataType(channel_index);
	long width, height;
	GetSize(&width, &height);
	unsigned char* buffer = (unsigned char*)malloc(buf_width*buf_height*GDT_NUM_BYTES[datatype]);

	switch (datatype)
	{
	case DT_8U:
		compute_min_max(channel_index,start_col,start_row, read_width, read_height, buf_width, buf_height, buffer, pmin_value, pmax_value);
		break;
	case DT_16U:
		compute_min_max(channel_index,start_col,start_row, read_width, read_height, buf_width, buf_height, (unsigned short*)buffer, pmin_value, pmax_value);	
		break;
	case DT_16S:
		compute_min_max(channel_index,start_col,start_row, read_width, read_height, buf_width, buf_height, (short*)buffer, pmin_value, pmax_value);
		break;
	case DT_32U:
		compute_min_max(channel_index,start_col,start_row, read_width, read_height, buf_width, buf_height, (unsigned long*)buffer, pmin_value, pmax_value);
		break;
	case DT_32S:
		compute_min_max(channel_index,start_col,start_row, read_width, read_height, buf_width, buf_height, (long*)buffer, pmin_value, pmax_value);
		break;
	case DT_32F:
		compute_min_max(channel_index,start_col,start_row, read_width, read_height, buf_width, buf_height, (float*)buffer, pmin_value, pmax_value);
		break;
	case DT_64F:
		compute_min_max(channel_index,start_col,start_row, read_width, read_height, buf_width, buf_height, (double*)buffer, pmin_value, pmax_value);
		break;
	default:
		free(buffer);
		return false;
	}

	free(buffer);
	return true;
}

bool ImageDriver::HasPyramid()
{
	assert(pdataset != NULL);

	//std::string rrd_name = path_name.substr(0, path_name.find_last_of(".")) + ".rrd";
	//std::fstream fFile;
	//fFile.open(rrd_name.c_str());
	//if(fFile)
	//{
	//}
	//else
	//{
	//	return false;
	//}


	GDALRasterBand *pband = pdataset->GetRasterBand(1);

	int overview_cnt = pband->GetOverviewCount();

	if (overview_cnt <= 0)
	{
		//ERROR_LOG("The result of GDALRasterBand GetOverviewCount function is equal 0!");
		return false;
	}

	long width,height;
	GetSize(&width,&height);

	int last_w = width, last_h = height;
	for(int i = 0; i != overview_cnt; ++i)
	{
		GDALRasterBand * overview_band = pband->GetOverview(i);
		long xsize = overview_band->GetXSize();
		long ysize = overview_band->GetYSize();

		if(xsize * 2.0 <= last_w + 1 && xsize * 2.0 >= last_w - 1 && ysize * 2.0 <= last_h + 1 && ysize * 2.0 >= last_h - 1)
		{
			last_w = xsize;
			last_h = ysize;
			continue;
		}
		else
		{
			return false;
		}
	}

	return true;
}


bool ImageDriver::DataReadPyramidBand(long level, long band_index, long col, long row, 
											long width, long height,long buffer_sizex, long buffer_sizey, void *pbuffer)
{
	assert(pdataset!=NULL && band_index>=1 && band_index<=band_num);

	long r_width,r_height;
	r_width = pdataset->GetRasterXSize();
	r_height = pdataset->GetRasterYSize();

	if (col > r_width || col < 0 || row > r_height || row < 0)
		return false;

	CPLErr error;

	GDALRasterBand *pband=pdataset->GetRasterBand(band_index);
	GDALDataType datatype=pband->GetRasterDataType();
	unsigned short ocount=pband->GetOverviewCount();



	assert(level>=0&&level<=ocount);

	if (level == 0)
	{
		error=pband->RasterIO(GF_Read,col-1,row-1,width,height,pbuffer,buffer_sizex, buffer_sizey,datatype,0,0);
	}
	else
	{
		GDALRasterBand *pover_band=pband->GetOverview(level-1);
		error=pover_band->RasterIO(GF_Read,col-1,row-1,width,height,pbuffer,buffer_sizex, buffer_sizey,datatype,0,0);
	}

	assert(CE_Failure != error);
	/*if (CE_Failure == error)
	{
		ERROR_LOG("GDALRasterBand RasterIO function failed!");
		return false;
	}*/

	return true;
}

bool ImageDriver::GetPyramidSize( long level, long *pwidth, long *pheight)
{
	assert(pdataset != NULL);

	GDALRasterBand *pband = pdataset->GetRasterBand(1);
	unsigned short over_count = pband->GetOverviewCount();

	/*assert(level>=0&&level<=over_count);*/
	if(level < 0 || level > over_count)
		return false;

	GDALRasterBand *pover_band = NULL;
	if (level == 0)
	{
		pover_band = pband;
	}
	else
	{
		pover_band = pband->GetOverview(level-1);
	}

	*pwidth  = pover_band->GetXSize();
	*pheight = pover_band->GetYSize();

	return true;
}

bool ImageDriver::GetPyramidCellSize(long level, double *pcellx, double *pcelly)
{
	assert(pdataset != NULL);

	GDALRasterBand *pband = pdataset->GetRasterBand(1);
	unsigned short over_count = pband->GetOverviewCount();

	assert(level>=0&&level<=over_count);

	KQEnvelope extent;
	long lWidth, lHeight;

	GetPyramidSize(level, &lWidth, &lHeight);
	GetExtent(&extent);

	*pcellx = (extent.getMaxX() - extent.getMinX()) / lWidth;
	*pcelly = (extent.getMaxY() - extent.getMinY()) / lHeight;

	return true;
}

bool ImageDriver::GetPyramidLevelNum(long *plevel_num)
{
	assert(pdataset != NULL);

	GDALRasterBand *pband = pdataset->GetRasterBand(1);
	*plevel_num = pband->GetOverviewCount() + 1;
	return true;
}

bool ImageDriver::SetInvalidVal(float fInvalidVal)
{
	m_useInvalidValue = true;
	m_invalidValue = fInvalidVal;

	return true;
}

bool   ImageDriver::GetInvalidVal(float* pfInvalidVal)
{
	*pfInvalidVal = m_invalidValue;
	return true;
}

void ImageDriver::GetGeoTransform(double* _geotransform)
{
	pdataset->GetGeoTransform(_geotransform);
}

template<class T>
bool ImageDriver::compute_min_max(long band_index, long col, long row, long width, long height, 
									 long buffer_sizex, long buffer_sizey, T* pbuffer,
									 double *pmin_value,double *pmax_value)
{
	if (!DataReadBand(band_index,col,row,width,height,buffer_sizex,buffer_sizey,pbuffer))
		return false;

	double cell_value;
	double dMin = 1e30;
	double dMax = -1e30;
	if(m_useInvalidValue)
	{
		for (long i=0; i<buffer_sizex*buffer_sizey; i++)
		{
			cell_value = double(*pbuffer);

			if(cell_value == m_invalidValue)
			{
				++pbuffer;
				continue;
			}

			if(cell_value<dMin)
				dMin = cell_value;
			if(cell_value>dMax)
				dMax = cell_value;

			++pbuffer;
		}
	}
	else 
	{
		for (long i=0; i<buffer_sizex*buffer_sizey; i++)
		{
			cell_value = double(*pbuffer);

			if(cell_value<dMin)
				dMin = cell_value;
			if(cell_value>dMax)
				dMax = cell_value;

			++pbuffer;
		}
	}
	
	*pmin_value = dMin;
	*pmax_value = dMax;

	return true;
}

template<class T>
bool ImageDriver::compute_statis_info(long band_index, float null_value, T* pbuffer,  double* min,double* max,double* mean, double* stddev)
{
	long width,height,data_height,block_num;
	double dMin = 1e30;
	double dMax = -1e30;
	double dMean = 0, dStddev = 0;
	long count = 0;

	GetSize(&width, &height);
	block_num = (height-1)/BUFFERHEIGHT + 1;

	for(unsigned short k=0; k<block_num; k++)
	{
		data_height = (k==(block_num-1)) ? (height - BUFFERHEIGHT*k) : BUFFERHEIGHT;
		DataReadBand(band_index, 1, k*BUFFERHEIGHT+1, width, data_height, width, data_height, pbuffer);

		for(long i=0; i<width*data_height; i++)
		{
			if(*(pbuffer+i) == null_value)
			{
				continue;
			}

			count++;

			dMean += *(pbuffer+i);

			if(*(pbuffer+i)<dMin)
				dMin = *(pbuffer+i);
			if(*(pbuffer+i)>dMax)
				dMax = *(pbuffer+i);
		}

	}

	dMean /= count;

	count = 0;

	for(unsigned short k=0; k<block_num; k++)
	{
		data_height = (k==(block_num-1)) ? (height - BUFFERHEIGHT*k) : BUFFERHEIGHT;
		DataReadBand(band_index, 1, k*BUFFERHEIGHT+1, width, data_height, width, data_height, pbuffer);

		for(long i=0; i<width*data_height; i++)
		{
			if(*(pbuffer+i) == null_value)
			{
				continue;
			}

			count++;

			dStddev += (*(pbuffer+i) - dMean) * (*(pbuffer+i) - dMean);
		}

	}

	dStddev /= count;
	dStddev = sqrt(dStddev);

	*min = dMin;
	*max = dMax;
	*mean = dMean;
	*stddev = dStddev;

	return true;
}

template<class T>
bool ImageDriver::compute_min_max(long band_index, float null_value, T* pbuffer, double *pmin_value,double *pmax_value)
{
	long width,height,data_height,block_num;
	double dMin = 1e30;
	double dMax = -1e30;

	GetSize(&width, &height);
	block_num = (height-1)/BUFFERHEIGHT + 1;

	for(unsigned short k=0; k<block_num; k++)
	{
		data_height = (k==(block_num-1)) ? (height - BUFFERHEIGHT*k) : BUFFERHEIGHT;
		DataReadBand(band_index, 1, k*BUFFERHEIGHT+1, width, data_height, width, data_height, pbuffer);

		for(long i=0; i<width*data_height; i++)
		{
			if(*(pbuffer+i) == null_value)
			{
				continue;
			}

			if(*(pbuffer+i)<dMin)
				dMin = *(pbuffer+i);
			if(*(pbuffer+i)>dMax)
				dMax = *(pbuffer+i);
		}

	}

	*pmin_value = dMin;
	*pmax_value = dMax;

	return true;

}

bool ImageDriver::DataWritePyramidBand(long level, long band_index, long col, long row,
	long width, long height, long buffer_sizex, long buffer_sizey, void *pbuffer)
{
	assert(pdataset != NULL && band_index >= 1 && band_index <= band_num);

	long r_width, r_height;
	r_width = pdataset->GetRasterXSize();
	r_height = pdataset->GetRasterYSize();

	if(col > r_width || col < 0 || row > r_height || row < 0)
	{
		return false;
	}

	CPLErr error;

	GDALRasterBand *pband = pdataset->GetRasterBand(band_index);
	GDALDataType datatype = pband->GetRasterDataType();
	int ocount = pband->GetOverviewCount();

	assert(level >= 0 && level <= ocount);

	if(level == 0)
	{
		error = pband->RasterIO(GF_Write, col - 1, row - 1,  width, height, pbuffer, buffer_sizex, buffer_sizey, datatype, 0, 0);
	}
	else
	{
		GDALRasterBand *pover_band = pband->GetOverview(level - 1);
		error = pover_band->RasterIO(GF_Write, col - 1, row - 1, width, height, pbuffer, buffer_sizex, buffer_sizey, datatype, 0, 0);
	}

	assert(CE_Failure != error);

	return true;
}


