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
		int StartPage;				//��������
		long long WaitTime;			//�����ȴ�ʱ��
		bool IsOffBgLed;			//�رձ�����
		bool IsViewPage;			//��ʾ����
		int ViewPageIndex;			//��ʾ������
		bool IsHideCursor;			//�Ƿ����������
		CursorSize CSize;			//����С
		bool DisableBuzzer;				//�Ƿ�رշ�����
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(StartPage, WaitTime, IsOffBgLed, IsViewPage,
				ViewPageIndex, IsHideCursor, CSize, DisableBuzzer);
		}
	};

	struct MonitorSet
	{
		bool IsVertical;			//�Ƿ�ֱ
		string ScreenModel;			//�ͺ�
		double PartRatioW;			//������ű���
		double PartRatioH;			//�߶����ű���
		double HScreenRatio;		//�ֱ��ʸ߶����ű���
		double WScreenRatio;		//�ֱ��ʿ�����ű���
		int ZoomMode;				//����ģʽ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsVertical, ScreenModel, PartRatioW, PartRatioH,
				HScreenRatio, WScreenRatio, ZoomMode);
		}
	};
	struct VncSet
	{
		int PwdLogin;				//VNC�������� 0���������� 1����������
		string VNCPwd;				//VNC����
		bool IsMonitorMode;			//�Ƿ�ʹ�ü���ģʽ
		bool IsRemoteCommFlag;		//�Ƿ�����Զ��ͨ��
		int RemoteCommPort;			//Զ��ͨ�ſ�
		string SafePattern;			//��ȫģʽ
		string Serverdomain;		//����������
		vector<int> ServerIp;		//������IP
		string DevicePwd;			//�豸����

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(PwdLogin, VNCPwd, IsMonitorMode, IsRemoteCommFlag,
				RemoteCommPort, SafePattern, Serverdomain, ServerIp, DevicePwd);
		}
	};

	struct DisplaySet
	{
		bool IsHmiCurIp;			//HMI�Ƿ�Ϊ��ǰIP
		bool IsConnectDevice;		//�Ƿ���ʾ�����豸
		bool IsClock;				//�Ƿ���ʾʱ��
		bool IsRemoteComm;			//�Ƿ���ʾԶ��ͨ��
		bool IsRemoteMonitor;		//�Ƿ���ʾԶ�̼�أ�VNC��
		bool Iscalibrate;			//�Ƿ�У׼
		bool IsFindUDisk;			//�Ƿ���ʾU�̲�����ʾ
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
		int ClockWriteViewFormat;		//ʱ��д���ʽ 0:ʮ����	1:ʮ������
		vector<DataVarId> TimeVids;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ClockWriteViewFormat, TimeVids);
		}
	};
	struct ClockSet
	{
		bool IsbanClockSet;			//�Ƿ��ֹʱ������
		int ClockViewFormat;		//ʱ�Ӷ�ȡ��ʽ 0:ʮ����	1:ʮ������
		int ClockSource;			//ʱ����Դ 0��HMI�ڲ� 1���ⲿ�豸
		vector<DataVarId> OutDeviceVarId;	//�ⲿ�豸�Ĵ�����ַ
		bool IsClockWriteinPlc;		//�Ƿ�HMIʱ��д���ⲿ�豸
		ClockWriteMode WriteMode;				//д�뷽ʽ
		DataVarId TriReadAddrVarId;	//����д��ģʽ�����ڴ����ļĴ���
		int TriPattern;				//����ģʽ 0:������ 1:�½���
		int WriteCycle;				//д������
		int CycleUnit;				//д�����ڵ�λ ms
		DataVarId CycleVarId;		//��ȡд�����ڵļĴ���
		int WriteInPlcDevicesNum;	//ͬ���豸��
		vector<WritePlcDevicesSet> WriteInPlcDevices;	//ͬ���豸�Ĵ�������
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
		SystemParamSet Param;		//��������
		MonitorSet Monitor;			//��ʾ������
		VncSet VNC;					//Զ������
		DisplaySet Display;			//��ʾ����
		ClockSet Clock;				//ʱ������
		ExchangeSet Exchange;		//��������
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Param, Monitor, VNC, Display, Clock, Exchange);
		}

		void InitData(std::string jstr);

	};
}

