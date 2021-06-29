#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "UIComm.h"
#include "AlarmInfoRes.h"
#include "AlarmBarView.h"
#include "AlarmBarControl.h"
#include "AlarmBarModel.h"
#include "ResourceService.h"
#include "StringUtility.h"
#include "System.h"
#include <math.h>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Scroll.H>
#define SPLTBYSTRING 1
namespace UI
{

	AlarmBarView::AlarmBarView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H), Inited(false), CurDx(0), MainStrLen(0), CurAlarmIndex(0)
	{
		memset(TitleTip, 0, 5);
	}

	AlarmBarView::~AlarmBarView()
	{
	}

	void AlarmBarView::draw()
	{
		shared_ptr<AlarmBarControl> ctrl = BaseView.GetControl<AlarmBarControl>();
		shared_ptr<AlarmBarModel> model = BaseView.GetModel<AlarmBarModel>();
		FinX = model->AlarmBarConfig.X + model->AlarmBarConfig.OffX;
		FinY = model->AlarmBarConfig.Y + model->AlarmBarConfig.OffY;
		int tipidx = 0;
		for (int i = 0;i< model->AlarmBarConfig.MultiLanConfig.size();++i)
		{
			if (model->AlarmBarConfig.MultiLanConfig[i].IsSelect)
			{
				//ShowMode |= (int)pow(2.0f, i);
				if (0 == model->AlarmBarConfig.MultiLanConfig[i].Describe.compare("序号"))
				{
					TitleTip[tipidx] = 1;
				}
				else if (0 == model->AlarmBarConfig.MultiLanConfig[i].Describe.compare("触发日期"))
				{
					TitleTip[tipidx] = 2;
				}
				else if (0 == model->AlarmBarConfig.MultiLanConfig[i].Describe.compare("触发时间"))
				{
					TitleTip[tipidx] = 3;
				}
				else if (0 == model->AlarmBarConfig.MultiLanConfig[i].Describe.compare("报警信息"))
				{
					TitleTip[tipidx] = 4;
				}
				else
				{
					TitleTip[tipidx] = 5;
				}
				++tipidx;
			}
		}
		DrawStickChart(ctrl.get(), model.get());
	}

	void AlarmBarView::DrawStickChart(AlarmBarControl * ctrl, AlarmBarModel * model)
	{
		//fl_push_clip(FinX, FinY, model->AlarmBarConfig.Width, model->AlarmBarConfig.Height);

		GraphicDrawHandle::PushClip(FinX, FinY, model->AlarmBarConfig.Width, model->AlarmBarConfig.Height);
		///画背景
		if (model->AlarmBarConfig.IsFill)
		{
			fl_rectf(FinX, FinY, model->AlarmBarConfig.Width, model->AlarmBarConfig.Height, RGBColor(model->AlarmBarConfig.FillColor));
		}
		///文字内容

		//匹配数据库
		
		if (ctrl->IsNewAlarm)
		{
			std::vector<Storage::AlarmRecord> records=Storage::AlarmStorageService::Ins()->SelectAlarmRecordByResolveTick();
			std::vector<Storage::AlarmRecord>().swap(AlarmRecordList);
#if SPLTBYSTRING
			std::vector<std::string>().swap(AlarmStrList);
#else
			for (std::vector<char*>::iterator itor = AlarmStrList.begin(); itor != AlarmStrList.end(); ++itor)
			{
				char* p = *itor;
				delete []p;
			}
			std::vector<char*>().swap(AlarmStrList);
#endif
			for (auto itor = records.begin(); itor != records.end(); ++itor)
			{
				int name = itor->AlarmGroup;
				if ((name >= model->AlarmBarConfig.GroupScopeFrom) || (name < model->AlarmBarConfig.GroupScopeTo))
				{
					AlarmRecordList.push_back(*itor);
				}
			}
			SplitText(ctrl,model);
			//ctrl->IsNewAlarm = false;
		}
		if (MainStrLen == 0)
			if (model->AlarmBarConfig.AlarmBarFont.Font.Size != 0)
				MainStrLen = model->AlarmBarConfig.Width / (model->AlarmBarConfig.AlarmBarFont.Font.Size / 4);
				//MainStrLen = model->AlarmBarConfig.Width / (model->AlarmBarConfig.AlarmBarFont.Font.Size/2);
		if (AlarmStrList.size()>0)
		{
			//设置字体
			fl_font(model->AlarmBarConfig.AlarmBarFont.Font.Style, model->AlarmBarConfig.AlarmBarFont.Font.Size);
			//设置字体颜色
			fl_color(fl_rgb_color(model->AlarmBarConfig.AlarmBarFont.RGBColor.R, model->AlarmBarConfig.AlarmBarFont.RGBColor.G, model->AlarmBarConfig.AlarmBarFont.RGBColor.B));
			int curStrlen = MainStrLen * 3;
			//char* mainStr = new char[curStrlen];
			DrawString = "";
			//memset(mainStr, 0, curStrlen);
			int strLen = 0;
			CurAlarmIndex = CurAlarmIndex >= AlarmStrList.size() ? 0 : CurAlarmIndex;
			int index = CurAlarmIndex;
			//画字符
			while (1)
			{
#if SPLTBYSTRING
				strLen += AlarmStrList[index].size();
				DrawString.append(AlarmStrList[index].c_str(), AlarmStrList[index].size()
).append("   ");
				if (strLen > MainStrLen)
					break;
				index = index >= AlarmStrList.size()-1 ? 0 : index + 1;

#else
				//出错返回
				if (AlarmStrList[index] == nullptr)
				{
					break;
				}
				strLen += CalcuCurStrBarLen(AlarmStrList[index]);
				if (DrawString.data()[0] != 0)
					DrawString.append("   ").append(AlarmStrList[index], AlarmStrList[index].size());
				else
					DrawString = AlarmStrList[index];
				if (strLen > MainStrLen)
					break;
				index = index == AlarmStrList.size() ? 0 : index + 1;
#endif
			}
			if (CurDx >= model->AlarmBarConfig.Width)
				CurDx = 0;
#if SPLTBYSTRING
			/*if (CurDx >= AlarmStrList[CurAlarmIndex].size())
				++CurAlarmIndex;*/
#else
			if (CurDx >= CalcuCurStrBarLen(AlarmStrList[CurAlarmIndex]))
				++CurAlarmIndex;
#endif

			UI::IResourceService::GB2312toUtf8(DrawString);
			fl_draw(DrawString.c_str(),FinX - CurDx, FinY + model->AlarmBarConfig.Height / 2 + model->AlarmBarConfig.AlarmBarFont.Font.Size / 2);
			//delete []mainStr;
		}
		else
		{
			//ctrl->Stop();
		}

		///画外框
		if (model->AlarmBarConfig.IsBorder)
		{
			fl_color( RGBColor(model->AlarmBarConfig.BorderColor));
			fl_rect(FinX, FinY, model->AlarmBarConfig.Width, model->AlarmBarConfig.Height);
		}
		fl_pop_clip();
	}
	void AlarmBarView::SplitText(AlarmBarControl * ctrl, AlarmBarModel * model)
	{
		for (std::vector<Storage::AlarmRecord>::iterator itorRec = AlarmRecordList.begin(); itorRec != AlarmRecordList.end(); ++itorRec)
		{
#if SPLTBYSTRING
			std::string StrAlarm;
			for (int tipidx = 0; tipidx < 5; ++tipidx)
			{
				switch (TitleTip[tipidx])
				{
				case 1:
					StrAlarm.append("Event").append(std::to_string(itorRec->AlarmGroup << 16 | itorRec->AlarmNo)).append(" ");
					break;
				case 2:
					StrAlarm.append(System::GetDateToString(itorRec->StartTick / 1000)).append(" ");
					break;
				case 3:
					StrAlarm.append(System::GetTimeToString(itorRec->StartTick / 1000)).append(" ");
					break;
				case 4:
					StrAlarm.append(itorRec->Title.c_str(), itorRec->Title.size()).append(" ");
					break;
				case 5:
					StrAlarm.append(" 报警次数:").append(to_string(itorRec->Times)).append(" ");
					break;
				}
			}
			AlarmStrList.push_back(StrAlarm);
#else
			char* StrAlarm = new char[ALARMTEXTLEN];
			memset(StrAlarm, 0, ALARMTEXTLEN);
			if (ShowMode&SHOWNO)
			{
				snprintf(StrAlarm, ALARMTEXTLEN, "Event%d,", itorRec->AlarmGroup << 16 | itorRec->AlarmNo);
			}
			tm time = System::GetFormatTimeBySec(itorRec->StartTick/1000);
			SplitTimeStr(time, (ShowMode&SHOWDATE)?model->AlarmBarConfig.DateFormat:-1, (ShowMode&SHOWTIME) ? model->AlarmBarConfig.TimeFormat:-1, StrAlarm, ALARMTEXTLEN);
			//StrAlarm += "Event "+itorRec->EventID+"";
			if (ShowMode&SHOWMSG)
			{
				snprintf(StrAlarm, ALARMTEXTLEN, "%s %s", StrAlarm, itorRec->Title.c_str());
			}
			if (ShowMode&SHOWCOUNT)
			{
				snprintf(StrAlarm, ALARMTEXTLEN, "%s 报警次数:%d", StrAlarm, itorRec->Times);
			}
			AlarmStrList.push_back(StrAlarm);
#endif
		}
	}
	//"YY/MM/DD", "DD/MM/YY", "MM/DD/YY", "YY年MM月DD日"
	//"HH:MM:SS", "HH:SS", "HH:MM:SS:MS", "HH时MM分SS秒"
	void AlarmBarView::SplitTimeStr(tm time, int datetype, int clocktype, char * str,int buffLen)
	{
		switch (datetype)
		{
		case 0:
			snprintf(str, buffLen, "%s %d/%d/%d", str, 1900+time.tm_year, time.tm_mon+1, time.tm_mday);
			break;
		case 1:
			snprintf(str, buffLen, "%s %d/%d/%d", str, time.tm_mday, time.tm_mon + 1, 1900 + time.tm_year);
			break;
		case 2:
			snprintf(str, buffLen, "%s %d/%d/%d", str, time.tm_mon + 1, time.tm_mday, 1900 + time.tm_year);
			break;
		case 3:
			char tempbuf[32];
			memset(tempbuf, 0, 32);
			sprintf(tempbuf, "%d年%d月%d日", 1900 + time.tm_year, time.tm_mon + 1, time.tm_mday);
			memcpy(str + strlen(str), tempbuf, strlen(tempbuf));
			break;
		}
		switch (clocktype)
		{
		case 0:
			snprintf(str, buffLen, "%s %d:%d:%d", str, time.tm_hour, time.tm_min, time.tm_sec);
			break;
		case 1:
			snprintf(str, buffLen, "%s %d:%d", str, time.tm_hour, time.tm_min);
			break;
		case 2:
			snprintf(str, buffLen, "%s %d:%d:%d", str, time.tm_hour, time.tm_min, time.tm_sec);
			break;
		case 3:
			char tempbuf[32];
			memset(tempbuf, 0, 32);
			sprintf(tempbuf, "%d时%d分%d秒", time.tm_hour, time.tm_min, time.tm_sec);
			memcpy(str + strlen(str), tempbuf, strlen(tempbuf));
			break;
		}
	}
	int AlarmBarView::CalcuCurStrBarLen(char* c)
	{
		return strlen(c) + 3;
	}
}
