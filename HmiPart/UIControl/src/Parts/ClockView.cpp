#include "GraphicDrawHandle.h"
#include "ClockView.h"
#include "ResourceService.h"
#include "stdafx.h"
#include "UIComm.h"
#include "ClockControl.h"
#include "ClockModel.h"
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Widget.H>
#include "FL/fl_draw.H"
#include "System.h"
#include "HMIWindow.h"
#include "PermUtility.h"

#define TIMELENGTH 32
namespace UI
{
	ClockView::ClockView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H), flImgPtr(nullptr) 
	{
		/*wscnBuff = new wstring();
		wscnBuff->resize(TIMELENGTH);*/
	}
	ClockView::~ClockView() {}

	void ClockView::InitBackPic(void* model)
	{
		flImgPtr = nullptr;
		if (model == nullptr)
			return;
		ClockModel* pmod = (ClockModel*)model;
		if (pmod->ClockConfig.BackUpImg.KeyVal == "")
		{
			return;
		}

		flImgPtr = IResourceService::Ins()->GetImage(pmod->ClockConfig.BackUpImg.KeyVal);

	}

	std::string ClockView::GetCurrentClock(int FormatType, int HourType)
	{
		char chDate[TIMELENGTH];
		memset(chDate, 0, TIMELENGTH);
		int hour, minute, second,milsec;
		System::GetCurrentClock(hour, minute, second, milsec);
		int showHour = hour;
		char chsuffix[4];
		memset(chsuffix, 0, 4);
		if (!HourType)
		{
			if (hour > 12)
			{
				memcpy(chsuffix, " pm", 4);
				showHour = hour - 12;
			}
			else
				memcpy(chsuffix, " am", 4);
		}
		switch (FormatType)
		{
		case 0://HH:MM:SS
			sprintf(chDate, "%02d:%02d:%02d%s", showHour, minute, second, chsuffix);
			break;
		case 1://HH:MM
			sprintf(chDate, "%02d:%02d%s", showHour, minute, chsuffix);
			break;
		case 2://HH:MM:SS:
			sprintf(chDate, "%02d:%02d:%02d:%04d%s", showHour, minute, second, milsec,chsuffix);
			break;
		case 3:
			sprintf(chDate, "%02d时%02d分%02d秒%s", showHour, minute, second, chsuffix);
			/*_TCHAR tempBuff[TIMELENGTH];
			wsprintf(tempBuff, _T("%02d%s%02d%s%02d%s%s"), showHour, _T("时"), minute, _T("分"), second, _T("秒"), chsuffix);
			fl_utf8fromwc(chDate, TIMELENGTH, tempBuff, wcslen(tempBuff));*/
			/*

			fl_utf8fromwc(utf8, MAX, wscnBuff->c_str(), wscnBuff->size());
			Fl_Box *box = new Fl_Box(20, 40, 260, 100, utf8);

			box->box(FL_UP_BOX);
			box->labelsize(36);
			box->labelfont(FL_BOLD + FL_ITALIC);
			box->labeltype(FL_SHADOW_LABEL);
*/
			break;
		}
		return std::move(std::string(chDate));
	}
	bool ClockView::HandleOperatePerm()
	{
		shared_ptr<ClockModel> model = BaseView.GetModel<ClockModel>();
		shared_ptr<ClockControl> ctrl = BaseView.GetControl<ClockControl>();

		//UI::PermUtility::HandleShowPerm(model->ClockConfig.Perm, this);
		
		if ((model->ClockConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->ClockConfig.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			if (model->ClockConfig.Perm.IsHideElement)			//隐藏原件
			{
				model->ClockConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->ClockConfig.Perm.IsPopwin)				//弹出提示窗口
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		return true;
	}
	void ClockView::draw()
	{

		shared_ptr<ClockControl> ctrl = BaseView.GetControl<ClockControl>();
		shared_ptr<ClockModel> model = BaseView.GetModel<ClockModel>();
		FinX = model->ClockConfig.X+model->ClockConfig.OffX;
		FinY = model->ClockConfig.Y + model->ClockConfig.OffY;
		if (!HandleOperatePerm())		//没有权限则返回
			return;
		flImgPtr = IResourceService::Ins()->GetImage(model->ClockConfig.BackUpImg.KeyVal);
		//优化思路：后期可以只重绘字符串而跳过背景
		if (flImgPtr != nullptr)
		{
			image(flImgPtr);
			align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
			box(FL_NO_BOX);
			draw_box();
		}
		//fl_push_clip(FinX, FinY, model->ClockConfig.Width, model->ClockConfig.Height);
		GraphicDrawHandle::PushClip(FinX, FinY, model->ClockConfig.Width, model->ClockConfig.Height);
		/*设置字体*/
		fl_font(model->ClockConfig.ClockFontStyle.Font.Style, model->ClockConfig.ClockFontStyle.Font.Size);
		/*设置字体颜色*/
		fl_color(fl_rgb_color(model->ClockConfig.ClockFontStyle.RGBColor.R, model->ClockConfig.ClockFontStyle.RGBColor.G, model->ClockConfig.ClockFontStyle.RGBColor.B));
		//获取文本
		std::string strDate = GetCurrentClock(model->ClockConfig.TimeFormat, model->ClockConfig.HourType);
		UI::IResourceService::GB2312toUtf8(strDate);
		//fl_text_extents(strDate.c_str(), model->ClockConfig.X, model->ClockConfig.Y, model->ClockConfig.Width, model->ClockConfig.Height);
		int len = strDate.length();
		int strmidx, strmidy= model->ClockConfig.ClockFontStyle.Font.Size/2;//字符串中点位置（以左上角为0,0）
		strmidx = len* model->ClockConfig.ClockFontStyle.Font.Size / 4;
		int dx = FinX + model->ClockConfig.Width / 2 - strmidx;
		int dy = FinY + model->ClockConfig.Height / 2 + strmidy;
		fl_draw(strDate.c_str(), dx, dy);

		fl_pop_clip();
	}

}