#include "SampleGModel.h"

namespace UI
{
	SampleGModel::SampleGModel() {

	}

	SampleGModel::~SampleGModel() {

	}

	void SampleGModel::GetRect(Rectangle & rect) {

	}

	void SampleGModel::InitTestData() {
		// 初始化原始属性
		// SampleGUnit.CtrlName = "SampleControl";
		// Project::SampleInfoRes res;
		// res.CollectMode = Project::SampleMode::Trigger;
		// res.BtyeCondition = Project::Conditions::LT;
		// res.ComPareValue = 10;
		// Project::SampleChannel channel;
		// channel.Channel = 1;
		// res.SimpleChannelLst.push_back(channel);
		// SampleGUnit.InfoLst.push_back(res);
		// SampleGUnit.X = 0;
		// SampleGUnit.Y = 0;
		// SampleGUnit.Width = 0;
		// SampleGUnit.Height = 0;
	}

	// 获取控件配置
	void * SampleGModel::GetTestConfig() {
		return &SampleGUnit;
	}

	// 解析json字符串
	void SampleGModel::InitData(string jsonstr) {
		rapidjson::Document json;
		// 解析json字符串
		json.Parse(jsonstr.c_str());
		// 获取解析结果
		if (json.HasParseError()) {
			cout << "Parse Error : ("
				<< json.GetParseError() << ": "
				<< json.GetErrorOffset() << ")" << endl;
		} else {
			//SimpleConfigProperty
			if (json.HasMember("DataSimple") && json["DataSimple"].IsObject() &&
				json["DataSimple"].HasMember("SimpleConfigProperty") &&
				json["DataSimple"]["SimpleConfigProperty"].IsObject())
			{
				rapidjson::Value &samplejson = json["DataSimple"]["SimpleConfigProperty"];
				if (samplejson.HasMember("DataSimpleUintsLst") && samplejson["DataSimpleUintsLst"].IsArray())
					Project::SampleInfoRes::Parse(SampleGUnit.InfoLst, samplejson["DataSimpleUintsLst"]);
			}
		}
	}

	void SampleGModel::InitData(vector<char> data) {
		return BaseModel::InitData<decltype(SampleGUnit)>(data, SampleGUnit);
	}

	unique_ptr<vector<char>> SampleGModel::GetData() {
		return BaseModel::GetData<decltype(SampleGUnit)>(SampleGUnit);
	}
}