#pragma once
#include "MoveableDisplayUnit.h"
#include "StatusRes.h"
#include "ImageResId.h"
#include "StringStyle.h"
#include "NoticesAction.h"
namespace Project
{
	struct MenuContontLst
	{
		int Index;			//索引
		DOUBLE Value;			//对应值
		StatusRes Content;	//标签内容
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Index, Value, Content);
		}
	};
	struct HMICommand
	{
		string Name;
		string Cmd;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Name, Cmd);
		}
	};
	class DropMenuConfig : public MoveableDisplayUnit
	{
	public:
		int MenuMode;							//0:下拉菜单 1:清单选框
		DataVarId ReadVar;						//读取写入地址
		int DataStyle;							//数据格式
		int MenuNum = 0;						//下拉条目数量
		int PopMode;							//弹出方式	0：向下 1：向上
		int ContentMode;						//内容模式 0：用户编辑 1：命令模式
		int ContentHeight = 18;						//高度
		vector<MenuContontLst> MenuInfo;		//菜单内容及对应值
		vector<ImageResId> PicKey;				//背景图片
		int RowHeight;							//行间距
		int HscollWidth = 16;					//滚动条宽度
		int SelectColor;						//选中项目颜色
		int BgColor;							//背景颜色
		int FrameColor;							//边框颜色
		vector<StringStyle> MenuStyle;			//菜单字体设置
		HMICommand HMICommandItem;				//命令内容
		Point BtnPosition;						//按键位置
		NoticesAction ErrorNotice;				//错误通知

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(MenuMode, ReadVar, DataStyle, MenuNum, PopMode,	MenuInfo, 
				ContentMode, HMICommandItem, BtnPosition, PicKey, RowHeight, ContentHeight,
				SelectColor, BgColor, FrameColor, MenuStyle, ErrorNotice, 
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}


