#pragma once
#include "BaseModel.h"
#include "XYTrendConfig.h"
#include "Logger.h"
namespace UI
{
	class XYTrendModel : public BaseModel
	{
	public:
		Project::XYTrendConfig TrendXYConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = TrendXYConfig.X + TrendXYConfig.OffX;
			rect.Y = TrendXYConfig.Y + TrendXYConfig.OffY;
			rect.W = TrendXYConfig.Width;
			rect.H = TrendXYConfig.Height;
		}
		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &TrendXYConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(TrendXYConfig)>(data, TrendXYConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(TrendXYConfig)>(TrendXYConfig);
		}
	};

}
