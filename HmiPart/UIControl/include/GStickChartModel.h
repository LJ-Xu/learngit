#pragma once
#include "BaseModel.h"
#include "GStickChartConfig.h"
namespace UI
{
	class GStickChartModel : public BaseModel
	{
	public:
		Project::GStickChartConfig StickChartConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = StickChartConfig.X + StickChartConfig.OffX;
			rect.Y = StickChartConfig.Y + StickChartConfig.OffY;
			rect.W = StickChartConfig.Width;
			rect.H = StickChartConfig.Height;
		}

		void InitData(std::string json) override;

		void InitTestData();

		void* GetTestConfig() {
			return &StickChartConfig
				;
		}

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(StickChartConfig)>(data, StickChartConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(StickChartConfig)>(StickChartConfig);
		}
	};
}
