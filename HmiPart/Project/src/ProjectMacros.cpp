#include "ProjectMacros.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
namespace Project
{
	void ProjectMacros::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			//解析map<string, PrjMacro> macros;
			if (json.HasMember("macros"))
			{
				if (json["macros"].IsObject() && !json["macros"].IsNull())
				{
					PrjMacro PItem;
					Value &macrosTmp = json["macros"];
					for (rapidjson::Value::MemberIterator iter = macrosTmp.MemberBegin();iter != macrosTmp.MemberEnd();iter++)
					{
						string name = (iter->name).GetString();
						Value& value = iter->value;
						if (value.IsObject() && !value.IsNull())
						{
							if (value.HasMember("XJType") && !value["XJType"].IsNull())
								PItem.type = value["XJType"].GetString();
							if (value.HasMember("Name") && !value["Name"].IsNull())
								PItem.name = value["Name"].GetString();
							if (value.HasMember("Context") && !value["Context"].IsNull())
								PItem.context = value["Context"].GetString();
						}
						macros.insert(pair<string, PrjMacro>(name, PItem));
					}

				}

			}
		}
	}
}
 