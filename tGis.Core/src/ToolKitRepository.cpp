#include "ToolKitRepository.h"
#include "ToolKit.h"
#include "StandaloneTool.h"
#include "tGisApplication.h"
#include "ToolKitSetImpl.inl"

#include "tinyxml2.h"

#include <memory>
#include <stdarg.h>
#include <vector>
#include <map>

using namespace std;
using namespace tinyxml2;

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


void ToolKitRepository::AddTool(const char * belong, ITool * tool)
{
	std::vector<std::string> belongParts;
	_tgis_str_split(belong, "/", belongParts);

	ToolKit* toFillKit = nullptr;
	ToolKitSet* kitSet = this;
	bool needNewKit = false;

	for (auto it = belongParts.begin(); it != belongParts.end(); it++)
	{
		string& strKit = *it;

		if (strKit.empty())
			continue;

		ToolKit* kit = needNewKit ? nullptr : kitSet->GetToolKit(strKit.c_str());
		if (kit == nullptr)
		{
			needNewKit = true;
			kit = new ToolKit(strKit.c_str());
			kitSet->AddToolKit(kit);
		}

		toFillKit = kit;
		kitSet = kit;
	}

	toFillKit->AddTool(tool);
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
	tinyxml2::XMLDocument doc;
	SaveStandaloneTool(&doc, this);
	string path = tGisApplication::INSTANCE()->GetExeDir();
	path.append(TGIS_PATH_SEPARATOR_STR);
	path.append("Plugins");
	path.append(TGIS_PATH_SEPARATOR_STR);
	path.append("StandaloneTool.xml");
	doc.SaveFile(path.c_str());
}

void ToolKitRepository::SaveStandaloneTool(tinyxml2::XMLDocument* doc, ToolKitSet * kitSet)
{
	for (auto it = kitSet->_impl_->_vecStandaloneTool.begin(); it != kitSet->_impl_->_vecStandaloneTool.end(); it++)
	{
		StandaloneTool* tool = *it;

		XMLElement* eleTool = doc->NewElement("StandaloneTool");
		doc->LinkEndChild(eleTool);

		string strToolBelong;
		ToolKit* parent = tool->GetParent();
		while (nullptr != parent)
		{
			strToolBelong.insert(0, parent->GetName());
			strToolBelong.insert(0, "/");
			parent = parent->GetParent();
		}

		XMLElement* eleName = doc->NewElement("name");
		eleTool->LinkEndChild(eleName);
		XMLText* txtName = doc->NewText(tool->GetName());
		//txtName->SetCData(true);
		eleName->LinkEndChild(txtName);

		XMLElement* eleToolBelong = doc->NewElement("belong");
		eleTool->LinkEndChild(eleToolBelong);
		XMLText* txtToolBelong = doc->NewText(strToolBelong.c_str());
		txtToolBelong->SetCData(true);
		eleToolBelong->LinkEndChild(txtToolBelong);

		XMLElement* eleExeFile = doc->NewElement("exe");
		eleTool->LinkEndChild(eleExeFile);
		XMLText* txtExeFile = doc->NewText(tool->GetExeFile());
		txtExeFile->SetCData(true);
		eleExeFile->LinkEndChild(txtExeFile);

		XMLElement* eleParam = doc->NewElement("param");
		eleTool->LinkEndChild(eleParam);

		size_t paramCount = tool->GetParamCount();
		for (size_t i = 0; i < paramCount; i++)
		{
			XMLElement* eleString = doc->NewElement("str");
			eleParam->LinkEndChild(eleString);
			eleString->SetText(tool->GetParam(i));
		}
	}

	for (auto it = kitSet->_impl_->_vecToolKit.begin(); it != kitSet->_impl_->_vecToolKit.end(); it++)
	{
		SaveStandaloneTool(doc, *it);
	}
}


END_NAME_SPACE(tGis, Core)

