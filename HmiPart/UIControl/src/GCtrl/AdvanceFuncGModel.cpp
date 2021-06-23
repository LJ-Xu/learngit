#include "AdvanceFuncGModel.h"
#include "Logger.h"

namespace UI
{

	void AdvanceFuncGModel::InitData(string jsonstr) {
		rapidjson::Document json;
		//����json�ַ���
		json.Parse(jsonstr.c_str());
		//��ȡ�������
		if (json.HasParseError()) {
			cout << "Parse Error : ("
				<< json.GetParseError() << " : "
				<< json.GetErrorOffset() << ")" << endl;
		}
		else {
			LOG_INFO_("Parse AdvanceFuncGMode json\n");
			if (json.HasMember("AdvanceFunctions") && json["AdvanceFunctions"].IsArray())
				Project::AdvanceFuncRes::Parse(AdvFuncGConfig.Functions, json["AdvanceFunctions"]);
		}
	}

	void AdvanceFuncGModel::InitData(vector<char> data) {
		LOG_INFO_("AdvanceFuncGMode InitData\n");
		return BaseModel::InitData<decltype(AdvFuncGConfig)>(data, AdvFuncGConfig);
	}


}