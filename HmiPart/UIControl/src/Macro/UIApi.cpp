#include "GraphicDrawHandle.h"
#include "UIApi.h"
#include "UIManager.h"
#include "HMIWindow.h"
#include "Logger.h"
#include "../../include/GFuncControl.h"
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
	void UIApi::DCMapClear(unsigned int DCMapId)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
		std::shared_ptr<GFuncControl> ctrl = std::static_pointer_cast<GFuncControl>(win->GetCtrlById((int)DCMapId));
		if (ctrl)
		{
			ctrl->AddrDrawTask(Project::DCMapClear, NULL, 0);
		}
	}
	void UIApi::DCMapSetBackColor(unsigned int DCMapId, unsigned int backColor)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
		std::shared_ptr<GFuncControl> ctrl = std::static_pointer_cast<GFuncControl>(win->GetCtrlById((int)DCMapId));
		if (ctrl)
		{
			ctrl->AddrDrawTask(Project::DCMapSetBackColor, (int*)&backColor, 1);
		}
	}
	void UIApi::DCMapDrawLine(unsigned int DCMapId, int x1, int y1, int x2, int y2, int linewidth, unsigned int color)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
		std::shared_ptr<GFuncControl> ctrl = std::static_pointer_cast<GFuncControl>(win->GetCtrlById((int)DCMapId));
		if (ctrl)
		{
			int arr[6];
			arr[0] = x1;
			arr[1] = y1;
			arr[2] = x2;
			arr[3] = y2;
			arr[4] = linewidth;
			arr[5] = color;
			ctrl->AddrDrawTask(Project::DCMapDrawLine, arr, 6);
		}
	}
	void UIApi::DCMapDrawRect(unsigned int DCMap, int x, int y, int width, int height, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
		std::shared_ptr<GFuncControl> ctrl = std::static_pointer_cast<GFuncControl>(win->GetCtrlById((int)DCMap));
		if (ctrl)
		{
			int arr[8];
			arr[0] = x;
			arr[1] = y;
			arr[2] = width;
			arr[3] = height;
			arr[4] = linewidth;
			arr[5] = color;
			arr[6] = fillrect?1:0;
			arr[7] = fillColor;
			ctrl->AddrDrawTask(Project::DCMapDrawRect, arr, 8);
		}
	}
	void UIApi::DCMapDrawCircle(unsigned int DCMap, int x, int y, int radius, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
		std::shared_ptr<GFuncControl> ctrl = std::static_pointer_cast<GFuncControl>(win->GetCtrlById((int)DCMap));
		if (ctrl)
		{
			int arr[7];
			arr[0] = x;
			arr[1] = y;
			arr[2] = radius;
			arr[3] = linewidth;
			arr[4] = color;
			arr[5] = fillrect ? 1 : 0;
			arr[6] = fillColor;
			ctrl->AddrDrawTask(Project::DCMapDrawCircle, arr, 7);
		}
	}
	void UIApi::DCMapDrawEllipse(unsigned int DCMap, int x, int y, int xAxisLen, int yAxisLen, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
		std::shared_ptr<GFuncControl> ctrl = std::static_pointer_cast<GFuncControl>(win->GetCtrlById((int)DCMap));
		if (ctrl)
		{
			int arr[8];
			arr[0] = x;
			arr[1] = y;
			arr[2] = xAxisLen;
			arr[3] = yAxisLen;
			arr[4] = linewidth;
			arr[5] = color;
			arr[6] = fillrect ? 1 : 0;
			arr[7] = fillColor;
			ctrl->AddrDrawTask(Project::DCMapDrawEllipse, arr, 8);
		}
	}
	void UIApi::DCMapDrawCircleArc(unsigned int DCMap, int x, int y, int radius, int linewidth, unsigned int color, int startAngle, int endAngle)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
		std::shared_ptr<GFuncControl> ctrl = std::static_pointer_cast<GFuncControl>(win->GetCtrlById((int)DCMap));
		if (ctrl)
		{
			int arr[7];
			arr[0] = x;
			arr[1] = y;
			arr[2] = radius;
			arr[3] = linewidth;
			arr[4] = color;
			arr[5] = startAngle;
			arr[6] = endAngle;
			ctrl->AddrDrawTask(Project::DCMapDrawCircleArc, arr, 7);
		}
	}
	void UIApi::DCMapDrawEllipseArc(unsigned int DCMap, int x, int y, int xAxisLen, int yAxisLen, int linewidth, unsigned int color, int startAngle, int endAngle)
	{
		HMIWindow* win = UIManager::Ins().CurrWin();
		if (!win)
		{
			LOG_ERROR("Can not find window!");
			return;
		}
		std::shared_ptr<GFuncControl> ctrl = std::static_pointer_cast<GFuncControl>(win->GetCtrlById((int)DCMap));
		if (ctrl)
		{
			int arr[8];
			arr[0] = x;
			arr[1] = y;
			arr[2] = xAxisLen;
			arr[3] = yAxisLen;
			arr[4] = linewidth;
			arr[5] = color;
			arr[6] = startAngle;
			arr[7] = endAngle;
			ctrl->AddrDrawTask(Project::DCMapDrawEllipseArc, arr, 8);
		}

		// »æÖÆ±ß¿ò
		
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



