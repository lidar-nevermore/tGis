#include "ToolKitRepository.h"
#include "ToolKit.h"
#include <memory>
#include <stdarg.h>
#include <vector>
#include <map>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

ToolKitRepository* ToolKitRepository::_instance = nullptr;


ToolKitRepository* ToolKitRepository::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new ToolKitRepository();
		static unique_ptr<ToolKitRepository> shit(_instance);
	}

	return _instance;
}

ToolKitRepository::ToolKitRepository()
{
}


ToolKitRepository::~ToolKitRepository()
{
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

void ToolKitRepository::SaveStandaloneTool()
{
	//TODO: 将StandaloneTool的信息保存至文件
}


END_NAME_SPACE(tGis, Core)

