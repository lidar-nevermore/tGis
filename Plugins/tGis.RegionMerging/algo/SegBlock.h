#ifndef _SEG_BLOCK_H
#define _SEG_BLOCK_H

#include "SegStruct.h"

namespace ortho_algo
{
	class CSegBlock  
	{
	public:
		CSegBlock();
	
		~CSegBlock();
		//
		void Init(float scale, float wcolor, unsigned char  **pData, long lCol, long lRow, long lWidth, long lHeight, long lCount);

		void Run();

		//
		void Commit(std::vector<PSTRegion>& regs, long lImgWidth);

	private:
		//
		void InitSeg();

		//
		void BuildTopo(float fscale);

		//
		void RegionCombine();

		void PixelsVar(std::vector<int>& pixels, float& var);

	private:

		//���ݿ�ָ��
		unsigned char **m_pData;

		//pixel reg's id
		int *m_regIds;

		//���ݿ���ʼλ��
		long m_lCol,m_lRow;
		float m_wcolor;

		//��ѡ�ϲ��б�
		set<rbNode> m_candidateList;
		double m_dblScale;

		//���ݴ�С
		long m_lWidth,m_lHeight;

		//����ά��
		long m_lCount;

		//
		std::vector<PSTRegion> m_stRegions;
		std::vector<STPair> m_stPairs;

	};

}

#endif
