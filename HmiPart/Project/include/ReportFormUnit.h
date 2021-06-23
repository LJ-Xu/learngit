#pragma once
#include "ImageResId.h"
#include "BaseLine.h"
#include "AlarmSeekUnit.h"
namespace Project
{
	enum  ReportSeekMode { ReportDATE, ReportTIME, ReportREG };
	class ReportFormUnit : public AlarmSeekUnit
	{
	public:
		int Appearance;				//�����ʽ 0��ʹ��ͼƬ��ʽ 1��ʹ���Զ������
		ImageResId Key;				//���ͼƬ

		int TableBgColor;			//�б�����ɫ
		int TitleBgColor;			//���ⱳ����ɫ
		bool IsUseBorder;			//�Ƿ�ʹ�����
		BaseLine TableBorderStyle;	//�������ʽ
		BaseLine GridBorderStyle;	//��������ʽ
		bool IsRowDividLine;		//�Ƿ�ʹ����������
		bool IsColDividLine;		//�Ƿ�ʹ����������
		ReportSeekMode SearchMode;	//��ѯģʽ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Appearance, Key, TableBgColor, TitleBgColor, IsUseBorder, TableBorderStyle,
				GridBorderStyle, IsRowDividLine, IsColDividLine, SearchMode);
		}
	};

}

