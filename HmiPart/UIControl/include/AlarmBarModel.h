#pragma once
#include "BaseModel.h"
#include "AlarmBarConfig.h"
namespace UI
{
	class AlarmBarModel : public BaseModel
	{
	public:
		AlarmBarModel() {}
		~AlarmBarModel() {}
		Project::AlarmBarConfig AlarmBarConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = AlarmBarConfig.X + AlarmBarConfig.OffX;
			rect.Y = AlarmBarConfig.Y + AlarmBarConfig.OffY;
			rect.W = AlarmBarConfig.Width;
			rect.H = AlarmBarConfig.Height;
		}
		void InitData(std::string json) override;

		void InitTestData();

		void* GetTestConfig() {
			return &AlarmBarConfig;
		}

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(AlarmBarConfig)>(data, AlarmBarConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(AlarmBarConfig)>(AlarmBarConfig);
		}
	};
}