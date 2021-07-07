#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "ImageResId.h"

namespace Project
{
	//显示内容
	struct  DynamicShowContext
	{
		std::vector<std::pair<std::string, int>> TextList;
		int ShowMode;//显示模式 0：总是显示 1：线圈控制
		DataVarId TriggerAddr;//控制地址
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(TextList, ShowMode, TriggerAddr);
		}
	};
	class DynamicTextBarConfig : public MoveableDisplayUnit
	{
	public:
		std::vector < DynamicShowContext> contextList;
		StringStyle Font;//刻度字体
		int MsgInstance;//每条消息间距
		int RollSpeed;//滚动速度 （每0.1S滚动的像素值）
		bool ShowFrame;//是否显示边框
		BaseLine FrameStyle;//边框样式
		bool ShowFillin;//显示填充
		int FadeoutPecent;//透明度百分比（这个暂时先不做）
		int FillColor;//填充颜色

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y, VOffX, VOffY, Perm, Width, Height, CtrlName, DataCtlSign, contextList, Font, MsgInstance, RollSpeed, ShowFrame, FrameStyle, ShowFillin, FadeoutPecent, FillColor);
		}
	};
}