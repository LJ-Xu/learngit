#pragma once
#include "stdafx.h"
#include <FL/Fl_Image.H>
#include <map>
#include "UIComm.h"

//#define HMI_PC_RENDER
namespace Project
{
	struct HMIProject;
}
namespace UI
{
 
	/******用户自定义函数**********/
	class FuncTask
	{


	};
	struct ImageInfo
	{
		short  Type;//类别
		short  Dep;//位深
		short  W; //宽度
		short  H; //高度
		int Size;//文件大小字节

	};
	struct SoundInfo
	{
		int Size;//文件大小字节
		short Type;//类别
	};
	struct FileInfo
	{
		int Size;//文件大小字节
		int Type;//类别
	};
	
		
	class IResourceService
	{
	public:
		/***********************************资源辅助工具**********************************/
		static int  GetFontIdx(std::string name);//通过字体名称获取字体索引

		/*******************************************************************************
		 * Name     : GB2312toUtf8
		 * Descript : 将中文GB2312编码格式转换为utf8格式
		 * Input    : gb2312 - GB2312编码格式的中文字符串
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		static void GB2312toUtf8(string &gb2312);

		static IResourceService* Ins();

		static void  SetIns(IResourceService* ins);

		/*************字符串资源的获取***************/
		///string** string[状态][语言]
		char* GetString(int tableId, int idx, int state = 0, int lang = 0) //无状态  string[0][语言]
		{
			return GetStringById(tableId, idx, state, lang);
		}
		char* GetString(string label, int state = 0, int lang = 0)
		{
			return GetStringByName(label.c_str(), state, lang);
		}
		virtual void InitImages(std::map<string, HMIImage*>& imgs) = 0;
		virtual HMIImage* GetImage(std::string name) = 0;  //通过HASH获取图片资源
		virtual HMIImage* GetImage(int idx) = 0;           //通过索引获取图片资源
		virtual void DestroyImage(HMIImage* img) = 0;

		virtual BYTE*  GetImageFileByName(const char* name, ImageInfo* imgInfo)=0;  //通过HASH获取文件资源
		virtual BYTE*  GetImageFileById(int idx, ImageInfo* imgInfo)=0;           //通过索引获取文件资源
		virtual BYTE*  GetFileByName(const char*, FileInfo* fileInfo)=0;  //通过HASH获取文件资源
		virtual BYTE*  GetFileByIdx(int idx, FileInfo* fileInfo)=0;           //通过索引获取文件资源
		virtual char*  GetStringById(int tableId, int idx, int state, int lang)=0;
		virtual char*  GetStringByName(const char*, int state, int lang) = 0;
		virtual bool IsRenderMode() = 0;
		virtual void SetRenderStatus(int& status, int maxstatus) = 0;
		virtual double GetZoom() = 0;
		void SetZoom(double z)
		{
			zoom_ = z;
		}
	protected:
		double zoom_ = 1.0;

	private :
		static IResourceService* ins_;
	};
}


