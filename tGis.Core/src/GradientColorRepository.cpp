#include "GradientColorRepository.h"
#include "GradientColor.h"
#include <memory>

#include <vector>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

GradientColorRepository* GradientColorRepository::_instance = nullptr;


class GradientColorRepositoryImpl
{
public:
	GradientColorRepositoryImpl(GradientColorRepository* owner)
	{
		_owner = owner;
	}

	GradientColorRepository* _owner;

	vector<GradientColor*> _vecGradientColor;
};

GradientColorRepository * GradientColorRepository::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new GradientColorRepository();

		GradientColor* inferno = new GradientColor();
		inferno->AddKeyColor(0, 0, 4, 0);
		inferno->AddKeyColor(30, 15, 72, 0.01);
		inferno->AddKeyColor(64, 10, 103, 0.2);
		inferno->AddKeyColor(207, 57, 82, 0.5);
		inferno->AddKeyColor(251, 166, 12, 0.8);
		inferno->AddKeyColor(242, 242, 125, 0.95);
		inferno->AddKeyColor(252, 255, 164, 1.0);
		_instance->_impl_->_vecGradientColor.push_back(inferno);

		GradientColor* red = new GradientColor();
		red->AddKeyColor(50, 50, 50, 0);
		red->AddKeyColor(251, 17, 10, 1.0);
		_instance->_impl_->_vecGradientColor.push_back(red);

		GradientColor* pureRed = new GradientColor();
		pureRed->AddKeyColor(255, 0, 0, 0);
		_instance->_impl_->_vecGradientColor.push_back(pureRed);

		GradientColor* yellow = new GradientColor();
		yellow->AddKeyColor(50, 50, 50, 0);
		yellow->AddKeyColor(251, 253, 10, 1.0);
		_instance->_impl_->_vecGradientColor.push_back(yellow);

		GradientColor* blue = new GradientColor();
		blue->AddKeyColor(50, 50, 50, 0);
		blue->AddKeyColor(17, 35, 253, 1.0);
		_instance->_impl_->_vecGradientColor.push_back(blue);

		GradientColor* pureBlue = new GradientColor();
		pureBlue->AddKeyColor(0, 0, 255, 0);
		_instance->_impl_->_vecGradientColor.push_back(pureBlue);

		GradientColor* green = new GradientColor();
		green->AddKeyColor(50, 50, 50, 0);
		green->AddKeyColor(17, 237, 25, 1.0);
		_instance->_impl_->_vecGradientColor.push_back(green);

		GradientColor* pureCyan = new GradientColor();
		pureCyan->AddKeyColor(255, 0, 255, 0);
		_instance->_impl_->_vecGradientColor.push_back(pureCyan);

		GradientColor* soilgrass = new GradientColor();
		soilgrass->AddKeyColor(0, 0, 0, 0);
		soilgrass->AddKeyColor(9, 9, 9, 0.2);
		soilgrass->AddKeyColor(59, 18, 14, 0.27);
		soilgrass->AddKeyColor(95, 64, 44, 0.35);
		soilgrass->AddKeyColor(118, 114, 87, 0.5);
		soilgrass->AddKeyColor(17, 237, 25, 1.0);
		_instance->_impl_->_vecGradientColor.push_back(soilgrass);

		GradientColor* spectral = new GradientColor();
		spectral->AddKeyColor(215, 25, 28, 0);
		spectral->AddKeyColor(253, 174, 97, 0.25);
		spectral->AddKeyColor(255, 255, 191, 0.5);
		spectral->AddKeyColor(171, 221, 164, 0.75);
		spectral->AddKeyColor(43, 131, 186, 1.0);
		_instance->_impl_->_vecGradientColor.push_back(spectral);

		GradientColor* oranges = new GradientColor();
		oranges->AddKeyColor(255, 245, 235, 0);
		oranges->AddKeyColor(253, 208, 162, 0.25);
		oranges->AddKeyColor(253, 141, 60, 0.5);
		oranges->AddKeyColor(217, 72, 1, 0.76);
		oranges->AddKeyColor(127, 39, 4, 1.0);
		_instance->_impl_->_vecGradientColor.push_back(oranges);

		GradientColor* viridis = new GradientColor();
		viridis->AddKeyColor(68, 1, 84, 0);
		viridis->AddKeyColor(65, 66, 135, 0.2);
		viridis->AddKeyColor(36, 170, 130, 0.61);
		viridis->AddKeyColor(127, 210, 80, 0.8);
		viridis->AddKeyColor(253, 231, 37, 1.0);		
		_instance->_impl_->_vecGradientColor.push_back(viridis);

		GradientColor* hue = new GradientColor();
		hue->AddKeyColor(255, 0, 0, 0);
		hue->AddKeyColor(255, 255, 0, 1);
		hue->AddKeyColor(0, 255, 0, 2);
		hue->AddKeyColor(0, 255, 255, 3);
		hue->AddKeyColor(0, 0, 255, 4);
		hue->AddKeyColor(255, 0, 255, 5);
		hue->AddKeyColor(255, 0, 0, 6);
		_instance->_impl_->_vecGradientColor.push_back(hue);

		static unique_ptr<GradientColorRepository> shit(_instance);
	}

	return _instance;
}

GradientColorRepository::GradientColorRepository()
{
	_impl_ = new GradientColorRepositoryImpl(this);
}

GradientColorRepository::~GradientColorRepository()
{
	for (auto it = _impl_->_vecGradientColor.begin(); it != _impl_->_vecGradientColor.end(); it++)
	{
		GradientColor* sl = *it;

		if (sl->_is_in_heap)
			sl->Release();
	}

	delete _impl_;
}


size_t GradientColorRepository::GetGradientColorCount()
{
	return _impl_->_vecGradientColor.size();
}

GradientColor * GradientColorRepository::GetGradientColor(size_t pos)
{
	GradientColor* color = _impl_->_vecGradientColor.at(pos);
	color->Reference();
	return color;
}

void GradientColorRepository::AddGradientColor(GradientColor * color)
{
	color->Reference();
	_impl_->_vecGradientColor.push_back(color);
}

void GradientColorRepository::FromXml(tinyxml2::XMLDocument * doc)
{
}

void GradientColorRepository::ToXml(tinyxml2::XMLDocument * doc)
{
}


END_NAME_SPACE(tGis, Core)

