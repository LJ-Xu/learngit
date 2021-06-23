#pragma once
#include "BaseModel.h"
#include "ScaleConfig.h"
namespace UI
{
	class ScaleModel : public BaseModel
	{
	public:
		ScaleModel() {}
		~ScaleModel() {}
		Project::ScaleConfig ScaleConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = ScaleConfig.X + ScaleConfig.OffX;
			rect.Y = ScaleConfig.Y + ScaleConfig.OffY;
			rect.W = ScaleConfig.Width;
			rect.H = ScaleConfig.Height;
		}
		void InitData(std::string json) override;

		void InitTestData();

		void* GetTestConfig() {
			return &ScaleConfig;
		}

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(ScaleConfig)>(data, ScaleConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(ScaleConfig)>(ScaleConfig);
		}
	};
}

