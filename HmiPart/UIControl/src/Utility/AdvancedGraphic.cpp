#include "stdafx.h"
#include "AdvancedGraphic.h"
#include <FL/Fl_Image.H>
#include <math.h>
//���޷�Χ
enum PointFieldOnQuadrant
{
	//CORCENTER = 0x00,//ԭ��
	QUADRANT1ST = 0x01,//��һ����,����������
	QUADRANT2ND = 0x02,//�ڶ�����,����������
	QUADRANT3RD = 0x04,//��������,����������
	QUADRANT4TH = 0x08,//��������,����������
	OnTime0 = 0x10,//����
	OnTime90 = 0x20,
	OnTime180 = 0x30,
	OnTime270 = 0x40,
	Circle//��Բ
};




namespace UI
{
	int AdvancedGarphic::GetQuaofpoint(int px, int py)
	{
		if ((!px) && (!py))
			return -1;
		if (px < 0)
		{
			//�ڶ�����
			if (py < 0)
				return QUADRANT2ND;
			//��������
			else if (py > 0)
				return QUADRANT3RD;
			else
				return OnTime180;
		}
		else if(px > 0)
		{
			//��������
			if (py > 0)
				return QUADRANT4TH;
			//��һ����
			else if (py < 0)
				return QUADRANT1ST;
			else
				return OnTime0;
		}
		else
		{
			if (py > 0)
				return OnTime90;
			else
				return OnTime270;
		}
	}

	
	void AdvancedGarphic::GetArcField(double startAngle, double endAngle, int &containquardences, int& existquardences)
	{
		containquardences = 0;//��ȫ��������
		existquardences = 0;//������(����������)����
		//�ж���ʼ�Ƕ����ڵ����ַ���(��0��3,ÿ90��һ������)
		//   ---------
		//	 |   |   |
		//   |   |   |
		//   | 1 | 0 |
		//   ---------
		//	 | 2 | 3 |
		//   |   |   |
		//   |   |   |
		//   ---------
		double rStAg, rEdAg;
		rStAg = (startAngle > 360 ? startAngle - 360 : startAngle);
		rEdAg = (endAngle > 360 ? endAngle - 360 : endAngle);
		if (rStAg == rEdAg)
			containquardences = Circle;
		//�жϽǶ���������
		int startQuadrant, endQuadrant;

		if (rStAg == 0)
			startQuadrant = OnTime0;
		else if (rStAg == 90)
			startQuadrant = OnTime90;
		else if (rStAg == 180)
			startQuadrant = OnTime180;
		else if (rStAg == 270)
			startQuadrant = OnTime270;
		else
		 startQuadrant = 1 << ((int)(startAngle / 90.0) % 4);
		if (rEdAg == 0)
			endQuadrant = OnTime0;
		else if (rEdAg == 90)
			endQuadrant = OnTime90;
		else if (rEdAg == 180)
			endQuadrant = OnTime180;
		else if (rEdAg == 270)
			endQuadrant = OnTime270;
		else
			endQuadrant = 1<<((int)(endAngle / 90.0) % 4);
		/*bool startOnAxis = ((int)startAngle % 90) == 0;
		bool endOnAxis = ((int)endAngle % 90) == 0;*/
		
		switch (startQuadrant)
		{
		case QUADRANT1ST:
		{
			existquardences |= QUADRANT1ST;
			switch (endQuadrant)
			{
			case QUADRANT1ST:
			{
				if (rEdAg < rStAg)
				{
					containquardences |= OnTime0;
					containquardences |= OnTime90;
					containquardences |= OnTime180;
					containquardences |= OnTime270;
					containquardences |= QUADRANT2ND;
					containquardences |= QUADRANT3RD;
					containquardences |= QUADRANT4TH;
				}
			}
			break;
			case QUADRANT2ND:
			{
				containquardences |= OnTime90;
				existquardences |= QUADRANT2ND;
			}
			break;
			case QUADRANT3RD:
			{
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= QUADRANT2ND;
				existquardences |= QUADRANT3RD;
			}
			break;
			case QUADRANT4TH:
			{
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT3RD;
				existquardences |= QUADRANT4TH;
			}
			break;
			case OnTime0:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= OnTime270;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT3RD;
				containquardences |= QUADRANT4TH;
			}
			break;
			case OnTime90:
			{
				containquardences |= OnTime90;
			}
			break;
			case OnTime180:
			{
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= QUADRANT2ND;
			}
			break;
			case OnTime270:
			{
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= OnTime270;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT3RD;
			}
			break;
			}
		}
			break;
		case QUADRANT2ND:
		{
			existquardences |= QUADRANT2ND;
			switch (endQuadrant)
			{
			case QUADRANT1ST:
			{
				containquardences |= OnTime90;
				existquardences |= QUADRANT1ST;
			}
			break;
			case QUADRANT2ND:
			{
				if (rEdAg < rStAg)
				{
					containquardences |= OnTime90;
					containquardences |= OnTime180;
					containquardences |= OnTime270;
					containquardences |= QUADRANT1ST;
					containquardences |= QUADRANT3RD;
					containquardences |= QUADRANT4TH;
				}
				
			}
			break;
			case QUADRANT3RD:
			{
				containquardences |= OnTime180;
				existquardences |= QUADRANT3RD;
			}
			break;
			case QUADRANT4TH:
			{
				containquardences |= OnTime180;
				containquardences |= OnTime270;
				containquardences |= QUADRANT3RD;
				existquardences |= QUADRANT4TH;
			}
			break;
			case OnTime0:
			{

				containquardences |= OnTime0;
				containquardences |= OnTime180;
				containquardences |= OnTime270;
				containquardences |= QUADRANT3RD;
				containquardences |= QUADRANT4TH;
			}
			break;
			case OnTime90:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= OnTime270;
				containquardences |= QUADRANT1ST;
				containquardences |= QUADRANT3RD;
				containquardences |= QUADRANT4TH;
			}
			break;
			case OnTime180:
			{
				containquardences |= OnTime180;
			}
			break;
			case OnTime270:
			{
				containquardences |= QUADRANT3RD;
				containquardences |= OnTime180;
				containquardences |= OnTime270;
			}
			break;
			}
		}
			break;
		case QUADRANT3RD:
		{
			existquardences |= QUADRANT3RD;
			switch (endQuadrant)
			{
			case QUADRANT1ST:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime270;
				containquardences |= QUADRANT4TH;
				existquardences |= QUADRANT1ST;
			}
			break;
			case QUADRANT2ND:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= OnTime270;
				containquardences |= QUADRANT1ST;
				containquardences |= QUADRANT4TH;
				existquardences |= QUADRANT2ND;
			}
			break;
			case QUADRANT3RD:
			{
				if (rEdAg < rStAg)
				{
					containquardences |= OnTime0;
					containquardences |= OnTime90;
					containquardences |= OnTime180;			
					containquardences |= OnTime270;
					containquardences |= QUADRANT1ST;
					containquardences |= QUADRANT2ND;
					containquardences |= QUADRANT4TH;
				}
			}
			break;
			case QUADRANT4TH:
			{
				containquardences |= OnTime270;
				existquardences |= QUADRANT4TH;
			}
			break;
			case OnTime0:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime270;
				containquardences |= QUADRANT4TH;
			}
			break;
			case OnTime90:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= OnTime270;
				containquardences |= QUADRANT1ST;
				containquardences |= QUADRANT4TH;
			}
			break;
			case OnTime180:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= OnTime270;
				containquardences |= QUADRANT1ST;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT4TH;
			}
			break;
			case OnTime270:
			{
				containquardences |= OnTime270;
			}
			break;
			}
		}
			break;
		case QUADRANT4TH:
		{
			existquardences |= QUADRANT4TH;
			switch (endQuadrant)
			{
			case QUADRANT1ST:
			{
				containquardences |= OnTime0;
				existquardences |= QUADRANT1ST;
			}
			break;
			case QUADRANT2ND:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= QUADRANT1ST;
				existquardences |= QUADRANT2ND;
			}
			break;
			case QUADRANT3RD:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT3RD;
				existquardences |= QUADRANT3RD;
			}
			break;
			case QUADRANT4TH:
			{
				if (rEdAg < rStAg)
				{
					containquardences |= OnTime0;
					containquardences |= OnTime90;
					containquardences |= OnTime180;
					containquardences |= OnTime270;
					containquardences |= QUADRANT1ST;
					containquardences |= QUADRANT2ND;
					containquardences |= QUADRANT3RD;
				}
			}
			break;
			case OnTime0:
			{
				containquardences |= OnTime0;
			}
			break;
			case OnTime90:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= QUADRANT1ST;
			}
			break;
			case OnTime180:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT3RD;
			}
			break;
			case OnTime270:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= OnTime270;
				containquardences |= QUADRANT1ST;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT3RD;
			}
			break;
			}
		}
			break;
		case OnTime0:
		{
			containquardences |= OnTime0;
			switch (endQuadrant)
			{
			case QUADRANT1ST:
			{
				existquardences |= QUADRANT1ST;
			}
			break;
			case QUADRANT2ND:
			{
				containquardences |= OnTime90;
				existquardences |= QUADRANT2ND;
			}
			break;
			case QUADRANT3RD:
			{
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= QUADRANT2ND;
				existquardences |= QUADRANT3RD;
			}
			break;
			case QUADRANT4TH:
			{
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= OnTime270;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT3RD;
				existquardences |= QUADRANT4TH;
			}
			break;
			case OnTime0:
			{
				//
			}
			break;
			case OnTime90:
			{
				containquardences |= QUADRANT1ST;
				containquardences |= OnTime90;
			}
			break;
			case OnTime180:
			{
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= QUADRANT1ST;
				containquardences |= QUADRANT2ND;
			}
			break;
			case OnTime270:
			{
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= OnTime270;
				containquardences |= QUADRANT1ST;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT3RD;
			}
			break;
			}
		}
			break;
		case OnTime90:
		{
			containquardences |= OnTime90;
			switch (endQuadrant)
			{
			case QUADRANT1ST:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime180;
				containquardences |= OnTime270;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT3RD;
				containquardences |= QUADRANT4TH;
				existquardences |= QUADRANT1ST;
			}
			break;
			case QUADRANT2ND:
			{
				existquardences |= QUADRANT2ND;
			}
			break;
			case QUADRANT3RD:
			{
				containquardences |= OnTime180;
				containquardences |= QUADRANT2ND;
				existquardences |= QUADRANT3RD;
			}
			break;
			case QUADRANT4TH:
			{
				containquardences |= OnTime180;
				containquardences |= OnTime270;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT3RD;
				existquardences |= QUADRANT4TH;
			}
			break;
			case OnTime0:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime180;
				containquardences |= OnTime270;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT3RD;
				containquardences |= QUADRANT4TH;
			}
			break;
			case OnTime90:
			{

			}
			break;
			case OnTime180:
			{
				containquardences |= OnTime180;
				containquardences |= QUADRANT2ND;
			}
			break;
			case OnTime270:
			{
				containquardences |= OnTime180;
				containquardences |= OnTime270;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT3RD;
			}
			break;
			}
		}
			break;
		case OnTime180:
		{
			containquardences |= OnTime180;
			switch (endQuadrant)
			{
			case QUADRANT1ST:
			{
				existquardences |= QUADRANT1ST;
				containquardences |= OnTime0;
				containquardences |= OnTime270;
				containquardences |= QUADRANT3RD;
				containquardences |= QUADRANT4TH;
			}
			break;
			case QUADRANT2ND:
			{
				existquardences |= QUADRANT2ND;
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= OnTime270;
				containquardences |= QUADRANT1ST;
				containquardences |= QUADRANT3RD;
				containquardences |= QUADRANT4TH;
			}
			break;
			case QUADRANT3RD:
			{
				existquardences |= QUADRANT3RD;
			}
			break;
			case QUADRANT4TH:
			{
				existquardences |= QUADRANT4TH;
				containquardences |= QUADRANT3RD;
				containquardences |= OnTime270;
			}
			break;
			case OnTime0:
			{
				containquardences |= OnTime270;
				containquardences |= QUADRANT3RD;
				containquardences |= QUADRANT4TH;
			}
			break;
			case OnTime90:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= OnTime270;
				containquardences |= QUADRANT1ST;
				containquardences |= QUADRANT3RD;
				containquardences |= QUADRANT4TH;
			}
			break;
			case OnTime180:
			{

			}
			break;
			case OnTime270:
			{
				containquardences |= QUADRANT3RD;
				containquardences |= OnTime270;
			}
			break;
			}
		}
			break;
		case OnTime270:
		{
			containquardences |= OnTime270;
			switch (endQuadrant)
			{
			case QUADRANT1ST:
			{
				existquardences |= QUADRANT1ST;
				containquardences |= OnTime0;
				containquardences |= QUADRANT4TH;
			}
			break;
			case QUADRANT2ND:
			{
				existquardences |= QUADRANT2ND;
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= QUADRANT1ST;
				containquardences |= QUADRANT4TH;
			}
			break;
			case QUADRANT3RD:
			{
				existquardences |= QUADRANT3RD;
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= QUADRANT1ST;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT4TH;
			}
			break;
			case QUADRANT4TH:
			{
				existquardences |= QUADRANT4TH;
			}
			break;
			case OnTime0:
			{
				containquardences |= OnTime0;
				containquardences |= QUADRANT4TH;
			}
			break;
			case OnTime90:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= QUADRANT1ST;
				containquardences |= QUADRANT4TH;
			}
			break;
			case OnTime180:
			{
				containquardences |= OnTime0;
				containquardences |= OnTime90;
				containquardences |= OnTime180;
				containquardences |= QUADRANT1ST;
				containquardences |= QUADRANT2ND;
				containquardences |= QUADRANT4TH;
			}
			break;
			case OnTime270:
			{

			}
			break;
			}
		}
			break;
		}
	}


	void AdvancedGarphic::DrawSector(double startAngle, double endAngle, unsigned long fillRadius, unsigned long hollowRadius, int red, int green, int blue, int alpha)
	{
		//��������
		//ͼƬ�ߴ�
		unsigned long circleRadius = fillRadius + hollowRadius;
		unsigned long imageWidth = circleRadius * 2;
		//���ݳ���
		unsigned long dataLen = imageWidth * imageWidth * 4;
		//ͼƬ����
		unsigned char* rgbData = new unsigned char[dataLen];
		memset(rgbData, 0, dataLen);
		//ͼƬ
		Fl_Image* sectorpic = new Fl_Image(imageWidth, imageWidth, 4);
		//��Բ��Ϊ0�㽨����ʮ������ϵ�µ�ĳ��ĺ�������(conx,cony)��õ㵽Բ�ĵľ���pt2ct
		unsigned long conx = 0, cony = 0;
		unsigned long pt2ct = 0;
		for (unsigned long py = 0; py < imageWidth; ++py)
		{
			for (unsigned long px = 0; px < imageWidth; ++px)
			{
				conx = px - circleRadius;
				cony = circleRadius - py;
				pt2ct = ::sqrt(conx*conx + cony * cony);
				//��������������
				if ((pt2ct <= circleRadius)||(pt2ct >= hollowRadius))
				{
					int fieldType = GetQuaofpoint(conx, cony);
					if (fieldType >= OnTime0)
					{

					}
				}
			}
		}
		
	}
	void AdvancedGarphic::DrawScale(bool isHorizontal,int alignMode, int dx, int dy, int len,int mcount, int scount, int mlen, int slen, int style,int color,int widget)
	{
		//���̶ȼ���͸��̶ȼ��
		int mOffset = (float)(--len) / (float)(mcount - 1),sOffset = 0;
		if (mcount > 1&& scount > 0)
		{
			sOffset = (float)(mOffset-1) / (float)(scount+1);
		}
		//������̶ȼ��֮�����������Ҫ��̯���̶ȵķֲ������У�
		int moffsetremainder = len - mOffset * (mcount - 1);
		int soffsetremainder = mOffset-1 - sOffset * (scount+1);
		//������
		int mRemainderInterval = 0;
		if(moffsetremainder)
			mRemainderInterval = (double)(mcount - 1) / (double)moffsetremainder;
		int sRemainderInterval = 0;
		if (soffsetremainder)
			sRemainderInterval = (double)(scount+1) / (double)soffsetremainder;
		//��ʱ�����������ͨ��Ϊ�̶���㣬����Ϊƽ������
		int tpMX=dx, tpMY=dy, tpSX=dx, tpSY=dy;
		
		fl_color(RGBColor(color));
		fl_line_style(style, widget);
		//����
		//ˮƽ����
		if (isHorizontal)
		{
			if (!alignMode)
			{
				tpSY = tpMY + mlen - slen;
			}
			fl_line(tpMX, tpMY, tpMX, tpMY + mlen);
			for (int im = 1; im < mcount; ++im)
			{
				int sremcaled = 0;
				for (int is = 1; is <= scount; ++is)
				{
					tpSX += sOffset;
					if ((soffsetremainder- sremcaled )&&is%sRemainderInterval == 1)
					{
						++tpSX;
						++sremcaled;
						//--soffsetremainder;
					}
					fl_line(tpSX, tpSY, tpSX, tpSY + slen);
				}
				tpMX += mOffset ;
				if (moffsetremainder&&im%mRemainderInterval == 0)
				{
					++tpMX;
					--moffsetremainder;
				}
				tpSX = tpMX;
				fl_line(tpMX, tpMY, tpMX, tpMY + mlen);
			}
		}
		//��ֱ����
		else
		{
			if (!alignMode)
			{
				tpSX = tpMX + mlen - slen;
			}
			fl_line(tpMX, tpMY, tpMX + mlen, tpMY);
			for (int im = 1; im < mcount; ++im)
			{
				int sremcaled = 0;
				for (int is = 1; is <= scount; ++is)
				{
					tpSY += sOffset;
					if ((soffsetremainder- sremcaled )&&is%sRemainderInterval == 0)
					{
						++tpSY;
						++sremcaled;
					}
					fl_line(tpSX, tpSY, tpSX + slen, tpSY);
				}
				tpMY += mOffset;
				if (moffsetremainder&&im%mRemainderInterval == 0)
				{
					++tpMY;
					--moffsetremainder;
				}
				tpSY = tpMY;
				fl_line(tpMX, tpMY, tpMX + mlen, tpMY);
			}
		}
		return;
	}

	void AdvancedGarphic::DrawScaleMark(int cx,int cy,const Project::StringStyle &fontStyle,int radius,int count,float minvalue,float maxvalue, double startAngle, double sweepAngle,char* stringFormat, int maxDgtLen,bool ShowPercent, int active)
	{
		/*��������*/
		fl_font(fontStyle.Font.Style, fontStyle.Font.Size);
		/*����������ɫ*/
		fl_color(active?fl_rgb_color(fontStyle.RGBColor.R, fontStyle.RGBColor.G, fontStyle.RGBColor.B): fl_inactive(fl_rgb_color(fontStyle.RGBColor.R, fontStyle.RGBColor.G, fontStyle.RGBColor.B)));

		float fValue = (float)minvalue;
		float gradInterval = (float)((maxvalue - minvalue) / (float)(count - 1));
		char cf[20];
		int iFontLen = 0;//�ַ�����
		int iFontSpace = 0;//�ַ�������1ʱ�Ĳ�������(��ͬ�Ƕ�����������)
		//double arcDistanceRate = 0.0;//�ַ��Ƕ�cosֵ
		double disX = 0.0, disY = 0.0;//�ַ�������ʼ����Կ̶ȶ�Ӧ�������ĵ��ƫ��
		double offX = 0.0, offY = 0.0;
		int markCount = count;
		if (abs(sweepAngle)>350)
		{
			--markCount;
		}
		double offsetAngle = sweepAngle / (count -1);
		double curAngle = startAngle;
		double rotateAngle = 0.0;
		int fontwidth = fontStyle.Font.Size;
		float tempValue = fValue;
		//double markDrawAngle = curAngle;
		for (int idxM = 0; idxM < markCount; ++idxM)
		{
#if 1
			memset(cf, 0, 20);
			if (ShowPercent)
				tempValue = maxvalue > minvalue ? fValue / (maxvalue - minvalue) * 100:0;
			else
				tempValue = fValue;
			int tempDgt = (int)tempValue;
			int len = 1;
			while (tempDgt > 0)
			{
				tempDgt /= 10;
				++len;
			}
			if (len > maxDgtLen)
			{
				cf[0] = '*';
			}
			else
			{
				if (ShowPercent)
				{
					sprintf(cf, stringFormat, tempValue);
				}
				else
				{
					sprintf(cf, stringFormat, tempValue);
				}
				
			}
#else
			memset(cf, 0, 20);
			sprintf(cf, stringFormat, fValue);
#endif
			iFontLen = strlen(cf);
			int offL = fl_width(cf, iFontLen)/2;
			int offH = fl_height()/2;
			//int offL = (iFontLen*fontStyle.Font.Size)/4;
			double arc = curAngle *M_PI / 180;
			int modAngle = curAngle;
			if (((int)(curAngle+90) % 180) != 0)
			{
				modAngle = curAngle - 90;
				offX = offL * sin(arc);
				offY = offH * cos(arc);
				disX = cx + (radius) * cos(arc);
				disY = cy - (radius) * sin(arc);
				fl_draw(modAngle, cf, disX- offX, disY- offY);
				
			}
			else
			{
				offX = offL ;
				offY = 0;
				disX = cx + (radius - offX) * cos(arc);
				disY = cy - (radius - offY) * sin(arc);
				fl_draw(cf, disX- offL, disY);
			}
			fValue += gradInterval;
			curAngle += offsetAngle;
		}

	}
	void AdvancedGarphic::DrawScaleMark(int direction,int dx, int dy, int len, const Project::StringStyle & fontStyle, int count, float minvalue, float maxvalue, char * stringFormat, int maxDgtLen, int active)
	{
		//��ʱ����������
		int tpDX, tpDY;
		//��ǰ����
		float fValue= (minvalue);
		//���ּ��
		float gradInterval = (float)(((float)maxvalue - (float)(minvalue)) / (float)(count - 1));
		//�̶ȼ��
		int mOffset = (float)len / (float)(count - 1);
		/*��������*/
		fl_font(fontStyle.Font.Style, fontStyle.Font.Size);
		/*����������ɫ*/
		fl_color(active ? fl_rgb_color(fontStyle.RGBColor.R, fontStyle.RGBColor.G, fontStyle.RGBColor.B): fl_inactive(fl_rgb_color(fontStyle.RGBColor.R, fontStyle.RGBColor.G, fontStyle.RGBColor.B)));
		char cf[20];
		switch (direction)
		{
		case 0:
			fValue = maxvalue;
			gradInterval = -gradInterval;
			tpDX = dx;
			break;
		case 1:
			gradInterval = gradInterval;
			tpDX = dx;
			break;
		case 2:
			fValue = maxvalue;
			gradInterval = -gradInterval;
			tpDY = dy;
			break;
		case 3:
			gradInterval = gradInterval;
			tpDY = dy;
			break;
		}
		for (int i = 0; i < count; ++i)
		{
			memset(cf, 0, 20);
			int tempDgt = (int)fValue;
			int len = 0;
			while (tempDgt > 0)
			{
				tempDgt /= 10;
				++len;
			}
			if (len > maxDgtLen)
			{
				cf[0] = '*';
			}
			else
			{
				sprintf(cf, stringFormat, fValue);
			}

			int offL = fl_width(cf, strlen(cf))/2;
			int offH = fl_height()/2;
			switch (direction)
			{
				//��
			case 0:
				//��
			case 1:
				//tpDY = dy + i * mOffset+fontStyle.Font.Size / 2;
				tpDY = dy + i * mOffset + offH;
				break;
				//��
			case 2:
				//��
			case 3:
				//tpDX = dx + i * mOffset - fontStyle.Font.Size*len / 4;

				tpDX = dx + i * mOffset - offL;
				break;
			}
			
			fl_draw(cf, tpDX, tpDY);
			fValue += gradInterval;
			fValue = fValue > maxvalue ? maxvalue : fValue;
			fValue = fValue < minvalue ? minvalue : fValue;
		}
		

	}
}

