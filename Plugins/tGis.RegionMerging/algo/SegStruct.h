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
	vector<float>				means;			//ƽ���Ҷ�ֵ
	int							area;			//�������λΪ����
	vector<int>					pixels;		    //��¼����������������
	int							len;			//�ܳ�
	float						dev;			//��׼��
	vector<int>					neibs;
	vector<int>                 neiblens;

}STRegion,*PSTRegion;

//���������
typedef struct st_RegPoly
{
	std::vector<STPoint>        polyPoints;     //ȫ���߽�㣬������ʾ
	int                         linesNum;       //�߽�������
	std::vector<long>           linePointsIndex; //��߽�ʱ�����ڼ�¼�±߽���ʼ�������

}STRegPoly,*PSTRegPoly;

//����������
typedef struct st_Pair
{
	int		r1;		//��һ����������
	int		r2;		//�ڶ�����������
	int		cost;	//�ϲ�������
	float	dev;	//����  
	int		len;	//�����߳�  

}STPair,*PSTPair;

//�ڵ�����
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

//��ѡ�ϲ��б�Ľڵ�,�����
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

    int  		cost;	//�ϲ�������
    int			k;	    //��������������ɵ�Pair�����
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