/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name		: ResourceService.cs
 * Author   : zhanglinbo
 * Date     : 2020/07/12
 * Descript	: ��Դ�ӿڷ��������ṩ��ͼƬ�����ַ�������Դ��ȡ
 * Version	: 1.0
 * modify	:
 *			:
 *******************************************************************************/
#include "stdafx.h"
#include <FL/Fl.H>
#include <map>
#include <istream>
#include "UIComm.h"
#include "BinBitmap.h"
#include "HMIProject.h"
#include "FL/Fl_BMP_Image.H"
#include "ResourceService.h"
namespace UI
{
	ResourceService::ResourceService(Project::HMIProject* prj)
	{
		prj_ = prj;
	}
	/***********************����ΪĬ�ϵ�ʵ�֣�ҲΪʵ����λ��������Ҫ�Ľӿں���**************/
	BYTE* ResourceService::GetImageFileByName(const char* name, ImageInfo* imgInfo)
	{
		Project::ResItem*  item = prj_->Reses.GetResourceItem(name);
		if (item == nullptr)
			return nullptr;
		imgInfo->Dep = item->GetResItemParam()->Pic.Dep;
		imgInfo->W = item->GetResItemParam()->Pic.W;
		imgInfo->H = item->GetResItemParam()->Pic.H;
		imgInfo->Size = item->Size;
		return  prj_->Reses.GetResourceData(item);
	}
	BYTE* ResourceService::GetImageFileById(int idx, ImageInfo* imgInfo)
	{
		return nullptr;
	}
	BYTE* ResourceService::GetFileByName(const char* name, FileInfo* fileInfo)
	{
		return nullptr;
	}
	BYTE* ResourceService::GetFileByIdx(int idx, FileInfo* fileInfo)
	{
		return nullptr;

	}
	char* ResourceService::GetStringById(int tableId, int idx, int state, int lang)
	{
		return (char*)prj_->Tables.StrLabel.GetString(tableId, idx,state, lang);
	}
	char* ResourceService::GetStringByName(const char* label, int state, int lang)
	{
		return (char*)prj_->Tables.StrStatus.GetString(label, state, lang);
	}
	void ResourceService::DestroyImage(HMIImage* img)
	{
		if(img)
			delete img;
	}

	/*******************************************************************************
	 * Name	    : GetImage
	 * Descript : ���ݴ��ݵ�ͼƬ��Դ��KEYֵ��ȡͼƬ��Դ
	 * Input	: name - ��ԴKEYֵ
	 * Output	: HMIImage* ��Դָ��
	 * Note	    :
	*******************************************************************************/
	HMIImage* ResourceService::GetImage(std::string name)
	{
		//TODO �����ڴ棬��Ҫ�ͷţ��Լ�ͬһ����Դ��λ�ȡͼƬ�����ڴ����

		if (resImages_.size())
		{
			if (resImages_.find(name) != resImages_.end())
				return resImages_[name];
		}
		return nullptr;
	
	}
	/*******************************************************************************
	 * Name	    : GetImage
	 * Descript : ���ݴ��ݵ�ͼƬ��Դ������ȡͼƬ��Դ��Ŀǰ���õ�KEY str��ʽ
	 * Input	: idx - ��Դ����
	 * Output	: HMIImage* ��Դָ��
	 * Note	    :
	*******************************************************************************/
	HMIImage* ResourceService::GetImage(int idx)
	{

		ImageInfo imgInfo;
		char* data = GetImageFileById(idx, &imgInfo);
		if (data == nullptr)return nullptr;
		HMIImage* image = (HMIImage*) new UI::BinBitmap(data, imgInfo.Size);

		return image;
	}
	 
 
	//static void WriteFile(char* data,int size)
	//{
	//	static int index = 0;
	//	ofstream out;
	//	char pathstr[100];
	//	sprintf(pathstr,"D://%d.bmp", index);
	//	//out.open(pathstr, std::ios::binary);
	//	//out.write(data, size);
	//	//out.close();
	//	FILE * fp = fopen(pathstr, "wb+");
	//	if (fp != nullptr) {
	//		int ret = fwrite(data, 1, size, fp);
	//		if (ret != size)
	//			return;
	//	}
	//	fclose(fp);
	//	
	//}
	
	//static HMIImage* GetImageByIfStream(ifstream& in0,Project::ResItem&item)
	//{
	//	ImageInfo imgInfo;	
	//	imgInfo.Dep = item.GetResItemParam()->Pic.Dep;
	//	imgInfo.W = item.GetResItemParam()->Pic.W;
	//	imgInfo.H = item.GetResItemParam()->Pic.H;
	//	imgInfo.Size = item.Size;
	//	char* data = new char[item.Size];
	//	streampos  i = in0.tellg();
	//	in0.read(data, item.Size);
	//	//WriteFile(data, item.size);
	//	HMIImage* image = (HMIImage*) new UI::BinBitmap(data, imgInfo.Size);
	//	delete[] data;
	//	return image;
	//}
	//void ResourceService::InitResources(Project::HMIProject* prj, const char* path)
	//{
	//	Project::BinHeader header;
	//	ifstream in0;
	//	{
	//		in0.open(path, std::ios::binary);
	//		in0.read((char*)&header, sizeof(header));
	//		std::map<string, Project::ResItem>::iterator iter;
	//		
	//		for (iter = prj->Reses.ResMap.begin(); iter != prj->Reses.ResMap.end(); iter++)
	//		{
	//			in0.clear();
	//			in0.seekg(header.ResOffset+ iter->second.Offset);
	//			//int i = in0.tellg();
	//			switch (iter->second.FileType)
	//			{
	//			case Project::ResFileType::Image:
	//				resImages.insert(std::pair<string, HMIImage*>(iter->first,
	//					GetImageByIfStream(in0, iter->second)));
	//				break;
	//				
	//			}
	//			
	//		}
	//	}
	//	in0.close();
	//}
	void ResourceService::InitResources(Project::HMIProject* prj, const char* path)
	{
		std::map<string, int>::iterator iter;
		unique_ptr<HMIImage*> vect(new HMIImage*[prj->Reses.ResMap.size()]());
		for (iter = prj->Reses.ResKeyIdxs.begin(); iter != prj->Reses.ResKeyIdxs.end(); iter++)
		{
			if (!vect.get()[iter->second])
			{
				vect.get()[iter->second] = (HMIImage*) new UI::BinBitmap(
					(const char*)prj->Reses.ResMap[iter->second].Offset + prj->Reses.Data,
					prj->Reses.ResMap[iter->second].Size);
			
			}
			resImages_.insert(std::pair<string, HMIImage*>(iter->first, vect.get()[iter->second]));
		}
			
	}
	void ResourceService::InitImages(std::map<string, HMIImage*>& imgs)
	{
		std::map<string, HMIImage*>::iterator iter;
		for (iter = imgs.begin(); iter != imgs.end(); iter++)
		{
			resImages_.insert(std::pair<string, HMIImage*>(iter->first, iter->second));
		}
	}
	bool ResourceService::IsRenderMode()
	{
		return false;
	}
	void  ResourceService::SetRenderStatus(int& status, int maxstatus)
	{
	}
	double ResourceService::GetZoom()
	{
		return 1.0;
	}
}
