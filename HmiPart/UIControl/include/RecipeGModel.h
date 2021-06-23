#pragma once
#include "BaseModel.h"
#include "DataRecipeConfig.h"
#include <iostream>
#include <vector>

using namespace std;

namespace UI
{
	class RecipeGModel : public BaseModel
	{
	public:
		RecipeGModel();
		~RecipeGModel();
		Project::DataRecipeConfig RecipeGConfig;

		void GetRect(Rectangle & rect);
		void InitTestData();
		void * GetTestConfig();
		void InitData(string json) override;
		void InitData(vector<char> data);
		unique_ptr<vector<char>> GetData();
	};
}
