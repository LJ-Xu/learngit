/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : OperatorGControl.cpp
 * Author   : qiaodan
 * Date     : 2021-05-12
 * Descript : 操作记录录入
 * Version  : 0.1.6
 * modify   :
 *          :
 *******************************************************************************/
#include "OperatorGControl.h"
#include "OperatorStorage.h"
#include "ResourceService.h"
#include "HMIWindow.h"
#include "UIData.h"
#include "UICommand.h"
#include "Logger.h"
#include "FileSave.h"
namespace UI
{

	/*void OperatorSaveFunc(void *data)
	{
		if (Storage::FileSave::GetFileSaveTool()->ReadySaveOperate())
		{
			Storage::FileSave::GetFileSaveTool()->SaveOperate(*(SaveFileRes*)data);
		}
	}*/


	OperatorGControl *OperatorGControl::ctrl_ = nullptr;

	OperatorGControl::OperatorGControl(HMIPage * page) : BaseGControl(page) {
		model_ = shared_ptr<OperatorGModel>(new OperatorGModel());
		InitMVCModel(model_);
		Storage::FileSave::GetFileSaveTool()->InitOperate(&model_->OperatorGConfig.SaveLst);
		ctrl_ = this;
	}
	OperatorGControl *OperatorGControl::Ins()
	{
		return ctrl_;
	}

	OperatorGControl::~OperatorGControl() {

	}

	void OperatorGControl::CreateView() {

	}
	void OperatorGControl::OnReady()
	{
		if (model_->OperatorGConfig.Enable)			//设置操作记录录入使能
		{
			LOG_INFO_("Enable Operator Record\n");
			startrecord_ = true;
		}
		else
			startrecord_ = false;
		//LocalData::SetBit(SYS_PSW_OperatorRECORD_CONTRONL, startrecord_);
		if (model_->OperatorGConfig.PrintSet.ControlAddress != Project::DataVarId::NullId)
			preprintState_ = UI::UIData::Bit(model_->OperatorGConfig.PrintSet.ControlAddress);
	}
	int OperatorGControl::PeekHMIMessage(Message::Msg * msg) {
		return 1;
	}

	void OperatorGControl::HandleDataNotify(Project::DataVarId id) 
	{
		if (model_->OperatorGConfig.CmdIdRef.Cmp(id))		//命令寄存器
		{
			int cmd = UI::UIData::Number<int>(model_->OperatorGConfig.CmdIdRef);
			switch ((Project::OperatorCmd)cmd)
			{
			case Project::Record_Enable:
				startrecord_ = true;
				break;
			case Project::Record_Unable:
				startrecord_ = false;
				break;
			case Project::Record_CopyToUAndClear:
			case Project::Record_CopyToSdAndClear:
			case Project::Record_Clear:
				DeleteAllRecord();
			case Project::Record_CopyToU:
			case Project::Record_CopyToSD:
			{
				model_->OperatorGConfig.SaveLst.SaveCmd = (Project::OperatorCmd)cmd;

				Storage::FileSave::GetFileSaveTool()->DoSave();
				//Win()->AddTimeout(0, OperatorSaveFunc,(void *));
				break;
			}
			default:
				break;
			}
		}
		//if (model_->OperatorGConfig.ControlIdRef.Cmp(id))	//控制寄存器
		//	startrecord_ = UI::UIData::Bit(model_->OperatorGConfig.ControlIdRef);
		if (model_->OperatorGConfig.SaveLst.FileNameDataVar.Cmp(id))
		{
			model_->OperatorGConfig.SaveLst.FileNameMode = (Project::FlieNameType)UI::UIData::Number<int>(model_->OperatorGConfig.SaveLst.FileNameDataVar);
		}
		if (model_->OperatorGConfig.SaveLst.StorePosVarId.Cmp(id))
		{
			model_->OperatorGConfig.SaveLst.FileStoreSite = (Project::FlieLocation)UI::UIData::Number<int>(model_->OperatorGConfig.SaveLst.StorePosVarId);
		}
		if (model_->OperatorGConfig.PrintSet.ControlAddress.Cmp(id))
		{
			bool state = UI::UIData::Bit(model_->OperatorGConfig.PrintSet.ControlAddress);
			if ((state && !preprintState_ && model_->OperatorGConfig.PrintSet.PrintCond == 1)
				|| (!state && preprintState_ && model_->OperatorGConfig.PrintSet.PrintCond == 0))
			{
				/*打印*/
			}
			preprintState_ = state;
		}
	}

	void OperatorGControl::DeleteAllRecord()
	{
		int ret = Storage::OperatorStorage::Ins()->CleanRcd();

		/*通知操作记录有更新*/
		if (ret == SQLITE_OK)		//删除成功
		{
			LOG_INFO_("Clean Operator Record\n");

			bool noticebit = LocalData::GetBit(SYS_PSB_OperatorRECORD_NOTICE);
			noticebit = !noticebit;
			LocalData::SetBit(SYS_PSB_OperatorRECORD_NOTICE, noticebit);
		}
	}

	void OperatorGControl::AddOperatorRecord(size_t win, string ctrlName, int action, Project::DataVarId vid, double orgVal, double newVal)
	{
		if (!startrecord_)
			return;
		int len = LocalData::GetLocalVarLen(SYS_PSW_LoginedName);
		char *username = new char[len + 1];
		memset(username, '\0', len + 1);
		LocalData::GetString(SYS_PSW_LoginedName, username);
		string name = username;
		string devname = "";
		Project::DataVarInfo *var = DataApi::GetDataVarInfo(vid);
		XJDataType tp = UIDataService::Ins().GetDataType(vid);

		if(var)
			devname = UI::UICommand::Ins()->GetDeviceName(var->DevId);

		int ret = Storage::OperatorStorage::Ins()->Trigger(name, devname, win, ctrlName, action, *var, orgVal, newVal, tp);

		/*通知操作记录有更新*/
		if (ret == SQLITE_OK)		//插入成功
		{
			LOG_INFO_("Add Operator Record\n");
			bool noticebit = LocalData::GetBit(SYS_PSB_OperatorRECORD_NOTICE);
			noticebit = !noticebit;
			LocalData::SetBit(SYS_PSB_OperatorRECORD_NOTICE, noticebit);
			/*保存*/
			model_->OperatorGConfig.SaveLst.SaveCmd = Project::OperatorCmd::Record_Save;
			Storage::FileSave::GetFileSaveTool()->DoSave();
			//Win()->AddTimeout(0, OperatorSaveFunc, (void *)&model_->OperatorGConfig.SaveLst);
		}
		delete[] username;
	}
	void OperatorGControl::AddOperatorRecord(size_t win, string ctrlName, Project::DataVarId vid, char * orgVal, char * newVal)
	{
		if (!startrecord_)
			return;
		int len = LocalData::GetLocalVarLen(SYS_PSW_LoginedName);
		char *username = new char[len + 1];
		memset(username, '\0', len + 1);
		LocalData::GetString(SYS_PSW_LoginedName, username);
		string name = username;
		string devname = "";
		Project::BaseVar *var = (DataApi::GetDataVarInfo(vid));
		if (var)
			devname = UI::UICommand::Ins()->GetDeviceName(var->DevId);

		int ret = Storage::OperatorStorage::Ins()->Trigger(name, devname, win, ctrlName, *var, orgVal, newVal);

		/*通知操作记录有更新*/
		if (ret == SQLITE_OK)		//插入成功
		{
			LOG_INFO_("Add Operator Record --- Set String\n");
			bool noticebit = LocalData::GetBit(SYS_PSB_OperatorRECORD_NOTICE);
			noticebit = !noticebit;
			LocalData::SetBit(SYS_PSB_OperatorRECORD_NOTICE, noticebit);
			model_->OperatorGConfig.SaveLst.SaveCmd = Project::OperatorCmd::Record_Save;
			Storage::FileSave::GetFileSaveTool()->DoSave();
			//Win()->AddTimeout(0, OperatorSaveFunc, (void *)&model_->OperatorGConfig.SaveLst);
		}
		delete[] username;
	}

	void OperatorGControl::AddOperatorRecord(size_t win, string ctrlName, Project::DataVarId vid, int action, string recipename, int count)
	{
		if (!startrecord_)
			return;
		int len = LocalData::GetLocalVarLen(SYS_PSW_LoginedName);
		char *username = new char[len + 1];
		memset(username, '\0', len + 1);
		LocalData::GetString(SYS_PSW_LoginedName, username);
		string name = username;
		string devname = "";
		Project::BaseVar *var = (DataApi::GetDataVarInfo(vid));
		if (var)
			devname = UI::UICommand::Ins()->GetDeviceName(var->DevId);
		int ret = Storage::OperatorStorage::Ins()->Trigger(name, devname, win, ctrlName, action, *var, recipename, count);
		/*通知操作记录有更新*/
		if (ret == SQLITE_OK)		//插入成功
		{
			LOG_INFO_("Add Operator Record --- Switch win\n");
			bool noticebit = LocalData::GetBit(SYS_PSB_OperatorRECORD_NOTICE);
			noticebit = !noticebit;
			LocalData::SetBit(SYS_PSB_OperatorRECORD_NOTICE, noticebit);
			/*保存*/
			model_->OperatorGConfig.SaveLst.SaveCmd = Project::OperatorCmd::Record_Save;
			Storage::FileSave::GetFileSaveTool()->DoSave();
			//Win()->AddTimeout(0, OperatorSaveFunc, (void *)&model_->OperatorGConfig.SaveLst);

		}
		delete[] username;
	}

	void OperatorGControl::AddOperatorRecord(size_t win, string ctrlName, int action, vector<string> funcname)
	{
		if (!startrecord_)
			return;
		int len = LocalData::GetLocalVarLen(SYS_PSW_LoginedName);
		char *username = new char[len + 1];
		memset(username, '\0', len + 1);
		LocalData::GetString(SYS_PSW_LoginedName, username);
		string name = username;
		int ret = Storage::OperatorStorage::Ins()->Trigger(name, win, ctrlName, action, funcname);
		/*通知操作记录有更新*/
		if (ret == SQLITE_OK)		//插入成功
		{
			LOG_INFO_("Add Operator Record --- Switch win\n");
			bool noticebit = LocalData::GetBit(SYS_PSB_OperatorRECORD_NOTICE);
			noticebit = !noticebit;
			LocalData::SetBit(SYS_PSB_OperatorRECORD_NOTICE, noticebit);
			/*保存*/
			model_->OperatorGConfig.SaveLst.SaveCmd = Project::OperatorCmd::Record_Save;
			Storage::FileSave::GetFileSaveTool()->DoSave();
			//Win()->AddTimeout(0, OperatorSaveFunc, (void *)&model_->OperatorGConfig.SaveLst);

		}
		delete[] username;
	}

	void OperatorGControl::AddOperatorRecord(size_t win, string ctrlName, int action, int startWin, int endWin)
	{
		if (!startrecord_)
			return;
		int len = LocalData::GetLocalVarLen(SYS_PSW_LoginedName);
		char *username = new char[len + 1];
		memset(username, '\0', len + 1);
		LocalData::GetString(SYS_PSW_LoginedName, username);
		string name = username;

		int ret = Storage::OperatorStorage::Ins()->Trigger(name, win, ctrlName, action, startWin, endWin);
		/*通知操作记录有更新*/
		if (ret == SQLITE_OK)		//插入成功
		{
			LOG_INFO_("Add Operator Record --- Switch win\n");
			bool noticebit = LocalData::GetBit(SYS_PSB_OperatorRECORD_NOTICE);
			noticebit = !noticebit;
			LocalData::SetBit(SYS_PSB_OperatorRECORD_NOTICE, noticebit);
			/*保存*/
			model_->OperatorGConfig.SaveLst.SaveCmd = Project::OperatorCmd::Record_Save;
			Storage::FileSave::GetFileSaveTool()->DoSave();
			//Win()->AddTimeout(0, OperatorSaveFunc, (void *)&model_->OperatorGConfig.SaveLst);
		}
		delete[] username;
	}
}

