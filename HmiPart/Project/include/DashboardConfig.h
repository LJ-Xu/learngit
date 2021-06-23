#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "ImageResId.h"

namespace Project
{
	class DashboardConfig : public MoveableDisplayUnit
	{
	public:
		
		DataVarId DataCtlSign;//监控信号(当前值)
		bool MaxUseAddr;//最大最小值是否取自地址值
		bool MinUseAddr;//最小值是否取自地址值 
		DataVarId MaxData;//最大值地址
		DataVarId MinData;//最小值地址
		double MaxValue;//最大值
		double MinValue;//最小值
		int DataColor;//正常范围内颜色
		int NormalRingWidth;//正常扇环宽度
		int RingToCircleCenterLength;//扇环到圆心距离
		bool IsUsedAlarmLimit;//是否启用报警上下限
		bool IsUpperWarnValueUsedRegister;//上限报警是否使用寄存器
		bool IsLowerWarnValueUsedRegister;//下限报警是否使用寄存器
		double UpperWarnValue;//警示上限
		DataVarId UpperWarnRegister;//报警上限寄存器
		double LowerWarnValue;//警示下限
		DataVarId LowerWarnRegister;//报警下限寄存器
		int UpperWarnColor;//上限警报颜色
		int LowerWarnColor;//下限警报颜色
		bool IsUsedDangerZone;//是否启用危险区间
		bool IsUpperDangerUsedRegister;//危险值上限是否使用寄存器
		bool IsLowerDangerUsedRegister;//危险值下限是否使用寄存器
		double UpperDangerValue;//危险值上限
		DataVarId UpperDangerRegister;//危险值上限寄存器
		double LowerDangerValue;//危险值下限
		DataVarId LowerDangerRegister;//危险值下限寄存器
		int DangerColor;//危险值颜色
		ImageResId DashBoardStyle;//仪表盘样式
		int ArcDirection;//方向 0顺时针 1逆时针
		int StartAngle;//起始角度
		int EndAngle;
		int Transparency;//透明度，单位%
		ImageResId WatchHandleStyle;//表针样式
		int WatchHandleWidth;//表针宽度
		int WatchHandleLength;//表针长度
		int WatchHandleInnerColor;//表针内部颜色
		int WatchHandleBorderColor;//表针边框颜色
		ImageResId AxisStyle;//轴心样式
		int AxisRadius;//轴心的半径，正方形时是边长
		int AxisInnerColor;//轴心内部颜色
		int AxisBorderColor;//轴心边框颜色
		bool ShowGraduate;//显示刻度
		int MainGraduateCount;//主刻度线数
		int SecondaryGraduateCount;//次刻度线数
		int MainGraduateLen;//主刻度等长度
		int SecondaryGraduateLen;//次刻度等长度
		int GraduateColor;//刻度线颜色
		int GraduatePosition;//刻度位置 0内测，1外侧，2居中
		int ShowGraduateSign;//显示刻度标记（数字）0不显示 1显示数字 2显示百分比
		StringStyle TypeFaceGraduate;//刻度字体
		int DecimalDigit;//小数位
		int IntegerDigit;//整数位

		bool IsCuttedBoarder;
		ImageResId CuttedBoardStyle;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y, VOffX, VOffY, Perm, Width, Height, CtrlName, DataCtlSign, MaxUseAddr, MinUseAddr, MaxData, MinData, MaxValue, MinValue, DataColor, NormalRingWidth, RingToCircleCenterLength,IsUsedAlarmLimit, IsUpperWarnValueUsedRegister, IsLowerWarnValueUsedRegister, UpperWarnValue, UpperWarnRegister, LowerWarnValue, LowerWarnRegister, UpperWarnColor, LowerWarnColor,  IsUsedDangerZone, IsUpperDangerUsedRegister, IsLowerDangerUsedRegister, UpperDangerValue, UpperDangerRegister, LowerDangerValue, LowerDangerRegister, DangerColor, DashBoardStyle, ArcDirection, StartAngle, EndAngle, Transparency, WatchHandleStyle, WatchHandleWidth, WatchHandleLength, WatchHandleInnerColor, WatchHandleBorderColor, AxisStyle, AxisRadius, AxisInnerColor, AxisBorderColor, ShowGraduate, MainGraduateCount, SecondaryGraduateCount, MainGraduateLen, SecondaryGraduateLen, GraduateColor, GraduatePosition, ShowGraduateSign, TypeFaceGraduate, DecimalDigit, IntegerDigit);
		}
	};
}