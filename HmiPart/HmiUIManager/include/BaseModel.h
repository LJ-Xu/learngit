#pragma once
#include <vector>
#include "CerealCommon.hpp"
#include "UIComm.h"
#include "BaseDisplayUnit.h"
namespace UI
{
	struct Rectangle;
	class BaseModel
	{
	public:

		/*******************************************************************************
		 * Name     : GetRect
		 * Descript : 用于获取当前部件的绘图范围
		 * Input    : rect - 用于存储当前部件的位置X\Y和长宽W\H
		 * Output   :
		 * Note	    :
		 *******************************************************************************/


		virtual void GetRect(UI::Rectangle& rect) = 0;

		void SetRect(int x, int y, int w, int h)
		{
			BaseDisplayUnit* cnf = (BaseDisplayUnit*)GetTestConfig();
			if (cnf)
			{
				cnf->X = x;
				cnf->Y = y;
				cnf->Width = w;
				cnf->Height = h;
			}
		}

		virtual void InitData(std::string json) = 0;

		virtual void InitTestData() {};

		virtual void* GetTestConfig() { return nullptr; };

		virtual void InitData(std::vector<char>) = 0;

		template <class T>
		void InitData(std::vector<char>& data, T& config)
		{

			StreamReadBuf osrb(data.data(), data.size());
			istream ins0(&osrb);
			//ins0.;
			{
				cereal::BinaryInputArchive archive(ins0);
				archive(config);
			}
		}

		virtual std::unique_ptr<std::vector<char>> GetData()
		{
			return std::make_unique<std::vector<char>>();
		};
		template <class T>
		std::unique_ptr<std::vector<char>> GetData(T& config)
		{

			HOStringstream outs0;
			//ostringstream outs0;
			{
				cereal::BinaryOutputArchive archive(outs0);
				archive(config);
			}
			int len = (int)outs0.tellp();
			const char* str = outs0.ptr();
			//const char* str = outs0.str().c_str();
			std::unique_ptr<std::vector<char>> pdata =
				std::make_unique<std::vector<char>>(str, str + len);
			return pdata;
		}
	};

}

