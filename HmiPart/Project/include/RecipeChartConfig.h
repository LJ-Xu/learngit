#pragma once
#include "MoveableDisplayUnit.h"
#include "RecipeInfoRes.h"
#include "StatusRes.h"
#include "StringStyle.h"
#include "ReportChannel.h"
#include "RecipeFormUnit.h"
#include "InputNumShowUnit.h"
namespace Project
{
	class RecipeChartConfig : public RecipeFormUnit
	{
	public:
		DataVarId UpdateNotice;							//�䷽���ݸ���֪ͨ
		string RecipeGroupId;							//�䷽������
		int ArrangMode;									//����ʽ 0:˳�� 1:����
		int TotalRowCount;								//�������
		int PerPageRowCount;							//ÿҳ����
		bool TitleUsed;									//�Ƿ���ʾ����
		int TitleHeight = 0;							//����߶�
		StatusRes TxtTitle;								//��������
		StringStyle TitleStyle;							//������ʽ
		int EachColTitleHeight = 0;						//�б���߶�
		bool ColTitleShown;								//�Ƿ�ʹ���б���
		StringStyle ColTitleStyle;						//�б�����ʽ
		vector<DisplayReportLst> InfoLst;				//��ʾ��Ϣ��Ŀ
		bool SerialNoShown;								//�Ƿ���ʾ���
		int SerialNumStyle;								//�����ʽ 0������ 1������
		bool IsUseMultiLan;								//�Ƿ�ʹ�ö�����
		bool AutoSize;									//�Ƿ��Զ��и��п�
		StringStyle ListFontStyle;						//�б�������ʽ
		KeyboardRes KeyboardNum;						//���ּ�������
		KeyboardRes KeyboardChar;						//�ַ���������

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(UpdateNotice, RecipeGroupId, ArrangMode, TotalRowCount, PerPageRowCount, TitleUsed,
				TitleHeight, TxtTitle, TitleStyle, EachColTitleHeight, ColTitleShown,
				ColTitleStyle, InfoLst, SerialNoShown, SerialNumStyle, IsUseMultiLan,
				AutoSize, ListFontStyle, KeyboardNum, KeyboardChar,AppearMode, Key, TableBgColor, TitleBackColor, ColTitleBackColor,
				ParityDiffColor, OddBackColor, EvenBackColor, IsUseBorder, TableBorderStyle,
				GridBorderStyle, IsRowDividLine, IsColDividLine, FocusFontColor, FocusCellColor,
				FocusRowColor, SearchMode, EnableSearch, SeekModeVar, SeekCtrlVar, SearchDate, SearchDateVarId,
				SearchTimeStart, SearchTimeEnd, SearchTimeVar, SearchGroup, SearchGroupVar, RegMode, SearchRegVarId,
				SearchNo, SearchNoVar, SearchLevel, SearchLevelVar, SearchChannelNo, SearchChannelNoVar, SearchKey, SearchKeyVar,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

