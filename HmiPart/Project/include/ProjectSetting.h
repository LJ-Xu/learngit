#pragma once
#include "CerealCommon.hpp"
namespace Project
{
	using namespace std;
	struct ProjectSetting
	{
		int Dir; // 1������ʾ
		int Width;  //��
		int Height; //��
		int StartWinNo;//��������
		string Category; //�������
		string HmiName;//�豸����

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Dir, Width, Height, Category, StartWinNo, HmiName);
		}

		void InitData(std::string jstr);

	};
}

