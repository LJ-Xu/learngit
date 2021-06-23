#pragma once
#include "ImageResId.h"
#include "BaseLine.h"
#include "AlarmSeekUnit.h"
#ifdef __linux
#ifdef None
#undef None
#endif
#endif
namespace Project
{
	enum  RecipeSeekMode { RecipeKey, RecipeData, RecipeREG };
	enum GridStyle
	{
		None = 0,
		Horizontal = 1,
		Vertical = 2,
		Inner = 3,
		Outer = 4,
		OuterHorizontal = 5,
		OuterVertical = 6,
		GridAll = 7
	};
	class RecipeFormUnit : public AlarmSeekUnit
	{
	public:  
		int AppearMode;				//�����ʽ 0��ʹ��ͼƬ��ʽ 1��ʹ���Զ������
		ImageResId Key;				//���ͼƬ

		int TitleBackColor;			//���ⱳ����ɫ
		int ColTitleBackColor;		//�б��ⱳ����ɫ
		int TableBgColor;			//�б�����ɫ
		bool ParityDiffColor;		//��ż�в�ͬ��ɫ
		int OddBackColor;			//������ɫ
		int EvenBackColor;			//ż����ɫ

		//GridStyle PresetBorder;		//�߿�����

		bool IsUseBorder;			//�Ƿ�ʹ�����
		BaseLine TableBorderStyle;	//�������ʽ
		BaseLine GridBorderStyle;	//��������ʽ
		bool IsRowDividLine;		//�Ƿ�ʹ����������
		bool IsColDividLine;		//�Ƿ�ʹ����������

		int FocusFontColor;
		int FocusRowColor;
		int FocusCellColor;
		RecipeSeekMode SearchMode;	//��ѯģʽ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(AppearMode, Key, TableBgColor, TitleBackColor, ColTitleBackColor,
				ParityDiffColor, OddBackColor, EvenBackColor, IsUseBorder, TableBorderStyle,
				GridBorderStyle, IsRowDividLine, IsColDividLine, FocusFontColor, FocusCellColor,
				FocusRowColor, SearchMode);
		}

	};
}

