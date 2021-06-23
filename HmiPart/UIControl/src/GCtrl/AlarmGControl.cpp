/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmGControl.cpp
 * Author   : TangYao
 * Date     : 2021/01/13
 * Descript : ��������
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "AlarmGControl.h"
#include "AlarmStorage.h"
#include "ResourceService.h"
#include "HMIWindow.h"
#include "UIData.h"
#include "SysCtrlApi.h"
#include <functional>
#include "FileSave.h"
namespace UI
{
	void AlarmBeepFunc(void *data)
	{
		AlarmGControl* ctrl = (AlarmGControl*)data;
		AlarmGView* pView = static_cast<AlarmGView*>(ctrl->GetView());
		if (--(pView->BeepTimeOut)>0)
		{
			SysCtrlApi::Beep(1);
			ctrl->Page()->AddTimeout(1000, AlarmBeepFunc, ctrl, false);
		}
		else
		{
			pView->IsBeeping = false;
		}
	}
	AlarmGControl *AlarmGControl::ctrl_ = nullptr;

	AlarmGControl::AlarmGControl(HMIPage * page) : BaseGControl(page) {
		model_ = shared_ptr<AlarmGModel>(new AlarmGModel());
		InitMVCModel(model_);
		ctrl_ = this;
		Storage::FileSave::GetFileSaveTool();
	}
	AlarmGControl *AlarmGControl::Ins()
	{
		return ctrl_;
	}

	void AnalyseNumberStr(const string & ssrc, string & sdst, int itgDgt, int dmcDgt, bool isPre)
	{
		sdst = "";
		sdst.reserve(itgDgt + dmcDgt + 1);

		//��С��
		if (dmcDgt > 0)
		{
			int ptidx = ssrc.find_first_of('.');
			if (ptidx == -1)
			{
				if (ssrc.size() > itgDgt)
				{
					sdst = "*";
				}
				else
				{
					if (isPre)
					{
						int lstDgt = itgDgt - ssrc.size();
						while (lstDgt--)
						{
							sdst.append("0");
						}
					}
					sdst.append(ssrc).append(".");
					int lstDgt = dmcDgt;
					while (lstDgt--)
					{
						sdst.append("0");
					}
				}

				return;
			}
			//������ЧС��
			else
			{
				if (ptidx > itgDgt)
				{
					sdst = "*";
				}
				else
				{
					//����λǰ��
					if (isPre)
					{
						int lstIntDgt = itgDgt - ptidx;
						while (lstIntDgt--)
						{
							sdst.append("0");
						}
					}
					sdst.append(ssrc.substr(0, ptidx));
					//���С��λ
					int lstDgt = dmcDgt - (ssrc.size() - 1 - ptidx);
					if (lstDgt <= 0)//С��λ������պ�
					{
						sdst.append(ssrc.substr(ptidx, dmcDgt + 1));
					}
					else //if (lstDgt > 0)//����û������С��λ
					{
						sdst.append(ssrc.substr(ptidx));
						while (lstDgt--)
						{
							sdst.append("0");
						}
					}
				}
			}
		}
		//����С��
		else
		{
			if (ssrc.size() > itgDgt)
			{
				sdst = "*";
			}
			else
			{
				if (isPre)
				{
					int lstIntDgt = itgDgt - ssrc.size();
					while (lstIntDgt--)
					{
						sdst.append("0");
					}
				}
				sdst.append(ssrc);
			}
		}
	}

	AlarmGControl::~AlarmGControl(){

	}

	void AlarmGControl::CreateView() {

		AlarmGView* view = new AlarmGView(0,0,0,0);

		InitMVCView(view);
	}

	int AlarmGControl::PeekHMIMessage(Message::Msg * msg) {
		return 1;
	}
	void AlarmGControl::HandleAlarmAction(Project::AlarmInfoRes& alarmres, bool triggeralarm)
	{
#if 0
		AlarmGView* pView = static_cast<AlarmGView*>(pView_);
		pView->Triggeralarm = triggeralarm;
		pView->UseBeep = alarmres.UseBeep;
		pView->UseAlarmPopWin = alarmres.UseAlarmPopWin;
		pView->PopWinNo = alarmres.PopWinNo;
		pView->IsCloseWin = alarmres.IsCloseWin;
		std::thread thr([&]() {
			if (triggeralarm)			//��������
			{
				if (alarmres.UseBeep)
				{
					//����������
				}
				if (alarmres.UseAlarmPopWin)	//�򿪵���
				{
					Win()->OpenDialogPage(alarmres.PopWinNo);
				}
			}
			else				//������ʧ
			{
				if (alarmres.UseAlarmPopWin && alarmres.IsCloseWin)	//�رյ���
					Win()->ClosePage(alarmres.PopWinNo);
			}
		});
		thr.detach();
		//pView_->damage();
		//pView_->redraw();
#else
		//�ļ�����
		
		
		AlarmGView* pView = static_cast<AlarmGView*>(pView_);
		/*pView->Triggeralarm = triggeralarm;
		pView->UseBeep = alarmres.UseBeep;
		pView->UseAlarmPopWin = alarmres.UseAlarmPopWin;
		pView->PopWinNo = alarmres.PopWinNo;
		pView->IsCloseWin = alarmres.IsCloseWin;*/
		if (pView->BeepTimeOut < alarmres.BeepTimeOut)
			pView->BeepTimeOut = alarmres.BeepTimeOut;
		pView->AlarmList.push(std::make_pair(triggeralarm, alarmres));
		Message::InvokeCBFunc func = [&](void*) {
			AlarmGView* pView = static_cast<AlarmGView*>(pView_);
			while (pView->AlarmList.size())
			{
				auto itor = pView->AlarmList.front();
				if (itor.first)			//��������
				{
					if (itor.second.UseBeep&&!pView->IsBeeping)
					{
						pView->IsBeeping = true;
						//����������
						SysCtrlApi::Beep(1);
						Page()->AddTimeout(1000, AlarmBeepFunc, this, false);
					}
					if (itor.second.UseAlarmPopWin)	//�򿪵���
					{
						Win()->OpenDialogPage(itor.second.PopWinNo);
						return;
					}
				}
				else				//������ʧ
				{
					if (itor.second.UseAlarmPopWin && itor.second.IsCloseWin)	//�رյ���
						Win()->ClosePage(itor.second.PopWinNo);
				}
				pView->AlarmList.pop();
			}
			
		};
		Win()->Invoke(func,this, alarmres.PopWinNo);
		
#endif
	}

	std::string GetContextString(const AlarmInfoContext & almctxt)
	{
		std::string strRet;
		for (auto itorStr = almctxt.AlarmContentList.begin(); itorStr != almctxt.AlarmContentList.end(); ++itorStr)
		{
			if (itorStr->second == -2)
			{
				strRet.append(itorStr->first);
			}
			else
			{
				for (int idx = 0; idx != almctxt.MonitorUnits.size(); ++idx)
				{
					if (itorStr->second == idx)
					{
						AlarmMonitorUnit itorMnt = almctxt.MonitorUnits[idx];
						/*if (itorMnt.VariableName.compare("123") == 0)
						{
							int xxxx = 123;
						}*/
						switch (itorMnt.MonitorType)
						{
							//��ص�ַ
						case 1:
						{
							switch (almctxt.MonitorUnits[idx].AddrType)
							{
							case 0://����
							{
								std::string strTemp;
								if (itorMnt.Decimaldigits > 0)
								{
									string strGet = to_string(DataApi::AppNumber<DOUBLE>(itorMnt.MonitorAddrRef));
									AnalyseNumberStr(strGet, strTemp, itorMnt.Intgerdigits, itorMnt.Decimaldigits, itorMnt.IsPreLead);
								}
								else
								{
									string strGet = to_string(DataApi::AppNumber<DWORD>(itorMnt.MonitorAddrRef));
									AnalyseNumberStr(strGet, strTemp, itorMnt.Intgerdigits, 0, itorMnt.IsPreLead);
								}
								strRet.append(strTemp, strTemp.size());
								break;
							}
							case 1://�ַ�
							{
								string strTemp = DataApi::AppString(itorMnt.MonitorAddrRef);
								strRet.append(strTemp,strTemp.size());
								break;
							}
							case 2://����
							{
								string strGet = DataApi::AppString(itorMnt.MonitorAddrRef);
								UI::IResourceService::GB2312toUtf8(strGet);
								strRet.append(strGet, strGet.size());
								break;
							}
							}
							break;
						}
							//��̬���ִ�
						case 0:
						{
							DWORD value = DataApi::AppNumber<DWORD>(itorMnt.MonitorAddrRef);
							for (auto itor = itorMnt.StatusStyle.begin();itor!= itorMnt.StatusStyle.end();++itor)
							{
								if (value == (DWORD)(itor->SrcId))
								{
									strRet.append(itor->Text.Texts[0], itor->Text.Texts[0].size());
									break;
								}
							}
							break;
						}
						}
					}
				}
			}
		}
		return strRet;
	}

	void AlarmGControl::HandleDataNotify(Project::DataVarId id) {

		std::string strContext;
		for (size_t i = 0; i < model_->AlarmGUnit.InfoLst.size(); ++i) {
			if (model_->AlarmGUnit.InfoLst[i].ReadRegVar == id) {
				Project::AlarmInfoRes& alarm = model_->AlarmGUnit.InfoLst[i];
				// �¼�ID
				//int eventId = alarm.GroupName << 16 | alarm.GroupNo;
				if (alarm.RegType == 0) {	// ʹ��λ�Ĵ���
					// ��ȡλ�Ĵ���������
					BOOL bitVal = UIData::Bit(alarm.ReadRegVar);
					if (model_->AlarmGUnit.InfoLst[i].TempByteState == -1)
						model_->AlarmGUnit.InfoLst[i].TempByteState = bitVal;

					switch (alarm.BitCondition)
					{
					case Project::BitConditions::ON:
						if (bitVal) {	// ����������¼
							strContext = GetContextString(alarm.Context);
							AddAlarmRecord(alarm.GroupName, alarm.GroupNo, strContext, ++alarm.Count, alarm.Category);
							HandleAlarmAction(alarm, true);
							/*if (model_->AlarmGUnit.IsSave)
							{	Storage::FileSave::GetFileSaveTool()->InsertAlarmResInQueue(&alarm);
							}*/
						} else {
							Storage::AlarmStorage::Ins()->UnTriggerByEventId(alarm.GroupName, alarm.GroupNo);
							HandleAlarmAction(alarm, false);
						}
						break;
					case Project::BitConditions::OFF:
						if (!bitVal) {	// ����������¼
							strContext = GetContextString(alarm.Context);
							AddAlarmRecord(alarm.GroupName, alarm.GroupNo, strContext, ++alarm.Count, alarm.Category);
							HandleAlarmAction(alarm, true);
							/*if (model_->AlarmGUnit.IsSave)
							{
								Storage::FileSave::GetFileSaveTool()->InsertAlarmResInQueue(&alarm);
							}*/
						} else {
							Storage::AlarmStorage::Ins()->UnTriggerByEventId(alarm.GroupName, alarm.GroupNo);
							HandleAlarmAction(alarm, false);
						}
						break;
					case Project::BitConditions::RISE:
						if (!model_->AlarmGUnit.InfoLst[i].TempByteState && bitVal) {
							strContext = GetContextString(alarm.Context);
							AddAlarmRecord(alarm.GroupName, alarm.GroupNo, strContext, ++alarm.Count, alarm.Category);
							HandleAlarmAction(alarm, true);
							/*if (model_->AlarmGUnit.IsSave)
							{
								Storage::FileSave::GetFileSaveTool()->InsertAlarmResInQueue(&alarm);
							}*/
						} else {
							Storage::AlarmStorage::Ins()->UnTriggerByEventId(alarm.GroupName, alarm.GroupNo);
							HandleAlarmAction(alarm, false);
						}
						break;
					case Project::BitConditions::FALL:
						if (model_->AlarmGUnit.InfoLst[i].TempByteState && !bitVal) {
							strContext = GetContextString(alarm.Context);
							AddAlarmRecord(alarm.GroupName, alarm.GroupNo, strContext, ++alarm.Count, alarm.Category);
							HandleAlarmAction(alarm, true);
							/*if (model_->AlarmGUnit.IsSave)
							{
								Storage::FileSave::GetFileSaveTool()->InsertAlarmResInQueue(&alarm);
							}*/
						} else {
							Storage::AlarmStorage::Ins()->UnTriggerByEventId(alarm.GroupName, alarm.GroupNo);
							HandleAlarmAction(alarm, false);
						}
						break;
					default:
						break;
					}
					// ��ȡ��ǰ�Ĵ���ֵ
					//currentState = bitVal;
					model_->AlarmGUnit.InfoLst[i].TempByteState = bitVal;
				} else if (alarm.RegType == 1) {	// ʹ���ּĴ���
					// ��ȡ�����Ƚ�ֵ
					DOUBLE cmpVal = alarm.ComPareValue;
					if (alarm.ComPareVar != Project::DataVarId::NullId) {
						cmpVal = UIData::Number<DOUBLE>(alarm.ComPareVar);
					}
					// ��ȡ�����Ĵ���ֵ
					DOUBLE readVal = UIData::Number<DOUBLE>(alarm.ReadRegVar);
					switch (alarm.BtyeCondition)
					{
					case Project::Conditions::LT:
						if (readVal < cmpVal) {	// ��������
							strContext = GetContextString(alarm.Context);
							AddAlarmRecord(alarm.GroupName, alarm.GroupNo, strContext, ++alarm.Count, alarm.Category);
							HandleAlarmAction(alarm, true);
							/*if (model_->AlarmGUnit.IsSave)
							{
								Storage::FileSave::GetFileSaveTool()->InsertAlarmResInQueue(&alarm);
							}*/
						} else {	// �������
							Storage::AlarmStorage::Ins()->UnTriggerByEventId(alarm.GroupName, alarm.GroupNo);
							HandleAlarmAction(alarm, false);
						}
						break;
					case Project::Conditions::GT:
						if (readVal > cmpVal) {
							strContext = GetContextString(alarm.Context);
							AddAlarmRecord(alarm.GroupName, alarm.GroupNo, strContext, ++alarm.Count, alarm.Category);
							HandleAlarmAction(alarm, true);
							/*if (model_->AlarmGUnit.IsSave)
							{
								Storage::FileSave::GetFileSaveTool()->InsertAlarmResInQueue(&alarm);
							}*/
						} else {
							Storage::AlarmStorage::Ins()->UnTriggerByEventId(alarm.GroupName, alarm.GroupNo);
							HandleAlarmAction(alarm, false);
						}
						break;
					case Project::Conditions::LE:
						if (readVal <= cmpVal) {
							strContext = GetContextString(alarm.Context);
							AddAlarmRecord(alarm.GroupName, alarm.GroupNo, strContext, ++alarm.Count, alarm.Category);
							HandleAlarmAction(alarm, true);
							/*if (model_->AlarmGUnit.IsSave)
							{
								Storage::FileSave::GetFileSaveTool()->InsertAlarmResInQueue(&alarm);
							}*/
						} else {
							Storage::AlarmStorage::Ins()->UnTriggerByEventId(alarm.GroupName, alarm.GroupNo);
							HandleAlarmAction(alarm, false);
						}
						break;
					case Project::Conditions::GE:
						if (readVal >= cmpVal) {
							strContext = GetContextString(alarm.Context);
							AddAlarmRecord(alarm.GroupName, alarm.GroupNo, strContext, ++alarm.Count, alarm.Category);
							HandleAlarmAction(alarm, true);
							/*if (model_->AlarmGUnit.IsSave)
							{
								Storage::FileSave::GetFileSaveTool()->InsertAlarmResInQueue(&alarm);
							}*/
						} else {
							Storage::AlarmStorage::Ins()->UnTriggerByEventId(alarm.GroupName, alarm.GroupNo);
							HandleAlarmAction(alarm, false);
						}
						break;
					case Project::Conditions::EQ:
						if (readVal == cmpVal) {
							strContext = GetContextString(alarm.Context);
							AddAlarmRecord(alarm.GroupName, alarm.GroupNo, strContext, ++alarm.Count, alarm.Category);
							HandleAlarmAction(alarm, true);
							/*if (model_->AlarmGUnit.IsSave)
							{
								Storage::FileSave::GetFileSaveTool()->InsertAlarmResInQueue(&alarm);
							}*/
						} else {
							Storage::AlarmStorage::Ins()->UnTriggerByEventId(alarm.GroupName, alarm.GroupNo);
							HandleAlarmAction(alarm, false);
						}
						break;
					case Project::Conditions::NE:
						if (readVal != cmpVal) {
							strContext = GetContextString(alarm.Context);
							AddAlarmRecord(alarm.GroupName, alarm.GroupNo, strContext, ++alarm.Count, alarm.Category);
							HandleAlarmAction(alarm, true);
							/*if (model_->AlarmGUnit.IsSave)
							{
								Storage::FileSave::GetFileSaveTool()->InsertAlarmResInQueue(&alarm);
							}*/
						} else {
							Storage::AlarmStorage::Ins()->UnTriggerByEventId(alarm.GroupName, alarm.GroupNo);
							HandleAlarmAction(alarm, false);
						}
						break;
					default:
						break;
					}
				}
				Changeflag = !Changeflag;
				// ֪ͨ������ʾ
				LocalData::SetNumberData(SYS_PSB_ALARMRECORD_NOTICE, (int)Changeflag);
				break;
			}
		}

		if (model_->AlarmGUnit.IsSave)
		{
			Storage::FileSave::GetFileSaveTool()->TryTrigSave(&model_->AlarmGUnit.SaveLst);
		}
	}

	void AlarmGControl::AddAlarmRecord(int groupname, int groupno, std::string& str, int times, Project::AlarmLevel level) {
		// ��ȡ������Ϣ
		//string msg = GetStringFromLibrary(res);
		// �洢������Ϣ
		Storage::AlarmStorage::Ins()->Trigger(groupname, groupno, str, times, level);
	}

	string AlarmGControl::GetStringFromLibrary(Project::StatusRes & res) {
		string str = "";
		if (!res.Label.empty()) {
			// ��ǩ���ȡ����
			str = IResourceService::Ins()->GetString(res.Label);
		} else if (res.TableNo >= 0 && res.RowNo >= 0) {
			// ���ֿ��ȡ����
			str = IResourceService::Ins()->GetString(res.TableNo, res.RowNo);
		} else {
			// ��ȡ��������
			if (res.Texts.size() > 0) {
				str = res.Texts[0];
			}
		}
		return str;
	}
}

