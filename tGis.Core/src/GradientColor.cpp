#include "GradientColor.h"
#include <cassert>
#include <vector>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct ColorNode
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	double pos;
};

class GradientColorImpl
{
public:
	GradientColorImpl(GradientColor* owner)
	{
		_owner = owner;
		_maxPos = 0.0;
	}

	GradientColor* _owner;

	vector<ColorNode> _vecColor;
	double _maxPos;

	void GetColor(double * r, double * g, double * b, double pos)
	{
		if (pos < 0)
			pos = 0;
		if (pos > 1)
			pos = 1;

		double colorPos = pos*_maxPos;

		ColorNode firstColor;
		ColorNode secondColor;

		for (auto it = _vecColor.begin(); it != _vecColor.end(); it++)
		{
			secondColor = *it;
			if (secondColor.pos > colorPos || secondColor.pos >= 1.0)
				break;
			firstColor = secondColor;
		}

		double d = (colorPos - firstColor.pos) / (secondColor.pos - firstColor.pos);
		double r1 = firstColor.r;
		double g1 = firstColor.g;
		double b1 = firstColor.b;
		double r2 = secondColor.r;
		double g2 = secondColor.g;
		double b2 = secondColor.b;

		double rr = r1 + d*(r2 - r1);
		double gr = g1 + d*(g2 - g1);
		double br = b1 + d*(b2 - b1);

		*r = rr >= 255 ? 255 : (rr < 0 ? 0 : rr);
		*g = gr >= 255 ? 255 : (gr < 0 ? 0 : gr);
		*b = br >= 255 ? 255 : (br < 0 ? 0 : br);
	}

};

GradientColor::GradientColor()
{
	_impl_ = new GradientColorImpl(this);
}

GradientColor::~GradientColor()
{
	delete _impl_;
}

void GradientColor::AddKeyColor(unsigned char r, unsigned char g, unsigned char b, double pos)
{
	assert(pos >= _impl_->_maxPos);

	ColorNode cn;
	cn.r = r;
	cn.g = g;
	cn.b = b;
	cn.pos = pos;
	_impl_->_maxPos = pos;
	_impl_->_vecColor.push_back(cn);
}

size_t GradientColor::GetKeyColorCount()
{
	return _impl_->_vecColor.size();
}

void GradientColor::GetKeyColor(size_t idx, unsigned char * r, unsigned char * g, unsigned char * b, double * pos)
{
	ColorNode& cn = _impl_->_vecColor[idx];
	*r = cn.r;
	*g = cn.g;
	*b = cn.b;
}

void GradientColor::SetKeyColor(size_t idx, unsigned char r, unsigned char g, unsigned char b, double pos)
{
#ifdef _DEBUG
	double prevPos = -DBL_MAX;
	if (idx > 0)
		prevPos = _impl_->_vecColor[idx - 1].pos;
	double nextPos = DBL_MAX;
	if ((idx + 1) < _impl_->_vecColor.size())
		nextPos = _impl_->_vecColor[idx + 1].pos;

	assert(pos > prevPos && pos < nextPos);
#endif // DEBUG

	ColorNode& cn = _impl_->_vecColor[idx];
	cn.r = r;
	cn.g = g;
	cn.b = b;
	cn.pos = pos;
}

void GradientColor::FromXml(tinyxml2::XMLElement * xelem)
{
}

void GradientColor::ToXml(tinyxml2::XMLElement * xelem)
{
}

void GradientColor::GetColor(unsigned char * r_, unsigned char * g_, unsigned char * b_, double pos)
{
	double r, g, b;
	_impl_->GetColor(&r, &g, &b, pos);
	*r_ = unsigned char(r);
	*g_ = unsigned char(g);
	*b_ = unsigned char(b);
}

void GradientColor::GetColor(double * r, double * g, double * b, double pos)
{
	double rr;
	double gr;
	double br;
	_impl_->GetColor(&rr, &gr, &br, pos);
	*r = rr / 255.0;
	*g = gr / 255.0;
	*b = br / 255.0;
}

END_NAME_SPACE(tGis, Core)
