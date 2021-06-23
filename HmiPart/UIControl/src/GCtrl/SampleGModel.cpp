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
		// ��ʼ��ԭʼ����
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

	// ��ȡ�ؼ�����
	void * SampleGModel::GetTestConfig() {
		return &SampleGUnit;
	}

	// ����json�ַ���
	void SampleGModel::InitData(string jsonstr) {
		rapidjson::Document json;
		// ����json�ַ���
		json.Parse(jsonstr.c_str());
		// ��ȡ�������
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