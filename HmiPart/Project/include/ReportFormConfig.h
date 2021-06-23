#pragma once
#include "SampleInfoRes.h"
#include "ReportChannel.h"
#include "AlarmDisplayConfig.h"
#include "ReportFormUnit.h"
namespace Project
{

	class ReportFormConfig	: public ReportFormUnit
	{
	public:
		int SimpleGroupName;				//������
		int SimpleGroupNo;					//�������
		DataVarId SampleVarId;				//֪ͨ�������ݱ仯�Ĵ���
		vector<ReportChannel> ReportChannelLst;		//����ͨ������
		bool IsViewSerialNo;				//�Ƿ���ʾ���
		int SerialNum;						//���λ��
		int SerialColor;					//�����ɫ
		bool IsViewDate;					//�Ƿ���ʾ����
		int DateColor;						//������ɫ
		DateMode DateStyle;					//���ڸ�ʽ
		int IsViewTime;						//�Ƿ���ʾʱ��
		int TimeColor;						//ʱ����ɫ
		TimeMode TimeStyle;					//ʱ���ʽ
		int MaxRows;						//�������
		int PerPageRows;					//ÿҳ����
		int ViewSort;						//��ʾ˳��	0��˳�� 1������
		bool IsModifyCellValue;				//�Ƿ���޸ĵ�Ԫ������
		bool IsViewTitle;					//�Ƿ���ʾ����
		int TitleHeight = 0;					//����߶�
		StatusRes TitleRes;					//������������
		StringStyle ElementTitleStyle;		//������ʽ
		bool IsViewEachColTitle;			//�Ƿ�ʹ���б���
		int EachColTitleHeight = 0;			//�б���߶�
		bool IsUseMultiLan;					//�ǵ�ʹ�ö�����
		bool IsAutoRowColWidth;				//�Ƿ��Զ��и��п�
		//�����Ҫ��ʾ����Ϣ
		vector<DisplayReportLst> InfoLst;	//��ʾ��Ϣ��Ŀ
		StringStyle TitleBarStyle;			//�б�����ʽ
		StringStyle ListFontStyle;			//�б�������ʽ


		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(SimpleGroupName, SimpleGroupNo, ReportChannelLst, IsViewSerialNo, SerialNum, SerialColor,
				IsViewDate, DateColor, DateStyle, IsViewTime, TimeColor, EachColTitleHeight, SampleVarId,
				TimeStyle, MaxRows, PerPageRows, ViewSort, IsModifyCellValue, IsViewTitle, TitleHeight,
				TitleRes, ElementTitleStyle, IsViewEachColTitle, IsUseMultiLan, IsAutoRowColWidth,
				InfoLst, TitleBarStyle, ListFontStyle, Appearance, Key, TableBgColor, TitleBgColor,
				IsUseBorder, TableBorderStyle, GridBorderStyle, IsRowDividLine, IsColDividLine,
				EnableSearch, SearchMode, SeekModeVar, SeekCtrlVar, SearchDate, SearchDateVarId,
				SearchTimeStart, SearchTimeEnd, SearchTimeVar, SearchGroup, SearchGroupVar,
				SearchNo, SearchNoVar, SearchLevel, RegMode, SearchRegVarId,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}
