/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SampleInfoRes
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 数据采样类
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
		int Channel;					//通道编号
		DataVarId SampleAddr;			//采样地址
		int WordNum;					//字节数
		VarDataType DataStyle;				//
		VarNumberType DataFormat;				//数据格式
		string Describe;				//通道描述

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Channel, SampleAddr, DataStyle, DataFormat, Describe);
		}
		void Parse(rapidjson::Value& jsonObj, int gname, int gno);
		static void Parse(std::vector<SampleChannel>& vector, rapidjson::Value& jsonObj,int gname,int gno);
		struct SampleContent
		{
			int SimpleGroupName;				//采样组别
			int SimpleGroupNo;					//采样编号
			int ChannelNo;						//采样通道
			string Data;						//数据
			SearchTime SampleTime;				//采样时间
		};
		struct ChannelContentInfo
		{
			vector<SampleContent> ChannelContent;				//采样内容列表
			int CoverFlag = 0;				//覆盖标识
		};
		ChannelContentInfo Contents;
		void ClearChannelContent()				//清除采样内容
		{
			std::vector<SampleContent>().swap(Contents.ChannelContent);
		}
	};
	enum FullAction:char {CLEAR, COVER, STOP };
#define MAXCHANNELNUM MAXSQLCOUNT //每个通道的最大采样数
	struct SampleStore
	{
		int StoreMode;					//存储模式 1:循环覆盖2.采满处理
		FullAction FullHandleMode;		//0:清除数据重新取样	1:停止取样  2:覆盖
		DataVarId FullNoticeVarId;		//采满通知
		DataVarId ClearDataVarId;		//数据清除寄存器
		int ClearDataMode;				//0:ON->OFF 1:OFF->ON
		int MaxSampleNum= MAXSQLCOUNT;				//最大采样条数(默认256)
		bool IsSave;//1不保存2保存
		SaveFileRes StoreFileInfo;		//保存文件设置

		//非配置信息
		int StopFlag = 0;//非零时停止采集
		int TempClearAddrStatus = -1;//数据清除寄存器边缘保存值
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
		int SimpleGroupName;				//采样组别
		int SimpleGroupNo;					//采样编号

		string SimpleDescribe;				//采样组描述
		DataVarId SimpleCtrlVarId;			//采样控制寄存器
		int SimpleStyle;					//采样控制方式 0:OFF状态 1:ON状态
		SampleMode CollectMode;				//采样模式
		/*周期采样*/
		DataVarId SimpleCycleVarId;			//采样频率指定寄存器
		int SimpleCycle;					//采样频率(ms)
		/*触发采样*/
		int TriggerBitMode;					//寄存器模式	0:位寄存器	1:字寄存器
		DataVarId TriggerReadAddr;			//读取寄存器地址
		Project::Conditions BtyeCondition;	//字寄存器触发条件：小于、大于、小于等于、大于等于、相等、不等于
		//int BitCondition;					//位寄存器触发条件	0:ON->OFF 1:OFF->ON
		Project::BitConditions BitCondition;		// Add By TangYao
		int ComPareValue;					//对比值
		DataVarId ComPareVar;				//对比寄存器地址
		/*固定模式*/
		long CollectTimeFrom;				//采样开始时间
		long CollectTimeTo;				//采样开始时间
		int SimpleFrequency;				//采样结束时间
		DataVarId FixSampleVarId;
		// int FixSampleFrequ;
		/*采样地址*/
		bool IsContinueAddr;				//采样地址模式	true:连续地址
		vector<SampleChannel> SimpleChannelLst;		//采样通道设置
		/*采样保存*/
		SampleStore SampleStoreInfo;

		bool ChangeFlag = false;
		char TempByteState = -1;//临时保存位状态

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(SimpleGroupName, SimpleGroupNo, SimpleDescribe, SimpleCtrlVarId, SimpleStyle, CollectMode,
				SimpleCycleVarId, SimpleCycle, TriggerBitMode, TriggerReadAddr, BtyeCondition,
				BitCondition, ComPareValue, ComPareVar, CollectTimeFrom, CollectTimeTo, SimpleFrequency, IsContinueAddr,
				SimpleChannelLst, SampleStoreInfo, ChangeFlag);
		}

		
		/******************************实现逻辑参数***********************************/
		bool IsFull = false;						//数据是否已满
		bool Changeflag = false;
		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<SampleInfoRes>& vector, rapidjson::Value& jsonObj);
	
		//void StartSample();					//开始数据采样
		//void EndSample();					//结束数据采样

		//bool SampleIsFull();				//判断存储采样是否已满
		//bool MeetFixedSampleTime();			//判断是否满足固定采样时间条件

		//void ClearSampleData();				//清除数据采样内容
		//void SetSampleContent();			//存储采样内容

		/*******************************************************************************
		 * Name     : GetSampleData
		 * Descript : 获取数据采样数据
		 * Input    : channelinfo - 采样通道信息
		 * Output   : string      - 采样数据
		 * Note	    :
		 *******************************************************************************/
		//string GetSampleData(SampleChannel &channelinfo);
		int startflag_ = 0;			//是否在采样标志

	private:

	};

}

