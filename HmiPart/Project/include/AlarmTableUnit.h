#pragma once
#include "BaseLine.h"
#include "AlarmSeekUnit.h"
namespace Project
{
	enum AlarmSeekMode { AlarmSeekDATE, AlarmSeekTIME, AlarmSeekGROUP, AlarmSeekNUM, AlarmSeekLEVEL, AlarmSeekREG };

	class AlarmTableUnit : public AlarmSeekUnit
	{
	public:
		int BackColor;			//背景颜色
		int TitleBgColor;		//标题背景颜色

		bool UseFrame;			//是否使用外框
		BaseLine FrameStyle;	//外框样式

		bool UseGrid;			//是否使用网格
		BaseLine GridStyle;		//网格样式

		int AlarmTextColor;		//报警触发时文字颜色
		int AlarmBackColor;		//报警触发时背景颜色

		bool UseConfirm;		//是否使用报警确认
		int ConfirmTextColor;	//报警确认时文字颜色
		int ConfirmBackColor;	//报警确认时背景颜色

		bool UseRecover;		//是否使用报警恢复
		int RecoverTextColor;	//报警恢复时文字颜色
		int RecoverBackColor;	//报警恢复时背景颜色

		AlarmSeekMode SearchMode;		//查询方式
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(BackColor, TitleBgColor, UseFrame, FrameStyle, UseGrid, GridStyle,
				AlarmTextColor, AlarmBackColor, UseConfirm, ConfirmTextColor, ConfirmBackColor,
				UseRecover, RecoverTextColor, RecoverBackColor, SearchMode);
		}
	};

}

