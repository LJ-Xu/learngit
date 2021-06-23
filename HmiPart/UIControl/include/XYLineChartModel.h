#pragma once
#include "BaseModel.h"
#include "XYLineChartConfig.h"
#include "Logger.h"

namespace UI
{
	class XYLineChartModel : public BaseModel
	{
	public:
		Project::XYLineChartConfig ChartXYLineConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = ChartXYLineConfig.X + ChartXYLineConfig.OffX;
			rect.Y = ChartXYLineConfig.Y + ChartXYLineConfig.OffY;
			rect.W = ChartXYLineConfig.Width;
			rect.H = ChartXYLineConfig.Height;
		}
		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &ChartXYLineConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(ChartXYLineConfig)>(data, ChartXYLineConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(ChartXYLineConfig)>(ChartXYLineConfig);
		}
	};

}

