#pragma once
#include "stdafx.h"
#include "BaseVar.h"
#include "ProjectPortVarsInfo.h"
#include "HMIProject.h"
#include "DataFormate.h"
#include "IDaAppCB.h"
#include <thread>
namespace UI
{

	typedef int(*DataRecvCB)(BYTE* ddata, int len);
	class Observer;
	class DataApi
	{
	public:

		static void Attatch(Observer * p);

		static void Init(Project::HMIProject* vars);

		static void Start();//启动数据服务

		static void Stop();//停止数据服务
		static void MacroTrigger(const char* funcname);//函数调用

		///存储DEVID信息---保存相关通信状态
		static void OnPageOpen(int pageid);//页面打开

		static void OnPageClose(int pageid);//页面关闭

		static void SyncLocalData(); //同步本地寄存器内容

		/****************数据访问*******************/
		static XJDataType GetDataType(Project::DataVarId vid);
		static char GetDataFlag(Project::DataVarId vid);
		/**********************直接指定VAR信息(异步或者访问系统本地寄存器)***********************************/
		static bool GetBit(Project::BaseVar& var);
		static void SetBit(Project::BaseVar& var, bool val);

		static void GetDataBytes(Project::BaseVar& var, BYTE* ddata, int len);
		static void SetDataBytes(Project::BaseVar& var, const BYTE* sdata, int len);

		static bool GetVarDatas(Project::BaseVar& var, int count, DataRecvCB cb = nullptr);
		static int  MoveDataVars(Project::BaseVar& svar, Project::BaseVar& dvar, int regcount);
		/**********************根据ID信息访问***********************************/
		static bool GetBit(Project::DataVarId& var);
		static void SetBit(Project::DataVarId& var, bool val);

		static void  GetDataBytes(Project::DataVarId& var, BYTE* ddata, int len);
		static void  SetDataBytes(Project::DataVarId& var, const BYTE* sdata, int len);

		static bool GetVarDatas(Project::DataVarId& var, int count);
		static int  MoveDataVars(Project::DataVarId& svar, Project::DataVarId& dvar, int regcount);
		/***************系统\本地数据访问************/


		static Project::DataVarInfo* GetDataVarInfo(Project::DataVarId& id);





	public:
		/***************************应用访问接口**************************************/
		//位访问
		static bool  AppBit(Project::DataVarId& vid);
		static void  AppBit(Project::DataVarId& vid, bool val);
		//字节访问
		static std::vector<BYTE>  AppBytes(Project::DataVarId& vid);
		static void  AppBytes(Project::DataVarId& vid, const char* data, int len);
		//可变长度访问(请求数据,调用以后等控件通知)
		static int AppReqVarDatas(Project::DataVarId& var, int count);
		//可变长度访问(返回数据后的获取)
		static std::vector<BYTE> AppVarDatas(Project::DataVarId& varid);
		//字符串访问
		static std::string  AppString(Project::DataVarId& vid);
		static int  AppString(Project::DataVarId& vid, const char* str);
		//数值访问
		template<typename T>
		static T AppNumber(Project::DataVarId& vid)
		{
			//return UIDataService::Ins().GetNumber<T>(vid);
			return Utility::DataFormate::GetNumber<T>(ReadNumberToDDWORD(vid), GetDataType(vid));
		}
		template<typename T>
		static void AppNumber(Project::DataVarId& vid, T val)
		{
			XJDataType tp = GetDataType(vid);
			DDWORD da = Utility::DataFormate::SetNumber<T>(val, tp);
			WriteNumberByDDWORD(vid, da);
		}
		/******************本地寄存器方式访问,目前只支持系统寄存器访问***************/
		static bool  LocalBit(int regid, int addr);
		static void  LocalBit(int regid, int addr, bool val);

		static std::vector<BYTE>  LocalBytes(int regid, int addr, int len);
		static void  LocalBytes(int regid, int addr, int len, const char* da);


		static std::string LocalString(int regid, int addr, int len);
		static void  LocalString(int regid, int addr, int len, const char* da);

		//数值访问
		template<typename T>
		static T LocalNumber(int regid, int addr, int len)
		{
			T val;
			Project::BaseVar var;
			InitLocalVar(var, regid, addr, sizeof(T));
			GetDataBytes(var, (BYTE*)&val, sizeof(T));
			return val;
		}
		template<typename T>
		static void LocalNumber(int regid, int addr, int len, T val)
		{
			T v = val;
			Project::BaseVar var;
			InitLocalVar(var, regid, addr, sizeof(T));
			SetDataBytes(var, (const BYTE*)&v, sizeof(T));
		}

		/*****************************配方操作*********************************/
		/*删除配方记录*/
		static bool RecipeRecordDelete(string groupname, unsigned int no);
		/*添加配方记录*/
		static bool RecipeRecordInsert(string groupname, unsigned int no);
		/*设置配方记录*/
		static bool RecipeDataSet(string groupname, string colname, unsigned int rowno, string data);
		/*上传配方记录到PSW*/
		static int RecipeToPLC(string groupname, int count, Project::DataVarId dstvar, vector<Project::ColDataTypeInfo> datatype, Project::DataVarId flagvar);
		/*从PSW下载配方记录*/
		static int RecipeFromPLC(string groupname, int count, Project::DataVarId srcvar, vector<Project::ColDataTypeInfo> datatype, Project::DataVarId flagvar);
		
		/*上传配方记录到PSW*/
		static int RecipeToPLC(string groupname, int count, int index, Project::DataVarId dstvar, vector<Project::ColDataTypeInfo> datatype, Project::DataVarId flagvar);
		/*从PSW下载配方记录*/
		static int RecipeFromPLC(string groupname, int count, int index, Project::DataVarId srcvar, vector<Project::ColDataTypeInfo> datatype, Project::DataVarId flagvar);

		/******************************CSV操作*********************************/
		/*从PSW导出CSV*/
		static void DataExportCSV(int storagemedia, string filename, int count, Project::DataVarId dstvar,
			vector<Project::ColDataTypeInfo> datatype, Project::DataVarId statusvar, Project::DataVarId resultvar, Project::DataVarId processvar);
		/*从CSV导入到PSW*/
		static void DataImportCSV(int storagemedia, string filename, int count, Project::DataVarId srcvar,
			vector<Project::ColDataTypeInfo> datatype, Project::DataVarId statusvar, Project::DataVarId resultvar, Project::DataVarId processvar);
private:
		static void InitLocalVar(Project::BaseVar&var, int regid, int addr, int count)
		{
			var.DevId = 0;
			var.RegType = regid;
			var.Addr = addr;
			var.Count = count;
			var.StaNo = 0;
		}
		static DDWORD ReadNumberToDDWORD(Project::DataVarId vid)
		{
			DDWORD val = 0;
			Project::DataVarInfo* info = GetDataVarInfo(vid);
			DataApi::GetDataBytes(vid, (char*)&val, MIN(info->Count, sizeof(val)));
			return val;
		}
		static void WriteNumberByDDWORD(Project::DataVarId vid, DDWORD da)
		{
			Project::DataVarInfo* info = GetDataVarInfo(vid);
			DataApi::SetDataBytes(vid, (char*)&da, MIN(info->Count, sizeof(da)));
		}
	};
	class LocalData
	{
	public:
		static void InitLocalVar(Project::BaseVar&var, int regid, int addr, int count)
		{
			var.DevId = 0;
			var.RegType = regid;
			var.Addr = addr;
			var.Count = count;
			var.StaNo = 0;
		}
		static bool GetBit(int regid, int addr)
		{
			Project::BaseVar var;
			InitLocalVar(var, regid, addr, 1);
			return DataApi::GetBit(var);
		}
		static void SetBit(int regid, int addr, bool val)
		{
			Project::BaseVar var;
			InitLocalVar(var, regid, addr, 1);
			DataApi::SetBit(var, val);
		}
		template<class T>
		static T GetNumberData(int regid, int addr)
		{
			T val;
			Project::BaseVar var;
			InitLocalVar(var, regid, addr, sizeof(T));
			DataApi::GetDataBytes(var, (BYTE*)&val, sizeof(T));
			return val;
		}
		template<class T>
		static void SetNumberData(int regid, int addr, T val)
		{
			T v = val;
			Project::BaseVar var;
			InitLocalVar(var, regid, addr, sizeof(T));
			DataApi::SetDataBytes(var, (const BYTE*)&v, sizeof(T));
		}

		static void GetBytes(int regid, int addr, int len, char* buf)
		{
			Project::BaseVar var;
			InitLocalVar(var, regid, addr, len);
			DataApi::GetDataBytes(var, buf, len);
		}
		static void SetBytes(int regid, int addr, int len, const char* buf)
		{
			Project::BaseVar var;
			InitLocalVar(var, regid, addr, len);
			DataApi::SetDataBytes(var, buf, len);
		}

		static void GetString(int regid, int addr, int len, char* str)
		{
			Project::BaseVar var;
			InitLocalVar(var, regid, addr, len);
			DataApi::GetDataBytes(var, str, len);
		}
		static void SetString(int regid, int addr, int len, const char* str)
		{
			Project::BaseVar var;
			InitLocalVar(var, regid, addr, len);
			DataApi::SetDataBytes(var, str, len);
		}
		static int GetLocalVarLen(int regid, int addr, int len)
		{
			return len;
		}
	};
	static int MacroLen(int regid, int addr, int len = 0)
	{
		return len;
	}
}
