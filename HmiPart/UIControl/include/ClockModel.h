#pragma once
#include "BaseModel.h"
#include "ClockConfig.h"
namespace UI
{
	class ClockModel :
		public BaseModel
	{
	public:
		ClockModel() {}
		~ClockModel() {}
		Project::ClockConfig ClockConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = ClockConfig.X + ClockConfig.OffX;
			rect.Y = ClockConfig.Y + ClockConfig.OffY;
			rect.W = ClockConfig.Width;
			rect.H = ClockConfig.Height;
		}

		void InitData(std::string json) override;

		void InitTestData();

		void* GetTestConfig()
		{
			return &ClockConfig;
		}

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(ClockConfig)>(data, ClockConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(ClockConfig)>(ClockConfig);
		}
	};

}