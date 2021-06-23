/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PrintSetControl.h
 * Author   : qiaodan
 * Date     : 2021-04-14
 * Descript : 打印设置头文件
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "Sysdef.h"
#include "BaseGControl.h"
#include "PrintSetModel.h"
#include <BaseControl.h>
#include <vector>

using namespace std;

#ifdef __linux
#define LONG int
#include "printer.h"
#endif

namespace UI
{
	typedef struct {
		DWORD   bfSize;
		WORD    bfReserved1;
		WORD    bfReserved2;
		DWORD   bfOffBits;
	} BITMAPFILEHEADER;

	typedef struct {
		DWORD      biSize;
		LONG       biWidth;
		LONG       biHeight;
		WORD       biPlanes;
		WORD       biBitCount;
		DWORD      biCompression;
		DWORD      biSizeImage;
		LONG       biXPelsPerMeter;
		LONG       biYPelsPerMeter;
		DWORD      biClrUsed;
		DWORD      biClrImportant;
	} BITMAPINFOHEADER;
	void SaveBitmapPic(Fl_Image *picdata);

	class PrintSetControl : public BaseGControl
	{
	public:
		PrintSetControl() : PrintSetControl(nullptr) { }
		PrintSetControl(HMIPage *);
	public:
		void OnReady();
		void CreateView() override;
		int PeekHMIMessage(Message::Msg * msg);
		void HandleDataNotify(Project::DataVarId id);
		void HandleDataVar(Project::DataVarId &id);
		static void PrintWinPic(Fl_Widget *target);
		//Fl_Image *PrintImg;
	private:
		shared_ptr<PrintSetModel> mode_;
		bool perbitstatus_ = false;
#ifdef __linux
		static Print_Para *param_;
#endif // __linux

	};
}

