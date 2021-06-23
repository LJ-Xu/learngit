#pragma once
#include "BaseModel.h"
#include "DateConfig.h"
namespace UI
{
	class DateModel : public BaseModel
	{
	public:
		DateModel() {}
		~DateModel() {}
		Project::DateConfig DateConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = DateConfig.X + DateConfig.OffX;
			rect.Y = DateConfig.Y + DateConfig.OffY;
			rect.W = DateConfig.Width;
			rect.H = DateConfig.Height;
		}

		void InitData(std::string json) override;

		void InitTestData();

		void* GetTestConfig() 
		{
			return &DateConfig;
		}

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(DateConfig)>(data, DateConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(DateConfig)>(DateConfig);
		}
	};
}
