#ifndef _SEG_STRUCT_H
#define _SEG_STRUCT_H

#include <vector>
#include <map>
#include <set>

using std::vector;
using std::map;
using std::set;

struct st_Region;
struct st_Pair;

#ifndef BOOL
#  define BOOL bool
#endif

#ifndef FALSE
#  define FALSE 0
#endif

#ifndef TRUE
#  define TRUE 1
#endif

typedef struct st_Point
{
	double x;
	double y;
} STPoint, *PSTPoint;

typedef struct st_Extent
{
	double xmin;
	double ymin;
	double xmax;
	double ymax;
} STExtent, *PSTExtent;

typedef struct st_PolyPoint
{
		long x;
		long y;	
} STPolyPoint, *PSTPolyPoint;

typedef struct st_Region
{
	bool                        valid;
	vector<float>				means;			//平均灰度值
	int							area;			//面积，单位为象素
	vector<int>					pixels;		    //记录区域包含的象素序号
	int							len;			//周长
	float						dev;			//标准差
	vector<int>					neibs;
	vector<int>                 neiblens;

}STRegion,*PSTRegion;

//多边形区域
typedef struct st_RegPoly
{
	std::vector<STPoint>        polyPoints;     //全部边界点，用于显示
	int                         linesNum;       //边界线条数
	std::vector<long>           linePointsIndex; //多边界时，用于记录新边界起始点的索引

}STRegPoly,*PSTRegPoly;

//相邻区域组
typedef struct st_Pair
{
	int		r1;		//第一个区域引用
	int		r2;		//第二个区域引用
	int		cost;	//合并的消耗
	float	dev;	//方差  
	int		len;	//公共边长  

}STPair,*PSTPair;

//节点类型
typedef struct STPointNode
{
		int adj[4];
		int type;
		BOOL valid;

		STPointNode()
		{
          adj[0]= 0 ;
		  adj[1]= 0 ;
		  adj[2]= 0 ;
		  adj[3]= 0 ;
		  type = 0;
		  valid = FALSE;
		};
} STPointNode, *PSTPointNode;

typedef struct STLine
{
	std::vector<STPoint> pstpLinePoints;
	long lLinePointsNum;
	float meanValue;
}STLine,*PSTLine;

//候选合并列表的节点,红黑树
struct rbNode
{
	rbNode()
	{

	}
	rbNode(int c, int l)
	{
		cost=c;
		k=l;
	}

    int  		cost;	//合并的消耗
    int			k;	    //两个相邻区域组成的Pair的序号
};

inline bool operator< (const rbNode &p,const rbNode &q)
{
	if(p.cost<q.cost)
	{
		return true;
	}
	else if(p.cost == q.cost)                                                                                                                                                                                    
	{
		if(p.k<q.k)
		{
			return true;
		}
		else                                                                                                                 
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


inline bool operator== (const rbNode &p,const rbNode &q)
{
	if( (p.cost==q.cost) && (p.k==q.k))
	{
		return true;
	}
	else 
	{
		return false;
	}
}

#endif