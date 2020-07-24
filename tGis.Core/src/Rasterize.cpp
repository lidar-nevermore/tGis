#include "Rasterize.h"
#include "math.h"

extern "C"
{
#include "linked_array.h"
#include "sorted_list.h"
}

BEGIN_NAME_SPACE(tGis, Core)

TGIS_CORE_API void RasterizeLine(int x0, int y0, int x1, int y1, RASTERIZE_FUNC func, void * ud)
{
	int x, y, dx, dy, e, i;

	if ((x1 >= x0 && y1 >= y0) || (x0 >= x1 && y0 >= y1))
	{
		if (x0 >= x1 && y0 >= y1)
		{
			x = x0; x0 = x1; x1 = x;
			y = y0; y0 = y1; y1 = y;
		}
		dx = x1 - x0;
		dy = y1 - y0;
		if (dx >= dy)
		{
			e = -dx; x = x0; y = y0;
			for (i = 0; i <= dx; i++)
			{
				func(ud, x, y);
				x = x + 1;
				e = e + 2 * dy;
				if (e >= 0)
				{
					y = y + 1;
					e = e - 2 * dx;
				}
			}
		}
		else
		{
			e = -dy; x = x0; y = y0;
			for (i = 0; i <= dy; i++)
			{
				func(ud, x, y);
				y = y + 1;
				e = e + 2 * dx;
				if (e >= 0)
				{
					x = x + 1;
					e = e - 2 * dy;
				}
			}
		}
	}
	if ((x1 >= x0 && y1 <= y0) || (x0 >= x1 && y0 <= y1))
	{
		if (x1 >= x0 && y1 <= y0)
		{
			x = x0; x0 = x1; x1 = x;
			y = y0; y0 = y1; y1 = y;
		}
		dx = x0 - x1;
		dy = y1 - y0;
		if (dx >= dy)
		{
			e = -dx; x = x0; y = y0;
			for (i = 0; i <= dx; i++)
			{
				func(ud, x, y);
				x = x - 1;
				e = e + 2 * dy;
				if (e >= 0)
				{
					y = y + 1;
					e = e - 2 * dx;
				}
			}
		}
		else
		{
			e = -dy; x = x0; y = y0;
			for (i = 0; i <= dy; i++)
			{
				func(ud, x, y);
				y = y + 1;
				e = e + 2 * dx;
				if (e >= 0)
				{
					x = x - 1;
					e = e - 2 * dy;
				}
			}
		}
	}
}

TGIS_CORE_API void RasterizePolyline(POINT_2I_T * polyline, size_t size, RASTERIZE_FUNC func, void * ud)
{
	POINT_2I_T *p1 = polyline;
	POINT_2I_T *p2;
	for (size_t i = 1; i < size; i++)
	{
		p2 = polyline + i;
		RasterizeLine(p1->x, p1->y, p2->x, p2->y, func, ud);
		p1 = p2;
	}
}

struct Edge
{
	int ymax;   //max y-coordinate of edge 
	float xofymin;  //x-coordinate of lowest edge point, updated only in aet 
	float slopeinverse;
};

//升序
static int compare_float(const void * a_, const void* b_)
{
	float* a = (float*)a_;
	float* b = (float*)b_;
	if (*a == *b)
		return 0;
	if (*a > *b)
		return 1;
	return -1;
}

static void InitEdgeTable(LINKED_ARRAY** edgeTable, POINT_2I_T * polygon, size_t size, int* yMin, int* yMax, RASTERIZE_FUNC func, void * ud);

static inline void StoreEdgeInTable(LINKED_ARRAY* edgeTable, int x1, int y1, int x2, int y2, int yMin, RASTERIZE_FUNC func, void * ud);

static void ScanLineFill(LINKED_ARRAY* edgeTable, int yMin, int yMax, RASTERIZE_FUNC func, void * ud);

static void FreeEdgeTable(LINKED_ARRAY** edgeTable, int yMin, int yMax);

TGIS_CORE_API void RasterizePolygon(POINT_2I_T * polygon, size_t size, RASTERIZE_FUNC func, void * ud)
{
	LINKED_ARRAY* edgeTable = nullptr;
	int yMin;
	int yMax;

	InitEdgeTable(&edgeTable, polygon, size, &yMin, &yMax, func, ud);
	ScanLineFill(edgeTable, yMin, yMax, func, ud);
	FreeEdgeTable(&edgeTable, yMin, yMax);
}

void InitEdgeTable(LINKED_ARRAY ** edgeTable, POINT_2I_T * polygon, size_t size, int * yMin, int * yMax, RASTERIZE_FUNC func, void * ud)
{
	*yMax = INT_MIN;
	*yMin = INT_MAX;
	POINT_2I_T * point = polygon;
	for (size_t i = 0; i < size;i++)
	{
		if (*yMax < point->y)
			*yMax = point->y;
		if (*yMin > point->y)
			*yMin = point->y;
		point++;
	}

	int scanLines = *yMax - *yMin + 1;
	*edgeTable = (LINKED_ARRAY*)malloc(scanLines * sizeof(LINKED_ARRAY));
	LINKED_ARRAY* edgeTableEntry = *edgeTable;
	for (int i = 0; i < scanLines; i++)
	{
		linked_array_init(edgeTableEntry, sizeof(Edge), 20, malloc, free, nullptr);
		edgeTableEntry++;
	}

	POINT_2I_T * point1 = polygon;
	POINT_2I_T * point2 = polygon + 1;
	for (size_t i = 1; i < size;i++)
	{
		StoreEdgeInTable(*edgeTable, point1->x, point1->y, point2->x, point2->y, *yMin, func, ud);
		point1 = point2;
		point2++;
	}
	StoreEdgeInTable(*edgeTable, point1->x, point1->y, polygon->x, polygon->y, *yMin, func, ud);
}

inline void StoreEdgeInTable(LINKED_ARRAY * edgeTable, int x1, int y1, int x2, int y2, int yMin, RASTERIZE_FUNC func, void * ud)
{
	float slopeinverse;
	int ymaxTS, xofyminTS, scanline; //ts stands for to store 

	// horizontal lines are not stored in edge table
	if (y1 == y2)
	{
		if (x1 > x2)
		{
			int temp = x2;
			x2 = x1;
			x1 = temp;
		}

		for (int x = x1; x <= x2;x++)
			func(ud, x, y1);

		return;
	}

	if (x2 == x1)
		slopeinverse = 0.0f;
	else
		slopeinverse = ((float)(x2 - x1)) / ((float)(y2 - y1));

	if (y1>y2)
	{
		scanline = y2-yMin;
		ymaxTS = y1;
		xofyminTS = x2;
	}
	else
	{
		scanline = y1-yMin;
		ymaxTS = y2;
		xofyminTS = x1;
	}

	LINKED_ARRAY* edgeTableEntry = edgeTable + scanline;

	Edge* edge = (Edge*)linked_array_enqueue(edgeTableEntry, nullptr);
	edge->slopeinverse = slopeinverse;
	edge->ymax = ymaxTS;
	edge->xofymin = (float)xofyminTS;
}

void ScanLineFill(LINKED_ARRAY * edgeTable, int yMin, int yMax, RASTERIZE_FUNC func, void * ud)
{
	//活动边表，指向aetSortHelper1或者aetSortHelper2
	//为得是方便排序，ST_LIST是用跳表实现的有序表
	ST_LIST* activeEdgeTable;

	ST_LIST aetSortHelper1;
	ST_LIST aetSortHelper2;
	sorted_list_init(&aetSortHelper1,
		sizeof(float),
		sizeof(Edge),
		malloc,
		free,
		compare_float,
		nullptr,
		nullptr
	);
	sorted_list_init(&aetSortHelper2,
		sizeof(float),
		sizeof(Edge),
		malloc,
		free,
		compare_float,
		nullptr,
		nullptr
	);

	activeEdgeTable = &aetSortHelper1;

	for (int y = yMin; y <= yMax; )
	{
		LINKED_ARRAY* edgeTableEntry = edgeTable + (y - yMin);
		if (edgeTableEntry->size > 0)
		{
			for (size_t i = 0; i < edgeTableEntry->size; i++)
			{
				Edge* edge = (Edge*)linked_array_at(edgeTableEntry, i);
				sorted_list_add(activeEdgeTable, &(edge->xofymin), edge);
			}
		}

		//开始填充横扫描线
		int edgePairNum = 0;
		Edge* edgePair[2] = { nullptr,nullptr };
		void* aetPos = sorted_list_first(activeEdgeTable);
		while (aetPos != nullptr)
		{
			edgePair[edgePairNum] = (Edge*)sorted_list_at(activeEdgeTable, aetPos, nullptr);
			aetPos = sorted_list_next(activeEdgeTable, aetPos);
			edgePairNum++;
			if (edgePairNum == 2)
			{
				edgePairNum = 0;
				int x1 = (int)round(edgePair[0]->xofymin);
				int x2 = (int)round(edgePair[1]->xofymin);
				for (int x = x1; x <= x2;x++)
					func(ud, x, y);
			}
		}

		y++;

		//更新活动边表中边的xofymin值，扫描线上移一行，对应边最下部的x值也要移动
		//移动完毕之后还要保证按照“最下部的x值”的升序排列
		ST_LIST* nextActiveEdgeTable = activeEdgeTable == &aetSortHelper1 ? &aetSortHelper2 : &aetSortHelper1;
		aetPos = sorted_list_first(activeEdgeTable);
		while (aetPos != nullptr)
		{
			Edge* edge = (Edge*)sorted_list_at(activeEdgeTable, aetPos, nullptr);
			aetPos = sorted_list_next(activeEdgeTable, aetPos);
			edge->xofymin += edge->slopeinverse;
			//不用保留的不加入，就相当于删除
			if(edge->ymax > y)
				sorted_list_add(nextActiveEdgeTable, &(edge->xofymin), edge);
		}
		sorted_list_clear(activeEdgeTable);
		activeEdgeTable = nextActiveEdgeTable;
	}

	sorted_list_release(&aetSortHelper1);
	sorted_list_release(&aetSortHelper2);
}

void FreeEdgeTable(LINKED_ARRAY ** edgeTable, int yMin, int yMax)
{
	int scanLines = yMax - yMin + 1;
	LINKED_ARRAY* edgeTableEntry = *edgeTable;
	for (int i = 0; i < scanLines; i++)
	{
		linked_array_release(edgeTableEntry);
		edgeTableEntry++;
	}
	free(*edgeTable);
}


END_NAME_SPACE(tGis, Core)
