#pragma once
#include "BaseModel.h"
#include "DynamicTextBarConfig.h"
#include <iostream>
#include <vector>

using namespace std;

namespace UI
{
	class DynamicTextContextModel : public BaseModel
	{
	public:
		DynamicTextContextModel();
		~DynamicTextContextModel();

		void GetRect(Rectangle & rect) 
		{
			rect.X = DynamicTextBarConfig.X + DynamicTextBarConfig.OffX;
			rect.Y = DynamicTextBarConfig.Y + DynamicTextBarConfig.OffY;
			rect.W = DynamicTextBarConfig.Width;
			rect.H = DynamicTextBarConfig.Height;
		}
		void InitTestData();
		void * GetTestConfig() {
			return DynamicTextBarUnit;
		}
		void InitData(string json) override;
		void InitData(vector<char> data)
		{
			return BaseModel::InitData<decltype(DynamicTextBarUnit)>(data, DynamicTextBarUnit);
		}
		unique_ptr<vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(DynamicTextBarUnit)>(DynamicTextBarUnit);
		}

	public:
		Project::DynamicTextBarConfig DynamicTextBarUnit;
	};
}

