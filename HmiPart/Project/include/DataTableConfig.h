#pragma once
#include "DataTableUnit.h"
#include "ReportChannel.h"
#include "StatusRes.h"
#include "StringStyle.h"
namespace Project
{
	class DataTableConfig :public DataTableUnit
	{
	public:
		bool UseContineAddr;					//�Ƿ�ʹ��������ַ
		int RowsNum;							//����
		int PerPageRows;						//ÿҳ����
		int ColsNum;							//����
		
		bool UseTitleBar;						//�Ƿ�ʹ�ñ���
		int TitleHeight;						//�����
		StatusRes TitleRes;						//��������
		bool ColTitleUsed;						//�Ƿ�ʹ���б���
		int ColTitleHeight;						//�б����
		bool ColTitleMultiLangsUsed;			//�б����Ƿ�ʹ�ö�����
		vector<DisplayReportLst> ColInfoLst;	//ÿ��������Ϣ
		int SerialNum;							//���λ��
		vector<StatusRes> StaticColName;		//��̬������
		bool UseSameStyle;						//�Ƿ�ʹ��ͬһ����
		StringStyle TitleBarStyle;				//������ʽ
		StringStyle ListFontStyle;				//�б�������ʽ
		StringStyle SameFontStyle;				//ͬ��ʽ

		vector<DataCell> DataCellInfoLst;		//���Ԫ������

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(UseContineAddr, RowsNum, PerPageRows, ColsNum, UseTitleBar, ColTitleHeight,
				ColInfoLst, SerialNum, StaticColName, UseSameStyle, TitleBarStyle, TitleRes,
				ListFontStyle, SameFontStyle, DataCellInfoLst, TitleHeight, ColTitleUsed,
				Appearance, Key, TableBgColor, TitleBgColor, IsUseBorder, ColTitleMultiLangsUsed,
				TableBorderStyle, GridBorderStyle, IsRowDividLine, IsColDividLine,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}


