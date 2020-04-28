#include "SymbolLibraryRender.h"
#include "IGeoSurface.h"
#include "ILayer.h"
#include "ILayerRender.h"
#include "ISymbolLibrary.h"

#include "ISymbol.h"
#include "IMarkerSymbol.h"
#include "ILineSymbol.h"
#include "IFillSymbol.h"

#include "glad.h"

#include <vector>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

const char* const SymbolLibraryRender::_type = "2613E299-B94E-4FC1-B5F4-CC7805FAADE8";

const char * SymbolLibraryRender::GetType()
{
	return _type;
}

const char * SymbolLibraryRender::S_GetType()
{
	return _type;
}

bool SymbolLibraryRender::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}


class SymbolLibraryRenderImpl
{
public:
	SymbolLibraryRenderImpl(SymbolLibraryRender* owner)
	{
		_owner = owner;
		_surfWidth = 0;
		_surfHeight = 0;
		_rowCount = 0;
		_colCount = 0;
		_selSymId = 0;
		_curSurY = INT_MAX;
	}

	SymbolLibraryRender* _owner;
	int _surfWidth;
	int _surfHeight;
	int _rowCount;
	int _colCount;
	int _selSymId;
	int _curSurY;
	//记录当前Surf上显示的符号的Id
	std::vector<int> _vecSymId;
};

SymbolLibraryRender::SymbolLibraryRender(ILayer* layer, ISymbolLibrary* symLib)
{
	_impl_ = new SymbolLibraryRenderImpl(this);
	_symWidth = 48;
	_symHalfWidth = 24;
	_symSpan = 16;
	_symOccupy = _symWidth + _symSpan + _symSpan;
	_layer = layer;
	_layer->SetRender(this);
	_symLib = symLib;
	_envelope.MinX = 0;
	_envelope.MinY = 0;
	_envelope.MaxX = 0;
	_envelope.MaxY = 0;
}


SymbolLibraryRender::~SymbolLibraryRender()
{
	delete _impl_;
}

void SymbolLibraryRender::SetSymbolLibrary(ISymbolLibrary * symLib)
{
	_symLib = symLib;
	_impl_->_vecSymId.clear();
	UpdateEnvelope(_impl_->_surfWidth, _impl_->_surfHeight);
}

void SymbolLibraryRender::SetSymbolWidth(int width)
{
	assert(width > 3);
	_symWidth = width;
	_symHalfWidth = _symWidth / 2;
	_symOccupy = _symWidth + _symSpan + _symSpan;
	UpdateEnvelope(_impl_->_surfWidth, _impl_->_surfHeight);
}

void SymbolLibraryRender::SetSymbolSpan(int span)
{
	assert(span > 3);
	_symSpan = span;
	_symOccupy = _symWidth + _symSpan + _symSpan;
	UpdateEnvelope(_impl_->_surfWidth, _impl_->_surfHeight);
}

void SymbolLibraryRender::UpdateEnvelope(int surfW, int surfH)
{
	_impl_->_surfHeight = surfH;
	_impl_->_surfWidth = surfW;

	int colCount = surfW / _symOccupy;
	if (colCount < 1)
		colCount = 1;

	int symCount = _symLib != nullptr? _symLib->GetSymbolCount() : 0;
	int rowCount = (symCount + colCount - 1) / colCount;

	//如果符号id不是连续的，留三行空，尽量让符号都显示全
	_impl_->_rowCount = rowCount + 3;
	_impl_->_colCount = colCount;

	_envelope.MaxX = colCount*_symOccupy;
	//y方向朝上，左上角为0，好计算
	_envelope.MinY = -(_impl_->_rowCount*_symOccupy);
}

int SymbolLibraryRender::SelectSymbol(int x, int y)
{
	int col = (x + _symOccupy) / _symOccupy;
	int row = (y - _impl_->_curSurY + _symOccupy) / _symOccupy;
	_impl_->_selSymId = -1;
	if (col <= _impl_->_colCount)
	{
		size_t pos = (row - 1)*_impl_->_colCount + col - 1;
		if (pos < _impl_->_vecSymId.size())
			_impl_->_selSymId = _impl_->_vecSymId.at(pos);
	}
	return _impl_->_selSymId;
}

int SymbolLibraryRender::SelectSymbol(int symId)
{
	if (_symLib != nullptr && _symLib->SymbolExists(symId))
	{
		_impl_->_selSymId = symId;
		return symId;
	}
	return -1;
}

void SymbolLibraryRender::DrawMarkerSymbol(IGeoSurface* surf, IMarkerSymbol * sym, int x, int y)
{
	int hw = _symHalfWidth;
	int dx = x + hw + _symSpan;
	int dy = y + hw + _symSpan;
	sym->SetWidth(_symWidth);
	sym->SetHeight(_symWidth);
	sym->SetLineWidth(2);
	sym->SetColor(0, 0, 0, 255);
	sym->Paint(surf, dx, dy);
}

void SymbolLibraryRender::DrawLineSymbol(IGeoSurface* surf, ILineSymbol * sym, int x, int y)
{
	int dx[2];
	int dy[2];
	dx[0] = x + _symSpan;
	dx[1] = dx[0] + _symWidth;
	dy[0] = y + _symSpan + _symHalfWidth;
	dy[1] = dy[0];

	sym->SetWidth(2);
	sym->SetColor(0, 0, 0, 255);
	sym->Paint(surf, 2, dx, dy);
}

void SymbolLibraryRender::DrawFillSymbol(IGeoSurface* surf, IFillSymbol * sym, int x, int y)
{
	int dx[4];
	int dy[4];
	dx[0] = x + _symSpan;
	dx[1] = dx[0] + _symWidth;
	dx[2] = dx[1];
	dx[3] = dx[0];
	dy[0] = y + _symSpan;
	dy[1] = dy[0];
	dy[2] = dy[0] + _symWidth;
	dy[3] = dy[2];
	sym->SetColor(0, 0, 0, 255);
	sym->Paint(surf, 4, dx, dy);
}

void SymbolLibraryRender::Paint(IGeoSurface * surf)
{
	if (_symLib == nullptr)
		return;

	int rowCount = _impl_->_rowCount;
	int colCount = _impl_->_colCount;

	//只允许上下滚动，所以surfX==0的地方spatialX==0
	double drwLeft, drwTop, drwRight, drwBottom;
	surf->GetViewPort()->GetEnvelope(&drwLeft, &drwTop, &drwRight, &drwBottom);
	
	//左上角的行列号
	//GeoViewPort中的Spatial坐标x轴向右y轴向上
	//drwTop不大于零
	//_envelope.MaxY == 0
	int upRow = (-int(drwTop) + _symOccupy) / _symOccupy;
	int curDrwX = 0;
	int curDrwY = - (upRow - 1)*_symOccupy;
	int drwYEnd = int(drwBottom);


	//假设符号id是连续的
	int nextSymId = (upRow - 1)*colCount;
	int curCol = 0;

	//更新当前Surf上显示的符号的Id
	_impl_->_vecSymId.clear();
	_impl_->_curSurY = INT_MAX;

	while (nextSymId >= 0)
	{
		int curSymId = nextSymId;
		ISymbol* sym = _symLib->GetSymbol(curSymId, &nextSymId);
		if (sym != nullptr)
		{
			int curSurfX, curSurfY;
			surf->GetViewPort()->Spatial2Surface(curDrwX, curDrwY, &curSurfX, &curSurfY);
			if (_impl_->_curSurY == INT_MAX)
				_impl_->_curSurY = curSurfY;

			if (sym->GetType() == SymbolType::Marker)
				DrawMarkerSymbol(surf, (IMarkerSymbol*)sym, curSurfX, curSurfY);
			else if (sym->GetType() == SymbolType::Line)
				DrawLineSymbol(surf, (ILineSymbol*)sym, curSurfX, curSurfY);
			else
				DrawFillSymbol(surf, (IFillSymbol*)sym, curSurfX, curSurfY);

			_impl_->_vecSymId.push_back(curSymId);

			int sleft = curSurfX + _symSpan - 3;
			int stop = curSurfY + _symSpan - 3;
			int sright = sleft + _symWidth + 6;
			int sbottom = stop + _symWidth + 6;
			GLfloat left, top, right, bottom;
			surf->Surface2glndc(sleft, stop, &left, &top);
			surf->Surface2glndc(sright, sbottom, &right, &bottom);
			if (curSymId == _impl_->_selSymId)
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			else
				glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
			glEnable(GL_LINE_SMOOTH);
			glBegin(GL_LINE_STRIP);
			glVertex3f(left, top, 0.0f);
			glVertex3f(right, top, 0.0f);
			glVertex3f(right, bottom, 0.0f);
			glVertex3f(left, bottom, 0.0f);
			glVertex3f(left, top, 0.0f);
			glEnd();

			_symLib->ReleaseSymbol(sym);

			curCol++;
			if (curCol == colCount)
			{
				curCol = 0;
				curDrwX = 0;
				curDrwY -= _symOccupy;
			}
			else
			{
				curDrwX += _symOccupy;
			}

			if (curDrwY <= drwYEnd)
				break;
		}
	}

}

const OGREnvelope * SymbolLibraryRender::GetEnvelope()
{
	return &_envelope;
}

const OGRSpatialReference * SymbolLibraryRender::GetSpatialReference()
{
	return nullptr;
}

bool SymbolLibraryRender::CanTransformTo(const OGRSpatialReference * spatialRef)
{
	if (spatialRef == nullptr)
		return true;

	return false;
}

float SymbolLibraryRender::GetOpacity()
{
	return 1.0f;
}

void SymbolLibraryRender::SetOpacity(float)
{
	return;
}


END_NAME_SPACE(tGis, Core)

