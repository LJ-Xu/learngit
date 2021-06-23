#pragma once
#include "MoveableDisplayUnit.h"
#include "Point.h"
#include "ImageResId.h"
namespace Project
{
	enum PopMode {ONCE,CYCLE};
	class AlarmWinConfig : public MoveableDisplayUnit
	{
	public:
		int WinNO;				//窗口编号
		string WinName;			//窗口名称
		int GroupName;			//报警组
		int GroupNo;			//编号
		PopMode ModePop;		//弹出一次/周期
		int CycleTime;			//周期设置
		bool AutoClose;			//报警结束后关闭窗口
		bool AloneMode;			//垄断模式
		bool CloseBtnMode;		//使用关闭按钮

		int BgMode;				//0:使用背景颜色	1:使用背景图片
		int BgColor;			//背景颜色
		ImageResId BgPicKey;	//背景图片
	};
}



