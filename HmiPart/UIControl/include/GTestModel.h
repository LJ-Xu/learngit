#pragma once
#include "BaseModel.h"
#include "GTestConfig.h"
namespace UI
{
	class GTestModel : public BaseModel
	{
	public:
		Project::GTestConfig Config;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = Config.X + Config.OffX;
			rect.Y = Config.Y + Config.OffY;
			rect.W = Config.Width;
			rect.H = Config.Height;
		}
		void InitTestData();
		void* GetTestConfig()
		{
			return &Config;
		}
		void InitData(std::string json) override;
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(Config)>(data, Config);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(Config)>(Config);
		}
	};

}