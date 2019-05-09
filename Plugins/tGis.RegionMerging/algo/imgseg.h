#ifndef _IMG_SEG_H
#define _IMG_SEG_H

#include <vector>
#include "SegStruct.h"
#include <gdal_priv.h>
#include <gdal_alg.h>
#include <ogrsf_frmts.h>

namespace ortho_algo
{
	class CImgSeg
	{

	public:
		CImgSeg();

		~CImgSeg();

	public:
		//
		void Init(double wColor, double wCompact, double wSmooth,double scale);

		//
		BOOL RunSeg(const char* strInFile, const char* strVecFile);

	private:
		//
		BOOL RunBlockSeg();

		//
		BOOL BuildTopo(float fscale);

		//
		BOOL RegionMerge();

		//
		BOOL RegionCombine();

		void PixelsVar(std::vector<int>& pixels, float& var);

		void PixelsBound(std::vector<int>& pixels, int bound[4]);

		//生成每个对象的外边界多边形，并保存入结构体
		void GenPolygon(std::vector<PSTRegion>& mRegions, std::vector<STRegPoly>& mRegPolys, std::vector<STLine>& capFindlines);

		//
		void GenShape(const char* strVecFile);

	private:

		//
		std::vector<PSTRegion> m_stRegions;
		std::vector<PSTRegion> m_stRltRegions;
		std::vector<STRegPoly> m_stRegPolys;

		std::vector<STPair> m_stPairs;

		//候选合并列表
		set<rbNode> m_candidateList;
	
		//数据块指针
		unsigned char **m_pData;

		//pixel reg's id
		int *m_regIds;

		//数据块大小
		long m_lWidth,m_lHeight;

		//数据维数
		long m_lCount;

		//
		GDALDataset* imgData;

		//
		double m_dblWColor;
		double m_dblWSmooth;
		double m_dblWCompact;
		double m_dblScale0,m_dblScale;

	};
}
#endif