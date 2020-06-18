#include "IGeoSurface.h"
#include "ILayer.h"
#include "ILayerRender.h"

#include "GradientColor.h"
#include "GradientColorRender.h"

#include "glad.h"

#include <vector>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

const char* const GradientColorRender::_type = "6657C29D-D01A-4B32-96C0-970B48897C0F";

const char * GradientColorRender::GetType()
{
	return _type;
}

const char * GradientColorRender::S_GetType()
{
	return _type;
}

bool GradientColorRender::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

GradientColorRender::GradientColorRender(ILayer* layer, GradientColor* color)
{
	_height = 20;

	_layer = layer;
	_layer->SetRender(this);
	_color = color;
	_envelope.MinX = 0;
	_envelope.MinY = -20;
	_envelope.MaxX = 0;
	_envelope.MaxY = 0;
}


GradientColorRender::~GradientColorRender()
{
}

void GradientColorRender::SetGradientColor(GradientColor* color)
{
	_color = color;
}

void GradientColorRender::SetHeight(int height)
{
	assert(height > 3);
	_height = height;
	_envelope.MinY = -_height;
}

void GradientColorRender::Paint(IGeoSurface * surf)
{
	if (_color == nullptr)
		return;

	int surfW, surfH;
	surf->GetSize(&surfW, &surfH);

	_envelope.MaxX = surfW;

	double dy;
	double lx = 0;

	surf->Surface2glndc(0, _height, &lx, &dy);

	for (double i = 0; i < _envelope.MaxX; i++)
	{
		double cx = (i + 1) / _envelope.MaxX;
		double rx = 2.0*cx - 1.0;
		double r, g, b;
		_color->GetColor(&r, &g, &b, cx);
		glColor3d(r, g, b);		

		glRectd(lx, 1.0, rx, dy);

		lx = rx;
	}
}

const OGREnvelope * GradientColorRender::GetEnvelope()
{
	return &_envelope;
}

const OGRSpatialReference * GradientColorRender::GetSpatialReference()
{
	return nullptr;
}

bool GradientColorRender::CanTransformTo(const OGRSpatialReference * spatialRef)
{
	if (spatialRef == nullptr)
		return true;

	return false;
}

float GradientColorRender::GetOpacity()
{
	return 1.0f;
}

void GradientColorRender::SetOpacity(float)
{
	return;
}


END_NAME_SPACE(tGis, Core)

