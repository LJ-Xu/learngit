/*******************************************************************************
 * Copyright (C) 2011-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * FileName	: HmiRenderLib.cpp
 * Author	: zhanglinbo
 * Descript	: 用于PC机上进行部件渲染,该代码只在上位机上使用
 * Version	: 0.1
 * Modify	:
 *			:
 *******************************************************************************/
#define HMI_PC_RENDER
#include <string.h>
#include <comdef.h>
#include <gdiplus.h>

using namespace Gdiplus;
#pragma  comment(lib, "gdiplus.lib")
#include "stdafx.h"
#include <Windows.h>
#include <math.h>
#include <map>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/fl_draw.h>

#include "BaseControl.h"
#include "ControlFactory.h"
#include "PcResourceService.h"
#include "zint.h"
#include "BarCodeConfig.h"
#include "BinBitmap.h"
using namespace std;
extern HDC fl_gc;
extern string GetVersionInfo();
static map<string, std::shared_ptr<UI::BaseControl>> controlMap;


/*****************************图片资源************************************/
//static std::map<std::string, Fl_Image*> resMap;
extern "C"  __declspec(dllexport) Fl_Image*  __stdcall  NewImage(char* data,int size)
{
	HMIImage* image = (HMIImage*) new UI::BinBitmap(data, size);
	return image;
}
extern "C"  __declspec(dllexport) void  __stdcall  ReleaseImage(Fl_Image* data)	
{
	if (data)
		delete data;
}
/*****************************图片资源************************************/
extern "C"  __declspec(dllexport) void  __stdcall  DrawString(
	HDC hdc, 
	TCHAR* text, 
	TCHAR* fontname,
	int fontsize,
	int clr,
	int x,
	int y,int w,int h)
{	
	Gdiplus::Color color(clr);
	Gdiplus::FontFamily fontFamily(fontname);
	Gdiplus::Font       font(&fontFamily, fontsize, Gdiplus::FontStyleRegular, Gdiplus::Unit::UnitPoint);
	Gdiplus::SolidBrush brush(color);
	Gdiplus::Graphics   graphics(hdc);
	PointF pointF(x, y);
	Gdiplus::Rect rect;
	rect.X = x;
	rect.Y = y;
	rect.Width = w;
	rect.Height = h;
	Gdiplus::Region region;
	region.MakeEmpty();
	graphics.GetClip(&region);
	if (!region.IsEmpty(&graphics))
		graphics.SetClip(rect);
	graphics.DrawString(text, (INT)wcslen(text), &font, pointF, &brush);
	if(!region.IsEmpty(&graphics))
		graphics.SetClip(&region);
}

#define WIN_W 1000
#define WIN_H 1000
static void SetDCParam(HDC dc, float zoom)
{
	SetGraphicsMode(dc, GM_ADVANCED); // to allow for rotations
	SetMapMode(dc, MM_ANISOTROPIC);
	SetTextAlign(dc, TA_BASELINE | TA_LEFT);
	SetBkMode(dc, TRANSPARENT);

	SetMapMode(dc, MM_TEXT);
	//先继承MM_TEXT的属性
	SetMapMode(dc, MM_ANISOTROPIC);
	SetWindowExtEx(dc, WIN_W, WIN_H, NULL);
	SetViewportExtEx(dc, (int)(zoom * WIN_W), (int)(zoom * WIN_H), NULL);
	SetViewportOrgEx(dc, 0, 0, NULL);


}
void ResetDCParam(HDC dc)
{
	SetWindowExtEx(dc, WIN_W, WIN_H, NULL);
	SetViewportExtEx(dc, WIN_W, WIN_H, NULL);
}
static void copy_offscreen(int x, int y, int w, int h, HBITMAP bitmap, int srcx, int srcy) {
	HDC new_gc = CreateCompatibleDC(fl_gc);
	int save = SaveDC(new_gc);
	SelectObject(new_gc, bitmap);
	BitBlt(fl_gc, x, y, w, h, new_gc, srcx, srcy, SRCCOPY);
	RestoreDC(new_gc, save);
	DeleteDC(new_gc);
}
extern "C"  __declspec(dllexport) int  __stdcall SetClip(HDC hDC, int x, int y, int w, int h)
{
	HRGN rg = CreateRectRgn(x, y, x + w, y + h);
	SelectClipRgn(hDC, rg);
	DeleteObject(rg);
	return 0;
}
extern "C"  __declspec(dllexport) int  __stdcall ControlRenderBG(HDC hDC, HBITMAP hbmp, int x, int y, int w, int h)
{
	//ghbmp = hbmp;
	fl_gc = hDC;  //切换绘画句柄
	/*SetDCParam(hDC, 1);
	fl_push_clip(x, y, w, h);*/
	copy_offscreen(x, y, w, h, hbmp, x, y);
	/*fl_pop_clip();
	ResetDCParam(hDC);*/
	return 0;
}

//extern "C"  __declspec(dllexport) int  __stdcall ControlRenderEx(HDC hDC,int x,int y)
//{
//	HDC tp = fl_gc;
//	fl_gc = hDC;  //切换绘画句柄
//	SetGraphicsMode(fl_gc, GM_ADVANCED); // to allow for rotations
//	SetMapMode(fl_gc, MM_ANISOTROPIC);
//	SetTextAlign(fl_gc, TA_BASELINE | TA_LEFT);
//	SetBkMode(fl_gc, TRANSPARENT);
//	SetDCParam(hDC, 1); 
//	if(0)
//	{
//		HBRUSH color = CreateSolidBrush(0xFE0000FF);
//		RECT rect;
//		rect.left = 0; rect.top = 0;
//		rect.right = 373+10; rect.bottom =279+100;
//		FillRect(fl_gc, &rect, color);
//	}
//
//	UI::GTestControl* ctrl = new UI::GTestControl();
//	ctrl->CreateView();
//	ctrl->Render();
//	ResetDCParam(hDC);
//	fl_gc = tp;
//	return 0;
//}
extern "C"  __declspec(dllexport) int  __stdcall ControlRender(HDC hDC,
	float zoom,
	char* name,
	char* json)
{
	std::shared_ptr<UI::BaseControl> ctr;
	if (controlMap.count(name) == 0)
	{
		ctr = UI::ControlFactory::GetNewControlByNameEx(name);
		if (ctr == nullptr)
			return -1;
		controlMap.insert(pair<string, std::shared_ptr<UI::BaseControl>>(name, ctr));
	}
	else
		ctr = controlMap[name];

	UI::IResourceService::SetIns(static_cast<UI::IResourceService*>(UI::PcResourceService::Ins()));

	ctr->CreateView(json);
	fl_gc = hDC;  //切换绘画句柄	
	SetDCParam(hDC, zoom);
	ctr->Render();
	ResetDCParam(hDC);
	UI::PcResourceService::Ins()->ReleasePCImg();
	return 0;
}
extern "C"  __declspec(dllexport) void  __stdcall ReleaseControl(void* ctrlptr)
{
	delete (std::shared_ptr<UI::BaseControl>*)ctrlptr;
}

#pragma pack(push,1)
struct RenderInfo
{
	float Zoom;//缩放比例
	int X;
	int Y;
	int Width;
	int Height;
	char Flag; //1 数据发生变化
};
#pragma pack(pop)
struct Rect
{ 
	int X;
	int Y;
	int Width;
	int Height;
};
extern "C"  __declspec(dllexport) void  __stdcall BeginRender(HDC hDC, float zoom)
{
	UI::IResourceService::SetIns(static_cast<UI::IResourceService*>(UI::PcResourceService::Ins()));
	fl_gc = hDC;  //切换绘画句柄
	SetDCParam(hDC, zoom);
	UI::IResourceService::Ins()->SetZoom(zoom);

}
extern "C"  __declspec(dllexport) void  __stdcall EndRender(HDC hDC)
{
	ResetDCParam(hDC);
	UI::PcResourceService::Ins()->ReleasePCImg(); //释放临时缓存的图片对象
}
//struct BGConfig
//{
//	::Rect Rct1;
//	::Rect Rct2;
//	int Color1;
//	int Color2;
//	int PtColor; 
//	int IV; //
//	int HV;
//};

extern "C"  __declspec(dllexport) void  __stdcall RenderBg(HDC hDC, ::Rect rect1, ::Rect rect2,int color1,int color2)
{
	fl_color(0xFF0000FF);
	fl_rectf(rect1.X, rect1.Y, rect1.Width, rect1.Height);
	fl_color(0xFFFFFFFF);
	fl_rectf(rect2.X, rect2.Y, rect2.Width, rect2.Height);
}
static RenderInfo RI;
extern "C"  __declspec(dllexport) void  __stdcall BeginRenderEx(HDC hDC, RenderInfo info)
{
	UI::IResourceService::SetIns(static_cast<UI::IResourceService*>(UI::PcResourceService::Ins()));
	fl_gc = hDC;  //切换绘画句柄
	SetDCParam(hDC, info.Zoom);
	//fl_push_clip(info.X, info.Y, info.Width, info.Height);
	RI = info;
	SetClip(hDC, RI.X, RI.Y, RI.Width, RI.Height);
	RI = info;
	UI::IResourceService::Ins()->SetZoom(info.Zoom);
    /*fl_color(0xFF0000FF);
	fl_rectf(info.X, info.Y, info.Width, info.Height);*/

}
extern "C"  __declspec(dllexport) void  __stdcall EndRenderEx(HDC hDC)
{
	ResetDCParam(hDC);
	//fl_pop_clip();
	UI::PcResourceService::Ins()->ReleasePCImg(); //释放临时缓存的图片对象
}
extern "C"  __declspec(dllexport) void*  __stdcall ControlRenderEx(
	HDC hDC,//画布句柄
	void* ctrlptr,//控件指针
	RenderInfo info,
	char* name,//名称
	char* json)
{
	SetClip(hDC, RI.X, RI.Y, RI.Width, RI.Height);
	std::shared_ptr<UI::BaseControl>* ctr = (std::shared_ptr<UI::BaseControl>*)ctrlptr;
	if (ctr != nullptr && !info.Flag)
	{
		//表示当前只是坐标发生变化
		(*ctr)->SetRect(info.X, info.Y, info.Width, info.Height);
	}
	else
	{
		if (ctr == nullptr)
		{
			ctr = new std::shared_ptr<UI::BaseControl>(UI::ControlFactory::GetNewControlByNameEx(name).release());
			if (ctr == nullptr)
				return nullptr;

		}
		(*ctr)->CreateView(json);
	}
	(*ctr)->Render();
	return ctr;
}
extern "C"  __declspec(dllexport) void  __stdcall ReleaseControlEx(void* ctrlptr)
{
	if(ctrlptr)
		delete (std::shared_ptr<UI::BaseControl>*)ctrlptr;
}
/*
extern "C"  __declspec(dllexport) void  __stdcall
			InitResourceAPI(UI::GetImageFileByNameFunc* func)
{
	UI::ResFuncs resFuncs = { 0 };
	resFuncs.GetImageFileByName = func;
	UI::ResourceService::Ins().SetResFuncs(resFuncs);
	UI::ResourceService::Ins().Init();
}
*/
extern "C"  __declspec(dllexport) void  __stdcall InitResourceAPI(UI::ResFuncs resFuncs)
{

	UI::PcResourceService::Ins()->SetResFuncs(resFuncs);
	UI::PcResourceService::Ins()->Init();

}
extern "C"  __declspec(dllexport) void  __stdcall SetStatusLang(short status, short lang)
{
	UI::PcResourceService::Ins()->SetLang(lang);
	UI::PcResourceService::Ins()->SetStatus(status);
}
extern "C"  __declspec(dllexport) double __stdcall GetStringWidth(HDC hDC, char *font, int size, char *txt)
{
	int fontIdx = 0, i = 0, k;
	double w = 0;
	HDC oldhdc = fl_gc;
	fl_gc = hDC;
	k = Fl::set_fonts(i ? (i > 1 ? "*" : 0) : "-*");
	for (i = 0; i < k; i++)
	{
		int t;
		const char *s = Fl::get_font_name((Fl_Font)i, &t);
		if (!font)
			fontIdx = 0;
		if (strstr(s, font)) {
			fontIdx = i;
			break;
		}
	}

	fl_font(fontIdx, size);
	string::size_type idx;
	string tmpstr = txt;
	if (tmpstr.find("\n") == string::npos)
	{
#ifdef WIN32
		/*中文转宽字符*/
		size_t len = strlen(txt) + 1;
		size_t wlen = MultiByteToWideChar(CP_ACP, 0, (const char*)txt, int(len), NULL, 0);
		std::unique_ptr<wchar_t> wc{ new wchar_t[wlen + 1] };
		std::unique_ptr<char> utf8{ new char[(wlen + 1) * 3] };
		memset(wc.get(), 0, wlen + 1);
		MultiByteToWideChar(CP_ACP, 0, (const char*)txt, int(len), wc.get(), int(wlen));
		fl_utf8fromwc(utf8.get(), (wlen + 1) * 3, wc.get(), wcslen(wc.get()));	//const char *label = utf8;
		w = fl_width((const char*)utf8.get(), strlen(utf8.get()));
#elif
		const char *label = txt;
		w = fl_width(label, strlen(label));
#endif
	}
	else
	{
		char* subtxt;
		char str[512] = { 0 };
		memcpy(str, txt, strlen(txt));
		subtxt = strtok(str, "\n");
		while (subtxt != NULL) {
#ifdef WIN32

			size_t len = strlen(subtxt) + 1;
			size_t wlen = MultiByteToWideChar(CP_ACP, 0, (const char*)subtxt, int(len), NULL, 0);
			std::unique_ptr<wchar_t> wc{ new wchar_t[wlen + 1] };
			std::unique_ptr<char> utf8{ new char[(wlen + 1) * 3] };
			memset(wc.get(), 0, wlen + 1);
			MultiByteToWideChar(CP_ACP, 0, (const char*)subtxt, int(len), wc.get(), int(wlen));
			fl_utf8fromwc(utf8.get(), (wlen + 1) * 3, wc.get(), wcslen(wc.get()));	//const char *label = utf8;
			if (w < fl_width((const char*)utf8.get(), strlen(utf8.get())))
				w = fl_width((const char*)utf8.get(), strlen(utf8.get()));
#elif
			const char *label = subtxt;
			w = fl_width(label, strlen(label));
#endif
			subtxt = strtok(NULL, "\n");
		}
	}
	fl_gc = oldhdc;
	return w;
}

extern "C"  __declspec(dllexport) int __stdcall GetStringHeight(HDC hDC, char * font, int size)
{
	int fontIdx = 0, i = 0, k;
	HDC oldhdc = fl_gc;
	fl_gc = hDC;
	k = Fl::set_fonts(i ? (i > 1 ? "*" : 0) : "-*");
	for (i = 0; i < k; i++)
	{
		int t;
		const char *s = Fl::get_font_name((Fl_Font)i, &t);
		if (!font)
			fontIdx = 0;
		if (strstr(s, font)) {
			fontIdx = i;
			break;
		}
	}

	fl_font(fontIdx, size);
	int h = fl_height();
	fl_gc = oldhdc;
	return h;

}

//验证二维码参数合法性
//返回错误码
extern "C"  __declspec(dllexport) int __stdcall VerifyBarCodeParam(int dimensionType,bool twodimen,char *codevalue, int size)
{
	int ret = 0;
	struct zint_symbol * symbol;
	symbol = ZBarcode_Create();
	if (symbol == nullptr)
		return -1;
	if (twodimen)
	{
		switch (dimensionType)
		{
		case Project::BarCodeConfig::QRCode:
			symbol->symbology = BARCODE_QRCODE;
			break;
		case Project::BarCodeConfig::DataMatrix:
			symbol->symbology = BARCODE_DATAMATRIX;
			break;
		case Project::BarCodeConfig::PDF417:
			symbol->symbology = BARCODE_PDF417;
			break;
		}
	}
	else
	{
		switch (dimensionType)
		{
		case Project::BarCodeConfig::EAN13:
			symbol->symbology = BARCODE_EAN14;
			break;
		case Project::BarCodeConfig::Code39:
			symbol->symbology = BARCODE_CODE39;
			break;
		case Project::BarCodeConfig::Code128:
			symbol->symbology = BARCODE_CODE128;
			break;
		}
	}
	ret = ZBarcode_Encode(symbol, (unsigned char *)codevalue, size);
	return ret;
}

extern "C"  __declspec(dllexport)const int __stdcall GetHmiVersion(char *ver)
{
	string version = GetVersionInfo();
	//char ver[20] = {0};
	//memset()
	memcpy(ver, version.c_str(),version.size());
	return 0;
}