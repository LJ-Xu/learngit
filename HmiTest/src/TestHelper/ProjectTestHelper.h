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
	/***************��������*********************/
	void SettingScreen(short w, short h);
	void SetStartWin(short winno);
	void AddRes(const char* key, const char* bmpPATH);
	void AddStrings(const char* label, int status, const  char* lang1,const char* lang2="lang2", const  char* lang3="lang3");
	//void AddStrings(char tableid, const char rowid, const  char* lang1, const char* lang2 = "lang2", const  char* lang3 = "lang3");
	void AddWin(short winno, int x, int y, short w, short h, int color, short catogray = 0);
	//����CTRLID
	int  AddCtrl(short winno, const char* clsname, const char* ctrlname, function<void(void* cnf)>cnfCB=nullptr);
	int  AddGCtrl(short winno, const char* clsname, const char* ctrlname, function<void(void* cnf)>cnfCB = nullptr);
	//vid
	void BindCtrlAndVar(const char* varname, const char* ctrlname);
	void BindCtrlIDAndVar(const char* varname, int ctrlid);
	//����varid
	int  AddDataVar(short  DevId, // �豸ID������
		short  StaNo, //վ��
		short  RegType, //�Ĵ��� 
		short  DataType, //�Ĵ�������   BIT WORD DDWORD 
		// dataType==BIT ��ʱ��CountΪλ��ƫ��ֵ��Ϊ����ʱ��ʾ�ֽڸ���
		short  ExType,
		int    Addr,  //��ַƫ�� 
		short  Count, //�ֽ�����  ->����regType��ȡ�����Ĵ����ֽ���
		const char* varname //�ؼ�����
		);
	unsigned int  GetVarID(const char* varname);
	void Update();

	

private:
	Project::HMIProject* prj_;
	std::map<const char*, unsigned int> varIdKV_;
	std::map<const char*, Project::WinCtrlID> ctrlidKV_;
};

