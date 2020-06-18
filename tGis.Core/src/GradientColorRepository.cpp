#include "GradientColorRepository.h"
#include "GradientColor.h"
#include <memory>

#include <vector>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

GradientColorRepository* GradientColorRepository::_instance = nullptr;


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
		_instance->AddGradientColor(inferno);

		GradientColor* red = new GradientColor();
		red->AddKeyColor(0, 0, 0, 0);
		red->AddKeyColor(237, 17, 10, 1.0);
		_instance->AddGradientColor(red);

		GradientColor* yellow = new GradientColor();
		yellow->AddKeyColor(0, 0, 0, 0);
		yellow->AddKeyColor(237, 235, 10, 1.0);
		_instance->AddGradientColor(yellow);

		GradientColor* spectral = new GradientColor();
		spectral->AddKeyColor(215, 25, 28, 0);
		spectral->AddKeyColor(253, 174, 97, 0.25);
		spectral->AddKeyColor(255, 255, 191, 0.5);
		spectral->AddKeyColor(171, 221, 164, 0.75);
		spectral->AddKeyColor(43, 131, 186, 1.0);
		_instance->AddGradientColor(spectral);

		GradientColor* oranges = new GradientColor();
		oranges->AddKeyColor(255, 245, 235, 0);
		oranges->AddKeyColor(253, 208, 162, 0.25);
		oranges->AddKeyColor(253, 141, 60, 0.5);
		oranges->AddKeyColor(217, 72, 1, 0.76);
		oranges->AddKeyColor(127, 39, 4, 1.0);
		_instance->AddGradientColor(oranges);

		GradientColor* viridis = new GradientColor();
		viridis->AddKeyColor(68, 1, 84, 0);
		viridis->AddKeyColor(65, 66, 135, 0.2);
		viridis->AddKeyColor(36, 170, 130, 0.61);
		viridis->AddKeyColor(127, 210, 80, 0.8);
		viridis->AddKeyColor(253, 231, 37, 1.0);		
		_instance->AddGradientColor(viridis);

		static unique_ptr<GradientColorRepository> shit(_instance);
	}

	return _instance;
}


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
			delete sl;
	}

	delete _impl_;
}


size_t GradientColorRepository::GetGradientColorCount()
{
	return _impl_->_vecGradientColor.size();
}

GradientColor * GradientColorRepository::GetGradientColor(size_t pos)
{
	return _impl_->_vecGradientColor.at(pos);
}

void GradientColorRepository::AddGradientColor(GradientColor * color)
{
	_impl_->_vecGradientColor.push_back(color);
}

void GradientColorRepository::FromXml(tinyxml2::XMLDocument * doc)
{
}

void GradientColorRepository::ToXml(tinyxml2::XMLDocument * doc)
{
}


END_NAME_SPACE(tGis, Core)

