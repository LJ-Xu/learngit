#pragma once
#include "IResourceService.h"
namespace UI
{
	typedef void  (VoidFunc)(void);
	typedef DOUBLE(HMIExpressFunc)(DOUBLE val);
	typedef Fl_Image*  GetImageByKeyFunc(const char* reskey);  //ͨ��HASH��ȡ�ļ���Դ
	typedef BYTE*  GetImageFileByNameFunc(const char* name, ImageInfo* imgInfo);  //ͨ��HASH��ȡ�ļ���Դ
	typedef BYTE*  GetImageFileByIdFunc(int idx, ImageInfo* imgInfo);           //ͨ��������ȡ�ļ���Դ
	typedef BYTE*  GetFileByNameFunc(const char*, FileInfo* fileInfo);  //ͨ��HASH��ȡ�ļ���Դ
	typedef BYTE*  GetFileByIdxFunc(int idx, FileInfo* fileInfo);           //ͨ��������ȡ�ļ���Դ
	typedef char* GetStringByIdFunc(int tableId, int idx, int state, int lang);
	typedef char* GetStringByNameFunc(const char*, int state, int lang);
	/***************��ȡ��ֵת�����ʽ**************************/

	typedef HMIExpressFunc*  GetExpressFunctionFunc(const char* key);

	/***************��ȡ�û��Զ��庯��**************************/
	typedef FuncTask*  GetFuncTaskFunc(const char* funName);




	//#ifdef HMI_PC_RENDER	
	struct ResFuncs
	{
		GetImageFileByNameFunc*   GetImageFileByName;
		GetImageFileByIdFunc  *   GetImageFileById;
		GetFileByNameFunc     *   GetFileByName;
		GetFileByIdxFunc      *   GetFileByIdx;
		GetStringByIdFunc     *   GetStringById;
		GetStringByNameFunc   *   GetStringByName;
		//GetExpressFunctionFunc* GetExpressFunction;
		//GetFuncTaskFunc       * GetFuncTask;
		GetImageByKeyFunc     *   GetImageByKey;
		VoidFunc                 *   Reserved1;
		VoidFunc                 *   Reserved2;
		VoidFunc                 *   Reserved3;
		VoidFunc                 *   Reserved4;
		VoidFunc                 *   Reserved5;
	};
	struct PCGlobalVars
	{
		UINT8 Status; //  ��ǰ״̬
		UINT8 Lang;  //  ��ǰ����
		ResFuncs Funcs;
	};
	//#endif
	class PcResourceService :public IResourceService
	{
	public:
		static PcResourceService* Ins();

		virtual void InitImages(std::map<string, HMIImage*>& imgs) override;
		virtual HMIImage* GetImage(std::string name) override;  //ͨ��HASH��ȡͼƬ��Դ
		virtual HMIImage* GetImage(int idx) override;           //ͨ��������ȡͼƬ��Դ
		virtual void DestroyImage(HMIImage* img) override;

		virtual BYTE*  GetImageFileByName(const char* name, ImageInfo* imgInfo) override;  //ͨ��HASH��ȡ�ļ���Դ
		virtual BYTE*  GetImageFileById(int idx, ImageInfo* imgInfo) override;           //ͨ��������ȡ�ļ���Դ
		virtual BYTE*  GetFileByName(const char*, FileInfo* fileInfo) override;  //ͨ��HASH��ȡ�ļ���Դ
		virtual BYTE*  GetFileByIdx(int idx, FileInfo* fileInfo) override;           //ͨ��������ȡ�ļ���Դ
		virtual char*  GetStringById(int tableId, int idx, int state, int lang) override;
		virtual char*  GetStringByName(const char*, int state, int lang) override;
		virtual  bool IsRenderMode() override;
		virtual  void SetRenderStatus(int& status, int maxstatus) override;
		virtual  double GetZoom() override;


		void SetLang(UINT8 lang);
		void SetStatus(UINT8 status);
		void ReleasePCImg();
		void Init();

		void SetResFuncs(ResFuncs funcs) { pcVar_.Funcs = funcs; };
	private:
		PcResourceService()=default;
		PcResourceService(PcResourceService&) = default;
		std::map<string, std::shared_ptr<HMIImage>> resImages_;
		PCGlobalVars pcVar_;//ͼ����ʾ��Ҫ����Դ�ӿں���		
		/*std::vector<std::shared_ptr<HMIImage>> imgs_;
		std::map<std::string, std::shared_ptr<HMIImage>> imgmap_;*/
	};

}