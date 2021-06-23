#pragma once
#include "BaseModel.h"
#include "CHirePurchase.h"
#include <iostream>
#include <vector>
using namespace std;

namespace UI
{
	class HirePurchaseGModel : public BaseModel
	{
	public:
		HirePurchaseGModel() {}
		~HirePurchaseGModel() {}

		void GetRect(Rectangle & rect) {}
		void InitTestData() {}
		void * GetTestConfig() {
			return &HirePurchaseConfig;
		}
		void InitData(string json) override;
		void InitData(vector<char> data);
		unique_ptr<vector<char>> GetData();

	public:
		Project::CHirePurchase HirePurchaseConfig;
	};
}

