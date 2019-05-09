#ifndef _IMAGE_DRIVER_H
#define _IMAGE_DRIVER_H

class GDALDataset;

struct KQRasterInfo
{
	KQRasterInfo():tiled(false){}
	long width;
	long height;
	long channel_num;
	long data_type;
	bool tiled;
	unsigned short tilesize;
};

typedef unsigned char BYTE;

	struct KQEnvelope 
	{
	public:
		/*! x坐标最小 */
		double minx;
		/*! x坐标最大 */
		double maxx;
		/*! y坐标最小 */
		double miny;
		/*! y坐标最大 */
		double maxy;

	public:

		void init(double x1, double x2, double y1, double y2)
		{
			if (x1 < x2) 
			{
				minx = x1;
				maxx = x2;
			} 
			else 
			{
				minx = x2;
				maxx = x1;
			}
			if (y1 < y2)
			{
				miny = y1;
				maxy = y2;
			} 
			else 
			{
				miny = y2;
				maxy = y1;
			}
		}


		double getMaxY() const{return maxy;}

		double getMaxX() const{return maxx;}

		double getMinY() const{return miny;}

		double getMinX() const{return minx;}

		double getwidth() const{return maxx - minx;}

		double getheight() const{return maxy - miny;}

	};

	/**
	* @brief 图像数据类型，8U，16U等
	*/
	typedef enum {
		/*! 未知数据类型 */			DT_Unknown	= 0,
		/*! 8位无符号整形 */        DT_8U		= 1,
		/*! 16位无符号整形 */       DT_16U		= 2,
		/*! 16位有符号整形 */       DT_16S		= 3,
		/*! 32位无符号整形 */		DT_32U		= 4,
		/*! 32位有符号整形 */       DT_32S		= 5,
		/*! 32位浮点 */				DT_32F		= 6,
		/*! 64位浮点 */				DT_64F		= 7,
		/*! 复数 Int16 */           DT_16CI		= 8,
		/*! 复数 Int32 */           DT_32CI		= 9,
		/*! 复数 Float32 */         DT_32CF		= 10,
		/*! 复数 Float64 */         DT_64CF		= 11,
		/* 最大类型 + 1 */			DT_TypeCount= 12
	} KQBandType;

	class ImageDriver
	{
	public:
		ImageDriver();
		~ImageDriver();

	public:

		void Register();

		bool Open(const char* name, bool read_only = true);

		bool Open_update(const char* name);

		bool ReOpen();

		bool IsOpen();

		bool Create(const char* name, KQRasterInfo* fileInfo);

		bool Close();

		std::string GetFullName() const {return path_name;}

		bool IsWritable();

		long GetBandCount();

		bool GetExtent(KQEnvelope *pExtent) const;

		bool GetSize(long *pwidth, long *pheight);

		bool GetCellSize (double *xsize, double *ysize);

		KQBandType GetBandDataType(long channel_index);

		bool GetBandMMValue(long channel_index, double *pmax_value, double *pmin_value);

		bool GetBandMMValue(long channel_index, float null_value, double *pmax_value, double *pmin_value);

		bool SetExtent(KQEnvelope *pExtent);

		bool GeoToPixel (double x, double y, long *col,long *row, bool check = false);

		bool GeoToPixel (double x, double y, double *col,double *row, bool check = false);

		bool PixelToGeo (long col, long row, double *x, double *y);

		bool PixelToGeo (double col, double row, double *x, double *y);

		std::string GetProjection();

		bool SetProjection(const char *psz_wkt);

		bool GetChannelPalette(long channel_index, BYTE *pred, BYTE *pgreen, BYTE *pblue);

		bool SetChannelPalette(long channel_index, BYTE *pred, BYTE *pgreen, BYTE *pblue);

		bool GetBandStatisInfo(long band_index, double* min,double* max,double* mean, double* stddev);

		bool GetBandStatisInfo(long band_index, float null_value, double* min,double* max,double* mean, double* stddev);

		bool GetBlockMinMax(long channel_index, long start_col, long start_row, 
			long read_width, long read_height, long buf_width, long buf_height, double *pmin_value, double *pmax_value);

		//数据读写
		void SetNormalizeLocal(bool local) { m_local = local; }
		bool DataReadBandNormalize(long band_index, long col, long row, long width, long height, 
									  long buffer_sizex, long buffer_sizey, 
									  unsigned char *pbuffer, double& min_value,double& max_value);

		bool DataReadPyramidBandNormalize(long level, long band_index, long col, long row, 
											long width, long height,long buffer_sizex, long buffer_sizey, unsigned char *pbuffer, double& min_value,double& max_value);

		bool DataReadBand(long band_index, long col, long row, long width, long height, 
							long buffer_sizex, long buffer_sizey, void *pbuffer);

		bool DataWriteBand(long band_index, long col, long row, long width, long height, void *pbuffer);

		bool GetPixel(long band_index, long row, long col, void *pvalue);

		bool SetPixel(long band_index, long row, long col, void *pvalue);

		//金字塔相关操作
		bool HasPyramid();

		bool GetPyramidSize(long level, long *pwidth, long *pheight);

		bool GetPyramidCellSize(long level, double *pcellx, double *pcelly);

		bool GetPyramidLevelNum(long *plevel_num);

		bool DataReadPyramidBand(long level, long band_index, long col, long row, 
									long width, long height,long buffer_sizex, long buffer_sizey, void *pbuffer);

		bool DataWritePyramidBand(long level, long band_index, long col, long row,
									long width, long height, long buffer_sizex, long buffer_sizey, void *pbuffer);

		bool DataReadPyramid(long level,long col, long row, 
									long width, long height,long buffer_sizex, long buffer_sizey, void *pbuffer);

		//设置无效值
		bool   SetInvalidVal(float fInvalidVal);

		bool   GetInvalidVal(float* pfInvalidVal);

		void GetGeoTransform(double* _geotransform);
		std::string getfullname() const {return path_name;}
	private:

		void initialize();

		template<class T>
		bool pyramid_normal(T dummy_temp,long level,long band_index, long col, long row, 
							long width, long height, long buffer_sizex, long buffer_sizey, 
							BYTE *pbuffer,double min_value,double max_value);

		template<class T>
		bool compute_min_max(long band_index, long col, long row, long width, long height, 
							 long buffer_sizex, long buffer_sizey, T* pbuffer,
							 double *pmin_value,double *pmax_value);

		template<class T>
		bool compute_min_max(long band_index, float null_value, T* pbuffer, double *pmin_value,double *pmax_value);

		template<class T>
		bool compute_statis_info(long band_index, float null_value, T* pbuffer,  double* min,double* max,double* mean, double* stddev);

	

	private:
		GDALDataset	*pdataset;

		bool mread_only;

		long band_num;

		double *pmin_values;
		double *pmax_values;

		std::string path_name;
		bool m_local;

		float m_invalidValue;
		bool m_useInvalidValue;

	public:
		static bool bsoftlock;
	};

#endif
