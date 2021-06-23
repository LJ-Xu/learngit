#include "GraphicDrawHandle.h"
#include "UIApi.h"
#include "UIManager.h"
#include "HMIWindow.h"
#include "Logger.h"
using namespace std;
namespace UI
{
	void UIApi::Draw(int id, APIParam* parma)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		std::shared_ptr<BaseControl> ctrl = win->GetCtrlById(id);
		if (ctrl)
		{			
			win->SendAutoFreeMessage<APIParam>((void*)id, WM_EVENT_UI_FUNC, parma);
		}
	}

	void UIApi::DrawCircle(int x, int y, int r)
	{
		cout << "Client DrawCircle" << endl;
	}
	void UIApi::DCMapClear(unsigned int DCMapId, unsigned int backColor)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
#if 1
		GraphicDrawHandle::PushClip(0,
			0,
			win->w(),
			win->h());
		// 设置背景色
		fl_color(RGBColor(backColor));
		// 绘制画布背景颜色
		fl_rectf(0,
			0,
			win->w(),
			win->h());
		fl_pop_clip();
#else

		std::shared_ptr<BaseControl> ctrl = win->GetCtrlById((int)DCMapId);
		if (ctrl)
		{
			UI::Rectangle rect;
			ctrl->GetMode()->GetRect(rect);
			GraphicDrawHandle::PushClip(rect.X,
				rect.Y,
				rect.W,
				rect.H);
			// 设置原始背景色
	/*		Fl_Color originColor = fl_rgb_color(255, 255, 255);
			fl_color(originColor);*/

			fl_color(RGBColor(backColor));
			// 绘制画布背景颜色
			fl_rectf(rect.X,
				rect.Y,
				rect.W,
				rect.H);
			fl_pop_clip();
			//win->SendAutoFreeMessage<APIParam>((void*)id, WM_EVENT_UI_FUNC, parma);
		}
		else
		{
			GraphicDrawHandle::PushClip(win->x(),
				win->y(),
				win->w(),
				win->h());
			// 设置背景色
			fl_color(RGBColor(backColor));
			// 绘制画布背景颜色
			fl_rectf(win->x(),
				win->y(),
				win->w(),
				win->h());
			fl_pop_clip();
		}
#endif
	}
	void UIApi::DCMapSetBackColor(unsigned int DCMapId, unsigned int backColor)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
#if 1
		GraphicDrawHandle::PushClip(0,
			0,
			win->w(),
			win->h());
		// 设置背景色
		fl_color(RGBColor(backColor));
		// 绘制画布背景颜色
		fl_rectf(0,
			0,
			win->w(),
			win->h());
		fl_pop_clip();
#else
		std::shared_ptr<BaseControl> ctrl = win->GetCtrlById((int)DCMapId);
		if (ctrl)
		{
			UI::Rectangle rect;
			ctrl->GetMode()->GetRect(rect);
			GraphicDrawHandle::PushClip(rect.X,
				rect.Y,
				rect.W,
				rect.H);
			// 设置背景色
			fl_color(RGBColor(backColor));
			// 绘制画布背景颜色
			fl_rectf(rect.X,
				rect.Y,
				rect.W,
				rect.H);
			fl_pop_clip();
		}
		else
		{
			GraphicDrawHandle::PushClip(win->x(),
				win->y(),
				win->w(),
				win->h());
			// 设置背景色
			fl_color(RGBColor(backColor));
			// 绘制画布背景颜色
			fl_rectf(win->x(),
				win->y(),
				win->w(),
				win->h());
			fl_pop_clip();
		}
#endif
	}
	void UIApi::DCMapDrawLine(unsigned int DCMapId, int x1, int y1, int x2, int y2, int linewidth, unsigned int color)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
#if 1
		GraphicDrawHandle::PushClip(0,
			0,
			win->w(),
			win->h());
		// 绘制线条
		if (linewidth > 0) {
			// 设置线条样式
			fl_line_style(0, linewidth);
			// 设置线条颜色
			fl_color(RGBColor(color));
			// 绘制线条
			fl_line(x1,
				y1,
				x2,
				y2);
		}
		fl_pop_clip();
#else
		std::shared_ptr<BaseControl> ctrl = win->GetCtrlById((int)DCMapId);
		if (ctrl)
		{
			UI::Rectangle rect;
			ctrl->GetMode()->GetRect(rect);

			GraphicDrawHandle::PushClip(rect.X,
				rect.Y,
				rect.W,
				rect.H);
			// 绘制线条
			if (linewidth > 0) {
				// 设置线条样式
				fl_line_style(0, linewidth);
				// 设置线条颜色
				fl_color(RGBColor(color));
				// 绘制线条
				fl_line(rect.X+x1,
					rect.Y+y1,
					rect.X + x2,
					rect.Y + y2);
			}
			fl_pop_clip();
		}
		else
		{
			/*HDC new_gc = CreateCompatibleDC(fl_gc);
			int save = SaveDC(new_gc);
			SelectObject(new_gc, bitmap);
			BitBlt(fl_gc, x, y, w, h, new_gc, srcx, srcy, SRCCOPY);
			RestoreDC(new_gc, save);
			DeleteDC(new_gc);*/
		}
#endif
	}
	void UIApi::DCMapDrawRect(unsigned int DCMap, int x, int y, int width, int height, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
#if 1
		GraphicDrawHandle::PushClip(0,
			0,
			win->w(),
			win->h());//填充
		if (fillrect)
		{
			fl_color(RGBColor(fillColor));
			fl_rectf(x, y, width, height);
		}
		// 绘制线条
		if (linewidth > 0) {
			// 设置线条样式
			fl_line_style(0, linewidth);
			// 设置线条颜色
			fl_color(RGBColor(color));
			// 绘制线条
			fl_rect(x,
				y,
				width,
				height);
		}
		fl_pop_clip();
#else
		std::shared_ptr<BaseControl> ctrl = win->GetCtrlById((int)DCMap);
		if (ctrl)
		{
			UI::Rectangle rect;
			ctrl->GetMode()->GetRect(rect);

			GraphicDrawHandle::PushClip(rect.X,
				rect.Y,
				rect.W,
				rect.H);
			//填充
			if (fillrect)
			{
				fl_color(RGBColor(fillColor));
				fl_rectf(rect.X + x, rect.Y + y, width, height);
			}
			// 绘制线条
			if (linewidth > 0) {
				// 设置线条样式
				fl_line_style(0, linewidth);
				// 设置线条颜色
				fl_color(RGBColor(color));
				// 绘制线条
				fl_rect(x,
					y,
					width,
					height);
			}
			fl_pop_clip();
		}
		else
		{

		}
#endif
	}
	void UIApi::DCMapDrawCircle(unsigned int DCMap, int x, int y, int radius, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
#if 1
		GraphicDrawHandle::PushClip(0,
			0,
			win->w(),
			win->h());
		//填充
		// 是否填充
		if (fillrect)
		{
			// 设置填充颜色
			fl_color(RGBColor(fillColor));
			// 填充内部
			fl_pie(x, y,
				radius * 2,
				radius * 2, 0, 360);
		}
		// 绘制边框
		if (linewidth > 0) {
			// 设置线条样式
			fl_line_style(0, linewidth);
			// 设置线条颜色
			fl_color(RGBColor(color));
			// 绘制边框
			fl_arc(x, y,
				radius * 2,
				radius * 2, 0, 360);
		}
		fl_pop_clip();
#else
		std::shared_ptr<BaseControl> ctrl = win->GetCtrlById((int)DCMap);
		if (ctrl)
		{
			UI::Rectangle rect;
			ctrl->GetMode()->GetRect(rect);
			GraphicDrawHandle::PushClip(rect.X,
				rect.Y,
				rect.W,
				rect.H);
			// 是否填充
			if (fillrect)
			{
				// 设置填充颜色
				fl_color(RGBColor(fillColor));
				// 填充内部
				fl_pie(rect.X + x, rect.Y + y,
					radius * 2,
					radius * 2, 0, 360);
			}
			// 绘制边框
			if (linewidth > 0) {
				// 设置线条样式
				fl_line_style(0, linewidth);
				// 设置线条颜色
				fl_color(RGBColor(color));
				// 绘制边框
				fl_arc(rect.X + x, rect.Y + y,
					radius * 2,
					radius * 2, 0, 360);
			}
			fl_pop_clip();
		}
		else
		{

		}
#endif
	}
	void UIApi::DCMapDrawEllipse(unsigned int DCMap, int x, int y, int xAxisLen, int yAxisLen, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
#if 1
		GraphicDrawHandle::PushClip(0,
			0,
			win->w(),
			win->h());
		// 是否填充
		if (fillrect)
		{
			// 设置填充颜色
			fl_color(RGBColor(fillColor));
			// 填充内部
			fl_pie(x, y,
				xAxisLen * 2,
				yAxisLen * 2, 0, 360);
		}
		// 绘制边框
		if (linewidth > 0) {
			// 设置线条样式
			fl_line_style(0, linewidth);
			// 设置线条颜色
			fl_color(RGBColor(color));
			// 绘制边框
			fl_arc(x, y,
				xAxisLen * 2,
				yAxisLen * 2, 0, 360);
		}
		fl_pop_clip();
#else
		std::shared_ptr<BaseControl> ctrl = win->GetCtrlById((int)DCMap);
		if (ctrl)
		{
			UI::Rectangle rect;
			ctrl->GetMode()->GetRect(rect);
			GraphicDrawHandle::PushClip(rect.X,
				rect.Y,
				rect.W,
				rect.H);
			// 是否填充
			if (fillrect)
			{
				// 设置填充颜色
				fl_color(RGBColor(fillColor));
				// 填充内部
				fl_pie(rect.X + x, rect.Y + y,
					xAxisLen * 2,
					yAxisLen * 2, 0, 360);
			}
			// 绘制边框
			if (linewidth > 0) {
				// 设置线条样式
				fl_line_style(0, linewidth);
				// 设置线条颜色
				fl_color(RGBColor(color));
				// 绘制边框
				fl_arc(rect.X + x, rect.Y + y,
					xAxisLen * 2,
					yAxisLen * 2, 0, 360);
			}
			fl_pop_clip();
		}
		else
		{

		}
#endif
	}
	void UIApi::DCMapDrawCircleArc(unsigned int DCMap, int x, int y, int radius, int linewidth, unsigned int color, int startAngle, int endAngle)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
		GraphicDrawHandle::PushClip(0,
			0,
			win->w(),
			win->h());
		// 绘制边框
		if (linewidth > 0) {
			// 设置线条样式
			fl_line_style(0, linewidth);
			// 设置线条颜色
			fl_color(RGBColor(color));
			// 绘制边框
			fl_arc(x, y,
				radius * 2,
				radius * 2, startAngle, endAngle);
		}
		fl_pop_clip();
	}
	void UIApi::DCMapDrawEllipseArc(unsigned int DCMap, int x, int y, int xAxisLen, int yAxisLen, int linewidth, unsigned int color, int startAngle, int endAngle)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
		GraphicDrawHandle::PushClip(0,
			0,
			win->w(),
			win->h());
		// 绘制边框
		if (linewidth > 0) {
			// 设置线条样式
			fl_line_style(0, linewidth);
			// 设置线条颜色
			fl_color(RGBColor(color));
			// 绘制边框
			fl_arc(x, y,
				xAxisLen * 2,
				yAxisLen * 2, startAngle, endAngle);
		}
		fl_pop_clip();
	}
	void UIApi::OpenWindow(unsigned short winNo, unsigned short winX, unsigned short winY)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();	
		win->Invoke([winNo,winX,winY](void* ) {
			HMIWindow* win = UIManager::Ins().CurrWin();
			win->SwitchPage(winNo, winX, winY);
		},nullptr);
	}
	void UIApi::CloseWindow(unsigned short winNo)
	{	
		HMIWindow* win = UIManager::Ins().CurrWin();
		win->Invoke([winNo](void* ) {
			HMIWindow* win = UIManager::Ins().CurrWin();
			win->ClosePage(winNo);
		}, 0);
	}
	void UIApi::ScreenJump(unsigned short screenNo)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		win->Invoke([screenNo](void* ) {
			HMIWindow* win = UIManager::Ins().CurrWin();
			((HMIWindow*) win)->SwitchPage(screenNo);
		},0);
		
	}
}



