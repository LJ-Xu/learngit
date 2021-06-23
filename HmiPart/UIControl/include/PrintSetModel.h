#pragma once
#include "BaseModel.h"
#include "PrintSetConfig.h"
#include <iostream>
#include <vector>

using namespace std;

namespace UI
{
	class PrintSetModel : public BaseModel
	{
	public:
		PrintSetModel() {};
		~PrintSetModel() {};
		Project::PrintSetConfig PrintConfig;

		void GetRect(Rectangle & rect) {};
		void * GetTestConfig()
		{
			return &PrintConfig;
		};
		void InitData(string json) override;
		void InitData(vector<char> data);
		unique_ptr<vector<char>> GetData()
		{
			return BaseModel::GetData<decltype(PrintConfig)>(PrintConfig);
		};
	};
}

