#pragma once
#include "BaseModel.h"
#include "AlarmInputConfig.h"
namespace UI
{
	class AlarmInputModel : public BaseModel
	{
	public:
		Project::AlarmInputConfig InputAlarmConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = InputAlarmConfig.X + InputAlarmConfig.OffX;
			rect.Y = InputAlarmConfig.Y + InputAlarmConfig.OffY;
			rect.W = InputAlarmConfig.Width;
			rect.H = InputAlarmConfig.Height;
		}
		void InitData(std::string json) override;

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(InputAlarmConfig)>(data, InputAlarmConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(InputAlarmConfig)>(InputAlarmConfig);
		}
	};

}
