#pragma once
#include "stdafx.h"
#include <FL/Fl_Image.H>
#include <UIComm.h>

#include "IResourceService.h"
 
namespace Project
{
	struct HMIProject;
}
namespace UI
{	
	class ResourceService :public IResourceService
	{
	public:
		ResourceService(Project::HMIProject* prj);
		void InitResources(Project::HMIProject* prj, const char* path);
		virtual void InitImages(std::map<string, HMIImage*>& imgs);
		virtual HMIImage* GetImage(std::string name) override;  //通过HASH获取图片资源
		virtual HMIImage* GetImage(int idx) override;           //通过索引获取图片资源
		virtual void DestroyImage(HMIImage* img) override;

		virtual BYTE*  GetImageFileByName(const char* name, ImageInfo* imgInfo) override;  //通过HASH获取文件资源
		virtual BYTE*  GetImageFileById(int idx, ImageInfo* imgInfo) override;           //通过索引获取文件资源
		virtual BYTE*  GetFileByName(const char*, FileInfo* fileInfo) override;  //通过HASH获取文件资源
		virtual BYTE*  GetFileByIdx(int idx, FileInfo* fileInfo) override;           //通过索引获取文件资源
		virtual char*  GetStringById(int tableId, int idx, int state, int lang) override;
		virtual char*  GetStringByName(const char*, int state, int lang) override;
		virtual  bool IsRenderMode() override;
		virtual  void SetRenderStatus(int& status, int maxstatus) override;
		virtual  double GetZoom() override;
	private:
		std::map<string, HMIImage*> resImages_;
		Project::HMIProject* prj_ = nullptr;
	};
}


