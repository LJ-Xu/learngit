#pragma once
#include "BaseModel.h"
#include "AlarmWinConfig.h"
namespace UI
{
	class AlarmWinModel : public BaseModel
	{
	public:
		Project::AlarmWinConfig WinAlarmConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = WinAlarmConfig.X + WinAlarmConfig.OffX;
			rect.Y = WinAlarmConfig.Y + WinAlarmConfig.OffY;
			rect.W = WinAlarmConfig.Width;
			rect.H = WinAlarmConfig.Height;
		}

		void InitData(std::string json) override;

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(WinAlarmConfig)>(data, WinAlarmConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(WinAlarmConfig)>(WinAlarmConfig);
		}
	};

}

