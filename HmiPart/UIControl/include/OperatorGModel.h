#pragma once
#include "BaseModel.h"
#include "OperatorRecordConfig.h"
#include <iostream>
#include <vector>

using namespace std;

namespace UI
{
	class OperatorGModel : public BaseModel
	{
	public:
		OperatorGModel();
		~OperatorGModel();

		void GetRect(Rectangle & rect);
		void InitTestData();
		void * GetTestConfig();
		void InitData(string json) override;
		void InitData(vector<char> data);
		unique_ptr<vector<char>> GetData();

	public:
		Project::OperatorRecordConfig OperatorGConfig;
	};
}

