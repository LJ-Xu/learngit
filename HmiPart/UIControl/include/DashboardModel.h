#pragma once
#include "BaseModel.h"
#include "DashboardConfig.h"
namespace UI
{
	class DashboardModel : public BaseModel
	{
	public:
		DashboardModel() {}

		~DashboardModel() {}

		Project::DashboardConfig DashboardConfig;

		void GetRect(UI::Rectangle& rect)
		{
			rect.X = DashboardConfig.X + DashboardConfig.OffX;
			rect.Y = DashboardConfig.Y + DashboardConfig.OffY;
			rect.W = DashboardConfig.Width;
			rect.H = DashboardConfig.Height;
		}

		void InitData(std::string json) override;

		void InitTestData();

		void* GetTestConfig() {
			return &DashboardConfig;
		}

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(DashboardConfig)>(data, DashboardConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(DashboardConfig)>(DashboardConfig);
		}

	};

}