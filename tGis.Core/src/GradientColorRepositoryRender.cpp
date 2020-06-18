#include "IGeoSurface.h"
#include "ILayer.h"
#include "ILayerRender.h"

#include "GradientColor.h"
#include "GradientColorRepository.h"
#include "GradientColorRepositoryRender.h"

#include "glad.h"

#include <vector>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

const char* const GradientColorRepositoryRender::_type = "AA298003-7EF2-416D-A370-8B127361A648";

const char * GradientColorRepositoryRender::GetType()
{
	return _type;
}

const char * GradientColorRepositoryRender::S_GetType()
{
	return _type;
}

bool GradientColorRepositoryRender::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

class GradientColorRepositoryRenderImpl
{
public:
	GradientColorRepositoryRenderImpl(GradientColorRepositoryRender* owner)
	{
		_owner = owner;
	}

	GradientColorRepositoryRender* _owner;
	std::vector<GradientColor*> _vecColor;
};

GradientColorRepositoryRender::GradientColorRepositoryRender(ILayer* layer)
{
	_impl_ = new GradientColorRepositoryRenderImpl(this);

	_height = 20;
	_margin = 5;
	_occupy = _height + _margin + _margin;
	_selColor = nullptr;
	_drwTopSurfY = INT_MAX;

	_layer = layer;
	_layer->SetRender(this);

	_envelope.MinX = 0;
	_envelope.MinY = -20.0 * GradientColorRepository::INSTANCE()->GetGradientColorCount();
	_envelope.MaxX = 0;
	_envelope.MaxY = 0;
}


GradientColorRepositoryRender::~GradientColorRepositoryRender()
{
	delete _impl_;
}

void GradientColorRepositoryRender::SetHeight(int height)
{
	assert(height > 3);
	_height = height;
	_occupy = _height + _margin + _margin;
	_envelope.MinY = double(-_occupy * GradientColorRepository::INSTANCE()->GetGradientColorCount());
}

void GradientColorRepositoryRender::SetMargin(int margin)
{
	assert(margin > 3);
	_margin = margin;
	_occupy = _height + _margin + _margin;
	_envelope.MinY = double(-_occupy * GradientColorRepository::INSTANCE()->GetGradientColorCount());
}

void GradientColorRepositoryRender::UpdateEnvelope(int surfW, int surfH)
{
	int minW = _margin + _margin + 50;
	if(surfW < minW)
		_envelope.MaxX = minW;
	else
		_envelope.MaxX = surfW;
}

GradientColor * GradientColorRepositoryRender::SelectGradientColor(int x, int y)
{
	int row = (y - _drwTopSurfY) / _occupy;
	size_t colorIdx = row + _topColorIdx;

	_selColor = nullptr;

	if (colorIdx < GradientColorRepository::INSTANCE()->GetGradientColorCount())
		_selColor = GradientColorRepository::INSTANCE()->GetGradientColor(colorIdx);

	return _selColor;
}

void GradientColorRepositoryRender::SelectGradientColor(GradientColor * color)
{
	_selColor = color;
}

void GradientColorRepositoryRender::Paint(IGeoSurface * surf)
{
	int surfW, surfH;
	surf->GetSize(&surfW, &surfH);

	double drwLeft, drwTop, drwRight, drwBottom;
	surf->GetViewPort()->GetEnvelope(&drwLeft, &drwTop, &drwRight, &drwBottom);

	//色条的宽度
	int stripW = int(drwRight - drwLeft - _margin - _margin);

	int drwColorIdxBegin = (-int(drwTop)) / _occupy;
	int drwYBegin = -drwColorIdxBegin*_occupy;
	int drwYEnd = int(drwBottom);
	size_t colorCount = GradientColorRepository::INSTANCE()->GetGradientColorCount();

	_drwTopSurfY = INT_MAX;
	_topColorIdx = drwColorIdxBegin;

	while (drwYBegin >= drwYEnd && drwColorIdxBegin < colorCount )
	{
		GradientColor * color = GradientColorRepository::INSTANCE()->GetGradientColor(drwColorIdxBegin);

		int curSurfX, curSurfY;
		surf->GetViewPort()->Spatial2Surface(0, drwYBegin, &curSurfX, &curSurfY);
		if (_drwTopSurfY == INT_MAX)
			_drwTopSurfY = curSurfY;

		//Surface坐标转换为OGL的ndc坐标
		double ty = 1.0 - (2.0*(curSurfY + _margin)) / surfH;
		double dy = 1.0 - (2.0*(curSurfY + _height + _margin)) / surfH;
		double lx = (2.0*(curSurfX + _margin)) / surfW - 1.0;;

		for (double i = 0; i < stripW; i++)
		{
			double cx = (i + 1) / stripW;			
			double r, g, b;
			color->GetColor(&r, &g, &b, cx);
			glColor3d(r, g, b);

			double rx = (2.0*(curSurfX + _margin+i+1)) / surfW - 1.0;
			glRectd(lx, ty, rx, dy);

			lx = rx;
		}

		if (color == _selColor)
		{
			int sleft = curSurfX + _margin - 3;
			int stop = curSurfY + _margin - 3;
			int sright = sleft + stripW + 6;
			int sbottom = stop + _height + 6;
			GLfloat left, top, right, bottom;
			surf->Surface2glndc(sleft, stop, &left, &top);
			surf->Surface2glndc(sright, sbottom, &right, &bottom);
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			glEnable(GL_LINE_SMOOTH);
			glBegin(GL_LINE_STRIP);
			glVertex3f(left, top, 0.0f);
			glVertex3f(right, top, 0.0f);
			glVertex3f(right, bottom, 0.0f);
			glVertex3f(left, bottom, 0.0f);
			glVertex3f(left, top, 0.0f);
			glEnd();
		}

		drwColorIdxBegin++;
		drwYBegin -= _occupy;
	}
}

const OGREnvelope * GradientColorRepositoryRender::GetEnvelope()
{
	return &_envelope;
}

const OGRSpatialReference * GradientColorRepositoryRender::GetSpatialReference()
{
	return nullptr;
}

bool GradientColorRepositoryRender::CanTransformTo(const OGRSpatialReference * spatialRef)
{
	if (spatialRef == nullptr)
		return true;

	return false;
}

float GradientColorRepositoryRender::GetOpacity()
{
	return 1.0f;
}

void GradientColorRepositoryRender::SetOpacity(float)
{
	return;
}


END_NAME_SPACE(tGis, Core)

