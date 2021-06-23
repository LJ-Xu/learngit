/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SampleInfoRes.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 数据采样类json解析
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "SampleInfoRes.h"
namespace Project
{
	void SampleChannel::Parse(rapidjson::Value& jsonObj, int gname, int gno)
	{
		if (jsonObj.HasMember("Channel"))
			Channel = jsonObj["Channel"].GetInt()|(gname<<16)|(gno<<24);
		if (jsonObj.HasMember("WordNum"))
			WordNum = jsonObj["WordNum"].GetInt();
		if (jsonObj.HasMember("ChannelVarId") && jsonObj["ChannelVarId"].IsObject())
			SampleAddr.Parse(jsonObj["ChannelVarId"]);
		if (jsonObj.HasMember("DataStyle") && !jsonObj["DataStyle"].IsNull())
		{
			std::string strStyle= jsonObj["DataStyle"].GetString();
			if (strStyle.compare("Word"))
			{
				DataStyle = VarDataType::DT_WORD;
			}
			else if (strStyle.compare("DWord"))
			{
				DataStyle = VarDataType::DT_DWORD;
			}
			else if (strStyle.compare("DDWord"))
			{
				DataStyle = VarDataType::DT_DDWORD;
			}
			else
			{
				DataStyle = VarDataType::DT_String;
			}
		}
		if (jsonObj.HasMember("DataFormat") && !jsonObj["DataFormat"].IsNull())
		{
			std::string strfmt= jsonObj["DataFormat"].GetString();
			if (strfmt.compare("Float"))
			{
				DataFormat = VarNumberType::NT_Float;
			}
			else if (strfmt.compare("Hex"))
			{
				DataFormat = VarNumberType::NT_Hex;
			}
			else if (strfmt.compare("Signed"))
			{
				DataFormat = VarNumberType::NT_Signed;
			}
			else if (strfmt.compare("Unsigned"))
			{
				DataFormat = VarNumberType::NT_Unsigned;
			}
			else
			{
				DataFormat = VarNumberType::NT_BCD;
			}
		}
		if (jsonObj.HasMember("Describe") && !jsonObj["Describe"].IsNull())
			Describe = jsonObj["Describe"].GetString();
	}
	void SampleChannel::Parse(std::vector<SampleChannel>& vector, rapidjson::Value& jsonObj, int gname, int gno)
	{
		std::vector<SampleChannel>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::SampleChannel res;
				res.Parse(jsonObj[i],gname,gno);
				vector.push_back(res);
			}
		}
	}

	void SampleStore::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("StoreMode"))
			StoreMode = jsonObj["StoreMode"].GetInt();
		if (jsonObj.HasMember("FullHandleMode"))
			FullHandleMode = (FullAction)jsonObj["FullHandleMode"].GetInt();
		if (jsonObj.HasMember("FullNoticeVarId") && jsonObj["FullNoticeVarId"].IsObject())
			FullNoticeVarId.Parse(jsonObj["FullNoticeVarId"]);
		if (jsonObj.HasMember("ClearDataVarId") && jsonObj["ClearDataVarId"].IsObject())
			ClearDataVarId.Parse(jsonObj["ClearDataVarId"]);
		if (jsonObj.HasMember("ClearDataWay"))
			ClearDataMode = jsonObj["ClearDataWay"].GetInt();
		/*if (jsonObj.HasMember("MaxSampleNum"))
			MaxSampleNum = jsonObj["MaxSampleNum"].GetInt();*/
		if (jsonObj.HasMember("IsSave"))
			IsSave = jsonObj["IsSave"].GetBool();
		/*if (jsonObj.HasMember("StoreFileInfo") && jsonObj["StoreFileInfo"].IsObject())*/
		StoreFileInfo.Parse(jsonObj);
	}
	void SampleInfoRes::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("SimpleGroupName"))
			SimpleGroupName = jsonObj["SimpleGroupName"].GetInt();
		if (jsonObj.HasMember("SimpleGroupNo"))
			SimpleGroupNo = jsonObj["SimpleGroupNo"].GetInt();
		if (jsonObj.HasMember("SimpleDescribe") && !jsonObj["SimpleDescribe"].IsNull())
			SimpleDescribe = jsonObj["SimpleDescribe"].GetString();
		if (jsonObj.HasMember("SimpleCtrlVarId") && jsonObj["SimpleCtrlVarId"].IsObject())
			SimpleCtrlVarId.Parse(jsonObj["SimpleCtrlVarId"]);
		if (jsonObj.HasMember("SimpleStyle"))
			SimpleStyle = jsonObj["SimpleStyle"].GetInt();
		if (jsonObj.HasMember("CollectMode"))
			CollectMode  = (SampleMode)jsonObj["CollectMode"].GetInt();

		if (jsonObj.HasMember("SimpleCycleVarId") && jsonObj["SimpleCycleVarId"].IsObject())
			SimpleCycleVarId.Parse(jsonObj["SimpleCycleVarId"]);
		if (jsonObj.HasMember("SimpleCycle"))
			SimpleCycle = jsonObj["SimpleCycle"].GetInt();
		if (jsonObj.HasMember("TriggerBitMode"))
			TriggerBitMode = jsonObj["TriggerBitMode"].GetInt();
		if (jsonObj.HasMember("TrrigerReadAddr") && jsonObj["TrrigerReadAddr"].IsObject())
			TriggerReadAddr.Parse(jsonObj["TrrigerReadAddr"]);
		if (jsonObj.HasMember("TrrigerSimpleMode"))
		{
			if(TriggerBitMode)
				BtyeCondition = (Project::Conditions)jsonObj["TrrigerSimpleMode"].GetInt();
			else
				BitCondition = (Project::BitConditions)jsonObj["TrrigerSimpleMode"].GetInt();
		}
		if (jsonObj.HasMember("TrigerValue"))
			ComPareValue = jsonObj["TrigerValue"].GetInt();
		if (jsonObj.HasMember("TrigerValueVarId") && jsonObj["TrigerValueVarId"].IsObject())
			ComPareVar.Parse(jsonObj["TrigerValueVarId"]);
		if (jsonObj.HasMember("CollectTimeFrom"))
			CollectTimeFrom = jsonObj["CollectTimeFrom"].GetInt64();
		if (jsonObj.HasMember("CollectTimeTo"))
			CollectTimeTo = jsonObj["CollectTimeTo"].GetInt64();
		if (jsonObj.HasMember("SimpleFrequency"))
			SimpleFrequency = jsonObj["SimpleFrequency"].GetInt();
		if (jsonObj.HasMember("IsContinueAddr"))
			IsContinueAddr = jsonObj["IsContinueAddr"].GetBool();
		if (jsonObj.HasMember("ChannelSetLst"))
			SampleChannel::Parse(SimpleChannelLst, jsonObj["ChannelSetLst"],SimpleGroupName,SimpleGroupNo);
		//if (jsonObj.HasMember("SampleStoreInfo"))
		SampleStoreInfo.Parse(jsonObj);
		SampleStoreInfo.MaxSampleNum = SimpleChannelLst.size()*MAXCHANNELNUM;
	}



	void SampleInfoRes::Parse(std::vector<SampleInfoRes>& vector, rapidjson::Value& jsonObj)
	{
		std::vector<SampleInfoRes>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::SampleInfoRes res;
				res.Parse(jsonObj[i]);
				vector.push_back(res);
			}
		}
	}
#if 0
	/*定时器回调函数*/
	void SampleTimerCb(void *data)
	{
		SampleInfoRes * res = (SampleInfoRes *)data;
		res->SetSampleContent();
		if (res->SimpleCycleVarId != Project::DataVarId::NullId)
			res->SimpleCycle = UI::UIDataService::Ins().GetNumber<int>(res->SimpleCycleVarId);
		Fl::repeat_timeout(res->SimpleCycle / 1000, SampleTimerCb, data);

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
	string SampleInfoRes::GetSampleData(SampleChannel &channelinfo)
	{
		string datastyle = channelinfo.DataStyle;
		string dataformat = channelinfo.DataStyle;
		DataVarId varid = channelinfo.SampleAddr; 

		if (datastyle == "Word")			//16位
		{
			if (dataformat == "BCD")
			{
				short data = UI::UIDataService::Ins().GetNumber<short>(varid);
				char bcd[16];
				DectoBCD<short>(data, bcd, 16);
				return bcd;
			}
			if (dataformat == "Hex")
			{
				short data = UI::UIDataService::Ins().GetNumber<short>(varid);
				char hex[16];
				DectoHex<short>(data, hex, 16);
				return hex;
			}
			if(dataformat == "Signed")
			{
				short data = UI::UIDataService::Ins().GetNumber<short>(varid);
				return to_string(data);
			}
			if (dataformat == "Unsigned")
			{
				unsigned short data = UI::UIDataService::Ins().GetNumber<unsigned short>(varid);
				return to_string(data);
			}
		
		}
		if (datastyle == "DWord")
		{
			if (dataformat == "BCD")
			{
				int data = UI::UIDataService::Ins().GetNumber<int>(varid);
				char bcd[32];
				DectoBCD<int>(data, bcd, 32);
				return bcd;
			}
			if (dataformat == "Hex")
			{
				int data = UI::UIDataService::Ins().GetNumber<int>(varid);
				char hex[32];
				DectoHex<int>(data, hex, 32);
				return hex;
			}
			if (dataformat == "Signed")
			{
				int data = UI::UIDataService::Ins().GetNumber<int>(varid);
				return to_string(data);
			}
			if (dataformat == "Unsigned")
			{
				unsigned int data = UI::UIDataService::Ins().GetNumber<unsigned int>(varid);
				return to_string(data);
			}
			if (dataformat == "Float")
			{
				float data = UI::UIDataService::Ins().GetNumber<float>(varid);
				return to_string(data);
			}

		}
		if (datastyle == "DDWord")
		{
			if (dataformat == "BCD")
			{
				long long data = UI::UIDataService::Ins().GetNumber<long long>(varid);
				char bcd[64];
				DectoBCD<long long>(data, bcd, 64);
				return bcd;
			}
			if (dataformat == "Hex")
			{
				long long data = UI::UIDataService::Ins().GetNumber<long long>(varid);
				char hex[64];
				DectoHex<long long>(data, hex, 64);
				return hex;
			}
			if (dataformat == "Signed")
			{
				long long data = UI::UIDataService::Ins().GetNumber<long long>(varid);
				return to_string(data);
			}
			if (dataformat == "Unsigned")
			{
				unsigned long long data = UI::UIDataService::Ins().GetNumber<unsigned long long>(varid);
				return to_string(data);
			}
			if (dataformat == "Float")
			{
				double data = UI::UIDataService::Ins().GetNumber<double>(varid);
				return to_string(data);
			}

		}
		if (datastyle == "String")
		{
			char data[256];
			UI::UIDataService::Ins().GetDataString(varid, data, channelinfo.WordNum);
		}


	}
	bool SampleInfoRes::SampleIsFull()
	{
		for (unsigned i = 0; i < SimpleChannelLst.size(); i++)
		{
			if(SimpleChannelLst[i].ChannelContent.size() > SampleStoreInfo.MaxSampleNum)
				return true;
			else
				return IsFull;
		}
		
	}
	bool SampleInfoRes::MeetFixedSampleTime()
	{
		time_t now = time(NULL);
		if (now >= CollectTimeFrom && now <= CollectTimeTo)
			return true;
		else
			return false;
	}
	void SampleInfoRes::SetSampleContent()
	{
		//发送消息通知趋势图重绘

		if (SampleIsFull())
		{
			switch (SampleStoreInfo.FullHandleMode)
			{
			case CLEAR:
				ClearSampleData();
				IsFull = false;
				break;
			case STOP:
				return EndSample();
			case COVER:
			{
				static int pos;
				for (unsigned i = 0; i < SimpleChannelLst.size(); i++)
				{
					if (pos < SimpleChannelLst[i].ChannelContent.size())	//覆盖
					{
						SimpleChannelLst[i].ChannelContent[pos].Data = GetSampleData(SimpleChannelLst[i]);
						SimpleChannelLst[i].ChannelContent[pos].Data = UI::UIDataService::Ins().GetNumber<int>(SimpleChannelLst[i].SampleAddr);
						SimpleChannelLst[i].ChannelContent[pos].ChannelNo = SimpleChannelLst[i].Channel;
						SimpleChannelLst[i].ChannelContent[pos].SampleTime = time(NULL);
						SimpleChannelLst[i].ChannelContent[pos].SimpleId = SimpleId;
						pos++;
					}
					else
						pos = 0;
				}
				break;
			}
			default:
				break;
			}
		}
		else
		{
			for (unsigned i = 0; i < SimpleChannelLst.size(); i++)
			{
				SampleChannel::SampleContent newContent;
				newContent.Data = GetSampleData(SimpleChannelLst[i]);
				newContent.ChannelNo = SimpleChannelLst[i].Channel;
				newContent.SampleTime = time(NULL);
				newContent.SimpleId = SimpleId;
				SimpleChannelLst[i].ChannelContent.push_back(newContent);
			}
		}
	}

	void SampleInfoRes::ClearSampleData()
	{
		for (unsigned i = 0; i < SimpleChannelLst.size(); i++)
			SimpleChannelLst[i].ClearChannelContent();		//清除每个通道保存的内容
		if (SampleStoreInfo.FullHandleMode == STOP && IsFull)
		{
			IsFull = false;
			StartSample();
		}
	}
	
	void SampleInfoRes::StartSample()
	{
		int sampleCtrlValue;
		if (SimpleCtrlVarId != DataVarId::NullId)	//是否使用采样开始控制
			sampleCtrlValue = UI::UIDataService::Ins().GetNumber<int>(SimpleCtrlVarId);
		else
			sampleCtrlValue = SimpleStyle;

		if (sampleCtrlValue == SimpleStyle && startflag_ == 0)			//开始采样
		{
			startflag_ = 1;
			switch (CollectMode)					//判断采样模式
			{
			case Project::Cycle:
			{
				if (SimpleCycleVarId != Project::DataVarId::NullId)
					SimpleCycle = UI::UIDataService::Ins().GetNumber<int>(SimpleCycleVarId);
				Fl::add_timeout(SimpleCycle / 1000, SampleTimerCb, (void*)this);
				break;
			}
			case Project::Fixed:
			{
				//设置定时器
				if (MeetFixedSampleTime())
				{
					if (SimpleCycleVarId != Project::DataVarId::NullId)
						SimpleCycle = UI::UIDataService::Ins().GetNumber<int>(SimpleCycleVarId);
					Fl::add_timeout(SimpleCycle / 1000, SampleTimerCb, (void*)this);

				}
				break;
			}
			case Project::Trigger:
			{
				//采样
				SetSampleContent();
				break;
			}
			default:
				break;
			}
		}
	}
	void SampleInfoRes::EndSample()
	{
		startflag_ = 0;
		switch (CollectMode)					//判断采样模式
		{
		case Project::Cycle:
		case Project::Fixed:
			Fl::remove_timeout(SampleTimerCb, (void*)this);
			break;
		case Project::Trigger:
			break;
		default:
		break;
		}
		
	}
#endif

}