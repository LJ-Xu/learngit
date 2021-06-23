#pragma once
#include "MoveableDisplayUnit.h"
namespace Project
{
	enum PrinterType {SerialPrinter,USBPrinter,NetPrinter};
	enum CutPaperStyle {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10,
		B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10	};
	enum PaperSize {NoCut,FullCut,HalfCut};
	enum PrintWin {PrintCurrentWin,PrintRegWin,PrintWinNo};
	class PrintSetConfig :public MoveableDisplayUnit
	{
	public:
		bool PrintEnable;				//是否使用打印功能
		PrinterType Type;				//打印机种类
		string PrinterBrand;			//打印机制造商
		string PrinterModele;			//打印机型号
		string ComName;					//串口名
		string ComType;					//串口类型 232 485 422
		int ComBaudreate;				//串口波特率
		int ComParity;					//串口奇偶校验 0:none 1:odd 2:even 3:mark 4:space
		int ComDataBit;					//串口数据位 0:7 1:8
		int ComStopBit;					//串口停止位 0:none 1:1 2:2 3:(1.5)
		CutPaperStyle CutPaperConfig;
        Project::PaperSize PaperSize;			//纸张大小
		vector<int> IpAddress;			//打印机IP地址
		int NetPort;					//打印机网络端口
		int PrintDirection;				//打印方向 0:水平 1:垂直
		DataVarId PrintTrigVar;			//打印触发寄存器
		int BitTriggerCond;				//位触发条件 0:下降沿 1:上升沿
		PrintWin PrintSource;			//打印来源 0:当前画面 1:寄存器指定 2:指定画面编号
		DataVarId PrintSourceVar;		//指定打印窗口寄存器
		int PrintSourceWinNo;			//打印画面编号
		double PrintZoom;				//画面缩放比例
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(PrintEnable, Type, PrinterBrand, PrinterModele, ComName, ComType,
				ComBaudreate, ComParity, ComDataBit, ComStopBit, CutPaperConfig, PaperSize,
				IpAddress, NetPort, PrintDirection, PrintTrigVar, PrintSource, PrintSourceVar,
				PrintSourceWinNo, PrintZoom, VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}
