#include "GraphicDrawHandle.h"
#include "DateView.h"
#include "ResourceService.h"
#include "stdafx.h"
#include "stdafx.h"
#include "UIComm.h"
#include "DateControl.h"
#include "DateModel.h"
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Widget.H>
#include "FL/fl_draw.H"
#include "System.h"
#include "HMIWindow.h"
#include "PermUtility.h"
#define SHORTCNLEN 16
#define DATELENGTH 48
#ifdef __linux
#define _TCHAR char
#define _T 
#endif
namespace UI
{
	DateView::DateView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H), flImgPtr(nullptr){}
	DateView::~DateView() {}

	void DateView::InitBackPic(void* model)
	{
		flImgPtr = nullptr;
		if (model == nullptr)
			return;
		DateModel* pmod = (DateModel*)model;
		if (pmod->DateConfig.BackUpImg.KeyVal == "")
		{
			return;
		}
		flImgPtr = IResourceService::Ins()->GetImage(pmod->DateConfig.BackUpImg.KeyVal);

	}

	std::string DateView::GetCurrentDate(int FormatType, bool showFullYear, bool showWeek)
	{
		char chDate[DATELENGTH];
		int year, month, day,week;
		System::GetCurrentDate(year, month, day, week);
		if (!showFullYear)
		{
			year = year % 100;
		}
		char chweek[SHORTCNLEN];
		memset(chweek, 0, SHORTCNLEN);
		switch (week)
		{
		case 1:
			if(FormatType != 2)
				memcpy(chweek,"Mon", 4);
			else
			{
				memcpy(chweek, "星期一", 6);
			}
			break;
		case 2:
			if (FormatType != 2)
				memcpy(chweek, "Tue", SHORTCNLEN);
			else
			{
				memcpy(chweek, "星期二", 6);
			}
			break;
		case 3:
			if (FormatType != 2)
				memcpy(chweek, "Wed", 4);
			else
			{
				memcpy(chweek, "星期三", 6);
			}
			break;	
		case 4:
			if (FormatType != 3)
				memcpy(chweek, "Thu", 4);
			else
			{
				memcpy(chweek, "星期四", 6);
			}
			break;
		case 5:
			if (FormatType != 2)
				memcpy(chweek, "Fri", 4);
			else
			{
				memcpy(chweek, "星期五", 6);
			}
			break;
		case 6:
			if (FormatType != 2)
				memcpy(chweek, "Sat", 4);
			else
			{
				memcpy(chweek, "星期六", 6);
			}
				
			break;
		case 7:
			if (FormatType != 2)
				memcpy(chweek, "Sun", 4);
			else
			{
				memcpy(chweek, "星期日", 6);
			}
			break;
		}
		switch (FormatType)
		{
		case 0:
		{
			if (showWeek)
			{
				snprintf(chDate, DATELENGTH, "%2d/%02d/%d %s", month, day, year, chweek);
			}
			else
			{
				snprintf(chDate, DATELENGTH, "%2d/%02d/%02d", month, day, year);
			}
			break;
		}
		case 1:
		{
			if (showWeek)
			{
				snprintf(chDate, DATELENGTH, "%d/%02d/%02d %s", day ,month, year, chweek);
			}
			else
			{
				snprintf(chDate, DATELENGTH, "%d/%02d/%02d", day , month, year);
			}
			break;
		}
		case 2:
		{
			_TCHAR tempBuff[DATELENGTH];
			memset(tempBuff, 0, DATELENGTH);
			if (showWeek)
			{
				snprintf(chDate, DATELENGTH, "%d年%02d月%02d日 %s", year, month, day, chweek);
			}
			else
			{
				snprintf(chDate, DATELENGTH, "%d年%02d月%02d日", year, month, day);
			}
			break;
		}
		case 3:
		{
			if (showWeek)
			{
				snprintf(chDate, DATELENGTH, "%d/%02d/%02d %s", year, month, day, chweek);
			}
			else
			{
				snprintf(chDate, DATELENGTH, "%d/%02d/%02d", year, month, day);
			}
			break;
		}
		}
		return std::string(chDate);
	}
	bool DateView::HandleOperatePerm()
	{
		shared_ptr<DateModel> model = BaseView.GetModel<DateModel>();
		shared_ptr<DateControl> ctrl = BaseView.GetControl<DateControl>();
		if ((model->DateConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->DateConfig.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			if (model->DateConfig.Perm.IsHideElement)			//隐藏原件
			{
				model->DateConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->DateConfig.Perm.IsPopwin)				//弹出提示窗口
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		return true;
	}
	void DateView::draw()
	{
		if (!HandleOperatePerm())		//没有权限则返回
			return;
		shared_ptr<DateControl> ctrl = BaseView.GetControl<DateControl>();
		shared_ptr<DateModel> model = BaseView.GetModel<DateModel>();

		FinX = model->DateConfig.X + model->DateConfig.OffX;
		FinY = model->DateConfig.Y + model->DateConfig.OffY;
		
		flImgPtr = IResourceService::Ins()->GetImage(model->DateConfig.BackUpImg.KeyVal);
		//优化思路：后期可以只重绘字符串而跳过背景
		if (flImgPtr != nullptr)
		{
			image(flImgPtr);
			align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
			box(FL_NO_BOX);
			draw_box();
		}

		GraphicDrawHandle::PushClip(FinX, FinY, model->DateConfig.Width, model->DateConfig.Height);
		/*设置字体*/
		fl_font(model->DateConfig.DateFontStyle.Font.Style, model->DateConfig.DateFontStyle.Font.Size);
		/*设置字体颜色*/
		fl_color(active() ? fl_rgb_color(model->DateConfig.DateFontStyle.RGBColor.R, model->DateConfig.DateFontStyle.RGBColor.G, model->DateConfig.DateFontStyle.RGBColor.B): fl_inactive(fl_rgb_color(model->DateConfig.DateFontStyle.RGBColor.R, model->DateConfig.DateFontStyle.RGBColor.G, model->DateConfig.DateFontStyle.RGBColor.B)));
		//获取文本
		std::string strDate = GetCurrentDate(model->DateConfig.DateFormat, model->DateConfig.ShowFullYear,model->DateConfig.ShowWeek);

		UI::IResourceService::GB2312toUtf8(strDate);

		int len = strDate.length();
		int strmidx, strmidy = model->DateConfig.DateFontStyle.Font.Size / 2;//字符串中点位置（以左上角为0,0）
		strmidx = len * model->DateConfig.DateFontStyle.Font.Size / 4;
		int dx = FinX + model->DateConfig.Width / 2 - strmidx;
		int dy = FinY + model->DateConfig.Height / 2 + strmidy;
		//fl_draw(strDate.c_str(), dx, dy);
		fl_draw(strDate.c_str(), FinX, FinY, model->DateConfig.Width, model->DateConfig.Height, model->DateConfig.DateFontStyle.Align);
		fl_pop_clip();

	}
}
