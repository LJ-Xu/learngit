#include "ProjectResources.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
namespace Project
{
	void ProjectResources::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			//解析map<string, ResItem> resMap;
			if (json.HasMember("ResMap"))
			{
				if (json["ResMap"].IsObject() && !json["ResMap"].IsNull())
				{
					Value &ResTmp = json["ResMap"];
					for (rapidjson::Value::MemberIterator iter = ResTmp.MemberBegin();iter != ResTmp.MemberEnd();iter++)
					{
						ResItem RItem;
						string name = (iter->name).GetString();
						Value& value = iter->value;
						if (value.IsObject() && !value.IsNull())
						{
							if (value.HasMember("FileType"))
								RItem.FileType = value["RItem"].GetString()[0];
							if (value.HasMember("Size"))
								RItem.Size = value["Size"].GetInt();
							if (value.HasMember("Offset"))
								RItem.Offset = value["Offset"].GetInt();

							if (value.HasMember("Param") && value["Param"].IsArray())
							{
								Value& array = value["Param"];
								for (unsigned i = 0; i < array.Size(); i++) {
									Value& tmp = array[i];
									RItem.Param[i] = array[i].GetString()[0];
								}
							}
						}
						//ResMap.insert(pair<string, ResItem>(name, RItem));
						ResMap.push_back(RItem);
					}

				}

			}
		
		}
	}
}
 