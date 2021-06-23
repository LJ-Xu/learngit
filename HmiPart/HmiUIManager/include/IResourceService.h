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
 
	/******�û��Զ��庯��**********/
	class FuncTask
	{


	};
	struct ImageInfo
	{
		short  Type;//���
		short  Dep;//λ��
		short  W; //���
		short  H; //�߶�
		int Size;//�ļ���С�ֽ�

	};
	struct SoundInfo
	{
		int Size;//�ļ���С�ֽ�
		short Type;//���
	};
	struct FileInfo
	{
		int Size;//�ļ���С�ֽ�
		int Type;//���
	};
	
		
	class IResourceService
	{
	public:
		/***********************************��Դ��������**********************************/
		static int  GetFontIdx(std::string name);//ͨ���������ƻ�ȡ��������

		/*******************************************************************************
		 * Name     : GB2312toUtf8
		 * Descript : ������GB2312�����ʽת��Ϊutf8��ʽ
		 * Input    : gb2312 - GB2312�����ʽ�������ַ���
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		static void GB2312toUtf8(string &gb2312);

		static IResourceService* Ins();

		static void  SetIns(IResourceService* ins);

		/*************�ַ�����Դ�Ļ�ȡ***************/
		///string** string[״̬][����]
		char* GetString(int tableId, int idx, int state = 0, int lang = 0) //��״̬  string[0][����]
		{
			return GetStringById(tableId, idx, state, lang);
		}
		char* GetString(string label, int state = 0, int lang = 0)
		{
			return GetStringByName(label.c_str(), state, lang);
		}
		virtual void InitImages(std::map<string, HMIImage*>& imgs) = 0;
		virtual HMIImage* GetImage(std::string name) = 0;  //ͨ��HASH��ȡͼƬ��Դ
		virtual HMIImage* GetImage(int idx) = 0;           //ͨ��������ȡͼƬ��Դ
		virtual void DestroyImage(HMIImage* img) = 0;

		virtual BYTE*  GetImageFileByName(const char* name, ImageInfo* imgInfo)=0;  //ͨ��HASH��ȡ�ļ���Դ
		virtual BYTE*  GetImageFileById(int idx, ImageInfo* imgInfo)=0;           //ͨ��������ȡ�ļ���Դ
		virtual BYTE*  GetFileByName(const char*, FileInfo* fileInfo)=0;  //ͨ��HASH��ȡ�ļ���Դ
		virtual BYTE*  GetFileByIdx(int idx, FileInfo* fileInfo)=0;           //ͨ��������ȡ�ļ���Դ
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


