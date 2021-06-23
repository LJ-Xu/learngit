#pragma once
#include "BaseModel.h"
#include "BtnFunctionConfig.h"
#include "MultiStatusKeypadConfig.h"
#include "Logger.h"

namespace UI
{
	class BtnFunctionModel : public BaseModel
	{
	public:
		Project::BtnFunctionConfig FuncBtnConfig;

		void GetRect(UI::Rectangle& rect)
		{
			rect.X = FuncBtnConfig.X + FuncBtnConfig.OffX;
			rect.Y = FuncBtnConfig.Y + FuncBtnConfig.OffY;
			rect.W = FuncBtnConfig.Width;
			rect.H = FuncBtnConfig.Height;
		}
		void* GetTestConfig()
		{
			return &FuncBtnConfig;
		}
		void InitData(std::string json) override;
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(FuncBtnConfig)>(data, FuncBtnConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(FuncBtnConfig)>(FuncBtnConfig);
		}
	};
}

