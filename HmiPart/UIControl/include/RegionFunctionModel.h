#pragma once
#include "BaseModel.h"
#include "RegionFunctionConfig.h"
#include "Logger.h"
namespace UI
{
	class RegionFunctionModel : public BaseModel
	{
	public:
		Project::RegionFunctionConfig FuncRegionConfig;
		//void GetFuncTypeByName(std::string name);

		void GetRect(UI::Rectangle& rect)
		{
			rect.X = FuncRegionConfig.X + FuncRegionConfig.OffX;
			rect.Y = FuncRegionConfig.Y + FuncRegionConfig.OffY;
			rect.W = FuncRegionConfig.Width;
			rect.H = FuncRegionConfig.Height;
		}
		void* GetTestConfig()
		{
			return &FuncRegionConfig;
		}
		void InitData(std::string json) override;
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(FuncRegionConfig)>(data, FuncRegionConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(FuncRegionConfig)>(FuncRegionConfig);
		}
	};
}