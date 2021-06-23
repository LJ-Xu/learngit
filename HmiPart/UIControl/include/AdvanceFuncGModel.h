#pragma once
#include "BaseModel.h"
#include "AdvanceFuncConfig.h"
#include <iostream>
#include <vector>

using namespace std;

namespace UI
{
	class AdvanceFuncGModel : public BaseModel
	{
	public:
		AdvanceFuncGModel() {};
		~AdvanceFuncGModel() {};
		Project::AdvanceFuncConfig AdvFuncGConfig;

		void GetRect(Rectangle & rect) {};
		void * GetTestConfig()
		{
			return &AdvFuncGConfig;
		};
		void InitData(string json) override;
		void InitData(vector<char> data);
		unique_ptr<vector<char>> GetData()
		{
			return BaseModel::GetData<decltype(AdvFuncGConfig)>(AdvFuncGConfig);
		};
	};
}

