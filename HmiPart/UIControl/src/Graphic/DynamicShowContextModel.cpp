#include "stdafx.h"
#include "DynamicTextBarModel.h"
#include <FL/Enumerations.H>

#include "JsonComm.h"
using namespace rapidjson;
namespace UI
{
	void DynamicTextBarModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}

		DynamicTextBarUnit.InitBaseData(json);

		
	}
}