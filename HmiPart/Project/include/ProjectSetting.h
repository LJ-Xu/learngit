#pragma once
#include "CerealCommon.hpp"
namespace Project
{
	using namespace std;
	struct ProjectSetting
	{
		int Dir; // 1竖屏显示
		int Width;  //宽
		int Height; //长
		int StartWinNo;//启动画面
		string Category; //类别名称
		string HmiName;//设备名称

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Dir, Width, Height, Category, StartWinNo, HmiName);
		}

		void InitData(std::string jstr);

	};
}

