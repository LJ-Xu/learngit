#pragma once
#include "BaseModel.h"
#include "GifPicConfig.h"
#include "Logger.h"
namespace UI
{
	class GifPicModel : public BaseModel
	{
	public:
		Project::GifPicConfig GifConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = GifConfig.X + GifConfig.OffX;
			rect.Y = GifConfig.Y + GifConfig.OffY;
			rect.W = GifConfig.Width;
			rect.H = GifConfig.Height;
		}

		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &GifConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(GifConfig)>(data, GifConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(GifConfig)>(GifConfig);
		}
	};
}

