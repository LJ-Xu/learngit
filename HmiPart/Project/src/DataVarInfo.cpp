#include "DataVarInfo.h"
namespace Project
{
	DataVarId DataVarId::NullId;
	void DataVarId::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("VarID") && jsonObj["VarID"].IsObject())
		{
			if (jsonObj["VarID"].HasMember("VID"))
				Vid = jsonObj["VarID"]["VID"].GetInt();
		}
		if (jsonObj.HasMember("VID"))
			Vid = jsonObj["VID"].GetInt();
	}
}