#pragma once
#include "MacroRes.h"
#include "MoveableDisplayUnit.h"
#include <vector>
#include "Point.h"
#include "ImageResId.h"
namespace Project
{
	struct Pipe
	{
	public:
		int PipeWidth;		//导管宽度
		int PipeColor;		//导管颜色
		int FrameWidth;		//边框宽度
		int FrameColor;		//边框颜色
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(PipeWidth, PipeColor, FrameWidth, FrameColor);
		}
	};

	class Slider
	{
	public:
		int SliderType;				//0:矩形;1:箭头
		int SliderWidth;			//宽度
		int SliderLength;			//长度
		int SliderIntervalWidget;	//间隔
		int ArrowAngle = 6;				//箭头倾斜度
		int SliderColorON;			//ON状态滑块颜色
		int SliderColorOFF;			//OFF状态滑块颜色

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(SliderType, SliderWidth, SliderLength, SliderIntervalWidget,
				ArrowAngle, SliderColorON, SliderColorOFF);
		}
	};

	class FluidUnitConfig : public MoveableDisplayUnit
	{
	public:
		int ActionMode;					//动作模式 0:一直流动，1:受位控制
		int FlowCondition;				//流动条件 0:ON 1:OFF
		DataVarId FlowVarId;			//读取地址
		int FlowDirection;				//方向 0:正向 1:反向
		double FlowSpeed;				//流动速度 1-25个等级对应为1.5s-60ms
		DataVarId SpeedVarId;
		int PointNum;					//坐标个数
		vector<Point> Coordinates;		//坐标点
		Pipe PipeParam;					//关于管道设置
		Slider SliderParam;				//关于滑块设置

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ActionMode, FlowCondition, FlowVarId, FlowDirection, FlowSpeed, 
				SpeedVarId,	Coordinates, PipeParam, SliderParam, 
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

