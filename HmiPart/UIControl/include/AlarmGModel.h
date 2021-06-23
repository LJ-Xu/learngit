#pragma once
#include "BaseModel.h"
#include "AlarmInputConfig.h"
#include <iostream>
#include <vector>

using namespace std;

namespace UI
{
	class AlarmGModel : public BaseModel
	{
	public:
		AlarmGModel();
		~AlarmGModel();

		void GetRect(Rectangle & rect);
		void InitTestData();
		void * GetTestConfig();
		void InitData(string json) override;
		void InitData(vector<char> data);
		void ParseAlarmSaveLst(Project::SaveFileRes& svrs, rapidjson::Value& jsonObj);
		unique_ptr<vector<char>> GetData();

	public:
		Project::AlarmInputConfig AlarmGUnit;
	};
}

