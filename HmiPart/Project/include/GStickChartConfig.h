#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "ImageResId.h"
namespace Project
{

	class GStickChartConfig : public MoveableDisplayUnit
	{
	public:
		////对象（地址数据）
		DataVarId BarVar; //数据

		////范围（范围）
		///最大值
		int MaxValue;//最大值(常数)
		DataVarId MaxData;//最大值(地址)
		bool UseMaxAddr;//最大值是否由寄存器指定 

		///最小值
		int MinValue;//最小值（常数）
		DataVarId MinData;//最小值(地址)
		bool UseMinAddr;//最小值是否由寄存器指定

		///目标区间
		bool UseDstField; //使用定义的目标区间

		int DstValue;//目标值(常数)
		DataVarId DstValueAddr;//目标值(地址)
		bool UseDstValueByAddr;//使用寄存器指定目标值

		int DstRange;//误差范围(常数)
		DataVarId DstRangeAddr;//误差范围(地址)
		bool UseDstRangeByAddr;//使用寄存器指定误差范围

		int DstColor;//误差区间颜色

		///范围报警
		bool UseRangeWarn;//启用范围报警

		int UpperWarnValue;//上警报值(常数)
		DataVarId UpperWarnAddr;//上警报值(地址)
		bool UseUpperWarnAddr; //寄存器指定上警报值

		int LowerWarnValue;//下警报值(常数)
		DataVarId LowerWarnAddr;//下警报值(地址)
		bool UseLowerWarnAddr; //寄存器指定下警报值

		int UpperWarnFillinColor;//上警报填充色
		int UpperWarnFrontColor;//上警报前景色
		int LowerWarnFillinColor;//下警报填充色
		int LowerWarnFrontColor;//下警报前景色

		////外观
		int Shape;//当前选择的形状：0直条、1扇形

		///自定义外观（仅直条）
		bool IsUserDefineBar;
		////自定义外观可变区域的左上角坐标（取像素）
		//Point BarAreaRectanglePoint;
		//自定义外观可变区域的长宽
		//BaseDisplayUnit BarAreaRectangle;
		int BarAreaRectanglePointX;
		int BarAreaRectanglePointY;
		int BarAreaRectangleSizeWidth;
		int BarAreaRectangleSizeHeight;
		///直条
		int BarDirection; //方向：0上；1下；2左；3右

		///扇形
		int ArcInnerCirclePercent;//内外环比例
		int ArcStartAngle;//起始角度
		int ArcSweepAngle;//涵盖角度
		int ArcDirection; //方向：0顺时针；1逆时针

		///边框
		bool ShowFrame;//显示边框
		int ColorShowFrame;//前景色

		///背景
		bool ShowBackground;//显示背景色
		int ColorBack;//背景色

		///填充
		int FIllinColor;//填充色

		///图案填充
		bool UseFillinStyle;////图案填充
		int FillinStyle;//样式
		int ColorFillinStyle;//前景色

		///渐变
		bool UseFillinShade;//渐变
		int ShadeStyle;//样式:0→ 1↓ 2↘ 3↗
		int ColorShadeStyle;//前景色
		int FadeoutPercent;//透明度

		////刻度与标记
		///刻度
		bool ShowGraduate;//显示刻度
		//刻度与棒图的相对位置，根据棒图自身的方向与样式变动
		//值：      0        1
		//竖：      左       右
		//横：      上       下
		//扇：      外       内
		int RelatedGraduateSeat;
		int MainGraduateCount; //主刻度数
		int MainGraduateLen;//主刻度长度
		int SecondaryGraduateCount; //副刻度数
		int SecondaryGraduateLen;//副刻度长度

		///刻度线
		int LineColor;//刻度线颜色
		int LineStyle;//刻度线样式
		int LineWidget;//刻度线宽度

		///数字标记
		bool ShowGraduateSign; //显示数字标记
		int IntegralDigit;//整数位
		int DecimalDigit; //小数位
		StringStyle TypeFaceGraduate;//刻度字体

		///百分比
		bool ShowPercent;
		StringStyle TypeFacePecent; //刻度字体
		bool ShowAxis;//显示轴线

		///安全设置
		//ControlPerm CtlPerm;
		//临时条目（测试完成前不代表正式添加）
		ImageResId BarRes;//液面/自定义棒图图片
		ImageResId LinearGradientRes;//液面/自定义棒图图片

	public:
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y, Width, Height, VOffX, VOffY,CtrlName, Perm,IsUserDefineBar, IsRecord,
				BarVar,
				MaxData, UseMaxAddr,MaxValue, 
				MinValue, MinData, UseMinAddr,
				UseDstField, DstValue, DstValueAddr, UseDstValueByAddr, DstRange, DstRangeAddr, UseDstRangeByAddr,
				DstColor, UseRangeWarn, UpperWarnValue, UpperWarnAddr, UseUpperWarnAddr, LowerWarnValue, LowerWarnAddr, UseLowerWarnAddr,UpperWarnFillinColor, UpperWarnFrontColor, LowerWarnFillinColor, LowerWarnFrontColor,
				Shape, BarDirection, LinearGradientRes,
				ArcInnerCirclePercent, ArcStartAngle, ArcSweepAngle, ArcDirection,
				ShowFrame, ColorShowFrame,
				ShowBackground, ColorBack, FIllinColor,
				UseFillinStyle, FillinStyle, ColorFillinStyle,
				UseFillinShade, ShadeStyle, ColorShadeStyle, FadeoutPercent,
				ShowGraduate, RelatedGraduateSeat, MainGraduateCount, 
				BarAreaRectanglePointX,
				BarAreaRectanglePointY,
				BarAreaRectangleSizeWidth,
				BarAreaRectangleSizeHeight,
				MainGraduateLen, SecondaryGraduateCount, SecondaryGraduateLen,
				LineColor, LineStyle, LineWidget,
				ShowGraduateSign, IntegralDigit, DecimalDigit, TypeFaceGraduate,
				ShowPercent, TypeFacePecent, ShowAxis,
				BarRes
			);
		}
	};

}