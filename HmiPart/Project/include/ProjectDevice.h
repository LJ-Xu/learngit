#pragma once
#include<map>
#include<vector>
#include "DataVarInfo.h"
using namespace std;
namespace Project
{
	enum PortType :char
	{
		Download = 0,//���ؿ�
		PLCPort,//PLC��
		NetPort,//����
		PortNum
	};
	enum PointType :char
	{
		RS232, RS485, RS422, CAN, PointNum
	};
	struct PrjBasicParam
	{
		int OutTime;//ͨ�ų�ʱms
		int RetryTimes;//���Դ���
		int DelayTime;//��ʱms
		int PeriodTime;//���ʱ��
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
		string Addr;//�����ַ
		int    Port;//�˿ں�
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Addr, Port);
		}
		void Parse(rapidjson::Value& jsonObj);
	};
	struct PrjUartParam
	{
		int  Baud; //������
		int Parity;//��żУ��λ
		char DataBits;//����λ
		char StopBits;//ֹͣλ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Baud, Parity, DataBits, StopBits);
		}
		void Parse(rapidjson::Value& jsonObj);
	};
	// ������ͨ�Ų�������
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

	// �˿ڶ�Ӧ���豸�ӿ�
	struct PrjDev
	{
		string DevName; // �豸����
		short PortID;//��Ӧ�Ķ˿�
		PointType Point;//�ö˿��µĽӿ�   ���ؿ��µĽӿ� RS485 RS422 RS232        
		short StaNo;//Ĭ���豸վ��  
		string ProtocolId;//�豸��ͨ��Э��Id
		PrjCommParam CommParam;//�ýӿڵ�ͨ�Ų���
		vector<char> DevParamData;//˽�����ò���param data��Ϣ ͨ������Ϣ��ProtocolId����ת��Ϊ����ṹ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(DevName, PortID, Point, StaNo, ProtocolId, CommParam, DevParamData);
		}
		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<PrjDev>& vector, rapidjson::Value& jsonObj);
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
		}NetParam;//��ǰ�˿������ַ��Ϣ

		struct
		{
			char NamePath[30];//ex:/dev/tty3   COM3
			int  Baud; //������
			char Parity;//��żУ��λ
			char DataBits;//����λ
			char StopBits;//ֹͣλ

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(NamePath, Baud, Parity, DataBits, StopBits);
			}
		}SeriaParam;//��ǰ�˿ڴ���������Ϣ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(NetParam, SeriaParam);
		}
		void Parse(rapidjson::Value& jsonObj);
	};
	struct HMIPort
	{
		string    Name; //�˿�����  com1 com2  net1
		PortType  Port;//�˿�����
		HMIPortParam Param;//�˿ڳ�ʼ������
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
		// ÿ��port�¿����ж���ӿ��豸
		std::vector<HMIPort> Ports;
		std::vector<PrjDev> Devs; //�豸�ڵ� device ID Ϊ������         
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Ports, Devs);
		}
		void InitData(std::string jstr);
	};
}
