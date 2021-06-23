#pragma once
namespace Project
{
	class ControlModel
	{
	public:
		int modetype;	//0:实时 1:历史
		int modesort;	//显示信息顺序
		int modeconfirm;	//0:单机	1:双击
		int modecontrol;	//寄存器控制显示
	};

}