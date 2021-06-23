#pragma once
#include "BaseModel.h"
#include "BuzzerConfig.h"
namespace UI
{
	class BuzzerModel : public BaseModel
	{
	public:
		Project::BuzzerConfig BuzzerConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = BuzzerConfig.X + BuzzerConfig.OffX;
			rect.Y = BuzzerConfig.Y + BuzzerConfig.OffY;
			rect.W = BuzzerConfig.Width;
			rect.H = BuzzerConfig.Height;
		}
		void InitData(std::string json) override;

		void InitTestData();

		void* GetTestConfig() {
			return &BuzzerConfig;
		}

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(BuzzerConfig)>(data, BuzzerConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(BuzzerConfig)>(BuzzerConfig);
		}
	};
}


