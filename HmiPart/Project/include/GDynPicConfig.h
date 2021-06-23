#pragma once
#include "MoveableDisplayUnit.h"
#include <string>
#include "ImageFormat.h"
#include "BaseLine.h"
#include "ImageResId.h"
namespace Project
{
	enum PicSwitchMode:char
	{
		Mode_OneWay = 0,	//单程
		Mode_GondBack		//往返
	};

	enum PicSwitchSeq:char
	{
		Seq_Order = 0,		//顺序
		Seq_Reverse,		//逆序
		Seq_Random			//随机
	};
	class GDynPicConfig : public MoveableDisplayUnit
	{
	public:
		int CycleTime;						//图片展示一圈的周期(毫秒)
		DataVarId CycleTimeVarId;			//图片展示一圈的周期（寄存器指定）
		PicSwitchMode SwitchMode;			//图片切换模式
		PicSwitchSeq SwitchSeq;				//图片切换模式
		std::string SwitchOrder;			//图片切换顺序
		vector<ImageFormat>ImageFmtList;	//图片集合
		vector<ImageResId>OriginalImages;	//原始图片集合
		int PicNum;							//图片个数
		bool UseStartSign;					//是否使用开始信号
		bool UseStopSign;					//是否使用结束信号
		DataVarId StartSignVarId;			//开始信号vid
		DataVarId StopSignVarId;			//结束信号vid
		int StartTrigCondition;				//开始信号条件 0:上升沿 1:下降沿
		int EndTrigCondition;				//结束信号条件 0:上升沿 1:下降沿


		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(CycleTime, CycleTimeVarId, SwitchMode, SwitchSeq, SwitchOrder, OriginalImages, EndTrigCondition,
				ImageFmtList, PicNum, UseStartSign, UseStopSign, StartSignVarId, StopSignVarId,
				VOffX, VOffY, Width, Height, Perm, X, Y, IsEnable, CtrlName, StartTrigCondition
				, IsRecord, CtrlName);
		}

		int CurrentPic = 0;					//当前图片
		int Direction = 0;					//运行正反序		
	};
}


