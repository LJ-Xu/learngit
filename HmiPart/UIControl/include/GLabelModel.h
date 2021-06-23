#pragma once
#include "BaseModel.h"
#include "GLabelConfig.h"
namespace UI
{
	class GLabelModel : public BaseModel
	{
	public:
		Project::GLabelConfig m_config;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = m_config.X + m_config.OffX;
			rect.Y = m_config.Y + m_config.OffY;
			rect.W = m_config.Width;
			rect.H = m_config.Height;
		}

		void InitData(std::string json) override;

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(m_config)>(data, m_config);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(m_config)>(m_config);
		}
	};
}

