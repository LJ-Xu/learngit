#pragma once
#include "HMIProject.h"
#include "BaseControl.h"
#include "BaseGControl.h"
class VarHelperInfo
{
	short Winno;
	short CtrlId;
};
class ProjectTestHelper
{
public:
	ProjectTestHelper(Project::HMIProject* prj);
	//void Init(Project::HMIProject* prj);
	/***************窗体设置*********************/
	void SettingScreen(short w, short h);
	void SetStartWin(short winno);
	void AddRes(const char* key, const char* bmpPATH);
	void AddStrings(const char* label, int status, const  char* lang1,const char* lang2="lang2", const  char* lang3="lang3");
	//void AddStrings(char tableid, const char rowid, const  char* lang1, const char* lang2 = "lang2", const  char* lang3 = "lang3");
	void AddWin(short winno, int x, int y, short w, short h, int color, short catogray = 0);
	//返回CTRLID
	int  AddCtrl(short winno, const char* clsname, const char* ctrlname, function<void(void* cnf)>cnfCB=nullptr);
	int  AddGCtrl(short winno, const char* clsname, const char* ctrlname, function<void(void* cnf)>cnfCB = nullptr);
	//vid
	void BindCtrlAndVar(const char* varname, const char* ctrlname);
	void BindCtrlIDAndVar(const char* varname, int ctrlid);
	//返回varid
	int  AddDataVar(short  DevId, // 设备ID索引号
		short  StaNo, //站号
		short  RegType, //寄存器 
		short  DataType, //寄存器类型   BIT WORD DDWORD 
		// dataType==BIT 的时候，Count为位的偏移值，为其他时表示字节个数
		short  ExType,
		int    Addr,  //地址偏移 
		short  Count, //字节数量  ->根据regType获取单个寄存器字节数
		const char* varname //控件名称
		);
	unsigned int  GetVarID(const char* varname);
	void Update();

	

private:
	Project::HMIProject* prj_;
	std::map<const char*, unsigned int> varIdKV_;
	std::map<const char*, Project::WinCtrlID> ctrlidKV_;
};

