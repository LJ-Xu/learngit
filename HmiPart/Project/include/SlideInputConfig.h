/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SlideInputConfig.h
 * Author   : qiaodan
 * Date     : 2020-12-17
 * Descript : 用于存放滑动输入控件相关参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "ImageResId.h"
#include "NoticesAction.h"
#include "TrendViewUnit.h"
#include "BaseLine.h"
namespace Project
{
	enum SlideDir {SildeUp, SildeDown, SildeLeft, SildeRight};
	class SlideInputConfig : public MoveableDisplayUnit
	{
	public:
		DataVarId ReadVar;				//读写地址
		int DataStyle;					//数据格式
		double AttrMax;					//最大值
		DataVarId AttrMaxVar;	
		double AttrMin;					//最小值
		DataVarId AttrMinVar;

		SlideDir Dir;					//方向
		int MinScale;					//最小刻度
		bool UseIncDecPerClick;
		int IncDecPerClickScale;		//每次点击增加倍数
		bool UseChangeInput;			//拖拽显示数值

		int SlideRailWidth;				//滑轨宽度
		int SlideRailHeight;			//滑轨高度
		int SlideRailBackColor;			//滑轨背景颜色
		int SlideRailBorderColor;		//滑轨边框颜色
		ImageResId SlideRailKey;		//滑轨样式图片
		Point SlideRailPos;				//滑轨位置

		int SlideWidth;					//滑块宽度
		int SlideHeight;				//滑块高度
		int SlideBackColor;				//滑块背景颜色
		int SlideBorderColor;			//滑块边框颜色
		ImageResId SlideKey;			//滑块样式图片

		bool ShowScale;					//显示刻度
		int ScalePos;					//刻度位置	0（上方/左方） 1（下方/右方）
		Point AxisPos;					//轴线坐标
		int PrimaryScaleNum;			//主刻度数
		int PrimaryScaleLen;			//主刻度长度
		int SecondaryScaleNum;			//副刻度数
		int SecondaryScaleLen;			//副刻度长度
		BaseLine ScaleLineStyle;		//线条属性
		vector<Project::ScaleInfo> ScaleInfo;	//轴标注位置

		bool ShowMark;					//显示数字标注
		int Act;						//整数位
		int Bct;						//小数位
		StringStyle MarkStyle;			//数字标注格式

		bool ShowPer;					//显示百分比
		StringStyle PerStyle;			//百分比标注格式

		bool ShowAxis;					//显示轴线

		NoticesAction ResBef;			//写入前通知
		NoticesAction ResAft;			//写入后通知

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ReadVar, DataStyle, AttrMax, AttrMaxVar, AttrMin, AttrMinVar,
				Dir, MinScale, UseIncDecPerClick, IncDecPerClickScale, UseChangeInput, SlideRailPos,
				SlideRailWidth, SlideRailHeight, SlideRailBackColor, SlideRailBorderColor, SlideRailKey,
				SlideWidth, SlideHeight, SlideBackColor, SlideBorderColor, SlideKey, ScaleInfo,
				ShowScale, ScalePos, AxisPos,PrimaryScaleNum, PrimaryScaleLen, SecondaryScaleNum, SecondaryScaleLen,
				ScaleLineStyle, ShowMark, Act, Bct, MarkStyle, ShowPer, PerStyle, ShowAxis, ResBef, ResAft,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}
