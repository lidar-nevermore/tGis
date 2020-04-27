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
	}

	SymbolLibraryRender* _owner;
	int _surfWidth;
	int _surfHeight;
	int _rowCount;
	int _colCount;
	int _selSymId;
	std::vector<int> _vecSymId;
};

SymbolLibraryRender::SymbolLibraryRender(ILayer* layer, ISymbolLibrary* symLib)
{
	_impl_ = new SymbolLibraryRenderImpl(this);
	_symWidth = 48;
	_symHalfWidth = 24;
	_symSpan = 16;
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

int SymbolLibraryRender::SelectSymbol(int x, int y)
{
	int symOccupy = _symWidth + _symSpan + _symSpan;
	int col = (x + symOccupy) / symOccupy;
	int row = (y + symOccupy) / symOccupy;
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

	int symOccupy = _symWidth + _symSpan + _symSpan;
	int surfWidth;
	int surfHeight;
	surf->GetSize(&surfWidth, &surfHeight);
	bool resetIdPos = false;
	if (_impl_->_surfHeight != surfHeight || _impl_->_surfWidth != surfWidth)
	{
		resetIdPos = true;
		_impl_->_vecSymId.clear();
	}

	_impl_->_surfHeight = surfHeight;
	_impl_->_surfWidth = surfWidth;

	int colCount = surfWidth / symOccupy;
	if (colCount < 1)
		colCount = 1;

	int symCount = _symLib->GetSymbolCount();
	int rowCount = (symCount + colCount - 1) / colCount;
	_envelope.MaxX = colCount*symOccupy;
	_envelope.MaxY = rowCount*symOccupy;

	_impl_->_rowCount = rowCount;
	_impl_->_colCount = colCount;

	int curCol = 0;
	int curRow = 0;
	int curX = 0;
	int curY = 0;

	int nextSymId = 0;

	while (nextSymId >= 0)
	{
		int curSymId = nextSymId;
		ISymbol* sym = _symLib->GetSymbol(curSymId, &nextSymId);
		if (sym != nullptr)
		{
			if (sym->GetType() == SymbolType::Marker)
				DrawMarkerSymbol(surf, (IMarkerSymbol*)sym, curX, curY);
			else if (sym->GetType() == SymbolType::Line)
				DrawLineSymbol(surf, (ILineSymbol*)sym, curX, curY);
			else
				DrawFillSymbol(surf, (IFillSymbol*)sym, curX, curY);

			if (resetIdPos)
				_impl_->_vecSymId.push_back(curSymId);

			int sleft = curX + _symSpan - 3;
			int stop = curY + _symSpan - 3;
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
				curRow++;
				curX = 0;
				curY += symOccupy;
			}
			else
			{
				curX += symOccupy;
			}

			if (curRow == rowCount)
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


