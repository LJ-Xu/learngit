#pragma once
#include "stdafx.h"
#include "BasicView.h"
#include "StringStyle.h"

namespace UI
{
	class AdvancedGarphic : public Fl_Widget
	{
	public:
		//��ȡ�����ڵ�����
		static int GetQuaofpoint(int px, int py);
		//��ȡԲ����������
		static void GetArcField(double startAngle, double endAngle, int &containquardences, int& existquardences);
	public:
		//������
		static void DrawSector(double startAngle,double endAngle, unsigned long fillRadius, unsigned long hollowRadius,int red,int green,int blue,int alpha=255);
		//����ֱ���̶�
		//isHorizontal:trueΪˮƽ���У�����ֱ����
		//alignMode:���뷽ʽ��0Ϊ��/�϶��룬1Ϊ��/�¶���
		//dx dy:������ʼ��
		//len:���䳤��
		//mcount scount:���̶��� ���̶���
		//mlen slen:���̶ȳ��� ���̶ȳ���
		static void DrawScale(bool isHorizontal, int alignMode,int dx,int dy,int len,int mcount,int scount,int mlen,int slen,int style, int color, int widget);
		//�������ο̶�
		//static void DrawScale();

		//�������ο̶ȶ�Ӧ�����ֱ�ǩ
		//x��y:����ϵ���ĵ�����
		//fontStyle:�ַ���ǩ������
		//radius:�ַ���ǩ���Ƶ����ĵ�ľ���뾶
		//count:�ָ���ַ���ǩ��
		//minvalue��maxvalue:�ַ���ǩ������
		//startAngle��sweepAngle:��ʼ�Ƕȡ������Ƕȣ�sweepAngleΪ��ʱ��ʱ��ת��
		//stringFormat:�ַ���ʽ
		static void DrawScaleMark(int x, int y, const Project::StringStyle &fontStyle, int radius, int count, float minvalue, float maxvalue, double startAngle, double sweepAngle, char* stringFormat,int maxDgtLen, bool ShowPercent = false, int active = 1);
		//����ֱ���̶ȶ�Ӧ�����ֱ�ǩ
		//direction:����0�� 1�� 2�� 3��
		//x:������ʼ����
		//y:
		//len:���췽��ĳ���
		//fontStyle:����
		//count:��ǩ�ָ���
		//minvalue:��Сֵ
		//maxvalue:���ֵ
		//stringFormat:
		//intDgt:
		//decDgt:
		static void DrawScaleMark(int direction, int x, int y,int len, const Project::StringStyle &fontStyle, int count,float minvalue, float maxvalue, char* stringFormat,int MaxDgtLen,int active=1);
	};
}
