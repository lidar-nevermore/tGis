#include "ToolKitRepository.h"
#include "ToolKit.h"
#include <stdarg.h>

BEGIN_NAME_SPACE(tGis, Core)

ToolKitRepository* ToolKitRepository::_instance = nullptr;


ToolKitRepository & ToolKitRepository::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new ToolKitRepository();
		static _tGisObjectDestructor<ToolKitRepository> shit(_instance);
	}

	return *_instance;
}

ToolKitRepository::ToolKitRepository()
{
}


ToolKitRepository::~ToolKitRepository()
{
}

void ToolKitRepository::AddToolKit(ToolKit * kit)
{
	ToolKit* toFillKit = GetToolKit(kit->GetName());
	if (toFillKit == NULL)
	{
		_vecToolKit.push_back(kit);
		_mapToolKit.insert(map<string, ToolKit*>::value_type(kit->GetName(), kit));
	}
	else
	{
		size_t toolCount = kit->GetToolCount();
		for (size_t i = 0; i < toolCount; i++)
		{
			toFillKit->AddTool(kit->GetTool(i));
		}

		size_t toolkitCount = kit->GetToolKitCount();
		for (size_t i = 0; i < toolkitCount; i++)
		{
			toFillKit->AddToolKit(kit->GetToolKit(i));
		}
	}
}


void ToolKitRepository::AddToolKit(int count, ...)
{
	ToolKit* toFillKit = nullptr;
	va_list toAddKits;
	va_start(toAddKits, count);
	for (int i = 0; i < count; i++)
	{
		ToolKit* preToFillKit = toFillKit;
		ToolKit* kit = va_arg(toAddKits, ToolKit*);
		if (i == 0)
			toFillKit = GetToolKit(kit->GetName());
		else
			toFillKit = toFillKit->GetToolKit(kit->GetName());

		if (toFillKit == nullptr)
		{
			if (i == 0)
				AddToolKit(kit);
			else
				preToFillKit->AddToolKit(kit);

			toFillKit = kit;
		}
		else
		{
			size_t toolCount = kit->GetToolCount();
			for (size_t j = 0; j < toolCount; j++)
			{
				toFillKit->AddTool(kit->GetTool(j));
			}

			size_t toolkitCount = kit->GetToolKitCount();
			for (size_t i = 0; i < toolkitCount; i++)
			{
				toFillKit->AddToolKit(kit->GetToolKit(i));
			}
		}
	}
	va_end(toAddKits);
}


size_t ToolKitRepository::GetToolKitCount()
{
	return _vecToolKit.size();
}

ToolKit * ToolKitRepository::GetToolKit(size_t pos)
{
	return _vecToolKit.at(pos);
}

ToolKit * ToolKitRepository::GetToolKit(const char * name)
{
	map<string, ToolKit*>::iterator pos = _mapToolKit.find(name);

	if (pos != _mapToolKit.end())
		return (*pos).second;
	return nullptr;
}


END_NAME_SPACE(tGis, Core)

