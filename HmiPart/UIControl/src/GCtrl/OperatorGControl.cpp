/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : OperatorGControl.cpp
 * Author   : qiaodan
 * Date     : 2021-05-12
 * Descript : ������¼¼��
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
namespace UI
{
	OperatorGControl *OperatorGControl::ctrl_ = nullptr;

	OperatorGControl::OperatorGControl(HMIPage * page) : BaseGControl(page) {
		model_ = shared_ptr<OperatorGModel>(new OperatorGModel());
		InitMVCModel(model_);
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
		if (model_->OperatorGConfig.Enable)			//���ò�����¼¼��ʹ��
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
		if (model_->OperatorGConfig.CmdIdRef.Cmp(id))		//����Ĵ���
		{
			int cmd = UI::UIData::Number<int>(model_->OperatorGConfig.CmdIdRef);
			if (cmd == 1)
				DeleteAllRecord();
			else if(cmd == 6)
				startrecord_ = true;
			else if(cmd == 7)
				startrecord_ = false;
		}
		//if (model_->OperatorGConfig.ControlIdRef.Cmp(id))	//���ƼĴ���
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
				/*��ӡ*/
			}
			preprintState_ = state;
		}
	}

	void OperatorGControl::DeleteAllRecord()
	{
		int ret = Storage::OperatorStorage::Ins()->CleanRcd();

		/*֪ͨ������¼�и���*/
		if (ret == SQLITE_OK)		//ɾ���ɹ�
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

		/*֪ͨ������¼�и���*/
		if (ret == SQLITE_OK)		//����ɹ�
		{
			LOG_INFO_("Add Operator Record\n");
			bool noticebit = LocalData::GetBit(SYS_PSB_OperatorRECORD_NOTICE);
			noticebit = !noticebit;
			LocalData::SetBit(SYS_PSB_OperatorRECORD_NOTICE, noticebit);
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

		/*֪ͨ������¼�и���*/
		if (ret == SQLITE_OK)		//����ɹ�
		{
			LOG_INFO_("Add Operator Record --- Set String\n");
			bool noticebit = LocalData::GetBit(SYS_PSB_OperatorRECORD_NOTICE);
			noticebit = !noticebit;
			LocalData::SetBit(SYS_PSB_OperatorRECORD_NOTICE, noticebit);
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
		/*֪ͨ������¼�и���*/
		if (ret == SQLITE_OK)		//����ɹ�
		{
			LOG_INFO_("Add Operator Record --- Switch win\n");
			bool noticebit = LocalData::GetBit(SYS_PSB_OperatorRECORD_NOTICE);
			noticebit = !noticebit;
			LocalData::SetBit(SYS_PSB_OperatorRECORD_NOTICE, noticebit);
		}
		delete[] username;
	}
}

