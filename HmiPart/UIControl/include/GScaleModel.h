#pragma once
#include "BaseModel.h"
#include "GScaleConfig.h"
namespace UI
{
	class GScaleModel : public BaseModel
	{
	public:
		Project::GScaleConfig m_ScaleConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = m_ScaleConfig.X + m_ScaleConfig.OffX;
			rect.Y = m_ScaleConfig.Y + m_ScaleConfig.OffY;
			rect.W = m_ScaleConfig.Width;
			rect.H = m_ScaleConfig.Height;
		}

		void InitData(std::string json) override;

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(m_ScaleConfig)>(data, m_ScaleConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(m_ScaleConfig)>(m_ScaleConfig);
		}
	};

}