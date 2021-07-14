/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SampleGCtrl.cpp
 * Author   : TangYao
 * Date     : 2021/01/13
 * Descript : 数据采样控制
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "SampleGCtrl.h"
#include "SampleStorage.h"
#include "HMIWindow.h"
#include "UIData.h"
#include "System.h"
#include "SampleRecord.h"
#include "FileSave.h"
namespace UI
{
	/*void SampleSaveFunc(void *data)
	{
		if (Storage::FileSave::GetFileSaveTool()->ReadySaveSample())
		{
			Storage::FileSave::GetFileSaveTool()->SaveSample(*(SampleInfoRes*)data);
		}
	}*/

	SampleGCtrl *SampleGCtrl::ctrl_ = nullptr;
	SampleGCtrl* SampleGCtrl::Ins()
	{
		return ctrl_;
	}
	SampleGCtrl::SampleGCtrl(HMIPage * pg) : BaseGControl(pg)
	{
		model_ = shared_ptr<SampleGModel>(new SampleGModel());
		InitMVCModel(model_);
		ctrl_ = this;
		Storage::FileSave::GetFileSaveTool();
		/*for (size_t i = 0; i < model_->SampleGUnit.InfoLst.size(); ++i)
		{
			if (model_->SampleGUnit.InfoLst[i].SampleStoreInfo.IsSave)
			{
				Storage::FileSave::GetFileSaveTool()->LoadCSVData(Storage::FileType_Sample, &model_->SampleGUnit.InfoLst[i].SampleStoreInfo.StoreFileInfo);
			}
			
		}*/
		
	}

	SampleGCtrl::~SampleGCtrl()
	{
		if (Page() == nullptr)
		{
			return;
		}
		// 删除周期采集定时器
		for (size_t i = 0; i < model_->SampleGUnit.InfoLst.size(); ++i) {
			Project::SampleInfoRes sampleGroup = model_->SampleGUnit.InfoLst[i];
			if (sampleGroup.CollectMode == Project::SampleMode::Cycle) {
				Page()->RemoveTimeout(SampleValue_CB, (void *)&model_->SampleGUnit.InfoLst[i]);
			}
		}
		// 删除定时采集定时器
		for (size_t i = 0; i < FixedSampleGroups.size(); ++i) {
			if (FixedSampleTimer[i]) {
				Page()->RemoveTimeout(SampleValue_CB, (void*)&FixedSampleGroups[i]);
			}
		}
		//Page()->RemoveTimeout(AddFixedTimer_CB, this);
		// 删除数据库备份寄存器
		//Page()->RemoveTimeout(SampleFlush_CB, NULL);
	}

	void SampleGCtrl::CreateView() {

	}

	int SampleGCtrl::PeekHMIMessage(Message::Msg* msg)
	{
		return 1;
	}

	void SampleGCtrl::HandleDataNotify(Project::DataVarId vid)
	{
		// 获取当前时间
		DDWORD date = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		for (size_t i = 0; i < model_->SampleGUnit.InfoLst.size(); i++) {
			Project::SampleInfoRes& sampleGroup = model_->SampleGUnit.InfoLst[i];

			if (NULL_VID_VALUE!=sampleGroup.SimpleCtrlVarId.Vid)
			{
				//判定触发开关
				bool bTrigClt = (bool)sampleGroup.SimpleStyle == DataApi::AppBit(sampleGroup.SimpleCtrlVarId);

				if (vid.Cmp(sampleGroup.SimpleCtrlVarId))
				{
					if (sampleGroup.CollectMode == Project::SampleMode::Cycle) {		// 周期采样
						DDWORD ms = sampleGroup.SimpleCycle;
						if (sampleGroup.SimpleCycleVarId != Project::DataVarId::NullId) {
							ms = UIData::Number<DDWORD>(sampleGroup.SimpleCycleVarId);
						}
						//不满足则关闭周期采样
						if (!bTrigClt)
							Page()->RemoveTimeout(SampleValue_CB, (void *)&model_->SampleGUnit.InfoLst[i]);
						else
							Page()->AddTimeout(ms, SampleValue_CB, (void *)&model_->SampleGUnit.InfoLst[i], true);
						continue;
					}
					else
					{
						if (!bTrigClt)
							continue;
					}
				}
			}
			if (NULL_VID_VALUE != sampleGroup.SampleStoreInfo.ClearDataVarId.Vid&&vid.Cmp(sampleGroup.SampleStoreInfo.ClearDataVarId))
			{
				BOOL bitVal = DataApi::AppBit(sampleGroup.SampleStoreInfo.ClearDataVarId);
				if (sampleGroup.SampleStoreInfo.TempClearAddrStatus == -1)
					sampleGroup.SampleStoreInfo.TempClearAddrStatus = bitVal;
				switch (sampleGroup.SampleStoreInfo.ClearDataMode)
				{
				case 0:
					if (bitVal)
					{
						Storage::SampleStorage::Ins()->CleanByGroup(sampleGroup.SimpleGroupName, sampleGroup.SimpleGroupNo);
					}
					break;
				case 1:
					if (!bitVal)
					{
						Storage::SampleStorage::Ins()->CleanByGroup(sampleGroup.SimpleGroupName, sampleGroup.SimpleGroupNo);
					}
					break;
				case 2:
					if (sampleGroup.SampleStoreInfo.TempClearAddrStatus && !bitVal)
					{
						Storage::SampleStorage::Ins()->CleanByGroup(sampleGroup.SimpleGroupName, sampleGroup.SimpleGroupNo);
					}
					sampleGroup.SampleStoreInfo.TempClearAddrStatus = bitVal;
					break;
				case 3:
					if (!sampleGroup.SampleStoreInfo.TempClearAddrStatus && bitVal)
					{
						Storage::SampleStorage::Ins()->CleanByGroup(sampleGroup.SimpleGroupName, sampleGroup.SimpleGroupNo);
					}
					sampleGroup.SampleStoreInfo.TempClearAddrStatus = bitVal;
					break;
				}
			}
			else
			{
				switch (sampleGroup.CollectMode)
				{
				case Project::SampleMode::Trigger:	
					//if(sampleGroup.TriggerReadAddr)
					// 触发采样
					if (sampleGroup.TriggerBitMode == 0) {	// 位寄存器触发
						BOOL bitVal = UIData::Bit(sampleGroup.TriggerReadAddr);

						if (model_->SampleGUnit.InfoLst[i].TempByteState == -1)
							model_->SampleGUnit.InfoLst[i].TempByteState = bitVal;
						switch (sampleGroup.BitCondition+2)
						{
						/*case Project::BitConditions::ON:
							if (bitVal) {
								if (!sampleGroup.SampleStoreInfo.StopFlag)
								{
									SampleValue(sampleGroup.SimpleChannelLst, date, sampleGroup.SampleStoreInfo.MaxSampleNum);
								}
								updateFlag_ = true;
							}
							break;
						case Project::BitConditions::OFF:
							if (!bitVal) {
								if (!sampleGroup.SampleStoreInfo.StopFlag)
								{
									SampleValue(sampleGroup.SimpleChannelLst, date, sampleGroup.SampleStoreInfo.MaxSampleNum);
								}
								updateFlag_ = true;
							}
							break;*/
						case Project::BitConditions::FALL:
						//case Project::BitConditions::ON:
							if (model_->SampleGUnit.InfoLst[i].TempByteState && !bitVal) {
								if (!sampleGroup.SampleStoreInfo.StopFlag)
								{
									SampleValue(sampleGroup.SimpleChannelLst, date, sampleGroup.SampleStoreInfo.MaxSampleNum);
								}
								updateFlag_ = true;
							}
							break;
						case Project::BitConditions::RISE:
						//case Project::BitConditions::OFF:
							if (!model_->SampleGUnit.InfoLst[i].TempByteState && bitVal) {
								if (!sampleGroup.SampleStoreInfo.StopFlag)
								{
									SampleValue(sampleGroup.SimpleChannelLst, date, sampleGroup.SampleStoreInfo.MaxSampleNum);
								}
								updateFlag_ = true;
							}
							break;
						default:
							break;
						}
						model_->SampleGUnit.InfoLst[i].TempByteState = bitVal;
					}
					else if (sampleGroup.TriggerBitMode == 1) {	// 字寄存器触发
						DOUBLE cmpVal = sampleGroup.ComPareValue;
						if (sampleGroup.ComPareVar != Project::DataVarId::NullId) {
							cmpVal = UIData::Number<DOUBLE>(sampleGroup.ComPareVar);
						}
						DOUBLE readVal = UIData::Number<DOUBLE>(sampleGroup.TriggerReadAddr);
						switch (sampleGroup.BtyeCondition)
						{
						case Project::Conditions::LT:
							if (readVal < cmpVal) {
								//if (JudgeCondition::MeetCondition(Project::Conditions::LT, readVal, cmpVal)) {
								if (!sampleGroup.SampleStoreInfo.StopFlag)
								{
									SampleValue(sampleGroup.SimpleChannelLst, date, sampleGroup.SampleStoreInfo.MaxSampleNum);
								}
								updateFlag_ = true;
							}
							break;
						case Project::Conditions::GT:
							if (readVal > cmpVal) {
								if (!sampleGroup.SampleStoreInfo.StopFlag)
								{
									SampleValue(sampleGroup.SimpleChannelLst, date, sampleGroup.SampleStoreInfo.MaxSampleNum);
								}
								updateFlag_ = true;
							}
							break;
						case Project::Conditions::LE:
							if (readVal <= cmpVal) {
								if (!sampleGroup.SampleStoreInfo.StopFlag)
								{
									SampleValue(sampleGroup.SimpleChannelLst, date, sampleGroup.SampleStoreInfo.MaxSampleNum);
								}
								updateFlag_ = true;
							}
							break;
						case Project::Conditions::GE:
							if (readVal >= cmpVal) {
								if (!sampleGroup.SampleStoreInfo.StopFlag)
								{
									SampleValue(sampleGroup.SimpleChannelLst, date, sampleGroup.SampleStoreInfo.MaxSampleNum);
								}
								updateFlag_ = true;
							}
							break;
						case Project::Conditions::EQ:
							if (readVal == cmpVal) {
								if (!sampleGroup.SampleStoreInfo.StopFlag)
								{
									SampleValue(sampleGroup.SimpleChannelLst, date, sampleGroup.SampleStoreInfo.MaxSampleNum);
								}
								updateFlag_ = true;
							}
							break;
						case Project::Conditions::NE:
							if (readVal == cmpVal) {
								if (!sampleGroup.SampleStoreInfo.StopFlag)
								{
									SampleValue(sampleGroup.SimpleChannelLst, date, sampleGroup.SampleStoreInfo.MaxSampleNum);
								}
								updateFlag_ = true;
							}
						default:
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			
			if (updateFlag_) {
				DealStroeCount(model_->SampleGUnit.InfoLst[i]);
				//Win()->AddTimeout(0, SampleSaveFunc, &model_->SampleGUnit.InfoLst[i]);
				Storage::FileSave::GetFileSaveTool()->SaveSample(&model_->SampleGUnit.InfoLst[i]);
				//Storage::FileSave::GetFileSaveTool()->InsertSampleResInQueue(&model_->SampleGUnit.InfoLst[i]);
				//MayToFile(model_->SampleGUnit.InfoLst[i]);
				
				model_->SampleGUnit.InfoLst[i].ChangeFlag ^= true;
				// 采集新数据，发送更新通知
				LocalData::SetNumberData(SYS_PSB_DATASAMPLE_NOTICE, (int)model_->SampleGUnit.InfoLst[i].ChangeFlag);
				updateFlag_ = false;
				LocalData::SetNumberData(SYS_PSB_DATASAMPLE_NOTICE, false);
			}
			//break;
		}
	}

	void SampleGCtrl::SampleValue(vector<Project::SampleChannel> & channels, DDWORD date,int recordFlag) {
		for (size_t i = 0; i < channels.size(); ++i) {
			Storage::SampleStorage::Ins()->Trigger(channels[i].Channel, UIData::Number<DDWORD>(channels[i].SampleAddr), UIDataService::Ins().GetDataType(channels[i].SampleAddr), date,recordFlag);
		}
	}

	void SampleGCtrl::DealStroeCount(SampleInfoRes & spIfRs)
	{
		if (Storage::SampleStorage::Ins()->GetAllCountByNo(spIfRs.SimpleGroupName, spIfRs.SimpleGroupNo,0) >= spIfRs.SampleStoreInfo.MaxSampleNum)
		{
			if (NULL_VID_VALUE!=spIfRs.SampleStoreInfo.FullNoticeVarId.Vid)
			{
				DataApi::AppBit(spIfRs.SampleStoreInfo.FullNoticeVarId, true);
			}
			if (spIfRs.SampleStoreInfo.StoreMode==2)
			{
				switch (spIfRs.SampleStoreInfo.FullHandleMode)
				{
				case FullAction::CLEAR:
				case FullAction::COVER://当前上位机没有设置覆盖
					//清除数据
					Storage::SampleStorage::Ins()->CleanByGroup(spIfRs.SimpleGroupName, spIfRs.SimpleGroupNo);
					break;
				case FullAction::STOP:
					spIfRs.SampleStoreInfo.StopFlag = 1;
					break;
				}
			}
		}
		else
		{
			spIfRs.SampleStoreInfo.StopFlag = 0;
		}
	}


	//int SampleGCtrl::CopyIntegerToChar(char * ch, int value)
	//{
	//	char digit[20];
	//	sprintf(digit, "%d", value);
	//	memcpy(ch, digit, strlen(digit));
	//	return strlen(digit);
	//}

	//int SampleGCtrl::CopyDataTypeStrToChar(char* dst,const Project::BaseVar* var)
	//{
	//	if (!var)
	//		return 0;
	//	int pos=0;
	//	switch (var->RegType)
	//	{
	//	case Project::LocalRegType::TP_PSB:
	//		memcpy(dst, "PSB", 3);
	//		pos += 3;
	//		break;
	//	case Project::LocalRegType::TP_SPSB:
	//		memcpy(dst, "SPSB", 4);
	//		pos += 4;
	//		break;
	//	case Project::LocalRegType::TP_PSW:
	//		memcpy(dst, "PSW", 3);
	//		pos += 3;
	//		break;
	//	case Project::LocalRegType::TP_SPSW:
	//		memcpy(dst, "SPSW", 4);
	//		pos += 4;
	//		break;
	//	case Project::LocalRegType::TP_PFW:
	//		memcpy(dst, "PFW", 3);
	//		pos += 3;
	//		break;
	//	case Project::LocalRegType::TP_SPFW:
	//		memcpy(dst, "SPFW", 4);
	//		pos += 4;
	//		break;
	//	default:
	//		break;
	//	}
	//	pos += CopyIntegerToChar(dst + pos, var->Addr);
	//	return pos;
	//}

	//string SampleGCtrl::GetSavePath(int pathMode, Project::DataVarId& addrPath, int nameMode, string fileName, Project::DataVarId& addrName)
	//{
	//	std::string filePath = "";
	//	switch (pathMode)
	//	{
	//	case 1://Hmi
	//		filePath.append("HMI//");
	//		break;
	//	case 2://SD
	//		filePath.append("SDCARD//");
	//		break;
	//	case 3://USB
	//		break;
	//		filePath.append("USB//");
	//	case 4:
	//		if (NULL_VID_VALUE != addrPath.Vid)
	//		{
	//			filePath.append(DataApi::AppString(addrPath));
	//			filePath.append("//");
	//		}
	//		break;
	//	}
	//	switch (nameMode)
	//	{
	//	case 1://固定文件名
	//		filePath.append(fileName);
	//		break;
	//	case 2://日期指定
	//		filePath.append(System::GetCurrentDateToString());
	//		break;
	//	case 3://动态文件名
	//		if (NULL_VID_VALUE != addrName.Vid)
	//		{
	//			filePath.append(DataApi::AppString(addrName));
	//			filePath.append("//");
	//		}
	//		break;
	//	}
	//	filePath.append(".csv");
	//	return filePath;
	//}

	void SampleValue_CB(void * param) {
		Project::SampleInfoRes * sampleGroup = (Project::SampleInfoRes *)param;
		if (sampleGroup->SampleStoreInfo.StopFlag)
		{
			return;
		}
		DDWORD date = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		BOOL updateFlag = false;
		for (size_t i = 0; i < sampleGroup->SimpleChannelLst.size(); ++i) {
			Project::SampleChannel channel = sampleGroup->SimpleChannelLst[i];
			double a = UIData::Number<double>(channel.SampleAddr);
			if(sampleGroup->SampleStoreInfo.StoreMode==2)
				Storage::SampleStorage::Ins()->Trigger(channel.Channel, UIDataService::Ins().GetNumber(channel.SampleAddr),
				UIDataService::Ins().GetDataType(channel.SampleAddr), 
					date);
			else
			{
				Storage::SampleStorage::Ins()->Trigger(channel.Channel, UIDataService::Ins().GetNumber(channel.SampleAddr),
					UIDataService::Ins().GetDataType(channel.SampleAddr),
					date, sampleGroup->SampleStoreInfo.MaxSampleNum);
			}
			updateFlag = true;
		}
		if (updateFlag) {
			//DealStroeCount
			if (Storage::SampleStorage::Ins()->GetAllCountByNo(sampleGroup->SimpleGroupName, sampleGroup->SimpleGroupNo,0) >= sampleGroup->SampleStoreInfo.MaxSampleNum)
			{
				if (NULL_VID_VALUE != sampleGroup->SampleStoreInfo.FullNoticeVarId.Vid)
				{
					DataApi::AppBit(sampleGroup->SampleStoreInfo.FullNoticeVarId, true);
				}
				if (sampleGroup->SampleStoreInfo.StoreMode==2)
				{
					switch (sampleGroup->SampleStoreInfo.FullHandleMode)
					{
					case FullAction::CLEAR:
					case FullAction::COVER://当前上位机没有设置覆盖
						//清除数据
						Storage::SampleStorage::Ins()->CleanByGroup(sampleGroup->SimpleGroupName, sampleGroup->SimpleGroupNo);
						break;
					case FullAction::STOP:
						sampleGroup->SampleStoreInfo.StopFlag = 1;
						break;
					}
				}
			}
			else
			{
				sampleGroup->SampleStoreInfo.StopFlag = 0;
			}
			if(SampleGCtrl::Ins())

				Storage::FileSave::GetFileSaveTool()->SaveSample(sampleGroup);
				//SampleGCtrl::Ins()->Win()->AddTimeout(0, SampleSaveFunc, sampleGroup);
			//Storage::FileSave::GetFileSaveTool()->InsertSampleResInQueue(sampleGroup);
			// 采集新数据，发送更新通知
			sampleGroup->ChangeFlag ^= true;
			LocalData::SetNumberData(SYS_PSB_DATASAMPLE_NOTICE, (bool)sampleGroup->ChangeFlag);
		}
	}

	void AddFixedTimer_CB(void * param) {
		SampleGCtrl * ctrl = (SampleGCtrl *)param;
		// 获取当前时间
		DDWORD date = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
		// 添加采集定时器
		for (size_t i = 0; i < ctrl->FixedSampleGroups.size(); ++i) {
			Project::SampleInfoRes sampleGroup = ctrl->FixedSampleGroups[i];
			bool bTrigClt = true;
			if (sampleGroup.SimpleCtrlVarId.Vid != NULL_VID_VALUE)
			{
				//判定触发开关
				bTrigClt = (bool)sampleGroup.SimpleStyle == DataApi::AppBit(sampleGroup.SimpleCtrlVarId);
			}
			if (date >= sampleGroup.CollectTimeFrom && date < sampleGroup.CollectTimeTo&&!ctrl->FixedSampleTimer[i]) {
				if (bTrigClt)
				{
					// 添加采集数据定时器
					ctrl->Page()->AddTimeout(sampleGroup.SimpleFrequency, SampleValue_CB, (void *)&ctrl->FixedSampleGroups[i], true);
					ctrl->FixedSampleTimer[i] = true;
				}
				else
				{
					ctrl->Page()->RemoveTimeout( SampleValue_CB, (void *)&ctrl->FixedSampleGroups[i]);
					ctrl->FixedSampleTimer[i] = false;
				}
			}
			if (date > sampleGroup.CollectTimeTo && ctrl->FixedSampleTimer[i]) {
				// 删除采集数据定时器
				ctrl->Page()->RemoveTimeout(SampleValue_CB, (void *)&ctrl->FixedSampleGroups[i]);
				ctrl->FixedSampleTimer[i] = false;
			}
		}
	}

	void SampleFlush_CB(void * param) {
		Storage::SampleStorage::Ins()->Flush();
	}

	void SampleGCtrl::OnReady() {
		for (size_t i = 0; i < model_->SampleGUnit.InfoLst.size(); ++i) {
			Project::SampleInfoRes sampleGroup = model_->SampleGUnit.InfoLst[i];
			if (sampleGroup.CollectMode == Project::SampleMode::Cycle) {		// 周期采样

				if (sampleGroup.SimpleCtrlVarId.Vid != NULL_VID_VALUE)
				{
					/*if ((bool)sampleGroup.SimpleStyle != DataApi::AppBit(sampleGroup.SimpleCtrlVarId))*/
						continue;
					
				}
				else
				{
					DDWORD ms = sampleGroup.SimpleCycle;
					if (sampleGroup.SimpleCycleVarId != Project::DataVarId::NullId) {
						ms = UIData::Number<DDWORD>(sampleGroup.SimpleCycleVarId);
					}
					Page()->AddTimeout(ms, SampleValue_CB, (void *)&model_->SampleGUnit.InfoLst[i], true);
				}
				
			} else if (sampleGroup.CollectMode == Project::SampleMode::Fixed) {	// 固定采样
				// 添加固定采集数据组
				FixedSampleGroups.push_back(sampleGroup);
				FixedSampleTimer.push_back(false);
			}
		}
		Page()->AddTimeout(500, AddFixedTimer_CB, this, true);
		// 添加定时同步文件数据库定时器
		//Page()->AddTimeout(10000, SampleFlush_CB, NULL, true);
	}
}

