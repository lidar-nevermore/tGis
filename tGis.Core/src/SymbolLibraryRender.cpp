#include "SymbolLibraryRender.h"
#include "IGeoSurface.h"
#include "ILayer.h"
#include "ILayerRender.h"
#include "ISymbolLibrary.h"

#include "ISymbol.h"
#include "IMarkerSymbol.h"
#include "ILineSymbol.h"
#include "IFillSymbol.h"



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

SymbolLibraryRender::SymbolLibraryRender(ILayer* layer, ISymbolLibrary* symLib)
{
	_symWidth = 32;
	_symSpan = 10;
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
}

void SymbolLibraryRender::DrawMarkerSymbol(IGeoSurface* surf, IMarkerSymbol * sym, int x, int y)
{
	int hw = _symWidth / 2;
	int dx = x + hw + _symSpan;
	int dy = y + hw + _symSpan;
	sym->SetWidth(_symWidth);
	sym->SetHeight(_symWidth);
	sym->SetLineWidth(2);

	sym->Paint(surf, dx, dy);
}

void SymbolLibraryRender::DrawLineSymbol(IGeoSurface* surf, ILineSymbol * sym, int x, int y)
{
}

void SymbolLibraryRender::DrawFillSymbol(IGeoSurface* surf, IFillSymbol * sym, int x, int y)
{
}

void SymbolLibraryRender::Paint(IGeoSurface * surf)
{
	if (_symLib == nullptr)
		return;

	int symOccupy = _symWidth + _symSpan + _symSpan;
	int surfWidth;
	int surfHeight;
	surf->GetSize(&surfWidth, &surfHeight);

	int rowSymCount = surfWidth / symOccupy;
	if (rowSymCount < 1)
		rowSymCount = 1;

	int symCount = _symLib->GetSymbolCount();
	_envelope.MaxX = rowSymCount*symOccupy;
	_envelope.MaxY = ((symCount + rowSymCount - 1) / rowSymCount)*symOccupy;;

	int curCol = 0;
	int curRow = 0;
	int curX = 0;
	int curY = 0;

	int nextSymId = 0;

	while (nextSymId >= 0)
	{
		ISymbol* sym = _symLib->GetSymbol(nextSymId, &nextSymId);
		if (sym != nullptr)
		{
			if (sym->IsTypeOf(IMarkerSymbol::S_GetType()))
				DrawMarkerSymbol(surf, (IMarkerSymbol*)sym, curX, curY);
			else if (sym->IsTypeOf(ILineSymbol::S_GetType()))
				DrawLineSymbol(surf, (ILineSymbol*)sym, curX, curY);
			else
				DrawFillSymbol(surf, (IFillSymbol*)sym, curX, curY);
			_symLib->RevertSymbol(sym);
			curCol++;
			if (curCol == rowSymCount)
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


