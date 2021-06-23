#pragma once
#include "SampleInfoRes.h"
#include "ReportChannel.h"
#include "AlarmDisplayConfig.h"
namespace Project
{
	enum OperatorSeekMode	{ OperatorByDate, OperatorByTime, OperatorByName, OperatorByReg};
	class OperatorChartConfig : public AlarmSeekUnit
	{
	public:
		vector<DisplayReportLst> DisplayItems;		//��Ҫ��ʾ����Ϣ
		DataVarId RecordVarId;						//֪ͨ������¼�任�Ĵ���

		bool UseMultiLanguage;						//�Ƿ�ʹ�ö�����
		int SortMode;								//0��ʱ��˳��1��ʱ������
		TimeMode TimeFormate;						//ʱ���ʽ
		DateMode DateFormate;						//���ڸ�ʽ
		int RowHeight;								//�и�
		bool UsedTitle;								//�Ƿ�ʹ�ñ���
		StatusRes TitleStringRes;					//����
		int TitleHeight;							//�����и�

		bool IsUseSameStyle;						//�Ƿ�ʹ����ͬ����
		StringStyle TitleStringStyle;				//����������ʽ
		StringStyle RecordItemStringStyle;			//�б�������ʽ

		int TableBgColor;							//�б�����ɫ
		int TitleBgColor;							//���ⱳ����ɫ
		bool IsUseBorder;							//�Ƿ�ʹ�����
		BaseLine TableBorderStyle;					//�������ʽ
		BaseLine GridBorderStyle;					//��������ʽ
		bool IsRowDividLine;						//�Ƿ�ʹ����������
		bool IsColDividLine;						//�Ƿ�ʹ����������

		OperatorSeekMode SearchMode;					//��ѯģʽ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(DisplayItems, UseMultiLanguage, SortMode, TimeFormate, DateFormate, RecordVarId,
				RowHeight, UsedTitle, TitleStringRes, TitleHeight, IsUseSameStyle, TitleStringStyle,
				RecordItemStringStyle,TableBgColor, TitleBgColor, IsUseBorder, TableBorderStyle, 
				GridBorderStyle, IsRowDividLine, IsColDividLine, EnableSearch, SearchMode, SeekModeVar, 
				SeekCtrlVar, SearchDate, SearchDateVarId, SearchTimeStart, SearchTimeEnd, RegMode, SearchRegVarId,
				SearchKey, SearchKeyVar, VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}
