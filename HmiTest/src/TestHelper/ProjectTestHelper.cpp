#include "ProjectTestHelper.h"
#include "BinBitmap.h"
#include "ControlFactory.h"

struct ResAddInfo
{
	int Idx;
	char* NextPtr;
	int BufSize;
	char* Buf;
};
ResAddInfo resAddInfo;
void ProjectTestHelper::SettingScreen(short w, short h)
{
	prj_->Setting.Height = h;
	prj_->Setting.Width  = w;
}
ProjectTestHelper::ProjectTestHelper(Project::HMIProject* prj)
{
	prj_ = prj;
	resAddInfo.BufSize = 1024 * 1024 * 10;
	resAddInfo.NextPtr = resAddInfo.Buf = (char*)malloc(resAddInfo.BufSize);
	prj_->Reses.Data = (int)resAddInfo.Buf;
	AddWin(0, 0,0,1, 1, 0, 0xFF);
}
//void ProjectTestHelper::Init(Project::HMIProject* prj)
//{
//	
//}
static int ReadFile(const char* path,char* buf)
{
	ifstream ifs(path, std::ios::binary | std::ios::in);
	if (!ifs.is_open())
		return 0;
	ifs.seekg(0, std::ios::end);
	int len = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	ifs.read(buf,len);
	return len;
}
void ProjectTestHelper::SetStartWin(short winno)
{
	prj_->Setting.StartWinNo = winno;
}
void ProjectTestHelper::AddRes(const char* key,const char* bmpPATH)
{
	Project::ResItem item;
	item.FileType = 'B';
	item.Offset = resAddInfo.NextPtr - resAddInfo.Buf;
	item.Size =  ReadFile(bmpPATH,resAddInfo.NextPtr);
	if (!item.Size)return;
	UI::BinBitmap bitmap(resAddInfo.NextPtr, item.Size);
	resAddInfo.NextPtr += item.Size;
	Project::ResItemParam param;
	param.Pic.Dep = bitmap.d();
	param.Pic.H = bitmap.h();
	param.Pic.W = bitmap.w();
	memcpy(&item.Param, &param,sizeof(Project::ResItemParam));
	prj_->Reses.ResMap.push_back(item);
	prj_->Reses.ResKeyIdxs.insert(std::pair<string, int>(key, prj_->Reses.ResMap.size()-1));
}
void ProjectTestHelper::AddStrings(const char* label, int status,const  char* lang1, const char* lang2, const  char* lang3)
{
	
	if (prj_->Tables.StrStatus.StringStatusMap.find(label) ==
		prj_->Tables.StrStatus.StringStatusMap.end())
	{
		Project::StringStatusItem statusItem;
		statusItem.Id = 0;
		statusItem.Tag = label;
		prj_->Tables.StrStatus.StringStatusMap.insert(std::pair<string, Project::StringStatusItem>(label, statusItem));
	}
	Project::StringStatusItem& item = prj_->Tables.StrStatus.StringStatusMap[label];
	vector<string> vect;
	vect.push_back(lang1);
	vect.push_back(lang2);
	vect.push_back(lang3);
	item.StateLangs.push_back(vect);
	
}
//void ProjectTestHelper::AddStrings(char tableid, const char rowid, const  char* lang1, const char* lang2, const  char* lang3)
//{
//
//}
void ProjectTestHelper::AddWin(short winno,int x,int y, short w, short h, int color, short catogray)
{
	Project::PageInfo win;
	win.Pos.X = x;
	win.Pos.Y = y;
	win.Id = winno;
	win.Name = "win";
	win.Sz.H = h;
	win.Sz.W = w;
	win.WinColor = color;
	win.Catogray = catogray;
	prj_->Pages.Screens.insert(std::pair<int, Project::PageInfo>(winno, win));
}
//返回CTRLID
int  ProjectTestHelper::AddCtrl(short winno, const char* clsname, const char* ctrlname,function<void(void* cnf)>cnfCB)
{
	if (prj_->Pages.Screens.find(winno) == prj_->Pages.Screens.end())
		return -1;
	Project::PageInfo& win = prj_->Pages.Screens[winno];
	UI::BaseControl* ctrl =  UI::ControlFactory::GetNewControlByNameEx(clsname).release();
	std::shared_ptr<UI::BaseModel> mode = ctrl->GetMode();
	mode->InitTestData();
	if(cnfCB)
	cnfCB(mode->GetTestConfig());
	std::unique_ptr<std::vector<char>> vector = mode->GetData();
	win.Shapes.push_back(Project::UnitInfo(clsname));
	win.Shapes[win.Shapes.size() - 1].Data.swap(*vector);
	Project::WinCtrlID winctrlid = WINCTR_ID(winno, win.Shapes.size() - 1);
	ctrlidKV_.insert(std::pair<const char*, Project::WinCtrlID>(ctrlname,winctrlid));
	return winctrlid;
}
int  ProjectTestHelper::AddGCtrl(short winno, const char* clsname, const char* ctrlname, function<void(void* cnf)>cnfCB)
{
	if (prj_->Pages.Screens.find(winno) == prj_->Pages.Screens.end())
		return -1;
	Project::PageInfo& win = prj_->Pages.Screens[winno];
	UI::BaseControl* ctrl = UI::ControlFactory::GetNewControlByNameEx(clsname).release();
	std::shared_ptr<UI::BaseModel> mode = ctrl->GetMode();
	mode->InitTestData();
	if (cnfCB)
		cnfCB(mode->GetTestConfig());
	std::unique_ptr<std::vector<char>> vector = mode->GetData();
	win.Shapes.push_back(Project::UnitInfo(clsname));
	win.Shapes[win.Shapes.size() - 1].Data.swap(*vector);
	Project::WinCtrlID winctrlid = WINCTR_ID(winno, win.Shapes.size() - 1);
	ctrlidKV_.insert(std::pair<const char*, Project::WinCtrlID>(ctrlname, winctrlid));
	return winctrlid;
	/*
	if (prj_->Pages.Screens.find(0) == prj_->Pages.Screens.end())
		return -1;
	Project::HMIWindow& win = prj_->Pages.Screens[0];
	Project::UnitInfo uinfo;
	uinfo.CtrName = "undefine name!";
	std::shared_ptr<UI::BaseModel> mode = ctrl->GetMode();
	std::unique_ptr<std::vector<char>> vector = mode->GetData();
	win.Shapes[win.Shapes.size() - 1].Data.swap(*vector);
	win.Shapes.push_back(uinfo);
	*/
	return 1;
}

int  ProjectTestHelper::AddDataVar(
		short  DevId, // 设备ID索引号
		short  StaNo, //站号
		short  RegType, //寄存器 
		short  DataType, //寄存器类型   BIT WORD DDWORD 
		// dataType==BIT 的时候，Count为位的偏移值，为其他时表示字节个数
		short  ExType,
		int    Addr,  //地址偏移 
	    short  Count, //字节数量  ->根据regType获取单个寄存器字节数
		const char* varname //控件名称
	)
{
	Project::DataVarInfo info;
	info.DevId = DevId;
	info.StaNo = StaNo;
	info.RegType = RegType;
	info.DataType = DataType;
	info.Count = Count;
	info.Addr = Addr;
	info.Data = 0;
	info.DataExType = ExType;
	prj_->Vars.VarInfos.push_back(info);
	varIdKV_.insert(std::pair<const char*,unsigned int>(varname, prj_->Vars.VarInfos.size() - 1));
	return prj_->Vars.VarInfos.size() - 1;
}
void ProjectTestHelper::BindCtrlIDAndVar(const char* varname, int ctrlid)
{
	unsigned int varid = varIdKV_[varname];
	prj_->Vars.VarInfos[varid].CtrlId = ctrlid;
}
void ProjectTestHelper::BindCtrlAndVar(const char* varname, const char* ctrlname)
{
	Project::WinCtrlID ctrid = ctrlidKV_[ctrlname];
	unsigned int varid = varIdKV_[varname];
	prj_->Vars.VarInfos[varid].CtrlId = ctrid;
}
unsigned int  ProjectTestHelper::GetVarID(const char* varname)
{
	return varIdKV_[varname];
}
void ProjectTestHelper::Update()
{
	//int, WindowVar
	//prj_->Vars.WinVarInfos.
	for (size_t i = 0; i < prj_->Vars.VarInfos.size(); i++)
	{
		int winno = Project::GetWinNo(prj_->Vars.VarInfos[i].CtrlId);
		int ctrlno = Project::GetCtrlNo(prj_->Vars.VarInfos[i].CtrlId);
		if (prj_->Vars.WinVarInfos.find(winno) == prj_->Vars.WinVarInfos.end())			
		{
			prj_->Vars.WinVarInfos.insert(std::pair<int, Project::WindowVar>(winno, Project::WindowVar()));			
		}
		Project::WindowVar& winvar = prj_->Vars.WinVarInfos[winno];
		prj_->Vars.WinVarInfos[winno].VIds.push_back(Project::DataVarId(i));
	}
	std::map<int, Project::WindowVar>::iterator iter;
	iter = prj_->Vars.WinVarInfos.begin();
	while (iter != prj_->Vars.WinVarInfos.end()) {
		int size = 0;
		for (size_t i = 0; i < iter->second.VIds.size(); i++)
		{

			if (prj_->Vars.VarInfos[iter->second.VIds[i].Vid].Count > 4)
				size += prj_->Vars.VarInfos[iter->second.VIds[i].Vid].Count;
		}
		iter->second.BufSize = size;
		iter++;
	}

}