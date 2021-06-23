#include "DataSampleControl.h"
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DataSampleControl.cpp
 * Author   : qiaodan
 * Date     : 2020-11-12
 * Descript : 创建数据采样绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "DataSampleControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include <time.h>
namespace UI
{
	DataSampleControl::DataSampleControl(HMIPage* w) :BaseControl(w)
	{
		//创建KeyBtnModel
		mode_ = std::shared_ptr<DataSampleModel>(new DataSampleModel());
		InitMVCModel(mode_);
	}
	
	void DataSampleControl::CreateView()
	{
		//初始化定时时间
		for (unsigned i = 0; i < mode_->SampleDataConfig.InfoLst.size(); i++)
		{
			if (mode_->SampleDataConfig.InfoLst[i].CollectMode == Project::Fixed)
				RegisterCollectTime(mode_->SampleDataConfig.InfoLst[i].CollectTimeFrom, mode_->SampleDataConfig.InfoLst[i].CollectTimeTo);
		}
 
	}

	void DataSampleControl::AddVarData(vector<Project::DataVarId*> vars)
	{
	}

	
	bool DataSampleControl::MeetByteCondition(Project::Conditions cond, int src, int dst)
	{
		switch (cond)
		{
		case Project::LT:			//小于
			if (src < dst)
				return true;
			else
				return false;
		case Project::GT:			//大于
			if (src > dst)
				return true;
			else
				return false;
		case Project::LE:			//小于等于
			if (src <= dst)
				return true;
			else
				return false;
		case Project::GE:			//大于等于
			if (src >= dst)
				return true;
			else
				return false;
		case Project::EQ:			//相等
			if (src == dst)
				return true;
			else
				return false;
		case Project::NE:
			if (src != dst)
				return true;
			else
				return false;
		default:
			break;
		}
		return false;
	}

	/*************************未实现函数********************************/
	void DataSampleControl::HandleFileNameChange(Project::SampleInfoRes &)
	{
		//处理保存文件名称
	}
	void DataSampleControl::HandleStorePosChange(Project::SampleInfoRes &)
	{
		//处理保存文件位置
	}
	void DataSampleControl::RegisterCollectTime(time_t start, time_t end)
	{
		//注册固定采样时间
	}
	/*******************************************************************/

		/*定时器回调函数*/
	void SampleTimerCb(void *data)
	{
		TimerData *timeData = (TimerData *)data;
		//Project::SampleInfoRes * res = (Project::SampleInfoRes *)data;
		timeData->ctrl->SetSampleContent(*(timeData->info));
		if (timeData->info->SimpleCycleVarId != Project::DataVarId::NullId)
			timeData->info->SimpleCycle = UIDataService::Ins().GetNumber<int>(timeData->info->SimpleCycleVarId);
		Fl::repeat_timeout((double)timeData->info->SimpleCycle / 1000.0, SampleTimerCb, data);

	}

	bool DataSampleControl::SampleIsFull(Project::SampleInfoRes &info)
	{
		for (unsigned i = 0; i < info.SimpleChannelLst.size(); i++)
		{
			if (info.SimpleChannelLst[i].Contents.ChannelContent.size() > info.SampleStoreInfo.MaxSampleNum)
				return true;
			else
				return info.IsFull;
		}
		return true;
	}

	void DataSampleControl::ClearSampleData(Project::SampleInfoRes &info)
	{
		for (unsigned i = 0; i < info.SimpleChannelLst.size(); i++)
			info.SimpleChannelLst[i].ClearChannelContent();		//清除每个通道保存的内容
		if (info.SampleStoreInfo.FullHandleMode == Project::STOP && info.IsFull)
		{
			info.IsFull = false;
			StartSample(info);
		}
	}
	void DataSampleControl::EndSample(Project::SampleInfoRes &info)
	{
		info.startflag_ = 0;
		switch (info.CollectMode)					//判断采样模式
		{
		case Project::Cycle:
		case Project::Fixed:
			Fl::remove_timeout(SampleTimerCb, (void*)&info);
			break;
		case Project::Trigger:
			break;
		default:
			break;
		}

	}
	/*十进制转BCD*/
	template <class T>
	int DectoBCD(T Dec, char *Bcd, int length)
	{
		T i;
		T temp;
		for (i = length - 1; i >= 0; i--)
		{
			temp = Dec % 100;
			Bcd[i] = ((temp / 10) << 4) + ((temp % 10) & 0x0F);
			Dec /= 100;
		}
		return 0;
	}
	/*十进制转Hex*/
	template <class T>
	int DectoHex(T dec, char *hex, int length)
	{
		T i;
		for (i = length - 1; i >= 0; i--)
		{
			hex[i] = (dec % 256) & 0xFF;
			dec /= 256;
		}
		return 0;
	}
	string DataSampleControl::GetSampleData(Project::SampleChannel &channelinfo)
	{
		string datastyle = channelinfo.DataStyle;
		string dataformat = channelinfo.DataStyle;
		Project::DataVarId varid = channelinfo.SampleAddr;

		if (datastyle == "Word")			//16位
		{
			if (dataformat == "BCD")
			{
				short data = UIDataService::Ins().GetNumber<short>(varid);
				char bcd[16];
				DectoBCD<short>(data, bcd, 16);
				return bcd;
			}
			if (dataformat == "Hex")
			{
				short data = UIDataService::Ins().GetNumber<short>(varid);
				char hex[16];
				DectoHex<short>(data, hex, 16);
				return hex;
			}
			if (dataformat == "Signed")
			{
				short data = UIDataService::Ins().GetNumber<short>(varid);
				return to_string(data);
			}
			if (dataformat == "Unsigned")
			{
				unsigned short data = UIDataService::Ins().GetNumber<unsigned short>(varid);
				return to_string(data);
			}

		}
		if (datastyle == "DWord")
		{
			if (dataformat == "BCD")
			{
				int data = UIDataService::Ins().GetNumber<int>(varid);
				char bcd[32];
				DectoBCD<int>(data, bcd, 32);
				return bcd;
			}
			if (dataformat == "Hex")
			{
				int data = UIDataService::Ins().GetNumber<int>(varid);
				char hex[32];
				DectoHex<int>(data, hex, 32);
				return hex;
			}
			if (dataformat == "Signed")
			{
				int data = UIDataService::Ins().GetNumber<int>(varid);
				return to_string(data);
			}
			if (dataformat == "Unsigned")
			{
				unsigned int data = UIDataService::Ins().GetNumber<unsigned int>(varid);
				return to_string(data);
			}
			if (dataformat == "Float")
			{
				float data = UIDataService::Ins().GetNumber<float>(varid);
				return to_string(data);
			}

		}
		if (datastyle == "DDWord")
		{
			if (dataformat == "BCD")
			{
				long long data = UIDataService::Ins().GetNumber<long long>(varid);
				char bcd[64];
				DectoBCD<long long>(data, bcd, 64);
				return bcd;
			}
			if (dataformat == "Hex")
			{
				long long data = UIDataService::Ins().GetNumber<long long>(varid);
				char hex[64];
				DectoHex<long long>(data, hex, 64);
				return hex;
			}
			if (dataformat == "Signed")
			{
				long long data = UIDataService::Ins().GetNumber<long long>(varid);
				return to_string(data);
			}
			if (dataformat == "Unsigned")
			{
				unsigned long long data = UIDataService::Ins().GetNumber<unsigned long long>(varid);
				return to_string(data);
			}
			if (dataformat == "Float")
			{
				double data = UIDataService::Ins().GetNumber<double>(varid);
				return to_string(data);
			}

		}
		if (datastyle == "String")
		{
			char data[256];
			UIDataService::Ins().GetString(varid, data, channelinfo.WordNum);
		}


	}
	void DataSampleControl::SetSampleContent(Project::SampleInfoRes &info)
	{
		
		if (SampleIsFull(info))
		{
			switch (info.SampleStoreInfo.FullHandleMode)
			{
			case Project::CLEAR:
				ClearSampleData(info);
				info.IsFull = false;
				break;
			case Project::STOP:
				return EndSample(info);
			case Project::COVER:
			{
				static int pos;
				for (unsigned i = 0; i < info.SimpleChannelLst.size(); i++)
				{
					if (pos < info.SimpleChannelLst[i].Contents.ChannelContent.size())	//覆盖
					{
						info.SimpleChannelLst[i].Contents.ChannelContent[pos].Data = GetSampleData(info.SimpleChannelLst[i]);
						info.SimpleChannelLst[i].Contents.ChannelContent[pos].ChannelNo = info.SimpleChannelLst[i].Channel;
						timeval tv;
						gettimeofday(&tv, NULL);
						info.SimpleChannelLst[i].Contents.ChannelContent[pos].SampleTime.tv_sec = tv.tv_sec;
						info.SimpleChannelLst[i].Contents.ChannelContent[pos].SampleTime.tv_usec = tv.tv_usec;
						info.SimpleChannelLst[i].Contents.ChannelContent[pos].SimpleGroupName = info.SimpleGroupName;
						info.SimpleChannelLst[i].Contents.ChannelContent[pos].SimpleGroupNo = info.SimpleGroupNo;
						pos++;
						info.SimpleChannelLst[i].Contents.CoverFlag = pos;
					}
					else
					{
						pos = 0;
						info.SimpleChannelLst[i].Contents.CoverFlag = 0;
					}
				}
				break;
			}
			default:
				break;
			}
		}
		else
		{
			for (unsigned i = 0; i < info.SimpleChannelLst.size(); i++)
			{
				Project::SampleChannel::SampleContent newContent;
				newContent.Data = GetSampleData(info.SimpleChannelLst[i]);
				newContent.ChannelNo = info.SimpleChannelLst[i].Channel;
				timeval tv;
				gettimeofday(&tv, NULL);
				newContent.SampleTime.tv_sec = tv.tv_sec;
				newContent.SampleTime.tv_usec = tv.tv_usec;
				newContent.SimpleGroupName = info.SimpleGroupName;
				newContent.SimpleGroupNo = info.SimpleGroupNo;
				info.SimpleChannelLst[i].Contents.ChannelContent.push_back(newContent);
			}
		}
		//发送消息通知趋势图重绘
		for (unsigned i = 0; i < Trend.size(); i++)
		{
			if (Trend[i].SimpleGroupName == info.SimpleGroupName && Trend[i].SimpleGroupNo == info.SimpleGroupNo)
			{
				vector<Project::SampleChannel::SampleContent>().swap(Contents);
				for (unsigned j = 0; j < Trend[i].Channel.size(); j++)
				{
					int channelno = Trend[i].Channel[j],lastno;
					if (info.SimpleChannelLst[channelno].Contents.ChannelContent.size() == 1000)
						lastno = info.SimpleChannelLst[channelno].Contents.CoverFlag - 1;
					else
						lastno = info.SimpleChannelLst[channelno].Contents.ChannelContent.size() - 1;
					Contents.push_back(info.SimpleChannelLst[channelno].Contents.ChannelContent[lastno]);
				}
				Win()->SendAutoFreeMessage<vector<Project::SampleChannel::SampleContent>>((void *)Trend[i].CtrlId, TREND_SAMPLE_CHANGE, &Contents);
			}
		}
	}
	bool DataSampleControl::MeetFixedSampleTime(Project::SampleInfoRes &info)
	{
		time_t now = time(NULL);
		if (now >= info.CollectTimeFrom && now <= info.CollectTimeTo)
			return true;
		else
			return false;
	}
	void DataSampleControl::StartSample(Project::SampleInfoRes &info)
	{
		int sampleCtrlValue;
		if (info.SimpleCtrlVarId != Project::DataVarId::NullId)	//是否使用采样开始控制
			sampleCtrlValue = UIDataService::Ins().GetNumber<int>(info.SimpleCtrlVarId);
		else
			sampleCtrlValue = info.SimpleStyle;

		if (sampleCtrlValue == info.SimpleStyle && info.startflag_ == 0)			//开始采样
		{
			info.startflag_ = 1;
			switch (info.CollectMode)					//判断采样模式
			{
			case Project::Cycle:
			{
				if (info.SimpleCycleVarId != Project::DataVarId::NullId)
					info.SimpleCycle = UIDataService::Ins().GetNumber<int>(info.SimpleCycleVarId);
				TimerData *timeData = new TimerData();
				timeData->ctrl = this;
				timeData->info = &info;
				Fl::add_timeout((double)info.SimpleCycle / 1000.0, SampleTimerCb, (void*)timeData);
				break;
			}
			case Project::Fixed:
			{
				//设置定时器
				if (MeetFixedSampleTime(info))
				{
					if (info.SimpleCycleVarId != Project::DataVarId::NullId)
						info.SimpleCycle = UIDataService::Ins().GetNumber<int>(info.SimpleCycleVarId);
					Fl::add_timeout((double)info.SimpleCycle / 1000.0, SampleTimerCb, (void*)this);

				}
				break;
			}
			case Project::Trigger:
			{
				//采样
				SetSampleContent(info);
				break;
			}
			default:
				break;
			}
		}
	}
	void DataSampleControl::HandleDataVar(Project::DataVarId &varId)
	{
		vector<Project::SampleInfoRes>::iterator tmpRes;
		unsigned i;
		int compareValue, byteValue;
		bool bitValue;

		for (i = 0, tmpRes = mode_->SampleDataConfig.InfoLst.begin(); tmpRes != mode_->SampleDataConfig.InfoLst.end(); i++, tmpRes++)
		{
			if ((*tmpRes).SimpleCtrlVarId.Cmp(varId))		//控制寄存器
			{
				if (UIDataService::Ins().GetNumber<int>((*tmpRes).SimpleCtrlVarId))	//ON状态
					StartSample((*tmpRes));
				else
					EndSample((*tmpRes));
			}
			if ((*tmpRes).TriggerReadAddr.Cmp(varId) || (*tmpRes).ComPareVar.Cmp(varId))		//读取寄存器
			{
				if ((*tmpRes).TriggerBitMode)		//字寄存器
				{
					byteValue = UIDataService::Ins().GetNumber<int>((*tmpRes).TriggerReadAddr);
					if ((*tmpRes).ComPareVar == Project::DataVarId::NullId)
						compareValue = (*tmpRes).ComPareValue;
					else
						compareValue = UIDataService::Ins().GetNumber<int>((*tmpRes).ComPareVar);
					if (MeetByteCondition((*tmpRes).BtyeCondition, byteValue, compareValue))
						StartSample((*tmpRes));		//满足条件触发报警
				}
				else		//位寄存器
				{
					//获取位寄存器值
					bitValue = UIDataService::Ins().GetBit((*tmpRes).TriggerReadAddr);
					if ((*tmpRes).BitCondition)		//OFF->ON
					{
						if (prevbitValue_ == 0 && bitValue == 1)
							StartSample((*tmpRes));		//满足条件触发报警
					}
					else
					{
						if (prevbitValue_ == 1 && bitValue == 0)
							StartSample((*tmpRes));		//满足条件触发报警
					}
				}
			}
			if ((*tmpRes).SampleStoreInfo.FullNoticeVarId.Cmp(varId))
				(*tmpRes).IsFull = true;
			if ((*tmpRes).SampleStoreInfo.ClearDataVarId.Cmp(varId))
				ClearSampleData((*tmpRes));
			if ((*tmpRes).SampleStoreInfo.StoreFileInfo.FileNameDataVar.Cmp(varId))
				HandleFileNameChange(*tmpRes);
			if ((*tmpRes).SampleStoreInfo.StoreFileInfo.StorePosVarId.Cmp(varId))
				HandleStorePosChange(*tmpRes);
		}

	}

	int DataSampleControl::PeekHMIMessage(Message::Msg* msg)
	{
		Project::DataVarId varId;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = *(Project::DataVarId *)msg->LParam;
			HandleDataVar(varId);
			break;
		case WM_EVENT_TIMEUP:
			Win()->SendUserMessage((void*)this->CtrlId(), WM_EVENT_TIMETICK, 1000);//1s
			break;
		case WM_EVENT_READY:
			Win()->SendUserMessage((void*)this->CtrlId(), WM_EVENT_TIMETICK, 1000);//1s
			break;
		case SAMPLE_EVENT_START:		//开始采样
		{
			if (mode_->SampleDataConfig.InfoLst.size() != 0)
			{
				for (unsigned i = 0; i < mode_->SampleDataConfig.InfoLst.size(); i++)
				{
					if(msg->LParam)	//开始采样
						StartSample(mode_->SampleDataConfig.InfoLst[i]);
					else
						EndSample(mode_->SampleDataConfig.InfoLst[i]);
				}
			}
			break;
		}
		case SAMPLE_ALAEM_CLOCK:		//0:停止 1:开始
		{
			if (mode_->SampleDataConfig.InfoLst.size() != 0)
			{
				for (unsigned i = 0; i < mode_->SampleDataConfig.InfoLst.size(); i++)
				{
					if (mode_->SampleDataConfig.InfoLst[i].CollectMode = Project::Fixed)
					{
						if (MeetFixedSampleTime(mode_->SampleDataConfig.InfoLst[i]))
							StartSample(mode_->SampleDataConfig.InfoLst[i]);
						else
							EndSample(mode_->SampleDataConfig.InfoLst[i]);
					}
				}
			}
			
			break;
		}
		default:
			break;
		}
		return 1;//handled;
	}
}
