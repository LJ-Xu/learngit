#pragma once
#include "BaseModel.h"
#include "AlarmDisplayConfig.h"
namespace UI 
{
	class AlarmDisplayModel : public BaseModel
	{
	public:
		Project::AlarmDisplayConfig AlarmDisConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = AlarmDisConfig.X + AlarmDisConfig.OffX;
			rect.Y = AlarmDisConfig.Y + AlarmDisConfig.OffY;
			rect.W = AlarmDisConfig.Width;
			rect.H = AlarmDisConfig.Height;
		}

		void InitTestData();

		void * GetTestConfig() {
			return &AlarmDisConfig;
		}

		void InitData(std::string json) override;

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(AlarmDisConfig)>(data, AlarmDisConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{
			return BaseModel::GetData<decltype(AlarmDisConfig)>(AlarmDisConfig);
		}
	};

}