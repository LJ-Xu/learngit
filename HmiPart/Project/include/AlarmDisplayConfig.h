#pragma once
#include "ControlModel.h"
#include "StatusRes.h"
#include "StringStyle.h"
#include "AlarmTableUnit.h"
namespace Project
{
	enum ConfirmMode :char {Single, Double, Long};
	enum AlarmDisMode :char {REALTIME,HISTORY,SEARCH};		//ʵʱ����ʷ
	struct ColInfo
	{
		std::string ProjectName;			//��ʾ��Ŀ
		std::string DescribeName;			//��Ŀ����
		StatusRes TitleDescribe;			//��������Ŀ����
		int RowHeight;						//�и�
		int ColWidth;						//�п�
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ProjectName, DescribeName, TitleDescribe, RowHeight, ColWidth);
		}
	};
	enum TimeMode :char { HMS, HM, HMSMS,HMSChinese};
	enum DateMode :char { YMDSlash, DMYSlash, MDYSlash, YMDChinese };
	class AlarmDisplayConfig : public AlarmTableUnit
	{
	public:
		int StartGroupName;						//��ʾ�����ʼ
		int EndGroupName;						//��ʾ������

		bool UseTitle;							//�Ƿ�ʹ�ñ���
		bool UseSameStyle = false;				//�����б��Ƿ�ʹ��ͬ��ʽ
		StatusRes Title;						//��������
		StringStyle TitleStringStyle;			//��������
		StringStyle ListTitleStyle;				//�б�������
		StringStyle ListStringStyle;			//�б�����
		StringStyle SameStringStyle;			//��ͬ����

		AlarmDisMode AlarmMode;					//����ģʽ	0��ʵʱ	1����ʷ 2����ѯ
		int AlarmMaxNum;						//����������
		int PageNum;							//����ÿҳ����
		bool IsUseMultiLanguage;				//�Ƿ�ʹ�ö�����
		vector<ColInfo> AlarmOptions;			//������ʾѡ��
		int TitleHeight;						//����߶�
		int EachColTitleHeight;					//�б���߶�
		int SortMode;							//����ʽ	0��ʱ��˳�� 1��ʱ������
		TimeMode Timemode;						//ʱ���ʽ
		DateMode Datemode;						//���ڸ�ʽ
		bool TopNotRecover;						//�Ƿ��ö�δ�ָ���Ϣ 0�����ö� 1���ö�
		bool UseConfirmMode;					//�Ƿ�����ȷ��	0��������	1������
		ConfirmMode CMode;						//ȷ��ģʽ
		bool IsInfoHideCtrl;					//�Ƿ��������
		DataVarId HideVar;						//����������Ϣ�ļĴ���
													//bit0 ������ȷ����Ϣ
													//bit1 �����ѻָ���Ϣ
		DataVarId AlarmVarId;					//������Ϣ����
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(StartGroupName, EndGroupName, UseTitle, UseSameStyle, Title, EachColTitleHeight,
				TitleStringStyle, ListTitleStyle, ListStringStyle, SameStringStyle,
				AlarmMode, AlarmMaxNum, PageNum, IsUseMultiLanguage, AlarmOptions, TitleHeight,
				SortMode, Timemode, Datemode, TopNotRecover, UseConfirmMode, CMode, HideVar,
				BackColor, TitleBgColor, UseFrame, FrameStyle, UseGrid, GridStyle, AlarmVarId,
				AlarmTextColor, AlarmBackColor, UseConfirm, ConfirmTextColor, ConfirmBackColor,
				UseRecover, RecoverTextColor, RecoverBackColor, SearchMode, IsInfoHideCtrl,
				EnableSearch, SeekModeVar, SeekCtrlVar, SearchDate,  SearchDateVarId,
				SearchTimeStart, SearchTimeEnd, SearchTimeVar, SearchGroup, SearchGroupVar,
				SearchNo, SearchNoVar, SearchLevel, SearchLevelVar, RegMode, SearchRegVarId,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
		//int HideFlag;
		//ControlModel model;

	};
}