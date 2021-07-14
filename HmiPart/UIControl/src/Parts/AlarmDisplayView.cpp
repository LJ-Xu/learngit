#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "AlarmDisplayView.h"
#include "AlarmDisplayControl.h"
#include "AlarmDisplayModel.h"
#include "StringUtility.h"
#include "UIDataService.h"
#include "IResourceService.h"
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Group.H>
#include "FL/fl_draw.H"
#include "AlarmStorage.h"
#include <stdio.h> 
#include "ViewShowUtility.h"
#include "CodeFormatUtility.h"
#include "SysSetApi.h"
#include "Logger.h"
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
namespace UI
{
	
	AlarmDisplayView::AlarmDisplayView(int X, int Y, int W, int H, const char *l = 0) : HMIBaseTable(X, Y, W, H, l)
	{
		box(FL_NO_BOX);
		end();
	}
	AlarmDisplayView::~AlarmDisplayView() {}


	void AlarmDisplayView::InitDraw()
	{
		shared_ptr<AlarmDisplayControl> ctrl = BaseView.GetControl<AlarmDisplayControl>();
		shared_ptr<AlarmDisplayModel> model = BaseView.GetModel<AlarmDisplayModel>();
		//box(FL_FLAT_BOX);

		//设置是否使用行、列标题
		col_header(1);
		if (!model->AlarmDisConfig.UseTitle)
			model->AlarmDisConfig.TitleHeight = 0;
		col_header_height(model->AlarmDisConfig.TitleHeight + model->AlarmDisConfig.EachColTitleHeight);
		row_header(0);

		//设置是否可变宽高
		row_resize(0);
		col_resize(0);

		//设置字体
		if (model->AlarmDisConfig.UseSameStyle)
		{
			fontSize_ = model->AlarmDisConfig.SameStringStyle.Font.Size;
			fontStyle_ = IResourceService::Ins()->GetFontIdx(model->AlarmDisConfig.SameStringStyle.Font.Name);
			fontColor_ = fl_rgb_color(RGBColor(model->AlarmDisConfig.SameStringStyle.Colors));
			fontAlign_ = model->AlarmDisConfig.SameStringStyle.Align;
		}
		else
		{
			fontSize_ = model->AlarmDisConfig.ListStringStyle.Font.Size;
			fontStyle_ = IResourceService::Ins()->GetFontIdx(model->AlarmDisConfig.ListStringStyle.Font.Name);
			fontColor_ = fl_rgb_color(RGBColor(model->AlarmDisConfig.ListStringStyle.Colors));
			fontAlign_ = model->AlarmDisConfig.ListStringStyle.Align;
		}

		//设置列数
		cols(model->AlarmDisConfig.AlarmOptions.size());
		//设置行数
		rows(model->AlarmDisConfig.PageNum + 1);
		//设置行高
		row_height_all((model->AlarmDisConfig.Height - model->AlarmDisConfig.TitleHeight - model->AlarmDisConfig.EachColTitleHeight) / model->AlarmDisConfig.PageNum);
		//设置列宽
		if (IResourceService::Ins()->IsRenderMode())
		{
			Fl::scrollbar_size(0);
			vscrollbar->hide();
			hscrollbar->hide();
		}
		else
			Fl::scrollbar_size(16);
		int allcolwidth = 0;
		for (unsigned i = 0; i < model->AlarmDisConfig.AlarmOptions.size(); i++)
		{
			//col_width(i, model->AlarmDisConfig.AlarmOptions[i].ColWidth);
			if ((i == model->AlarmDisConfig.AlarmOptions.size() - 1) && model->AlarmDisConfig.Width - Fl::scrollbar_size() - allcolwidth > 0)
				col_width(i, (model->AlarmDisConfig.Width - Fl::scrollbar_size() - allcolwidth));
			else
				col_width(i, model->AlarmDisConfig.AlarmOptions[i].ColWidth);
			allcolwidth += model->AlarmDisConfig.AlarmOptions[i].ColWidth;
		}

		//设置背景颜色
		cell_bgcolor_ = fl_rgb_color(RGBColor(model->AlarmDisConfig.BackColor));
		//color(cell_bgcolor_);
		//设置报警触发颜色
		triggerTextColor_ = fl_rgb_color(RGBColor(model->AlarmDisConfig.AlarmTextColor));
		triggerbgColor_ = fl_rgb_color(RGBColor(model->AlarmDisConfig.AlarmBackColor));
		//设置报警确认颜色
		if (model->AlarmDisConfig.UseConfirm)
		{
			confirmTextColor_ = fl_rgb_color(RGBColor(model->AlarmDisConfig.ConfirmTextColor));
			confirmbgColor_ = fl_rgb_color(RGBColor(model->AlarmDisConfig.ConfirmBackColor));
		}
		else
		{
			confirmTextColor_ = fontColor_;
			confirmbgColor_ = cell_bgcolor_;
		}
		//设置报警恢复颜色
		if (model->AlarmDisConfig.UseRecover)
		{
			recoverTextColor_ = fl_rgb_color(RGBColor(model->AlarmDisConfig.RecoverTextColor));
			recoverbgColor_ = fl_rgb_color(RGBColor(model->AlarmDisConfig.RecoverBackColor));
		}
		else
		{
			recoverTextColor_ = fontColor_;
			recoverbgColor_ = cell_bgcolor_;
		}
		DisplayMode = model->AlarmDisConfig.AlarmMode;
	}


	int AlarmDisplayView::handle(int event)
	{
		shared_ptr<AlarmDisplayModel> model = BaseView.GetModel<AlarmDisplayModel>();
		if (DisplayMode == Project::AlarmDisMode::HISTORY && model->AlarmDisConfig.UseConfirmMode)
		{
			switch (event) {
			case FL_PUSH:				//控件按下
			{
				SysSetApi::TriggerBeep();
				int R, C;  				// row/column being worked on
				ResizeFlag resizeflag;		// which resizing area are we over? (0=none)
				TableContext context = cursor2rowcol(R, C, resizeflag);
				pushTime_ = high_resolution_clock::now();
				if (std::chrono::duration_cast<milliseconds>(high_resolution_clock::now() - firstTime_).count() > 300) {
					clickcount_ = 0;
				}
				clickcount_++;
				if (clickcount_ == 1) {
					firstTime_ = high_resolution_clock::now();
				}
				else if (clickcount_ == 2) {
					lastTime_ = high_resolution_clock::now();
					// 两次点击小于300ms 也就是连续点击  
					if (std::chrono::duration_cast<milliseconds>(lastTime_ - firstTime_).count() < 300
						&& model->AlarmDisConfig.CMode == Project::ConfirmMode::Double)// 判断是否是执行了双击事件  
					{
						if (DisplayInfo.size() > (size_t)R)
						{
							Storage::AlarmStorage::Ins()->UnConfirmByEventId(
								DisplayInfo[R].AlarmGroup, DisplayInfo[R].AlarmNo);
							LOG_INFO("UnConfirm Event, row = %d",R);
						}
					}
				}
				if (model->AlarmDisConfig.Perm.HasShowPerm && model->AlarmDisConfig.Perm.HasLimitShowPerm)
					redraw();
				printf("C = %d, R = %d\n", C, R);
				return HMIBaseTable::handle(event);
			}
			case FL_RELEASE:			//控件松开
			{
				int R, C;  				// row/column being worked on
				ResizeFlag resizeflag;		// which resizing area are we over? (0=none)
				TableContext context = cursor2rowcol(R, C, resizeflag);
				printf("C = %d, R = %d\n", C, R);
				if (when() & FL_WHEN_RELEASE)
				{
					releaseTime_ = high_resolution_clock::now();	//记录按键松开时间
					milliseconds timeInterval = std::chrono::duration_cast<milliseconds>(releaseTime_ - pushTime_);
					if (timeInterval.count() > 1000 && model->AlarmDisConfig.CMode == Project::ConfirmMode::Long)		//长按
					{
						if (DisplayInfo.size() > (size_t)R)
						{
							LOG_INFO("UnConfirm Event, row = %d", R);
							Storage::AlarmStorage::Ins()->UnConfirmByEventId(
								DisplayInfo[R].AlarmGroup, DisplayInfo[R].AlarmNo);
						}
					}
					else if (model->AlarmDisConfig.CMode == Project::ConfirmMode::Single)	//短按
					{
						if (DisplayInfo.size() > (size_t)R)
						{
							LOG_INFO("UnConfirm Event, row = %d", R);
							Storage::AlarmStorage::Ins()->UnConfirmByEventId(
								DisplayInfo[R].AlarmGroup, DisplayInfo[R].AlarmNo);
						}
					}
				}
				if (model->AlarmDisConfig.Perm.HasShowPerm && model->AlarmDisConfig.Perm.HasLimitShowPerm)
					redraw();
				return HMIBaseTable::handle(event);
			}
			default:
				return HMIBaseTable::handle(event);
			}
		}
		else
			return HMIBaseTable::handle(event);
	}
	string AlarmDisplayView::GetTimeString(DDWORD time, int mode)
	{
		shared_ptr<AlarmDisplayModel> model = BaseView.GetModel<AlarmDisplayModel>();
		char displayTime[100];
		string text;
		time_t tmSec = time / 1000;
		int tmMs = (int)(time % 1000);
		struct tm *local;
		local = localtime(&tmSec);
		if (time == 0 || !local)
			return "";
		if (mode)	//时间
		{
			switch (model->AlarmDisConfig.Timemode)
			{
			case Project::HMS:
				sprintf(displayTime, "%d:%d:%d", local->tm_hour, local->tm_min, local->tm_sec);
				break;
			case Project::HM:
				sprintf(displayTime, "%d:%d", local->tm_hour, local->tm_min);
				break;
			case Project::HMSMS:
				sprintf(displayTime, "%d:%d:%d:%d", local->tm_hour, local->tm_min, local->tm_sec, tmMs);
				break;
			case Project::HMSChinese:
				sprintf(displayTime, "%d时%d分%d秒", local->tm_hour, local->tm_min, local->tm_sec);
				break;
			default:
				break;
			}
		}
		else    //日期
		{
			switch (model->AlarmDisConfig.Datemode)
			{
			case Project::MDYSlash:
				sprintf(displayTime, "%d\\%d\\%d", local->tm_mon + 1, local->tm_mday, local->tm_year + 1900);
				break;
			case Project::DMYSlash:
				sprintf(displayTime, "%d\\%d\\%d", local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
				break;
			case Project::YMDChinese:
				sprintf(displayTime, "%d年%d月%d日", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
				break;
			case Project::YMDSlash:
				sprintf(displayTime, "%d\\%d\\%d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
				break;
			default:
				break;
			}
		}
		text = displayTime;
		//UI::IResourceService::GB2312toUtf8(text);
		return text;
	}

	string AlarmDisplayView::GetDrawString(string projectname, int row)
	{
		shared_ptr<AlarmDisplayModel> model = BaseView.GetModel<AlarmDisplayModel>();
		if (model->AlarmDisConfig.AlarmOptions.size() == 0)
			return nullptr;
		string serialString = "序号";
		string dateString = "触发日期";
		string timeString = "触发时间";
		string infoString = "报警信息";
		string countString = "报警次数";
		string confirmtime = "确认时间";
		string recovertime = "报警恢复时间";

		string text = "";
		if (DisplayInfo.empty() || DisplayInfo.size() <= (size_t)row)
		{
			LOG_INFO_("Can not Get Draw String\n");
			return text;
		}
		if (projectname == serialString)		//获取序号
			text = to_string(row + 1);
		else if (projectname == dateString)		//获取日期
			text = GetTimeString(DisplayInfo[row].StartTick, 0);
		else if (projectname == timeString)		//获取时间
			text = GetTimeString(DisplayInfo[row].StartTick, 1);
		else if (projectname == infoString)
			text = DisplayInfo[row].Title;
		else if(projectname == countString)
			text = to_string(DisplayInfo[row].Times);
		else if(projectname == confirmtime)
			text = GetTimeString(DisplayInfo[row].CheckTick, 1);
		else if(projectname == recovertime)
			text = GetTimeString(DisplayInfo[row].ResolveTick, 1);
		return text;
	}
	void AlarmDisplayView::SortAlarmInfo()
	{
		if (DisplayMode == Project::AlarmDisMode::REALTIME)
			return;
		vector<Storage::AlarmRecord> topVectorInfo;		//需要置顶消息
		vector<Storage::AlarmRecord>::iterator tmpInfo;

		for (tmpInfo = DisplayInfo.begin(); tmpInfo != DisplayInfo.end();)
		{
			if ((*tmpInfo).ResolveTick == 0)
			{
				topVectorInfo.push_back((*tmpInfo));
				tmpInfo = DisplayInfo.erase(tmpInfo);
			}
			else
				tmpInfo++;
		}
		topVectorInfo.insert(topVectorInfo.end(), DisplayInfo.begin(), DisplayInfo.end());
		DisplayInfo = topVectorInfo;
	}
	int AlarmDisplayView::GetInfoNum()
	{
		shared_ptr<AlarmDisplayModel> model = BaseView.GetModel<AlarmDisplayModel>();
		vector<Storage::AlarmRecord>().swap(DisplayInfo);
		switch (DisplayMode)
		{
		case Project::AlarmDisMode::REALTIME:
		{
			/*只显示未恢复消息*/
			DisplayInfo = Storage::AlarmStorage::Ins()->QueryByUnRecover(model->AlarmDisConfig.StartGroupName, model->AlarmDisConfig.EndGroupName);
			break;
		}
		case Project::AlarmDisMode::HISTORY:
		{
			if (model->AlarmDisConfig.IsInfoHideCtrl)		//隐藏控制
				DisplayInfo = Storage::AlarmStorage::Ins()->QueryByHide(HideFlag, model->AlarmDisConfig.StartGroupName, model->AlarmDisConfig.EndGroupName);
			else
				DisplayInfo = Storage::AlarmStorage::Ins()->QueryAll(model->AlarmDisConfig.StartGroupName, model->AlarmDisConfig.EndGroupName);
			break;
		}
		case Project::AlarmDisMode::SEARCH:
		{
			int record = 0;
			if (model->AlarmDisConfig.AlarmMode == 0)		//实时
				record = 2;
			switch (model->AlarmDisConfig.SearchMode)
			{
			case Project::AlarmSeekDATE:
			{
				LOG_INFO("Alarm Seek DATE = %ld\n", model->AlarmDisConfig.SearchDate);
				DisplayInfo = Storage::AlarmStorage::Ins()->QueryByDate(model->AlarmDisConfig.SearchDate,
					model->AlarmDisConfig.SearchDate + (DDWORD)86400000, model->AlarmDisConfig.StartGroupName, model->AlarmDisConfig.EndGroupName, record);
				break;
			}
			case Project::AlarmSeekTIME:
			{
				LOG_INFO("Alarm Seek Time %ld to %ld\n", model->AlarmDisConfig.SearchTimeStart, model->AlarmDisConfig.SearchTimeEnd);
				DisplayInfo = Storage::AlarmStorage::Ins()->QueryByTime(model->AlarmDisConfig.SearchTimeStart,
					model->AlarmDisConfig.SearchTimeEnd, model->AlarmDisConfig.StartGroupName, model->AlarmDisConfig.EndGroupName, record);
				break;
			}
			case Project::AlarmSeekGROUP:
			{
				LOG_INFO("Alarm Seek Group %d\n", model->AlarmDisConfig.SearchGroup);
				DisplayInfo = Storage::AlarmStorage::Ins()->QueryByGroupName(model->AlarmDisConfig.SearchGroup,
					model->AlarmDisConfig.StartGroupName, model->AlarmDisConfig.EndGroupName, record);
				break;
			}
			case Project::AlarmSeekNUM:
			{
				LOG_INFO("Alarm Seek Num %d\n", model->AlarmDisConfig.SearchNo);
				DisplayInfo = Storage::AlarmStorage::Ins()->QueryByGroupNo(model->AlarmDisConfig.SearchNo, 
					model->AlarmDisConfig.StartGroupName, model->AlarmDisConfig.EndGroupName, record);
				break;
			}
			case Project::AlarmSeekLEVEL:
				LOG_INFO("Alarm Seek Level %d\n", model->AlarmDisConfig.SearchLevel);
				DisplayInfo = Storage::AlarmStorage::Ins()->QueryByAlarmLevel(model->AlarmDisConfig.SearchLevel,
					model->AlarmDisConfig.StartGroupName, model->AlarmDisConfig.EndGroupName, record);
				break;
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
		if (DisplayInfo.empty())
		{
			LOG_INFO_("Alarm Info is NULL\n");
			return 0;
		}
		if (model->AlarmDisConfig.SortMode)		//时间逆序
			std::reverse(DisplayInfo.begin(), DisplayInfo.end());
		if (model->AlarmDisConfig.TopNotRecover)
			SortAlarmInfo();
		return DisplayInfo.size();
	}
	void AlarmDisplayView::SetRenderData()
	{
		shared_ptr<AlarmDisplayModel> model = BaseView.GetModel<AlarmDisplayModel>();

		vector<Storage::AlarmRecord>().swap(DisplayInfo);		//用来显示的信息
		for (size_t i = 0; i < 3; i++)
		{
			Storage::AlarmRecord record;
#ifdef WIN32
			record.AlarmGroup = 0;
			record.AlarmNo = (uint16_t)i;
			record.Times = 1;
			record.Level = i;
			record.StartTick = duration_cast<MilliSecs>(system_clock::now().time_since_epoch()).count();
			record.Title = "Alarm Event" + to_string(i);
#endif // WIN32
			DisplayInfo.push_back(record);
		}
	}

	void AlarmDisplayView::draw_cell(TableContext context,
		int R, int C, int X, int Y, int W, int H)
	{
		shared_ptr<AlarmDisplayModel> model = BaseView.GetModel<AlarmDisplayModel>();
		string text;
		//sprintf(text, "%d/%d", R + 1, C + 1);		// text for each cell
		switch (context)
		{
		case CONTEXT_STARTPAGE:
		{
			if (IResourceService::Ins()->IsRenderMode())
			{
				InitDraw();
				SetRenderData();
			}
			else
			{
				int num = GetInfoNum();
				//绘制背景
				if (num > model->AlarmDisConfig.PageNum)
					rows(num);
			}
			return;
		}
		case CONTEXT_COL_HEADER:				//绘制列标题
		{
			/*绘制标题*/
			if (C == 0 && model->AlarmDisConfig.UseTitle)
			{
				GraphicDrawHandle::PushClip(X, Y, model->AlarmDisConfig.Width, model->AlarmDisConfig.TitleHeight);
				{
					/*设置背景颜色*/
					//fl_color(fl_rgb_color(RGBColor(model->AlarmDisConfig.TitleBgColor)));
					fl_color(active() ? fl_rgb_color(RGBColor(model->AlarmDisConfig.TitleBgColor)) : 
						fl_inactive(fl_rgb_color(RGBColor(model->AlarmDisConfig.TitleBgColor))));

					fl_rectf(model->AlarmDisConfig.X + model->AlarmDisConfig.OffX - 2,
						model->AlarmDisConfig.Y + model->AlarmDisConfig.OffY - 2,
						w() + 6, model->AlarmDisConfig.TitleHeight + 6);
					/*获取关于字符串*/
					if (model->AlarmDisConfig.UseSameStyle)
					{
						fl_font(fontStyle_, fontSize_);
						fl_color(active() ? fontColor_ : fl_inactive(fontColor_));
						//fl_color(fontColor_);
					}
					else
					{
						fl_font(UI::IResourceService::GetFontIdx(model->AlarmDisConfig.TitleStringStyle.Font.Name),
							model->AlarmDisConfig.TitleStringStyle.Font.Size);
						Fl_Color textcolor = fl_rgb_color(RGBColor(model->AlarmDisConfig.TitleStringStyle.Colors));
						fl_color(active() ? textcolor : fl_inactive(textcolor));
					}
					string text = StringUtility::GetDrawString(IResourceService::Ins(), model->AlarmDisConfig.Title, 0);
					UI::IResourceService::GB2312toUtf8(text);
					/*绘制文本*/
					fl_draw(text.data(),
						model->AlarmDisConfig.X + model->AlarmDisConfig.OffX,
						model->AlarmDisConfig.Y + model->AlarmDisConfig.OffY,
						w(), model->AlarmDisConfig.TitleHeight,
						model->AlarmDisConfig.TitleStringStyle.Align);	//FL_ALIGN_CENTER
				}
				fl_pop_clip();
			}
			
			GraphicDrawHandle::PushClip(X, Y + model->AlarmDisConfig.TitleHeight, W, H - model->AlarmDisConfig.TitleHeight);
			{
				if (X <= model->AlarmDisConfig.X + model->AlarmDisConfig.OffX)
				{
					gridstartx_ = X;//X - model->AlarmDisConfig.X - model->AlarmDisConfig.OffX;
					gridstartcol_ = C;
				}
				if (IResourceService::Ins()->IsRenderMode() || !hscrollbar->visible())
				{
					gridstartx_ = model->AlarmDisConfig.X + model->AlarmDisConfig.OffX;
					gridstartcol_ = 0;
				}
				// BG COLOR
				fl_color(active() ? cell_bgcolor_ : fl_inactive(cell_bgcolor_));
				fl_rectf(X - 2, Y + model->AlarmDisConfig.TitleHeight - 2, W + 6, H - model->AlarmDisConfig.TitleHeight + 6);
				fl_color(active() ? fl_rgb_color(RGBColor(model->AlarmDisConfig.FrameStyle.Color)) : 
					fl_inactive(fl_rgb_color(RGBColor(model->AlarmDisConfig.FrameStyle.Color))));

				//fl_color(fl_rgb_color(RGBColor(model->AlarmDisConfig.FrameStyle.Color)));
				fl_line_style(model->AlarmDisConfig.FrameStyle.Type, model->AlarmDisConfig.FrameStyle.Weight);
				// TEXT
				if (model->AlarmDisConfig.AlarmOptions.size() > (size_t)C)
				{
					if (model->AlarmDisConfig.UseSameStyle)
					{
						fl_font(fontStyle_,fontSize_);
						fl_color(active() ? fontColor_ : fl_inactive(fontColor_));
					}
					else
					{
						fl_font(UI::IResourceService::GetFontIdx(model->AlarmDisConfig.ListTitleStyle.Font.Name),
							model->AlarmDisConfig.ListTitleStyle.Font.Size);
						Fl_Color textcolor = fl_rgb_color(RGBColor(model->AlarmDisConfig.ListTitleStyle.Colors));
						fl_color(active() ? textcolor : fl_inactive(textcolor));
					}
					/*获取text*/
					string text;
						text = StringUtility::GetDrawString(IResourceService::Ins(),
							model->AlarmDisConfig.AlarmOptions[C].TitleDescribe, 0);
					UI::IResourceService::GB2312toUtf8(text);
					fl_draw(text.c_str(), X, Y + model->AlarmDisConfig.TitleHeight, W, 
						H - model->AlarmDisConfig.TitleHeight, model->AlarmDisConfig.ListTitleStyle.Align);
				}
			}
			fl_pop_clip();
			return;
		}
		case CONTEXT_CELL:
		{
			//获取显示字符串
			//if (DisplayInfo.size() == 0)
			//	return;
			if (model->AlarmDisConfig.AlarmOptions.size() <(size_t)C)
				return;
			if (Y > model->AlarmDisConfig.Y + model->AlarmDisConfig.OffY + col_header_height()
				&& Y <= model->AlarmDisConfig.Y + model->AlarmDisConfig.OffY + col_header_height() + row_height(0))
				gridstarty_ = Y - model->AlarmDisConfig.Y - model->AlarmDisConfig.OffY - col_header_height();
			text = GetDrawString(model->AlarmDisConfig.AlarmOptions[C].ProjectName, R);
			if (DisplayInfo.size() == 0 || IResourceService::Ins()->IsRenderMode())
			{
				Fl_Color bgColor = cell_bgcolor_;
				Fl_Color textColor = fontColor_;
				GraphicDrawHandle::PushClip(X, Y, W, H);
				{
					// BG COLOR
					fl_color(active() ? bgColor : fl_inactive(bgColor));

					fl_rectf(X - 2, Y - 2, W + 6, H + 6);
					fl_color(active() ? fontColor_ : fl_inactive(fontColor_));
					fl_font(fontStyle_, fontSize_);
					if (!UI::CodeFormatUtility::IsStrUtf8(text.c_str()))
						UI::IResourceService::GB2312toUtf8(text);
					fl_draw(text.c_str(), X, Y, W, H, FL_ALIGN_CENTER);
				}
				fl_pop_clip();
				return;
			}
				Fl_Color textColor = fontColor_;
			Fl_Color bgColor = cell_bgcolor_;
			if ((size_t)R < DisplayInfo.size())
			{
				if (DisplayInfo[R].StartTick != 0)		//报警已触发
				{
					textColor = triggerTextColor_;
					bgColor = triggerbgColor_;
				}
				if (DisplayInfo[R].CheckTick != 0)		//报警已确认
				{
					textColor = confirmTextColor_;
					bgColor = confirmbgColor_;
				}
				if (DisplayInfo[R].ResolveTick != 0)		//报警已恢复
				{
					textColor = recoverTextColor_;
					bgColor = recoverbgColor_;
				}
				if (DisplayInfo[R].CheckTick != 0 && DisplayInfo[R].ResolveTick != 0)
				{
					if (DisplayInfo[R].ResolveTick < DisplayInfo[R].CheckTick)
					{
						textColor = confirmTextColor_;
						bgColor = confirmbgColor_;
					}
					else
					{
						textColor = recoverTextColor_;
						bgColor = recoverbgColor_;
					}
				}
			}
			GraphicDrawHandle::PushClip(X, Y, W, H);
			{
				// BG COLOR
				fl_color(active() ? bgColor : fl_inactive(bgColor));
				fl_rectf(X, Y, W, H);

				// TEXT
				fl_color(active() ? textColor : fl_inactive(textColor));
				fl_font(fontStyle_, fontSize_);
				if (!UI::CodeFormatUtility::IsStrUtf8(text.c_str()))
					UI::IResourceService::GB2312toUtf8(text);
				fl_draw(text.c_str(), X, Y, W, H, FL_ALIGN_CENTER);
			}
			fl_pop_clip();
			
			return;
		}

		case CONTEXT_TABLE:
			fprintf(stderr, "TABLE CONTEXT CALLED\n");
			return;

		case CONTEXT_ENDPAGE:		//画边框
		{
			if (model->AlarmDisConfig.UseGrid)
			{
				fl_color(active() ? fl_rgb_color(RGBColor(model->AlarmDisConfig.GridStyle.Color))
					: fl_inactive(fl_rgb_color(RGBColor(model->AlarmDisConfig.GridStyle.Color))));
				fl_line_style(model->AlarmDisConfig.GridStyle.Type, model->AlarmDisConfig.GridStyle.Weight);
				//绘制标题内行边框
				if (model->AlarmDisConfig.UseTitle)
					fl_line(model->AlarmDisConfig.X + model->AlarmDisConfig.OffX + model->AlarmDisConfig.FrameStyle.Weight / 2,
						model->AlarmDisConfig.Y + model->AlarmDisConfig.OffY + model->AlarmDisConfig.TitleHeight,
						model->AlarmDisConfig.X + model->AlarmDisConfig.OffX + model->AlarmDisConfig.Width - model->AlarmDisConfig.FrameStyle.Weight - Fl::scrollbar_size(),
						model->AlarmDisConfig.Y + model->AlarmDisConfig.OffY + model->AlarmDisConfig.TitleHeight);
				//绘制列表标题内行边框
				fl_line(model->AlarmDisConfig.X + model->AlarmDisConfig.OffX + model->AlarmDisConfig.FrameStyle.Weight / 2,
						model->AlarmDisConfig.Y + model->AlarmDisConfig.OffY + model->AlarmDisConfig.TitleHeight + model->AlarmDisConfig.EachColTitleHeight,
						model->AlarmDisConfig.X + model->AlarmDisConfig.OffX + model->AlarmDisConfig.Width - model->AlarmDisConfig.FrameStyle.Weight - Fl::scrollbar_size(),
						model->AlarmDisConfig.Y + model->AlarmDisConfig.OffY + model->AlarmDisConfig.TitleHeight + model->AlarmDisConfig.EachColTitleHeight);

				int vscrollsize, hscrollsize;
				vscrollsize = vscrollbar->visible() ? 16 : 0;
				hscrollsize = hscrollbar->visible() ? 16 : 0;
				if (IResourceService::Ins()->IsRenderMode())
					vscrollsize = 0;
				GraphicDrawHandle::PushClip(model->AlarmDisConfig.X + model->AlarmDisConfig.OffX + model->AlarmDisConfig.FrameStyle.Weight / 2,
					model->AlarmDisConfig.Y + model->AlarmDisConfig.OffY + model->AlarmDisConfig.TitleHeight,
					model->AlarmDisConfig.Width - vscrollsize - model->AlarmDisConfig.FrameStyle.Weight,
					model->AlarmDisConfig.Height - hscrollsize - model->AlarmDisConfig.TitleHeight);
				{
					for (int rowY = gridstarty_ + col_header_height(); rowY < model->AlarmDisConfig.Height;)
					{
						fl_line(model->AlarmDisConfig.X + model->AlarmDisConfig.OffX,
							model->AlarmDisConfig.Y + model->AlarmDisConfig.OffY + rowY,
							model->AlarmDisConfig.X + model->AlarmDisConfig.OffX + model->AlarmDisConfig.Width,
							model->AlarmDisConfig.Y + model->AlarmDisConfig.OffY + rowY);
						rowY += row_height(0);
					}
					for (int rowX = gridstartx_, i = gridstartcol_; i < cols();i++)
					{
						fl_line(rowX, model->AlarmDisConfig.Y + model->AlarmDisConfig.OffY,
							rowX, model->AlarmDisConfig.Y + model->AlarmDisConfig.OffY + model->AlarmDisConfig.Height);
						rowX += col_width(i);
					}
				}
				fl_pop_clip();
			}
			if (model->AlarmDisConfig.UseFrame)
			{
				fl_color(active() ? fl_rgb_color(RGBColor(model->AlarmDisConfig.FrameStyle.Color))
					: fl_inactive(fl_rgb_color(RGBColor(model->AlarmDisConfig.FrameStyle.Color))));
				fl_line_style(FL_CAP_SQUARE | FL_JOIN_MITER | model->AlarmDisConfig.FrameStyle.Type, model->AlarmDisConfig.FrameStyle.Weight);

				fl_begin_line();
				fl_vertex((double)model->AlarmDisConfig.X + (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffX,
					(double)model->AlarmDisConfig.Y + (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffY);
				fl_vertex((double)model->AlarmDisConfig.X - (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffX + (double)model->AlarmDisConfig.Width,
					(double)model->AlarmDisConfig.Y + (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffY);
				fl_end_line();

				fl_begin_line();
				fl_vertex((double)model->AlarmDisConfig.X + (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffX,
					(double)model->AlarmDisConfig.Y - (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffY + (double)model->AlarmDisConfig.Height);
				fl_vertex((double)model->AlarmDisConfig.X - (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffX + (double)model->AlarmDisConfig.Width,
					(double)model->AlarmDisConfig.Y - (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffY + (double)model->AlarmDisConfig.Height);
				fl_end_line();

				fl_begin_line();
				fl_vertex((double)model->AlarmDisConfig.X + (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffX,
					(double)model->AlarmDisConfig.Y + (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffY);
				fl_vertex((double)model->AlarmDisConfig.X + (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffX,
					(double)model->AlarmDisConfig.Y - (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffY + (double)model->AlarmDisConfig.Height);
				fl_end_line();

				fl_begin_line();
				fl_vertex((double)model->AlarmDisConfig.X - (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffX + (double)model->AlarmDisConfig.Width,
					(double)model->AlarmDisConfig.Y + (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffY);
				fl_vertex((double)model->AlarmDisConfig.X - (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffX + (double)model->AlarmDisConfig.Width,
					(double)model->AlarmDisConfig.Y - (double)model->AlarmDisConfig.FrameStyle.Weight / 2.0 + (double)model->AlarmDisConfig.OffY + (double)model->AlarmDisConfig.Height);
				fl_end_line();
				//fl_rect(model->AlarmDisConfig.X + model->AlarmDisConfig.OffX,
				//	model->AlarmDisConfig.Y + model->AlarmDisConfig.OffY,
				//	w(), model->AlarmDisConfig.Height);
			}
			return;
		}
		case CONTEXT_RC_RESIZE:
		case CONTEXT_NONE:
			return;
		case CONTEXT_ROW_HEADER:
			return;
		}
	}

	

}
