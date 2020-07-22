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
	}

	GradientColor* _owner;

	vector<ColorNode> _vecColor;

	void GetColor(double * r, double * g, double * b, double pos)
	{
		if (_vecColor.size() == 0)
			return;

		if (_vecColor.size() == 1)
		{
			ColorNode clr = _vecColor[0];
			*r = clr.r;
			*g = clr.g;
			*b = clr.b;

			return;
		}

		if (pos < 0)
			pos = 0;
		if (pos > 1)
			pos = 1;

		ColorNode firstColor = *(_vecColor.begin());
		ColorNode secondColor;

		double minPos = firstColor.pos;
		double maxPos = _vecColor.rbegin()->pos;

		double colorPos = minPos + pos*(maxPos - minPos);

		for (auto it = _vecColor.begin(); it != _vecColor.end(); it++)
		{
			secondColor = *it;
			if (secondColor.pos > colorPos || secondColor.pos >= maxPos)
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

GradientColor * GradientColor::Clone()
{
	GradientColor* color = new GradientColor();
	color->_impl_->_vecColor.insert(color->_impl_->_vecColor.begin(), _impl_->_vecColor.begin(), _impl_->_vecColor.end());

	return color;
}

void GradientColor::AddKeyColor(unsigned char r, unsigned char g, unsigned char b, double pos)
{
	assert(_impl_->_vecColor.size() == 0 || pos >= _impl_->_vecColor.rbegin()->pos);

	ColorNode cn;
	cn.r = r;
	cn.g = g;
	cn.b = b;
	cn.pos = pos;
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
	*pos = cn.pos;
}

void GradientColor::ClearKeyColor()
{
	_impl_->_vecColor.clear();
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
