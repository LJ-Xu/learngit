#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "ImageResId.h"

namespace Project
{
	class MotorConfig : public MoveableDisplayUnit
	{
	public:
		////对象（地址数据）
		DataVarId DataCtlSign; //读取地址:控制信号
		int CtlLogic;//逻辑：控制逻辑 0 为正逻辑 1为负逻辑
		///闪烁
		bool UseBlink;//开启闪烁
		int BlinkState;//闪烁状态:0为On状态闪烁，1为Off状态闪烁
		float BlinkFrequency;//闪烁频率

		////外观
		ImageResId KeyImgOn;//电机on状态的图片
		ImageResId KeyImgOff;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive( X, Y, VOffX, VOffY, Perm, Width, Height, CtrlName, DataCtlSign, CtlLogic, UseBlink, BlinkState, BlinkFrequency, KeyImgOn, KeyImgOff);
		}
	public:
		int CurrentStatus_;
	};
}