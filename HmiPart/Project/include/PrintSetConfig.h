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
		bool PrintEnable;				//�Ƿ�ʹ�ô�ӡ����
		PrinterType Type;				//��ӡ������
		string PrinterBrand;			//��ӡ��������
		string PrinterModele;			//��ӡ���ͺ�
		string ComName;					//������
		string ComType;					//�������� 232 485 422
		int ComBaudreate;				//���ڲ�����
		int ComParity;					//������żУ�� 0:none 1:odd 2:even 3:mark 4:space
		int ComDataBit;					//��������λ 0:7 1:8
		int ComStopBit;					//����ֹͣλ 0:none 1:1 2:2 3:(1.5)
		CutPaperStyle CutPaperConfig;
        Project::PaperSize PaperSize;			//ֽ�Ŵ�С
		vector<int> IpAddress;			//��ӡ��IP��ַ
		int NetPort;					//��ӡ������˿�
		int PrintDirection;				//��ӡ���� 0:ˮƽ 1:��ֱ
		DataVarId PrintTrigVar;			//��ӡ�����Ĵ���
		int BitTriggerCond;				//λ�������� 0:�½��� 1:������
		PrintWin PrintSource;			//��ӡ��Դ 0:��ǰ���� 1:�Ĵ���ָ�� 2:ָ��������
		DataVarId PrintSourceVar;		//ָ����ӡ���ڼĴ���
		int PrintSourceWinNo;			//��ӡ������
		double PrintZoom;				//�������ű���
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
