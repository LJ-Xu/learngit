/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PrintSetControl.cpp
 * Author   : qiaodan
 * Date     : 2021-3-23
 * Descript : 创建高级功能，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"

#include "PrintSetControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
#include "XJMacro.h"
#include "System.h"
#include <FL/Fl_Printer.H>
#include <FL/Fl_Copy_Surface.H>
#include <FL/Fl_Image_Surface.H>
#ifdef WIN32
#include <windows.h>
#include <process.h>
HANDLE PrintMutex;
#else
#include <pthread.h>
pthread_mutex_t PrintMutex;
#include <unistd.h>
#endif // WIN32
#include "Logger.h"
namespace UI
{
#ifdef __linux
	Print_Para *PrintSetControl::param_ = nullptr;
#endif // __linux
	PrintSetControl::PrintSetControl(HMIPage* w) :BaseGControl(w)
	{
		mode_ = shared_ptr<PrintSetModel>(new PrintSetModel());
		InitMVCModel(mode_);
		
#ifdef __linux
		/*初始化参数*/
		param_->Printer_Type = Printer_Type::PType_UART;
		param_->Print_Dir = mode_->PrintConfig.PrintDirection;
		param_->IsCut = 0;
		//param_->uart.dev = mode_->PrintConfig.ComName;
        strcpy(param_->uart.dev, mode_->PrintConfig.ComName.c_str());
		param_->uart.speed = mode_->PrintConfig.ComBaudreate;
		param_->uart.databits = mode_->PrintConfig.ComDataBit;
		param_->uart.stopbits = mode_->PrintConfig.ComStopBit;
		param_->uart.parity = mode_->PrintConfig.ComParity;
#endif // __linux
	}
	void PrintSetControl::CreateView() {	}

	void SaveBitmapPic(Fl_Image *picdata)
	{
//#pragma pack(2)//必须得写，否则sizeof得不到正确的结果
		unsigned char temp = 0;
		const int height = picdata->h();
		const int width = picdata->w();
		const int size = height * width * 3;
		uchar* srcrgbadata = new uchar[size];
		memcpy(srcrgbadata, picdata->data()[0], picdata->w() * picdata->h() * picdata->d());
		/*change R-G-B to B-G-R*/
		for (int i = 0; i < (height * width); i++)
		{
			temp = *(srcrgbadata + i * 3);
			*(srcrgbadata + i * 3) = *(srcrgbadata + i * 3 + 2);
			*(srcrgbadata + i * 3 + 2) = temp;
		}

		// Part.1 Create Bitmap File Header
		char bfType[2] = { 'B', 'M' };
		BITMAPFILEHEADER fileHeader;
		fileHeader.bfReserved1 = 0;
		fileHeader.bfReserved2 = 0;
		fileHeader.bfSize = sizeof(bfType) + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size;
		fileHeader.bfOffBits = sizeof(bfType) +sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		// Part.2 Create Bitmap Info Header
		BITMAPINFOHEADER bitmapHeader = { 0 };

		bitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapHeader.biHeight = -height;
		bitmapHeader.biWidth = width;
		bitmapHeader.biPlanes = 3;
		bitmapHeader.biBitCount = 24;
		bitmapHeader.biSizeImage = size;
		bitmapHeader.biCompression = 0; //BI_RGB

		FILE *output = fopen("output.bmp", "wb");

		if (output == NULL)
		{
			printf("Cannot open file!\n");
		}
		else
		{
			fwrite(bfType, sizeof(bfType), 1, output);
			fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, output);
			fwrite(&bitmapHeader, sizeof(BITMAPINFOHEADER), 1, output);
			fwrite(srcrgbadata, size, 1, output);
			fclose(output);
		}
		delete[] srcrgbadata;
		delete[] picdata;
	}
#ifdef WIN32
	DWORD WINAPI Printthread(LPVOID p)
	{
		LOG_INFO_("Printting Win\n");

		Fl_Image *img = (Fl_Image *)p;
		WaitForSingleObject(PrintMutex,INFINITE);
		/*调用打印接口*/
		SaveBitmapPic(img);
		ReleaseMutex(PrintMutex);
		return 0;
	}
#else

         
PRINTER_INFO *  __attribute__((weak)) PreparePrintData(Print_Para Para_Set)
{
            return NULL;
}
void* Printthread(void* arg)
	{
		LOG_INFO_("Printting Win\n");
		Set_Para *param = (Set_Para *)arg;
		pthread_mutex_lock(&PrintMutex); //加锁，用于对共享变量操作
		/*调用打印接口*/
		//PreparePrintData(*param);
		pthread_mutex_unlock(&PrintMutex); //解锁
	}
#endif // WIN32

	void PrintSetControl::PrintWinPic(Fl_Widget *target)
	{
		Fl_Image_Surface *rgb_surf;
		if (!target)
			return;
		int w, h;
		w = target->w();
		h = target->h();
		rgb_surf = new Fl_Image_Surface(w,h, 1);
		rgb_surf->set_current();

		rgb_surf->draw(target);
		Fl_Image *printImg = rgb_surf->highres_image();
		delete rgb_surf;
#ifdef WIN32
		HANDLE hthread;
		hthread = CreateThread(NULL, 0, Printthread, printImg, 0, NULL);
		CloseHandle(hthread);
		PrintMutex = CreateMutex(NULL, false, NULL);
#else
		pthread_t id;
		int i, ret;
		pthread_mutex_init(&PrintMutex, NULL);
		/*生成bmp图片*/
		SaveBitmapPic(printImg);
		/*获取bmp图片路径*/
		char buf[4096];
		memset(buf, '\0', 4096);
		string path = getcwd(buf, 4096);
		path += "\\output.bmp";
		LOG_INFO("Print Pic Path : %s\n", path.c_str());
		/*填充打印所需结构体内容*/
		//param_->bmp = path;
        strcpy(param_->bmp, path.c_str());
		ret = pthread_create(&id, NULL, Printthread, (void *)param_);
		if (ret != 0)
		{
			printf("Create pthread error!\n");
			return;
		}
		pthread_exit(NULL);
#endif // WIN32
	}

	void PrintSetControl::HandleDataNotify(Project::DataVarId id)
	{
		Page()->Win()->SendUserMessage((void*)CtrlId(), WM_EVENT_DATEUPDATE, id.Vid);
	}
	void PrintSetControl::OnReady()
	{
		if (mode_->PrintConfig.PrintTrigVar != Project::DataVarId::NullId)
			perbitstatus_ = UI::UIData::Bit(mode_->PrintConfig.PrintTrigVar);
		if (mode_->PrintConfig.PrintSourceVar != Project::DataVarId::NullId)
			mode_->PrintConfig.PrintSourceWinNo = UI::UIData::Number<int>(mode_->PrintConfig.PrintSourceVar);
	}
	void PrintSetControl::HandleDataVar(Project::DataVarId &id)
	{
	
		if (mode_->PrintConfig.PrintTrigVar.Cmp(id))
		{
			bool status = UI::UIData::Bit(mode_->PrintConfig.PrintTrigVar);
			if ((mode_->PrintConfig.BitTriggerCond == 0 && status == false && perbitstatus_ == true)
				|| (mode_->PrintConfig.BitTriggerCond == 1 && status == true && perbitstatus_ == false))
			{
				//打印
				int winno = mode_->PrintConfig.PrintSourceWinNo;
				Fl_Widget *target = Win()->FindPage(winno);
				if (!target)
					target = Win()->ProducePage(winno);
				PrintWinPic(target);
			}
			perbitstatus_ = status;
		}
		if (mode_->PrintConfig.PrintSourceVar.Cmp(id))
			mode_->PrintConfig.PrintSourceWinNo = UI::UIData::Number<int>(mode_->PrintConfig.PrintSourceVar);
	}
	int PrintSetControl::PeekHMIMessage(Message::Msg* msg)
	{
		Project::DataVarId varId;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			break;
		default:
			break;
		}

		return 1;//handled;
	}
}
