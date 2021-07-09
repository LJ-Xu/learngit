#pragma once
#include "CerealCommon.hpp"
#include "DataVar.h"
namespace Project
{
	using namespace std;
	enum CursorSize :char {
		B20x20, W20x20,
		B30x30, W30x30,
		B40x40, W40x40,
		B50x50, W50x50,
	};
	struct SystemParamSet
	{
		int StartPage;				//启动画面
		long long WaitTime;			//屏保等待时间
		bool IsOffBgLed;			//关闭背景灯
		bool IsViewPage;			//显示画面
		int ViewPageIndex;			//显示画面编号
		bool IsHideCursor;			//是否隐藏鼠标光标
		CursorSize CSize;			//光标大小
		bool DisableBuzzer;				//是否关闭蜂鸣器
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(StartPage, WaitTime, IsOffBgLed, IsViewPage,
				ViewPageIndex, IsHideCursor, CSize, DisableBuzzer);
		}
	};

	struct MonitorSet
	{
		bool IsVertical;			//是否垂直
		string ScreenModel;			//型号
		double PartRatioW;			//宽度缩放比例
		double PartRatioH;			//高度缩放比例
		double HScreenRatio;		//分辨率高度缩放比例
		double WScreenRatio;		//分辨率宽度缩放比例
		int ZoomMode;				//缩放模式
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsVertical, ScreenModel, PartRatioW, PartRatioH,
				HScreenRatio, WScreenRatio, ZoomMode);
		}
	};
	struct VncSet
	{
		int PwdLogin;				//VNC密码设置 0：无需密码 1：设置密码
		string VNCPwd;				//VNC密码
		bool IsMonitorMode;			//是否使用监视模式
		bool IsRemoteCommFlag;		//是否启用远程通信
		int RemoteCommPort;			//远程通信口
		string SafePattern;			//安全模式
		string Serverdomain;		//服务器域名
		vector<int> ServerIp;		//服务器IP
		string DevicePwd;			//设备密码

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(PwdLogin, VNCPwd, IsMonitorMode, IsRemoteCommFlag,
				RemoteCommPort, SafePattern, Serverdomain, ServerIp, DevicePwd);
		}
	};

	struct DisplaySet
	{
		bool IsHmiCurIp;			//HMI是否为当前IP
		bool IsConnectDevice;		//是否显示连接设备
		bool IsClock;				//是否显示时钟
		bool IsRemoteComm;			//是否显示远程通信
		bool IsRemoteMonitor;		//是否显示远程监控（VNC）
		bool Iscalibrate;			//是否校准
		bool IsFindUDisk;			//是否显示U盘插入提示
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsHmiCurIp, IsConnectDevice, IsClock, IsRemoteComm,
				IsRemoteMonitor, Iscalibrate, IsFindUDisk);
		}
	};
	enum ClockWriteMode :char { ClockWriteContinue, ClockWriteTrigger, ClockWriteCycle };
	struct WritePlcDevicesSet
	{
		int ClockWriteViewFormat;		//时钟写入格式 0:十进制	1:十六进制
		vector<DataVarId> TimeVids;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ClockWriteViewFormat, TimeVids);
		}
	};
	struct ClockSet
	{
		bool IsbanClockSet;			//是否禁止时钟设置
		int ClockViewFormat;		//时钟读取格式 0:十进制	1:十六进制
		int ClockSource;			//时钟来源 0：HMI内部 1：外部设备
		vector<DataVarId> OutDeviceVarId;	//外部设备寄存器地址
		bool IsClockWriteinPlc;		//是否讲HMI时钟写入外部设备
		ClockWriteMode WriteMode;				//写入方式
		DataVarId TriReadAddrVarId;	//触发写入模式下用于触发的寄存器
		int TriPattern;				//触发模式 0:上升沿 1:下降沿
		int WriteCycle;				//写入周期
		int CycleUnit;				//写入周期单位 ms
		DataVarId CycleVarId;		//读取写入周期的寄存器
		int WriteInPlcDevicesNum;	//同步设备数
		vector<WritePlcDevicesSet> WriteInPlcDevices;	//同步设备寄存器设置
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsbanClockSet, ClockViewFormat, ClockSource, OutDeviceVarId, 
				IsClockWriteinPlc, WriteMode, TriReadAddrVarId, TriPattern, WriteCycle,
				CycleUnit, CycleVarId, WriteInPlcDevicesNum, WriteInPlcDevices);
		}
	};

	struct ExchangeSet
	{
		bool IsCtrlPageExchange;
		DataVarId CtrlPageExchangeVarId;
		bool IsReportCurPageNo;
		DataVarId ReportCurPageNoVarId;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsCtrlPageExchange, CtrlPageExchangeVarId, IsReportCurPageNo, ReportCurPageNoVarId);
		}
	};
	struct ProjectSystemSet
	{
		SystemParamSet Param;		//参数设置
		MonitorSet Monitor;			//显示器设置
		VncSet VNC;					//远程设置
		DisplaySet Display;			//显示设置
		ClockSet Clock;				//时钟设置
		ExchangeSet Exchange;		//交互设置
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Param, Monitor, VNC, Display, Clock, Exchange);
		}

		void InitData(std::string jstr);

	};
}

