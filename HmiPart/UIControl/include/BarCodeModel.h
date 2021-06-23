#pragma once
#include "BaseModel.h"
#include "BarCodeConfig.h"

namespace UI
{
	class BarCodeModel : public BaseModel
	{
	public:
		Project::BarCodeConfig BarcodeConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = BarcodeConfig.X + BarcodeConfig.OffX;
			rect.Y = BarcodeConfig.Y + BarcodeConfig.OffY;
			rect.W = BarcodeConfig.Width;
			rect.H = BarcodeConfig.Height;
		}
		void InitData(std::string json) override;

		void InitTestData();

		void* GetTestConfig() {
			return &BarcodeConfig;
		}

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(BarcodeConfig)>(data, BarcodeConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(BarcodeConfig)>(BarcodeConfig);
		}
	};
}

