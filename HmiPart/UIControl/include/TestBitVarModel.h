#pragma once
#include "BaseModel.h"
#include "TestBitVarConfig.h"
namespace UI
{
	class TestBitVarModel : public BaseModel
	{
	public:
		Project::TestBitVarConfig BitConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = BitConfig.X + BitConfig.OffX;
			rect.Y = BitConfig.Y + BitConfig.OffY;
			rect.W = BitConfig.Width;
			rect.H = BitConfig.Height;
		}
		void InitTestData();
		void* GetTestConfig()
		{
			return &BitConfig;
		}
		void InitData(std::string json) override;
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(BitConfig)>(data, BitConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(BitConfig)>(BitConfig);
		}
	};

}
