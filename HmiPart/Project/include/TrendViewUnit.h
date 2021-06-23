#pragma once
#include "AlarmSeekUnit.h"
#include "BaseLine.h"
#include "StringStyle.h"
#include "AlarmDisplayConfig.h"
#include "ScaleInfo.h"

namespace Project
{
	class AxisSetting
	{
	public:
		bool IsdisplayScale;			//是否使用X轴刻度
		int ScaleColor;					//刻度颜色
		int MainScaleNum;				//主刻度数
		int MainScaleLen;				//主刻度线长度
		bool IsViceScaleNum;			//是否使用副刻度
		int ViceScaleNum;				//副刻度数
		int ViceScaleLen;				//副刻度线长度
		StringStyle ScaleStyle;			//字体格式

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsdisplayScale, ScaleColor, MainScaleNum, MainScaleLen, IsViceScaleNum,
				ViceScaleNum, ViceScaleLen, ScaleStyle);
		}

	};
	class XAxis : public AxisSetting
	{
	public:
		bool IsUseTimeScale;			//是否使用时间刻度
		bool IsdiaplayDate;				//显示日期
		DateMode DateStyle;				//日期格式
		bool IsdisplayTime;				//显示时间
		TimeMode TimeStyle;				//时间格式

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsUseTimeScale, IsdiaplayDate, DateStyle, IsdisplayTime, TimeStyle,
				IsdisplayScale, ScaleColor, MainScaleNum, MainScaleLen, IsViceScaleNum,
				ViceScaleNum, ViceScaleLen, ScaleStyle);
		}
	};
	enum YScaleMode :char { Custom, Channel, All };
	enum YScaleTag :char { NullSacle, NumSacle, PercentSacle };

	class YAxis : public AxisSetting
	{
	public:
		YScaleTag ScaleTag;					//刻度标记	0：不显示，1：数字，2：百分比
		int YIntegerNum;				//整数位数
		int YDecimalnNum;				//小数位数
		YScaleMode ScaleRange;			//刻度范围	0：自定义 1：使用通道最大最小值 2：显示所有通道范围
		int UseRangeChanbelNo;			//使用通道的编号
		double YScaleMax;				//Y轴最大值
		DataVarId YScalemaxVarId;		//控制Y轴最大值寄存器
		double YScaleMin;				//Y轴最小值
		DataVarId YScaleminVarId;		//控制Y轴最小值寄存器

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ScaleTag, YIntegerNum, YDecimalnNum, ScaleRange, YScaleMax, UseRangeChanbelNo,
				YScalemaxVarId, YScaleMin, YScaleminVarId, IsdisplayScale, ScaleColor,
				MainScaleNum, MainScaleLen, IsViceScaleNum, ViceScaleNum, ViceScaleLen, ScaleStyle);
		}
	};

	
	struct DataDisplay
	{
		string ProjectName;
		string DescribeName;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ProjectName, DescribeName);
		}
	};
	enum TrendSeekMode :char { DATE, TIME, CHANNELNO, REG };
	class TrendViewUnit : public AlarmSeekUnit
	{
	public:
		int BgColor;					//趋势图背景颜色
		int ScaleAreabgColor;			//刻度区背景颜色
		bool IsGriddisplay;				//是否使用栅格显示
		int XUniformNum;				//X轴栅格等分数
		int YUniformNum;				//Y轴栅格等分数
		BaseLine GridStyle;				//栅格线条样式

		XAxis AxisX;					//X轴设置
		YAxis AxisY;					//Y轴设置

		Point OriginPos;				//原点坐标
		int XaxisLen;					//X轴长
		int YaxisLen;					//Y轴长
		int ScrollHeight;				//滚动条高度
		vector<vector<ScaleInfo>> YScaleInfo;		//Y轴标注位置
		vector<ScaleInfo> XScaleInfo;				//X轴标注位置

		bool IsDataDisplay;							//是否显示选中的坐标
		vector<DataDisplay> DisplayContent;			//显示内容
		DateMode DisplayDateStyle;					//日期格式
		TimeMode DisplayTimeStyle;					//时间格式
		int InfoBgColor;							//信息窗口背景颜色
		int InfoFontColor;							//信息窗口字体颜色
		int InfoLineColor;							//信息数据线颜色

		DataVarId ZoomVarId;			//缩放
		DataVarId PageCtrlVarId;		//翻页
		
		TrendSeekMode SearchMode;			//查询方式

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(BgColor, ScaleAreabgColor, IsGriddisplay, XUniformNum,
				YUniformNum, GridStyle, AxisX, AxisY, OriginPos, XaxisLen, YaxisLen,
				ScrollHeight, YScaleInfo, XScaleInfo, ZoomVarId, PageCtrlVarId,
				IsDataDisplay, DisplayContent, DisplayDateStyle, DisplayTimeStyle,
				InfoBgColor, InfoFontColor, InfoLineColor, SearchMode);
		}
	};


}
