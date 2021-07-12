#pragma once
#include<map>
#include<vector>
#include "DataVarInfo.h"
using namespace std;
namespace Project
{
	enum PortType :char
	{
		Download = 0,//下载口
		PLCPort,//PLC口
		NetPort,//网口
		PortNum
	};
	enum PointType :char
	{
		RS232, RS485, RS422, CAN, PointNum
	};
	struct PrjBasicParam
	{
		int OutTime;//通信超时ms
		int RetryTimes;//重试次数
		int DelayTime;//延时ms
		int PeriodTime;//间隔时间
		int MaxReadWords;
		int MaxWriteWords;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(OutTime, RetryTimes, DelayTime, PeriodTime, MaxReadWords, MaxWriteWords);
		}
		void Parse(rapidjson::Value& jsonObj);
	};
	struct PrjNetParam
	{
		string Addr;//网络地址
		int    Port;//端口号
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Addr, Port);
		}
		void Parse(rapidjson::Value& jsonObj);
	};
	struct PrjUartParam
	{
		int  Baud; //波特率
		int Parity;//奇偶校验位
		char DataBits;//数据位
		char StopBits;//停止位
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Baud, Parity, DataBits, StopBits);
		}
		void Parse(rapidjson::Value& jsonObj);
	};
	// 基础的通信参数配置
	struct PrjCommParam
	{
		PrjBasicParam BasicParam;
		PrjNetParam   NetParam;
		PrjUartParam  UartParam;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(BasicParam, NetParam, UartParam);
		}
	};
	enum ByteSwapMode
	{
		CHANGE_BIT = 0x01,           //默认低位在前
		CHANGE_BYTE = 0x02,			 //默认小端模式，内存低字节在前
		CHANGE_WORD = 0x04,			 //默认小端模式，内存低字节在前
		CHANGE_DWORD = 0x08,		 //默认小端模式，内存低字节在前
	};

	struct HighLowByteSet
	{
		//字节序转换
		int WordEndian;
		int WordStringEndian;
		int DWordEndian;
		int DWordStringEndian;
		int DDWordEndian;
		int DDWordStringEndian;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(WordEndian, WordStringEndian, DWordEndian, DWordStringEndian,
				DDWordEndian, DDWordStringEndian);
		}
	};
	struct OfferLineSimInitMessage
	{
		short RegType;
		short RegDataType;
		int RegMaxCount;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(RegType, RegDataType, RegMaxCount);
		}
		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<OfferLineSimInitMessage>& vector, rapidjson::Value& jsonObj);

	};

	// 端口对应的设备接口
	struct PrjDev
	{
		string DevName; // 设备名称
		short PortID;//对应的端口
		PointType Point;//该端口下的接口   下载口下的接口 RS485 RS422 RS232        
		short StaNo;//默认设备站号  
		string ProtocolId;//设备的通信协议Id
		PrjCommParam CommParam;//该接口的通信参数
		std::vector<OfferLineSimInitMessage> OfferLineInitMsgs;
		HighLowByteSet HighLowByte;
		vector<char> DevParamData;//私有配置参数param data信息 通过该信息和ProtocolId可以转换为具体结构
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(DevName, PortID, Point, StaNo, ProtocolId, CommParam,
				/*OfferLineInitMsgs, */HighLowByte, DevParamData);
		}
		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<PrjDev>& vector, rapidjson::Value& jsonObj);
		bool IsEmpty()
		{
			return PortID == 0 && DevName.empty();
		}
		bool IsLocal()
		{
			return PortID == 0;
		}
	};
	struct HMIPortParam
	{
		struct
		{
			string Addr;
			string Mask;
			string GateWay;
			bool   IsAuto;
			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(Addr, Mask, GateWay, IsAuto);
			}
		}NetParam;//当前端口网络地址信息

		struct
		{
			char NamePath[30];//ex:/dev/tty3   COM3
			int  Baud; //波特率
			char Parity;//奇偶校验位
			char DataBits;//数据位
			char StopBits;//停止位

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(NamePath, Baud, Parity, DataBits, StopBits);
			}
		}SeriaParam;//当前端口串口配置信息
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(NetParam, SeriaParam);
		}
		void Parse(rapidjson::Value& jsonObj);
	};
	struct HMIPort
	{
		string    Name; //端口名称  com1 com2  net1
		PortType  Port;//端口类型
		HMIPortParam Param;//端口初始化参数
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Name, Port, Param);
		}
		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<HMIPort>& vector, rapidjson::Value& jsonObj);

	};
	struct ProjectDevice
	{
		// 每个port下可能有多个接口设备
		std::vector<HMIPort> Ports;
		std::vector<PrjDev> Devs; //设备节点 device ID 为索引号       
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Ports, Devs);
		}
		void InitData(std::string jstr);
	};
}
