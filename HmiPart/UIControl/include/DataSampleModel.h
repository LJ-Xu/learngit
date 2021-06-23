#pragma once
#include "BaseModel.h"
#include "DataSampleConfig.h"
namespace UI
{
	class DataSampleModel : public BaseModel
	{
	public:
		Project::DataSampleConfig SampleDataConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = SampleDataConfig.X + SampleDataConfig.OffX;
			rect.Y = SampleDataConfig.Y + SampleDataConfig.OffY;
			rect.W = SampleDataConfig.Width;
			rect.H = SampleDataConfig.Height;
		}
		void InitData(std::string json) override;

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(SampleDataConfig)>(data, SampleDataConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(SampleDataConfig)>(SampleDataConfig);
		}
	};

}
