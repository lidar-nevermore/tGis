#include "tGisApplication.h"
#include "PluginManager.h"
#include "ilog.h"

#include "ToolKit.h"
#include "ToolKitRepository.h"
#include "StandaloneTool.h"

#include <stdarg.h>
#include <string>

#include "tinyxml2.h"

using namespace tinyxml2;

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

tGisApplication* tGisApplication::_instance = nullptr;

Event<const char*, const char*, const char*> tGisApplication::LoadPluginEvent;

tGisApplication* tGisApplication::INSTANCE()
{
	return _instance;
}

void LoadStandaloneTool(const char* tgis);

tGisApplication::tGisApplication()
{
	if (_instance != NULL)
		throw exception("Only allow one instance!");
	_tgis_get_exe_dir(_exeDir, nullptr);
	std::string logPath = _exeDir;
	logPath.append(TGIS_PATH_SEPARATOR_STR);
	logPath.append("Log");
	logPath.append(TGIS_PATH_SEPARATOR_STR);
	logPath.append("tgis.");
	_loger = init_log(logPath.c_str(), LOG_DEBUG, 0);
	_instance = this;
	PluginManager::INSTANCE()->LoadPlugins();
	LoadStandaloneTool(_exeDir);
}

tGisApplication::~tGisApplication()
{
	finalize_log(_loger);
}

void LoadStandaloneTool(const char* tgis)
{
	string path = tgis;
	path.append(TGIS_PATH_SEPARATOR_STR);
	path.append("Plugins");
	path.append(TGIS_PATH_SEPARATOR_STR);
	path.append("StandaloneTool.xml");

	tinyxml2::XMLDocument doc;
	if (XML_SUCCESS == doc.LoadFile(path.c_str()))
	{
		XMLElement *eleTool = doc.FirstChildElement();
		while (eleTool != nullptr)
		{
			XMLElement* eleName = eleTool->FirstChildElement("name");
			StandaloneTool* tool = new StandaloneTool(eleName->GetText());

			XMLElement* eleExeFile = eleTool->FirstChildElement("exe");
			tool->SetExeFile(eleExeFile->GetText());

			XMLElement* eleParam = eleTool->FirstChildElement("param");
			XMLElement* eleParamItem = eleParam->FirstChildElement();
			while (eleParamItem != nullptr)
			{
				tool->AddParam(eleParamItem->GetText());
				eleParamItem = eleParamItem->NextSiblingElement();
			}

			XMLElement* eleToolBelong = eleTool->FirstChildElement("belong");
			ToolKitRepository::INSTANCE()->AddTool(eleToolBelong->GetText(), tool);

			//加载下一个工具
			eleTool = eleTool->NextSiblingElement();
		}
	}
}


END_NAME_SPACE(tGis, Core)
