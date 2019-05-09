
#include "imgSeg.h"
#include "SegBlock.h"
#include "math.h"
#include <iostream>
#include <queue>
#include <omp.h>

#define BYTE unsigned char

namespace ortho_algo
{
	//记录当前点周围弧段连接状态信息
	static BOOL IsBorderlinePoint(long row,long col,long width,long height,BYTE** ppBuffer, STPointNode **ppnode)
	{
		float fPixelValue[4];
		float* pfTemp;
		pfTemp = fPixelValue;

		long lRowBegin,lColBegin,lRowIndex,lColIndex;
		lRowBegin = row -1;
		lColBegin = col -1;
		for(lRowIndex=lRowBegin;lRowIndex<=row;lRowIndex++)
		{
			for(lColIndex=lColBegin;lColIndex<=col;lColIndex++)
			{
				if(lRowIndex < 0 || lRowIndex >= height || lColIndex < 0 || lColIndex >= width)
				{
					*pfTemp = 0;
				}
				else
				{
					*pfTemp = ppBuffer[lColIndex][lRowIndex];
				}
				pfTemp ++;
			}
		}
		//1
		if((fPixelValue[0]==fPixelValue[1])&&(fPixelValue[0]!=fPixelValue[2])&(fPixelValue[2]==fPixelValue[3]))
		{
			ppnode[col][row].adj[0] = 1;
			ppnode[col][row].adj[1] = 0;
			ppnode[col][row].adj[2] = 1;
			ppnode[col][row].adj[3] = 0;
			ppnode[col][row].type = 1;
			ppnode[col][row].valid = TRUE;
			return TRUE;
		}
		//2
		if((fPixelValue[0]!=fPixelValue[1])&&(fPixelValue[0]==fPixelValue[2])&&(fPixelValue[2]==fPixelValue[3]))
		{
			ppnode[col][row].adj[0] = 1;
			ppnode[col][row].adj[1] = 1;
			ppnode[col][row].adj[2] = 0;
			ppnode[col][row].adj[3] = 0;
			ppnode[col][row].type = 2;
			ppnode[col][row].valid = TRUE;
			return TRUE;
		}
		//3
		if((fPixelValue[0]!=fPixelValue[1])&&(fPixelValue[1]==fPixelValue[2])&&(fPixelValue[2]==fPixelValue[3]))
		{
			ppnode[col][row].adj[0] = 0;
			ppnode[col][row].adj[1] = 1;
			ppnode[col][row].adj[2] = 1;
			ppnode[col][row].adj[3] = 0;
			ppnode[col][row].type = 3;
			ppnode[col][row].valid = TRUE;
			return TRUE;
		}
		//4
		if((fPixelValue[0]!=fPixelValue[2])&&(fPixelValue[0]==fPixelValue[1])&&(fPixelValue[1]==fPixelValue[3]))
		{
			ppnode[col][row].adj[0] = 0;
			ppnode[col][row].adj[1] = 0;
			ppnode[col][row].adj[2] = 1;
			ppnode[col][row].adj[3] = 1;
			ppnode[col][row].type = 4;
			ppnode[col][row].valid = TRUE;
			return TRUE;
		}
		//5
		if((fPixelValue[0]!=fPixelValue[3])&&(fPixelValue[0]==fPixelValue[1])&&(fPixelValue[1]==fPixelValue[2]))
		{
			ppnode[col][row].adj[0] = 1;
			ppnode[col][row].adj[1] = 0;
			ppnode[col][row].adj[2] = 0;
			ppnode[col][row].adj[3] = 1;
			ppnode[col][row].type = 5;
			ppnode[col][row].valid = TRUE;
			return TRUE;
		}
		//6
		if((fPixelValue[0]==fPixelValue[2])&&(fPixelValue[0]!=fPixelValue[1])&(fPixelValue[1]==fPixelValue[3]))
		{
			ppnode[col][row].adj[0] = 0;
			ppnode[col][row].adj[1] = 1;
			ppnode[col][row].adj[2] = 0;
			ppnode[col][row].adj[3] = 1;
			ppnode[col][row].type = 6;
			ppnode[col][row].valid = TRUE;
			return TRUE;
		}
		//7 四联通 分两种不同情况 边界线拐向不同
		if((fPixelValue[0]==fPixelValue[3])&&(fPixelValue[0]!=fPixelValue[1])&(fPixelValue[1]==fPixelValue[2]))
		{
			ppnode[col][row].adj[0] = 1;
			ppnode[col][row].adj[1] = 1;
			ppnode[col][row].adj[2] = 1;
			ppnode[col][row].adj[3] = 1;
			ppnode[col][row].type = 7;
			ppnode[col][row].valid = TRUE;
			return TRUE;
		}
		return FALSE;
	}


	static void minAreaRect(std::vector<int>& pixels, int width, float& len)
	{
		std::vector<float> xs,ys;
		float col,row,cc,cr;
		int num = pixels.size();

		if(num==1)
		{
			len = 4;
			return;
		}

		xs.resize(num);
		ys.resize(num);

		cc = cr = 0;
		for(int i=0; i<num; i++)
		{
			col = pixels[i]%width+0.5 ;
			row = pixels[i]/width+0.5;

			cc += col;
			cr += row;

			xs[i] = col+0.5;
			ys[i] = row+0.5;
		}
		cc /= num;
		cr /= num;

		for(int i=0; i<num; i++)
		{
			xs[i] -= cc;
			ys[i] -= cr;
		}

		float m11,m20,m02,theta;

		m11=m20=m02=0;
		for(int i=0; i<num; i++)
		{
			m11 += xs[i]*ys[i];
			m20 += xs[i]*xs[i];
			m02 += ys[i]*ys[i];
		}

		float fcos,fsin,x,y,xmin,xmax,ymin,ymax;

		theta = 0.5*atan2(2*m11,m20-m02);
		fcos = cos(theta); 
		fsin = sin(theta);
		xmin = ymin = 1.0e+9;
		xmax = ymax = -1.0e+9;

		for(int i=0; i<num; i++)
		{
			x = xs[i]*fcos + ys[i]*fsin;
			y = xs[i]*fsin - ys[i]*fcos;

			if(x < xmin) xmin = x;
			if(x > xmax) xmax = x;
			if(y < ymin) ymin = y;
			if(y > ymax) ymax = y;
		}

		len = 2*(xmax-xmin)+2*(ymax-ymin)+4;

	}


	CImgSeg::CImgSeg()
	{

	}

	CImgSeg::~CImgSeg()
	{

	}

	void CImgSeg::Init(double wColor, double wCompact, double wSmooth, double scale)
	{
		m_dblWColor = wColor;
		m_dblWSmooth = wSmooth;
		m_dblWCompact = wCompact;
		m_dblScale = scale;

		m_candidateList.clear();
		m_stPairs.clear();
		m_stRegions.clear();
	}

	void CImgSeg::PixelsVar(std::vector<int>& pixels, float& var)
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

	void CImgSeg::PixelsBound(std::vector<int>& pixels, int bound[4])
	{
		int id,px,py;
		bound[0] = bound[2] = 1.0e+10;
		bound[1] = bound[3] = -1.0e+10;

		for(int i=0; i<pixels.size(); i++)
		{
			id = pixels[i];
			px = id%m_lWidth;
			py = id/m_lWidth;

			if(py < bound[0]) bound[0] = py;
			if((py+1)>bound[1]) bound[1] = py+1;
			if(px < bound[2]) bound[2] = px;
			if((px+1)>bound[3]) bound[3] = px+1;
		}
	}

	BOOL CImgSeg::RunSeg(const char* strInFile, const char* strVecFile)
	{
		//
		long lChannelDataType;
		//
		GDALAllRegister();
		imgData=(GDALDataset*)GDALOpen(strInFile,GA_ReadOnly);
		if (imgData==NULL)
			return false;
		m_lWidth=imgData->GetRasterXSize();
		m_lHeight=imgData->GetRasterYSize();
		m_lCount=imgData->GetRasterCount();
		m_pData=new BYTE*[m_lCount];
		for (int k=0;k<m_lCount;k++)
		{
			m_pData[k]=new BYTE[m_lWidth*m_lHeight];
			imgData->GetRasterBand(k+1)->RasterIO(GF_Read,0,0,m_lWidth,m_lHeight,m_pData[k],m_lWidth,m_lHeight,GDT_Byte,0,0);
		}

		//

		clock_t t1=clock();

		RunBlockSeg();

		//
		RegionMerge();

		clock_t t2=clock();
		std::cout<<"seg time is "<<1.0*(t2-t1)/ CLOCKS_PER_SEC<<std::endl;

		//
		for(int i=0; i<m_stRegions.size(); i++)
		{
			if(m_stRegions[i]->valid)
			{
				m_stRltRegions.push_back(m_stRegions[i]);
			}
			else
			{
				delete m_stRegions[i];
			}
		}

		//
		GenShape(strVecFile);

		for(int k=0; k<m_lCount; k++)
		{
			delete []m_pData[k];
		}

		return TRUE;
	}

	//
	BOOL CImgSeg::RunBlockSeg()
	{
		m_regIds = new int[m_lWidth*m_lHeight];
		for(int i=0; i<(m_lWidth*m_lHeight); i++)
		{
			m_regIds[i] = -1;
		}

		int i,j,k;
		//
		long lBlockSizeX, lBlockSizeY;
		long lRow,lCol;
		unsigned char **pData;
		long blockCountX, blockCountY,blockSize = 5000;
		long blockSizeX,blockSizeY;

		blockCountX = (m_lWidth-1)/blockSize+1;
		blockCountY = (m_lHeight-1)/blockSize+1;

		blockSizeX = m_lWidth / blockCountX + 1;
		blockSizeY = m_lHeight /blockCountY + 1;
		GDALDataset* img = (GDALDataset*)imgData;
		pData = new unsigned char*[m_lCount];

		for( k=0; k<m_lCount; k++)
		{
			pData[k] = new unsigned char[blockSizeX*blockSizeY];
		}

		std::cout<<"block num is "<<blockCountX<<","<<blockCountY<<std::endl;

		for( i=0; i<blockCountY; i++)
		{
			for( j=0; j<blockCountX; j++)
			{	
				lBlockSizeX = (j == (blockCountX-1)) ? (m_lWidth - blockSizeX * j) : blockSizeX;
				lBlockSizeY = (i == (blockCountY-1)) ? (m_lHeight - blockSizeY * i) : blockSizeY;
				std::cout<<"run the block id is "<<j<<","<<i<<std::endl;

				lRow = i*blockSizeY;
				lCol = j*blockSizeX;

				for( k=0; k<m_lCount; k++)
				{
					img->GetRasterBand(k+1)->RasterIO(GF_Read,lCol,lRow,lBlockSizeX,lBlockSizeY, pData[k],lBlockSizeX,lBlockSizeY,GDT_Byte,0,0);
				}

				clock_t t1=clock();
				CSegBlock seg;
				seg.Init(30, m_dblWColor, pData,  lCol, lRow, lBlockSizeX, lBlockSizeY,m_lCount);
				seg.Run();
				seg.Commit(m_stRegions, m_lWidth);

				clock_t t3=clock();

				std::cout<<"block seg time is "<<1.0*(t3-t1)/ CLOCKS_PER_SEC<<std::endl;

			}
		}

		std::cout<<"init region num is "<<m_stRegions.size()<<std::endl;

		for(int i=0; i<m_stRegions.size(); i++)
		{
			PSTRegion reg = m_stRegions[i];

			for(int k=0; k<reg->pixels.size(); k++)
			{
				m_regIds[reg->pixels[k]] = i;
			}
		}

		for( k=0; k<m_lCount; k++)
		{
			delete []pData[k];
		}
		delete []pData;

		return TRUE;

	}

	//
	BOOL CImgSeg::BuildTopo(float fscale)
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

		bool bsmooth = (fscale>50) ? true : false;

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
			float fSmoothCost = fCompactCost;

			if(0)
			{
				minAreaRect(pixels, m_lWidth, bl);
				minAreaRect(rgn1->pixels, m_lWidth, bl1);
				minAreaRect(rgn2->pixels, m_lWidth, bl2);

				fSmoothCost = rgn1->area*(1.0*len/bl-1.0*rgn1->len/bl1)+rgn2->area*(1.0*len/bl-1.0*rgn2->len/bl2);
			}

			pair->cost = m_dblWColor*fColorCost + (1-m_dblWColor)*m_dblWCompact*fCompactCost + (1-m_dblWColor)*m_dblWSmooth*fSmoothCost;
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

		return TRUE;
	}

	//
	BOOL CImgSeg::RegionMerge()
	{
		float scale = 30.0;
		int candi,iter = 0;

		m_stPairs.clear();
		m_candidateList.clear();

		do
		{
			std::cout<<"the iter is "<<iter<<std::endl;
			std::cout<<"the scale is "<<scale<<std::endl;

			BuildTopo(scale);

			RegionCombine();

			m_stPairs.clear();
			m_candidateList.clear();

			scale *=1.2;

			iter++;

		}while(scale<m_dblScale);

		iter = 0;
		do
		{
			std::cout<<"the iter is "<<iter<<std::endl;
			std::cout<<"the scale is "<<m_dblScale<<std::endl;

			BuildTopo(m_dblScale);

			RegionCombine();

			candi = m_candidateList.size();
			m_stPairs.clear();
			m_candidateList.clear();

			iter++;

		}while(candi>30 || iter>10);

		return TRUE;
	}

	//
	BOOL CImgSeg::RegionCombine()
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

		delete []bvist;

		return TRUE;
	}

	void CImgSeg::GenPolygon(std::vector<PSTRegion>& mRegions, std::vector<STRegPoly>& mRegPolys, std::vector<STLine>& capFindlines)
	{
		long lRow,lCol,lIndex,lPixelCount=0;

		PSTRegion pstRgn;
		PSTRegPoly pstPoly;
		long width,height;
		long row ,col;
		long validnum = 0;
		int bound[4];

		mRegPolys.resize(mRegions.size());

		for(int i=0; i<mRegions.size(); i++)
		{ //遍历REGION开始
			//int iLineNum=0; //记录当前区域寻找出的多边形条数
			std::vector<long> findPointsNum;

			pstRgn = (mRegions[i]);
			pstPoly = &(mRegPolys[i]);

			if(!(pstRgn->valid))
			{
				continue;
			}

			PixelsBound(pstRgn->pixels, bound);

			width = bound[3] - bound[2]+1;
			height = bound[1] - bound[0]+1;

			//遍历缓冲区，记录缓冲区上每一点的连接状态信息
			STPointNode **ppNode = NULL;
			ppNode = new STPointNode* [width+1];
			for(int n = 0; n < width+1; n ++)
			{
				ppNode[n] = new STPointNode [height+1];
			}
		
			BYTE **ppBuffer = NULL;
			ppBuffer = new BYTE* [width];
			memset(ppBuffer,0,sizeof(BYTE)*width);
			for(int k = 0; k < width; k ++)
			{
			   ppBuffer[k] = new BYTE [height];
			   memset(ppBuffer[k],0,sizeof(BYTE)*height);
			}
		

			lPixelCount += pstRgn->pixels.size();
			//设置BUFFER为0/1标记状态
			for(int j=0; j<pstRgn->pixels.size(); j++)
			{
				lIndex = pstRgn->pixels[j];

				lRow = lIndex/m_lWidth;
				lCol = lIndex%m_lWidth;

				ppBuffer[lCol-(bound[2])][lRow-(bound[0])] = 1;
			}
			//建立BUFFER内点的连接状态，因为只存在0/1值，只存在封闭弧段
			for (row=0;row<height+1;row++)
			{
				for (col=0;col<width+1;col++)
				{
					if(!IsBorderlinePoint(row,col,width,height,ppBuffer,ppNode))
						continue;
					else
					{
						if(ppNode[col][row].type == 7)
						{ validnum += 2;}
						else
						{ validnum += 1;}
						continue;
					}
				}
			}
			//跟踪封闭弧段,从一连接点开始搜索回到初始点结束，清除连接信息

			std::vector<STPoint> capClostLine;
			capClostLine.clear();
			STPoint stpStart,stpFind,stpAdd;
			STLine curLine;
			long lTryRow,lTryCol;
			int firstlink,dirlink;
			for (row=0;row<height+1;row++)
			{
				//if(iLineNum >= 1)
				//		break;

				if(validnum == 0 )
					break;

				for (col=0;col<width+1;col++)
				{
					//if(iLineNum >= 1)
					//	break;
					if(validnum == 0)
						break;

					stpStart.x = col;
					stpStart.y = row;
					stpFind.x = -1;
					stpFind.y = -1;
					lTryRow = row;
					lTryCol = col;
					if(ppNode[col][row].type>0)
					{
						 if((ppNode[col][row].adj[0]==0)&&(ppNode[col][row].adj[1]==0)&&(ppNode[col][row].adj[2]==0)&&(ppNode[col][row].adj[3]==0))
						 {	
							continue;
						 }
						//capClostLine.clear();
						stpAdd.x = stpStart.x + bound[2];
						stpAdd.y = stpStart.y + bound[0];
						capClostLine.push_back(stpAdd);
			    		for(;;)
						{
						   if((stpFind.x==stpStart.x)&&(stpFind.y==stpStart.y)&&(capClostLine.size()>1))
						   {
							  if(findPointsNum.size() == 0)
							  {  
								  findPointsNum.push_back(capClostLine.size()-1);
								  validnum = validnum -(capClostLine.size()-1);
								  //
								  pstPoly->linePointsIndex.push_back(capClostLine.size());
							  }
							  else
							  {
								  long beforenum =0;
								  for(int i=0;i<=findPointsNum.size()-1;i++)
									  beforenum += findPointsNum[i];
								  long newnum = capClostLine.size() - beforenum - findPointsNum.size() -1;
								   findPointsNum.push_back(newnum);
								  validnum = validnum - newnum;
								  //
								  pstPoly->linePointsIndex.push_back(capClostLine.size());
							  }
							  if(validnum == 0)
							  {
								  //
								  pstPoly->linesNum = findPointsNum.size();
								  //
								  curLine.lLinePointsNum = capClostLine.size();
								  //curLine.pstpLinePoints = new STPoint[curLine.lLinePointsNum];
								  curLine.pstpLinePoints.resize(curLine.lLinePointsNum);
								  for(int i=0;i < curLine.lLinePointsNum;i++)
								  {
									 curLine.pstpLinePoints[i] = capClostLine[i]; 
								  }
								  //先取第一层属性值
								  curLine.meanValue = pstRgn->means[0];
								  capFindlines.push_back(curLine);//不清楚会否调用delete()
								  //iLineNum += 1;
								  break;
							  }
							  else
							  {
								  break;
							  }

						   }

							//
						   if(ppNode[lTryCol][lTryRow].adj[0]>0)
						   {
							stpFind.x = lTryCol+1;
							stpFind.y = lTryRow;
							firstlink = 0;
							dirlink = (0+2)%4;
							ppNode[lTryCol][lTryRow].adj[0]=0;
							ppNode[lTryCol+1][lTryRow].adj[dirlink]=0;
							lTryRow = stpFind.y;
							lTryCol = stpFind.x;
							stpAdd.x = stpFind.x + bound[2];
							stpAdd.y = stpFind.y + bound[0];
							capClostLine.push_back(stpAdd);
							continue;
						   }
						   else if(ppNode[lTryCol][lTryRow].adj[1]>0)
						   {
					 		stpFind.x = lTryCol;
							stpFind.y = lTryRow -1;
							firstlink=1;
							dirlink = (1+2)%4;
							ppNode[lTryCol][lTryRow].adj[1]=0;
							ppNode[lTryCol][lTryRow -1].adj[dirlink]=0;
							lTryRow = stpFind.y;
							lTryCol = stpFind.x;					
							stpAdd.x = stpFind.x + bound[2];
							stpAdd.y = stpFind.y + bound[0];
							capClostLine.push_back(stpAdd);
							continue;
						   }
						   else if(ppNode[lTryCol][lTryRow].adj[2]>0)
						   {
							stpFind.x = lTryCol -1;
							stpFind.y = lTryRow;
							firstlink = 2;
							dirlink = (2+2)%4;
							ppNode[lTryCol][lTryRow].adj[2]=0;
							ppNode[lTryCol -1][lTryRow].adj[dirlink]=0;
							lTryRow = stpFind.y;
							lTryCol = stpFind.x;	
							stpAdd.x = stpFind.x + bound[2];
							stpAdd.y = stpFind.y + bound[0];
							capClostLine.push_back(stpAdd);
							continue;
						   }
						   else if(ppNode[lTryCol][lTryRow].adj[3]>0)
						   {
							stpFind.x = lTryCol;
							stpFind.y = lTryRow+1;
							firstlink = 3;
							dirlink = (3+2)%4;
							ppNode[lTryCol][lTryRow].adj[3]=0;
							ppNode[lTryCol][lTryRow+1].adj[dirlink]=0;
							lTryRow = stpFind.y;
							lTryCol = stpFind.x;					
							stpAdd.x = stpFind.x + bound[2];
							stpAdd.y = stpFind.y + bound[0];
							capClostLine.push_back(stpAdd);
							continue;
						   }					  
							//
					
						}

					}

				}
			}
			long lineNum = capClostLine.size();
			for(long l=0;l<lineNum;l++)
			{
				pstPoly->polyPoints.push_back(capClostLine[l]);
			}
		

			for(int w=0;w<width+1;w++)
			{
				delete[] ppNode[w];
			}
			for(int w=0;w<width;w++)
			{
				delete[] ppBuffer[w];
			}

		}//遍历REGION结束

	}

	void CImgSeg::GenShape(const char* strVecFile)
	{
		//寻找边界线
		std::vector<STLine> totalFindlines;

		GenPolygon(m_stRltRegions, m_stRegPolys, totalFindlines);

		//保存shp导出
		const char *pszDriverName="ESRI Shapefile";
		GDALDriver *poDriver;
		poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
		if (poDriver==NULL)
			return;
		GDALDataset *poDs;
		poDs=poDriver->Create(strVecFile,0,0,0,GDT_Unknown,NULL);//数据源
		if (poDs==NULL)
			return;

		OGRLayer *poLayer;
		poLayer =poDs->CreateLayer("point_out",NULL,wkbPolygon,NULL);//图层
		if (poLayer==NULL)
			return;

		OGRFieldDefn oField1("PointX",OFTReal);
		oField1.SetPrecision(3);
		if (poLayer->CreateField(&oField1)!=OGRERR_NONE)
			return;

		//批处理待修改
		for (int i=0;i<m_stRegPolys.size();i++)
		{
			OGRFeature *poFeature;
			poFeature=OGRFeature::CreateFeature(poLayer->GetLayerDefn());
			poFeature->SetField(0,i);

			STRegPoly& regPoly = m_stRegPolys[i];

			OGRPolygon poly;
			int starti,endi;
			double ref_x,ref_y;

			for(int j=0; j<regPoly.linesNum;j++)
			{
				starti = (j==0)? 0 :regPoly.linePointsIndex[j-1];
				endi = regPoly.linePointsIndex[j];

				OGRLinearRing ring;
				for(int k=starti; k<endi; k++)
				{
					ref_x = regPoly.polyPoints[k].x;
					ref_y = regPoly.polyPoints[k].y;
					ring.addPoint(ref_x,m_lHeight-ref_y);
				}
				ring.closeRings();

				OGRGeometry* ring2 = ring.SimplifyPreserveTopology(0.9);
				if(!ring2)
				{
					std::cout<<"simple failed:"<<std::endl;
				}
				poly.addRing(&ring);
			}


			poFeature->SetGeometry(&poly);
			if (poLayer->CreateFeature(poFeature)!=OGRERR_NONE)
				return;

			OGRFeature::DestroyFeature(poFeature);
		}

		GDALClose(poDs);

	}

}
