/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SampleInfoRes
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ݲ�����
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MultiPilotLampConfig.h"
#include "DataVarInfo.h"
#include "AlarmInputConfig.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "AlarmInfoRes.h"
#include <time.h>
#include <map>
namespace Project
{
	struct SampleChannel
	{
		int Channel;					//ͨ�����
		DataVarId SampleAddr;			//������ַ
		int WordNum;					//�ֽ���
		VarDataType DataStyle;				//
		VarNumberType DataFormat;				//���ݸ�ʽ
		string Describe;				//ͨ������

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Channel, SampleAddr, DataStyle, DataFormat, Describe);
		}
		void Parse(rapidjson::Value& jsonObj, int gname, int gno);
		static void Parse(std::vector<SampleChannel>& vector, rapidjson::Value& jsonObj,int gname,int gno);
		struct SampleContent
		{
			int SimpleGroupName;				//�������
			int SimpleGroupNo;					//�������
			int ChannelNo;						//����ͨ��
			string Data;						//����
			SearchTime SampleTime;				//����ʱ��
		};
		struct ChannelContentInfo
		{
			vector<SampleContent> ChannelContent;				//���������б�
			int CoverFlag = 0;				//���Ǳ�ʶ
		};
		ChannelContentInfo Contents;
		void ClearChannelContent()				//�����������
		{
			std::vector<SampleContent>().swap(Contents.ChannelContent);
		}
	};
	enum FullAction:char {CLEAR, COVER, STOP };
#define MAXCHANNELNUM MAXSQLCOUNT //ÿ��ͨ������������
	struct SampleStore
	{
		int StoreMode;					//�洢ģʽ 1:ѭ������2.��������
		FullAction FullHandleMode;		//0:�����������ȡ��	1:ֹͣȡ��  2:����
		DataVarId FullNoticeVarId;		//����֪ͨ
		DataVarId ClearDataVarId;		//��������Ĵ���
		int ClearDataMode;				//0:ON->OFF 1:OFF->ON
		int MaxSampleNum= MAXSQLCOUNT;				//����������(Ĭ��256)
		bool IsSave;//1������2����
		SaveFileRes StoreFileInfo;		//�����ļ�����

		//��������Ϣ
		int StopFlag = 0;//����ʱֹͣ�ɼ�
		int TempClearAddrStatus = -1;//��������Ĵ�����Ե����ֵ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(StoreMode, FullHandleMode, FullNoticeVarId, ClearDataVarId, ClearDataMode,
				MaxSampleNum, StoreFileInfo, IsSave);
		}
		void Parse(rapidjson::Value& jsonObj);

	};

	enum SampleMode:char {Cycle, Trigger, Fixed};
	
	class SampleInfoRes
	{
	public:
		int SimpleGroupName;				//�������
		int SimpleGroupNo;					//�������

		string SimpleDescribe;				//����������
		DataVarId SimpleCtrlVarId;			//�������ƼĴ���
		int SimpleStyle;					//�������Ʒ�ʽ 0:OFF״̬ 1:ON״̬
		SampleMode CollectMode;				//����ģʽ
		/*���ڲ���*/
		DataVarId SimpleCycleVarId;			//����Ƶ��ָ���Ĵ���
		int SimpleCycle;					//����Ƶ��(ms)
		/*��������*/
		int TriggerBitMode;					//�Ĵ���ģʽ	0:λ�Ĵ���	1:�ּĴ���
		DataVarId TriggerReadAddr;			//��ȡ�Ĵ�����ַ
		Project::Conditions BtyeCondition;	//�ּĴ�������������С�ڡ����ڡ�С�ڵ��ڡ����ڵ��ڡ���ȡ�������
		//int BitCondition;					//λ�Ĵ�����������	0:ON->OFF 1:OFF->ON
		Project::BitConditions BitCondition;		// Add By TangYao
		int ComPareValue;					//�Ա�ֵ
		DataVarId ComPareVar;				//�ԱȼĴ�����ַ
		/*�̶�ģʽ*/
		long CollectTimeFrom;				//������ʼʱ��
		long CollectTimeTo;				//������ʼʱ��
		int SimpleFrequency;				//��������ʱ��
		DataVarId FixSampleVarId;
		// int FixSampleFrequ;
		/*������ַ*/
		bool IsContinueAddr;				//������ַģʽ	true:������ַ
		vector<SampleChannel> SimpleChannelLst;		//����ͨ������
		/*��������*/
		SampleStore SampleStoreInfo;

		bool ChangeFlag = false;
		char TempByteState = -1;//��ʱ����λ״̬

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(SimpleGroupName, SimpleGroupNo, SimpleDescribe, SimpleCtrlVarId, SimpleStyle, CollectMode,
				SimpleCycleVarId, SimpleCycle, TriggerBitMode, TriggerReadAddr, BtyeCondition,
				BitCondition, ComPareValue, ComPareVar, CollectTimeFrom, CollectTimeTo, SimpleFrequency, IsContinueAddr,
				SimpleChannelLst, SampleStoreInfo, ChangeFlag);
		}

		
		/******************************ʵ���߼�����***********************************/
		bool IsFull = false;						//�����Ƿ�����
		bool Changeflag = false;
		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<SampleInfoRes>& vector, rapidjson::Value& jsonObj);
	
		//void StartSample();					//��ʼ���ݲ���
		//void EndSample();					//�������ݲ���

		//bool SampleIsFull();				//�жϴ洢�����Ƿ�����
		//bool MeetFixedSampleTime();			//�ж��Ƿ�����̶�����ʱ������

		//void ClearSampleData();				//������ݲ�������
		//void SetSampleContent();			//�洢��������

		/*******************************************************************************
		 * Name     : GetSampleData
		 * Descript : ��ȡ���ݲ�������
		 * Input    : channelinfo - ����ͨ����Ϣ
		 * Output   : string      - ��������
		 * Note	    :
		 *******************************************************************************/
		//string GetSampleData(SampleChannel &channelinfo);
		int startflag_ = 0;			//�Ƿ��ڲ�����־

	private:

	};

}

