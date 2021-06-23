#pragma once
#include "BaseModel.h"
#include <iostream>
#include <vector>
#include "SysSetGConfig.h"
using namespace std;

namespace UI
{
	class SysSetGModel : public BaseModel
	{
	public:
		SysSetGModel();
		~SysSetGModel();

		void GetRect(Rectangle & rect);
		void InitTestData();
		void * GetTestConfig();
		void InitData(string json) override;
		void InitData(vector<char> data);
		unique_ptr<vector<char>> GetData();

	public:
		Project::SysSetGConfig SysSetGUnit;
	};
}
