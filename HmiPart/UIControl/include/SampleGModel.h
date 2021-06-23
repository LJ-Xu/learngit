#pragma once
#include "BaseModel.h"
#include "DataSampleConfig.h"
#include <iostream>
#include <vector>

using namespace std;

namespace UI
{
	class SampleGModel : public BaseModel
	{
	public:
		SampleGModel();
		~SampleGModel();

		void GetRect(Rectangle & rect);
		void InitTestData();
		void * GetTestConfig();
		void InitData(string json) override;
		void InitData(vector<char> data);
		unique_ptr<vector<char>> GetData();

	public:
		Project::DataSampleConfig SampleGUnit;
	};
}

