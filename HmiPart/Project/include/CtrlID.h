#pragma once
namespace Project
{
	//typedef unsigned int  CtrlID;
	typedef unsigned int  WinCtrlID;
	typedef unsigned int  CtrlIDX;
	static WinCtrlID WinCtrlIDEmpty = -1;
	#define CTR_ID(winid,ctrlid) (((winid)<<16) | ((ctrlid)&0xFFFF))
	#define WINCTR_ID(winid,ctrlid) (((winid)<<16) | ((ctrlid)&0xFFFF))
	static int GetCtrlNo(Project::WinCtrlID ctrno)
	{
		return ctrno & 0xFFFF;
	}
	static int GetWinNo(Project::WinCtrlID ctrno)
	{
		return ctrno >> 16;
	}
}
