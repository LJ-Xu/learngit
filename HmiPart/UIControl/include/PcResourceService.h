#pragma once
#include "IResourceService.h"
namespace UI
{
	typedef void  (VoidFunc)(void);
	typedef DOUBLE(HMIExpressFunc)(DOUBLE val);
	typedef Fl_Image*  GetImageByKeyFunc(const char* reskey);  //通过HASH获取文件资源
	typedef BYTE*  GetImageFileByNameFunc(const char* name, ImageInfo* imgInfo);  //通过HASH获取文件资源
	typedef BYTE*  GetImageFileByIdFunc(int idx, ImageInfo* imgInfo);           //通过索引获取文件资源
	typedef BYTE*  GetFileByNameFunc(const char*, FileInfo* fileInfo);  //通过HASH获取文件资源
	typedef BYTE*  GetFileByIdxFunc(int idx, FileInfo* fileInfo);           //通过索引获取文件资源
	typedef char* GetStringByIdFunc(int tableId, int idx, int state, int lang);
	typedef char* GetStringByNameFunc(const char*, int state, int lang);
	/***************获取数值转换表达式**************************/

	typedef HMIExpressFunc*  GetExpressFunctionFunc(const char* key);

	/***************获取用户自定义函数**************************/
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
		UINT8 Status; //  当前状态
		UINT8 Lang;  //  当前语言
		ResFuncs Funcs;
	};
	//#endif
	class PcResourceService :public IResourceService
	{
	public:
		static PcResourceService* Ins();

		virtual void InitImages(std::map<string, HMIImage*>& imgs) override;
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


		void SetLang(UINT8 lang);
		void SetStatus(UINT8 status);
		void ReleasePCImg();
		void Init();

		void SetResFuncs(ResFuncs funcs) { pcVar_.Funcs = funcs; };
	private:
		PcResourceService()=default;
		PcResourceService(PcResourceService&) = default;
		std::map<string, std::shared_ptr<HMIImage>> resImages_;
		PCGlobalVars pcVar_;//图形显示需要的资源接口函数		
		/*std::vector<std::shared_ptr<HMIImage>> imgs_;
		std::map<std::string, std::shared_ptr<HMIImage>> imgmap_;*/
	};

}