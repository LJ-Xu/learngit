#pragma once
#include "BaseModel.h"
#include "MotorConfig.h"
namespace UI
{
	class MotorModel : public BaseModel
	{
	public:
		MotorModel(){}
		~MotorModel(){}
		Project::MotorConfig MotorConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = MotorConfig.X + MotorConfig.OffX;
			rect.Y = MotorConfig.Y + MotorConfig.OffY;
			rect.W = MotorConfig.Width;
			rect.H = MotorConfig.Height;
		}

		void InitData(std::string json) override;

		void InitTestData();

		void* GetTestConfig();

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(MotorConfig)>(data, MotorConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(MotorConfig)>(MotorConfig);
		}
	};
}