
#include "SegBlock.h"
#include "math.h"
#include <iostream>

namespace ortho_algo
{
	CSegBlock::CSegBlock()
	{

	}

	CSegBlock::~CSegBlock()
	{

	}

	void CSegBlock::Init(float scale,float wcolor, unsigned char  **pData, long lCol, long lRow, long lWidth, long lHeight, long lCount)
	{
		m_dblScale = scale;
		m_lWidth = lWidth;
		m_lHeight = lHeight;
		m_lCount = lCount;

		m_lCol = lCol;
		m_lRow = lRow;
		m_pData = pData;

		m_wcolor = wcolor;
	}

	//关联数据
	void CSegBlock::Run()
	{
		//
		InitSeg();

		float scale = 5.0;
		int iter = 0;

		m_stPairs.clear();
		m_candidateList.clear();

		do
		{
			std::cout<<"the iter is "<<iter<<std::endl;
			std::cout<<"the scale is "<<scale<<std::endl;

			BuildTopo(scale);

			RegionCombine();

			scale *=1.2;

			iter++;

		}while(scale<(m_dblScale));

	}

	void CSegBlock::InitSeg()
	{
		m_regIds = new int[m_lWidth*m_lHeight];
		for(int i=0; i<(m_lWidth*m_lHeight); i++)
		{
			m_regIds[i] = -1;
		}

		std::vector<float> pixval(m_lCount);
		float detv;
		int curid = 0;
		int ni,nj,idx,newidx;
		float mdev;
		int mid;

		for(int i=0; i<m_lHeight; i++)
		{
			for(int j=0; j<m_lWidth; j++)
			{
				idx = i*m_lWidth+j;
				if(m_regIds[idx] != -1)
					continue;

				m_regIds[idx] = curid;

				PSTRegion reg = new STRegion;
				reg->pixels.push_back(idx);
				reg->means.resize(m_lCount);

				for(int k=0; k<m_lCount; k++)
				{
					reg->means[k] = m_pData[k][idx];
				}

				std::vector<int> candiate;
				if(i>0 && m_regIds[idx-m_lWidth] == -1)
				{
					candiate.push_back(idx-m_lWidth);
					m_regIds[idx-m_lWidth] = -2;
				}

				if(j<(m_lWidth-1)&& m_regIds[idx+1] == -1)
				{
					candiate.push_back(idx+1);
					m_regIds[idx+1] = -2;
				}

				if(i<(m_lHeight-1)&& m_regIds[idx+m_lWidth] == -1)
				{
					candiate.push_back(idx+m_lWidth);
					m_regIds[idx+m_lWidth] = -2;

				}

				if(j>0&& m_regIds[idx-1] == -1)
				{
					candiate.push_back(idx-1);
					m_regIds[idx-1] = -2;
				}

				do 
				{
					if(candiate.empty() || reg->pixels.size()>30)
					{
						break;
					}

					mdev = 1.0e9;
					mid = -1;
					for(int m=0; m<candiate.size(); m++)
					{
						newidx = candiate[m];

						detv = 0;
						for(int k=0; k<m_lCount; k++)
						{
							pixval[k] = m_pData[k][newidx];
							detv += fabs(pixval[k] - reg->means[k]);
						}

						if(detv<mdev)
						{
							mdev = detv;
							mid = m;
						}
					}

					newidx = candiate[mid];

					if(mdev < 24.0)					
					{
						m_regIds[newidx] = curid;
						candiate.erase(candiate.begin()+mid);

						for(int k=0; k<m_lCount; k++)
						{
							reg->means[k] = (reg->means[k]*reg->pixels.size()+pixval[k])/(reg->pixels.size()+1);
						}
						reg->pixels.push_back(newidx);
						ni = newidx/m_lWidth;
						nj = newidx%m_lWidth;

						if(ni>0 && m_regIds[newidx-m_lWidth] == -1)
						{
							candiate.push_back(newidx-m_lWidth);
							m_regIds[newidx-m_lWidth] = -2;
						}

						if(nj<(m_lWidth-1)&& m_regIds[newidx+1] == -1)
						{
							candiate.push_back(newidx+1);
							m_regIds[newidx+1] = -2;
						}

						if(ni<(m_lHeight-1)&& m_regIds[newidx+m_lWidth] == -1)
						{
							candiate.push_back(newidx+m_lWidth);
							m_regIds[newidx+m_lWidth] = -2;

						}

						if(nj>0&& m_regIds[newidx-1] == -1)
						{
							candiate.push_back(newidx-1);
							m_regIds[newidx-1] = -2;
						}

					}
					else
					{
						break;
					}

				} while (1);

				for(int m=0; m<candiate.size(); m++)
				{
					newidx = candiate[m];
					m_regIds[newidx] = -1;
				}

				m_stRegions.push_back(reg);
				curid++;
			}
		}

		std::cout<<"init region num is "<<m_stRegions.size()<<std::endl;

#pragma omp parallel for
		for(int i=0; i<m_stRegions.size(); i++)
		{
			STRegion& reg = *(m_stRegions[i]);
			long r1,m,n,r[4];

			reg.len = 0;
			reg.valid = true;
			reg.area = reg.pixels.size();
			PixelsVar(reg.pixels, reg.dev);

			for(int k=0; k<reg.pixels.size(); k++)
			{
				idx = reg.pixels[k];
				n = idx / m_lWidth;
				m = idx % m_lWidth;
				r1 = m_regIds[idx];

				r[0] = (n == 0) ? -1 : m_regIds[idx-m_lWidth];
				r[1] = (m == (m_lWidth-1)) ? -1 : m_regIds[idx+1];
				r[2] = (n == (m_lHeight-1)) ? -1 : m_regIds[idx+m_lWidth];
				r[3] = (m == 0) ? -1 : m_regIds[idx-1];

				for(int s=0; s<4; s++)
				{
					if(r[s] == -1)
					{
						reg.len++;
						continue;
					}

					if(r1 != r[s])
					{
						reg.len++;
					}
				}
			}
		}
	}


	//
	void CSegBlock::BuildTopo(float fscale)
	{
		//
		std::vector<int> validReg;

		for(int i=0; i<m_stRegions.size(); i++)
		{
			PSTRegion reg = (m_stRegions[i]);
			if(!reg->valid)
				continue;

			validReg.push_back(i);
		}

#pragma omp parallel for
		for(int i=0; i<validReg.size(); i++)
		{
			PSTRegion reg = (m_stRegions[validReg[i]]);
			long m,n,idx;
			long r1,r[4],r2;

			reg->neiblens.clear();
			reg->neibs.clear();

			for(int k=0; k<reg->pixels.size(); k++)
			{
				idx = reg->pixels[k];
				n = idx / m_lWidth;
				m = idx % m_lWidth;
				r1 = m_regIds[idx];

				r[0] = (n == 0) ? -1 : m_regIds[idx-m_lWidth];
				r[1] = (m == (m_lWidth-1)) ? -1 : m_regIds[idx+1];
				r[2] = (n == (m_lHeight-1)) ? -1 : m_regIds[idx+m_lWidth];
				r[3] = (m == 0) ? -1 : m_regIds[idx-1];

				for(int s=0; s<4; s++)
				{
					if(r1 != r[s])
					{
						int t;
						for(t=0; t<reg->neibs.size(); t++)
						{
							if(reg->neibs[t] == r[s])
							{
								reg->neiblens[t]+=1;
								break;
							}
						}

						if(t == reg->neibs.size())
						{
							reg->neibs.push_back(r[s]);
							reg->neiblens.push_back(1);
						}
					}
				}
			}
		}

		for(int i=0; i<validReg.size(); i++)
		{
			PSTRegion reg = (m_stRegions[validReg[i]]);
			long r1,r2;

			r1 = validReg[i];
			//
			for(int k=0; k<reg->neibs.size(); k++)
			{
				r2 = reg->neibs[k];
				if(r1>r2)
				{
					continue;
				}

				STPair pair;
				pair.r1 = r1;
				pair.r2 = r2;
				pair.len = reg->neiblens[k];

				m_stPairs.push_back(pair);
			}

			reg->neiblens.clear();
			reg->neibs.clear();
		}

#pragma omp parallel for
		for(int i=0; i<m_stPairs.size(); i++)
		{
			PSTPair pair = &(m_stPairs[i]);
			PSTRegion rgn1 = (m_stRegions[pair->r1]);
			PSTRegion rgn2 = (m_stRegions[pair->r2]);
			float bl,bl1,bl2;

			std::vector<int> pixels;
			pixels.insert(pixels.end(),rgn1->pixels.begin(),rgn1->pixels.end());
			pixels.insert(pixels.end(),rgn2->pixels.begin(),rgn2->pixels.end());
			PixelsVar(pixels, pair->dev);

			long len=rgn1->len+rgn2->len-2*pair->len;
			float fColorCost = rgn1->area * (pair->dev-rgn1->dev) + rgn2->area*(pair->dev-rgn2->dev);
			float fCompactCost = sqrt(1.0*(rgn1->area+rgn2->area))*len-sqrt(1.0*rgn1->area)*rgn1->len-sqrt(1.0*rgn2->area)*rgn2->len;

			pair->cost = m_wcolor*fColorCost + (1-m_wcolor)*fCompactCost;
		}

		float fcost = fscale*fscale;
		for(int i=0; i<m_stPairs.size(); i++)
		{
			PSTPair pair = &(m_stPairs[i]);

			//
			if(pair->cost<= fcost)
			{
				m_candidateList.insert(rbNode(pair->cost,i));
			}
		}

		std::cout<<"candidate size is "<<m_candidateList.size()<<std::endl;
	}

	void CSegBlock::PixelsVar(std::vector<int>& pixels, float& var)
	{
		float* means = new float[m_lCount];
		float var2;

		for(int k=0; k<m_lCount; k++)
		{
			means[k] = 0;
			for(int i=0; i<pixels.size(); i++)
			{
				means[k] += m_pData[k][pixels[i]];
			}
			means[k] /= pixels.size();
		}

		var = 0;
		for(int k=0; k<m_lCount; k++)
		{
			var2 = 0;
			for(int i=0; i<pixels.size(); i++)
			{
				var2 += (means[k]- m_pData[k][pixels[i]])*(means[k]- m_pData[k][pixels[i]]);
			}
			var2 = sqrt(var2/pixels.size());
			var += var2;
		}

		delete []means;
	}

	void CSegBlock::RegionCombine()
	{
		std::set<rbNode>::iterator iter;
		PSTRegion pstRgn1,pstRgn2;
		PSTPair   pstPair;

		bool* bvist = new BOOL[m_stRegions.size()];
		memset(bvist, 0, m_stRegions.size());

		for(iter = m_candidateList.begin(); iter != m_candidateList.end(); iter++)
		{
			pstPair = &(m_stPairs[iter->k]);
			pstRgn1 = (m_stRegions[pstPair->r1]);
			pstRgn2 = (m_stRegions[pstPair->r2]);

			if(bvist[pstPair->r1] || bvist[pstPair->r2])
				continue;

			//
			int newArea=pstRgn1->area+pstRgn2->area;

			for(int k=0; k<m_lCount; k++)
				pstRgn1->means[k]=(pstRgn1->means[k]*pstRgn1->area+pstRgn2->means[k]*pstRgn2->area)/newArea;

			pstRgn1->area=newArea;
			pstRgn1->len=pstRgn1->len + pstRgn2->len - 2*pstPair->len;
			pstRgn1->pixels.insert(pstRgn1->pixels.end(),pstRgn2->pixels.begin(),pstRgn2->pixels.end());
			pstRgn1->dev = pstPair->dev;

			pstRgn2->valid = false;
			for(int i=0; i<pstRgn2->pixels.size(); i++)
			{
				m_regIds[pstRgn2->pixels[i]] = pstPair->r1;
			}
			pstRgn2->pixels.clear();

			bvist[pstPair->r1]=true;
			bvist[pstPair->r2]=true;
		}

		int regcount=0;
		for(int i=0; i<m_stRegions.size(); i++)
		{
			if(m_stRegions[i]->valid)
			{
				regcount++;
			}
		}

		std::cout<<"region num is "<<regcount<<std::endl;

		m_stPairs.clear();
		m_candidateList.clear();

		delete []bvist;

	}

	void CSegBlock::Commit(std::vector<PSTRegion>& regs, long lImgWidth)
	{
		int i,j;
		long lBlockRgnsNum;
		long lIndex,lRow,lCol;

		lBlockRgnsNum = m_stRegions.size();

		for(i=0; i<lBlockRgnsNum; i++)
		{
			if(!m_stRegions[i]->valid)
			{
				delete m_stRegions[i];
				continue;
			}

			//
			STRegion& region = *(m_stRegions[i]);

			for(j=0 ;j<region.pixels.size(); j++)
			{
				lIndex = region.pixels[j];

				lRow = lIndex/m_lWidth + m_lRow;
				lCol = lIndex%m_lWidth + m_lCol;

				lIndex = lRow*lImgWidth + lCol;
				region.pixels[j] = lIndex;
			}
			
			regs.push_back(m_stRegions[i]);
		}

		m_stRegions.clear();
		delete []m_regIds;
	}
}
