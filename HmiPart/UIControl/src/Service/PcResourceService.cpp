/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name		: ResourceService.cs
 * Author   : zhanglinbo
 * Date     : 2020/07/12
 * Descript	: 资源接口服务，这里提供了图片文字字符串等资源获取
 * Version	: 1.0
 * modify	:
 *			:
 *******************************************************************************/
#include "stdafx.h"
#include <FL/Fl.H>
#include <map>
#include <istream>
#include "UIComm.h"
#include "ResourceService.h"
#include "BinBitmap.h"
#include "HMIProject.h"
#include "FL/Fl_BMP_Image.H"
#include "PcResourceService.h"
namespace UI
{
	PcResourceService* PcResourceService::Ins()
	{
		static PcResourceService ins;
		return &ins;
	}
	bool PcResourceService::IsRenderMode()
	{
		return true;
	}
	void PcResourceService::SetRenderStatus(int& status, int maxstatus)
	{
		//status = 0;
		if (pcVar_.Status < maxstatus)
			status = pcVar_.Status;
		else
			status = maxstatus - 1;
	}
	double PcResourceService::GetZoom()
	{
		return zoom_;
	}
	void PcResourceService::InitImages(std::map<string, HMIImage*>& imgs)
	{
	
	}
	HMIImage* PcResourceService::GetImage(std::string name)
	{
		HMIImage* image;
		if (pcVar_.Funcs.GetImageByKey) //通过KEY直接获取HMIImage
		{
			image = pcVar_.Funcs.GetImageByKey(name.c_str());
			if (image)
				return image;
			return nullptr;
		}		
		if (resImages_.find(name) != resImages_.end())
		{
			return resImages_[name].get();
		}
		ImageInfo imgInfo;
		char* data = GetImageFileByName(name.c_str(), &imgInfo);
		if (data == nullptr)return nullptr;

		image = (HMIImage*) new UI::BinBitmap(data, imgInfo.Size);
		//imgs_.push_back(std::shared_ptr<HMIImage>(image));
		resImages_.insert(std::pair<std::string, std::shared_ptr<HMIImage>>(name, std::shared_ptr<HMIImage>(image)));
		return image;
	}
	HMIImage* PcResourceService::GetImage(int idx)
	{
		ImageInfo imgInfo;
		char* data = GetImageFileById(idx, &imgInfo);
		if (data == nullptr)return nullptr;
		HMIImage* image = (HMIImage*) new UI::BinBitmap(data, imgInfo.Size);
		return image;
	}
	void PcResourceService::DestroyImage(HMIImage* img)
	{
		if (img)
			delete img;
	}
	BYTE*  PcResourceService::GetImageFileByName(const char* name, ImageInfo* imgInfo)
	{
		return pcVar_.Funcs.GetImageFileByName(name, imgInfo);
	}
	BYTE*  PcResourceService::GetImageFileById(int idx, ImageInfo* imgInfo)
	{
		return pcVar_.Funcs.GetImageFileById(idx, imgInfo);
	}
	BYTE*  PcResourceService::GetFileByName(const char* name, FileInfo* fileInfo)
	{
		return pcVar_.Funcs.GetFileByName(name, fileInfo);
	}
	BYTE*  PcResourceService::GetFileByIdx(int idx, FileInfo* fileInfo)
	{
		return pcVar_.Funcs.GetFileByIdx(idx, fileInfo);
	}
	char*  PcResourceService::GetStringById(int tableId, int idx, int state, int lang)
	{
		return pcVar_.Funcs.GetStringById(tableId, idx, pcVar_.Status, pcVar_.Lang);
	}
	char*  PcResourceService::GetStringByName(const char* name, int state, int lang)
	{
		return pcVar_.Funcs.GetStringByName(name, pcVar_.Status, pcVar_.Lang);
	}
 

	/*以下是PC绘制时调用的接口*/
 
	void PcResourceService::SetLang(UINT8 lang)
	{
		pcVar_.Lang = lang;
	}
 
	void PcResourceService::SetStatus(UINT8 status)
	{
		pcVar_.Status = status;
	}
	void PcResourceService::ReleasePCImg()
	{
		resImages_.clear();
	}
 
	void PcResourceService::Init()
	{
 
	}

}
