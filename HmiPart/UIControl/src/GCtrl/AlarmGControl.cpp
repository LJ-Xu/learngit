/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmGControl.cpp
 * Author   : TangYao
 * Date     : 2021/01/13
 * Descript : 报警控制
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
#include "System.h"

namespace UI
{
	void AlarmBeepFunc(void *data)
	{
		AlarmInfoRes* res = (AlarmInfoRes*)data;
		if (res->IsBeeping)
		{
			if (--(res->CurBeepTime) > 0)
			{
				SysCtrlApi::Beep();
				if(AlarmGControl::Ins())
					AlarmGControl::Ins()->Win()->AddTimeout(1, AlarmBeepFunc, res);
			}
		}
	}
	void AlarmPopWinFunc(void *data)
	{
		AlarmInfoRes* res = (AlarmInfoRes*)data;
		if (res->IsPoping)
		{
			AlarmGControl::Ins()->Win()->OpenDialogPage(res->PopWinNo);
			AlarmGControl::Ins()->Win()->AddTimeout((double)res->PopTime/1000.0f, AlarmPopWinFunc, res);
		}
	}
	void AlarmSaveFunc(void *data)
	{
		if (Storage::FileSave::GetFileSaveTool()->ReadySaveAlarm())
		{
			Storage::FileSave::GetFileSaveTool()->SaveAlarm(*(SaveFileRes*)data);
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

		//带小数
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
			//存在有效小数
			else
			{
				if (ptidx > itgDgt)
				{
					sdst = "*";
				}
				else
				{
					//整数位前导
					if (isPre)
					{
						int lstIntDgt = itgDgt - ptidx;
						while (lstIntDgt--)
						{
							sdst.append("0");
						}
					}
					sdst.append(ssrc.substr(0, ptidx));
					//检查小数位
					int lstDgt = dmcDgt - (ssrc.size() - 1 - ptidx);
					if (lstDgt <= 0)//小数位过长或刚好
					{
						sdst.append(ssrc.substr(ptidx, dmcDgt + 1));
					}
					else //if (lstDgt > 0)//还有没填满的小数位
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
		//不带小数
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
		/*RunBeep = false;
		RunPop = false;*/
	}

	void AlarmGControl::CreateView() {

		AlarmGView* view = new AlarmGView(0, 0, 0, 0);

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
			if (triggeralarm)			//触发报警
			{
				if (alarmres.UseBeep)
				{
					//触发蜂鸣器
				}
				if (alarmres.UseAlarmPopWin)	//打开弹窗
				{
					Win()->OpenDialogPage(alarmres.PopWinNo);
				}
			}
			else				//报警消失
			{
				if (alarmres.UseAlarmPopWin && alarmres.IsCloseWin)	//关闭弹窗
					Win()->ClosePage(alarmres.PopWinNo);
			}
		});
		thr.detach();
		//pView_->damage();
		//pView_->redraw();
#else
		AlarmGView* pView = static_cast<AlarmGView*>(pView_);
		if (alarmres.UseBeep)
		{
			alarmres.IsBeeping = triggeralarm;
			if (triggeralarm)
			{
				alarmres.CurBeepTime = alarmres.BeepTimeOut;
				SysCtrlApi::Beep();
				Win()->AddTimeout(1, AlarmBeepFunc, &alarmres);
			}
		}
		if (alarmres.UseAlarmPopWin)
		{
			alarmres.IsPoping = triggeralarm;
			if (triggeralarm)
			{
				//Win()->OpenDialogPage(alarmres.PopWinNo);
				Win()->AddTimeout(0, AlarmPopWinFunc,&alarmres);
			}
		}
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
							//监控地址
						case 1:
						{
							switch (almctxt.MonitorUnits[idx].AddrType)
							{
							case 0://数字
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
								strRet.append(strTemp.c_str(), strTemp.size());
								break;
							}
							case 1://字符
							{
								string strTemp = DataApi::AppString(itorMnt.MonitorAddrRef);
								strRet.append(strTemp.c_str(),strTemp.size());
								break;
							}
							case 2://中文
							{
								string strGet = DataApi::AppString(itorMnt.MonitorAddrRef);
								UI::IResourceService::GB2312toUtf8(strGet);
								strRet.append(strGet.c_str(), strGet.size());
								break;
							}
							}
							break;
						}
							//动态文字串
						case 0:
						{
							DWORD value = DataApi::AppNumber<DWORD>(itorMnt.MonitorAddrRef);
							for (auto itor = itorMnt.StatusStyle.begin();itor!= itorMnt.StatusStyle.end();++itor)
							{
								if (value == (DWORD)(itor->SrcId))
								{
									strRet.append(itor->Text.Texts[0].c_str(), itor->Text.Texts[0].size());
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
				// 事件ID
				//int eventId = alarm.GroupName << 16 | alarm.GroupNo;
				if (alarm.RegType == 0) {	// 使用位寄存器
					// 获取位寄存器中数据
					BOOL bitVal = UIData::Bit(alarm.ReadRegVar);
					if (model_->AlarmGUnit.InfoLst[i].TempByteState == -1)
						model_->AlarmGUnit.InfoLst[i].TempByteState = bitVal;

					switch (alarm.BitCondition)
					{
					case Project::BitConditions::ON:
						if (bitVal) {	// 产生报警记录
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
						if (!bitVal) {	// 产生报警记录
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
					// 获取当前寄存器值
					//currentState = bitVal;
					model_->AlarmGUnit.InfoLst[i].TempByteState = bitVal;
				} else if (alarm.RegType == 1) {	// 使用字寄存器
					// 获取报警比较值
					DOUBLE cmpVal = alarm.ComPareValue;
					if (alarm.ComPareVar != Project::DataVarId::NullId) {
						cmpVal = UIData::Number<DOUBLE>(alarm.ComPareVar);
					}
					// 读取报警寄存器值
					DOUBLE readVal = UIData::Number<DOUBLE>(alarm.ReadRegVar);
					switch (alarm.BtyeCondition)
					{
					case Project::Conditions::LT:
						if (readVal < cmpVal) {	// 触发报警
							strContext = GetContextString(alarm.Context);
							AddAlarmRecord(alarm.GroupName, alarm.GroupNo, strContext, ++alarm.Count, alarm.Category);
							HandleAlarmAction(alarm, true);
							/*if (model_->AlarmGUnit.IsSave)
							{
								Storage::FileSave::GetFileSaveTool()->InsertAlarmResInQueue(&alarm);
							}*/
						} else {	// 解除报警
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
				// 通知报警显示
				LocalData::SetNumberData(SYS_PSB_ALARMRECORD_NOTICE, (int)Changeflag);
				break;
			}
		}

		if (model_->AlarmGUnit.IsSave)
		{
			Win()->AddTimeout(0, AlarmSaveFunc, &model_->AlarmGUnit.SaveLst);
		}
	}

	void AlarmGControl::AddAlarmRecord(int groupname, int groupno, std::string& str, int times, Project::AlarmLevel level) {
		// 获取报警信息
		//string msg = GetStringFromLibrary(res);
		// 存储报警信息
		Storage::AlarmStorage::Ins()->Trigger(groupname, groupno, str, times, level);
	}

	string AlarmGControl::GetStringFromLibrary(Project::StatusRes & res) {
		string str = "";
		if (!res.Label.empty()) {
			// 标签库获取文字
			str = IResourceService::Ins()->GetString(res.Label);
		} else if (res.TableNo >= 0 && res.RowNo >= 0) {
			// 文字库获取文字
			str = IResourceService::Ins()->GetString(res.TableNo, res.RowNo);
		} else {
			// 获取输入文字
			if (res.Texts.size() > 0) {
				str = res.Texts[0];
			}
		}
		return str;
	}
}

