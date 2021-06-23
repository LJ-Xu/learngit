#pragma once
#include "BaseModel.h"
#include "KeyBoardConfig.h"
namespace UI
{
	class KeyBoardModel : public BaseModel
	{
	public:
		Project::KeyBoardConfig m_KeyBoardConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = m_KeyBoardConfig.X + m_KeyBoardConfig.OffX;
			rect.Y = m_KeyBoardConfig.Y + m_KeyBoardConfig.OffY;
			rect.W = m_KeyBoardConfig.Width;
			rect.H = m_KeyBoardConfig.Height;
		}
		void InitData(std::string json) override;

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(m_KeyBoardConfig)>(data, m_KeyBoardConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(m_KeyBoardConfig)>(m_KeyBoardConfig);
		}
	};

}
