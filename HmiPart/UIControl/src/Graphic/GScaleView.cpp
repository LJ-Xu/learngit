#include "stdafx.h"
#include "UIComm.h"
#include "GScaleView.h"
#include "GScaleControl.h"
#include "GScaleModel.h"
#include "FL/fl_draw.H"
#include <math.h>
namespace UI
{
	GScaleView::GScaleView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H) {}
	GScaleView::~GScaleView() {}

	static int OffX = 0, OffY = 0;

	//�ϰ� �°� Բ��
	void GScaleView::Arc_Scale(GScaleModel* model)
	{
		double Angle, Scale_Angle;
		int Arc_Size;
		int S_x, S_y, E_x, E_y;
		int Point_x, Point_y;

		OffX /= 2;  	OffY /= 2;

		Point_x = model->m_ScaleConfig.X + model->m_ScaleConfig.Width / 2 + OffX;   //�̶���������
		Point_y = model->m_ScaleConfig.Y + model->m_ScaleConfig.Height / 2 + OffY;

		fl_color(RGBColor(model->m_ScaleConfig.Scale_line.Color));    //���ÿ̶��ߵ���ɫ   ��ʽ ���
		fl_line_style(model->m_ScaleConfig.Scale_line.Type, model->m_ScaleConfig.Scale_line.Weight);

		//�жϿ̶���ʽ �������� ����̶ȼ���Ƕ�
		if (model->m_ScaleConfig.Scale_style == model->m_ScaleConfig.Circle)      //Բ��
		{
			Scale_Angle = 360.0 / (model->m_ScaleConfig.Maj_scale*model->m_ScaleConfig.Second_scale);  //�̶���С�Ƕ�
			if (model->m_ScaleConfig.Height > model->m_ScaleConfig.Width)
				model->m_ScaleConfig.Height = model->m_ScaleConfig.Width;

			Arc_Size = model->m_ScaleConfig.Height - model->m_ScaleConfig.Scale_length;  //�̶�����Բ�ĳߴ�  �뾶
			if (Arc_Size <= 2)
			{
				Arc_Size = 2;
				model->m_ScaleConfig.Scale_length = model->m_ScaleConfig.Height - Arc_Size;   //���̶ȳ���
			}
			fl_circle(Point_x + OffX, Point_y + OffY, Arc_Size);
		}
		else       //��Բ   3/4Բ
		{
			if (model->m_ScaleConfig.Scale_style == model->m_ScaleConfig.Three_Four)
				Scale_Angle = 270.0 / (model->m_ScaleConfig.Maj_scale*model->m_ScaleConfig.Second_scale);  //�̶���С�Ƕ�
			else        //��Բ�ָ�ĽǶ�
				Scale_Angle = 180.0 / (model->m_ScaleConfig.Maj_scale*model->m_ScaleConfig.Second_scale);

			Arc_Size = model->m_ScaleConfig.Height - model->m_ScaleConfig.Scale_length;         //�̶�����Բ�ߴ�
			if (Arc_Size <= 2)
			{
				Arc_Size = 2;
				model->m_ScaleConfig.Scale_length = model->m_ScaleConfig.Height - Arc_Size;
			}
			if ((model->m_ScaleConfig.Scale_style == model->m_ScaleConfig.Up_Arc))                        //����Բ������
				fl_arc(Point_x - Arc_Size + OffX, Point_y - Arc_Size + OffY, Arc_Size * 2, Arc_Size * 2, 0, 180);
			else if ((model->m_ScaleConfig.Scale_style == model->m_ScaleConfig.Down_Arc))
				fl_arc(Point_x - Arc_Size + OffX, Point_y - Arc_Size + OffY, Arc_Size * 2, Arc_Size * 2, 180, 360);
			else
				fl_arc(Point_x - Arc_Size + OffX, Point_y - Arc_Size + OffY, Arc_Size * 2, Arc_Size * 2, 315, 225);
		}

		//���ƿ̶���
		for (int i = 0; i < model->m_ScaleConfig.Second_scale*model->m_ScaleConfig.Maj_scale + 1; i++)
		{
			if ((model->m_ScaleConfig.Scale_style == model->m_ScaleConfig.Down_Arc))  //�°�Բ
				Angle = Scale_Angle * i + 180;
			else if ((model->m_ScaleConfig.Scale_style == model->m_ScaleConfig.Three_Four))
				Angle = Scale_Angle * i + 315;                     //3/4Բ
			else                                                    //Բ���ϰ�Բ
				Angle = Scale_Angle * i;

			S_x = Arc_Size * (int)cos(Angle / 180 * 3.14);
			S_y = Arc_Size * (int)sin(Angle / 180 * 3.14);

			if (i%model->m_ScaleConfig.Second_scale == 0)      //���̶ȳ���
			{
				E_x = (int)(model->m_ScaleConfig.Height*cos(Angle / 180 * 3.14));
				E_y = (int)(model->m_ScaleConfig.Height*sin(Angle / 180 * 3.14));
			}
			else                                              //���̶ȳ���
			{
				E_x = (int)((model->m_ScaleConfig.Height*0.8)*cos(Angle / 180 * 3.14));
				E_y = (int)((model->m_ScaleConfig.Height*0.8)*sin(Angle / 180 * 3.14));
			}
			fl_line(Point_x + S_x + OffX, Point_y - S_y + OffY, Point_x + E_x + OffX, Point_y - E_y + OffY);
		}

	}

	void GScaleView::Line_Scale(GScaleModel* model)
	{
		double Space;
		fl_color(RGBColor(model->m_ScaleConfig.Scale_line.Color));    //���ÿ̶��ߵ���ɫ   ��ʽ ���
		fl_line_style(model->m_ScaleConfig.Scale_line.Type, model->m_ScaleConfig.Scale_line.Weight);

		if (model->m_ScaleConfig.Scale_style == model->m_ScaleConfig.Horizontal)             //ˮƽ�̶�
		{
			Space = model->m_ScaleConfig.Height*1.0 / (model->m_ScaleConfig.Maj_scale*model->m_ScaleConfig.Second_scale);   //�̶ȼ��

			for (int i = 0; i < model->m_ScaleConfig.Maj_scale*model->m_ScaleConfig.Second_scale + 1; i++)	//���ƿ̶���	
			{
				if (i%model->m_ScaleConfig.Second_scale == 0)         //���̶�
					fl_line(model->m_ScaleConfig.X + OffX, model->m_ScaleConfig.Y + (int)(Space * i) + OffY,
						model->m_ScaleConfig.X + model->m_ScaleConfig.Width + OffX, model->m_ScaleConfig.Y + (int)(Space * i) + OffY);
				else                                                  //���̶�
					fl_line(model->m_ScaleConfig.X + OffX, model->m_ScaleConfig.Y + (int)(Space * i )+ OffY,
					(int)((model->m_ScaleConfig.X + model->m_ScaleConfig.Width)*0.85) + OffX, model->m_ScaleConfig.Y + (int)(Space * i) + OffY);
			}
		}
		else         //��ֱ�̶�
		{
			Space = model->m_ScaleConfig.Width*1.0 / (model->m_ScaleConfig.Maj_scale*model->m_ScaleConfig.Second_scale);

			for (int i = 0; i < model->m_ScaleConfig.Maj_scale*model->m_ScaleConfig.Second_scale + 1; i++)//���ƿ̶���
			{
				if (i%model->m_ScaleConfig.Second_scale == 0)
					fl_line(model->m_ScaleConfig.X + (int)(Space * i) + OffX, model->m_ScaleConfig.Y + OffY,
						model->m_ScaleConfig.X + (int)(Space * i) + OffX, model->m_ScaleConfig.Y + model->m_ScaleConfig.Height + OffY);
				else
					fl_line(model->m_ScaleConfig.X + (int)(Space * i) + OffX, model->m_ScaleConfig.Y + OffY,
						model->m_ScaleConfig.X + (int)(Space * i) + OffX, (int)((model->m_ScaleConfig.Y + model->m_ScaleConfig.Height)*0.85 + OffY));
			}
		}
	}


	void GScaleView::DrawScale(GScaleControl* ctrl, GScaleModel* model)
	{
		fl_color(RGBColor(0x00FF00));
		fl_line_style(1, model->m_ScaleConfig.Scale_line.Weight);
		//fl_line(0, 0, 500, 0);		fl_line(0, 100, 500, 100);	fl_line(0, 200, 500, 200);
		//fl_line(0, 300, 500, 300);	fl_line(0, 400, 500, 400);	fl_line(50, 0, 50, 500);
		//fl_line(150, 0, 150, 500);	fl_line(250, 0, 250, 500);	fl_line(350, 0, 350, 500);
		//fl_line(450, 0, 450, 500);

		switch (model->m_ScaleConfig.Scale_style)
		{
		case model->m_ScaleConfig.Up_Arc:			Arc_Scale(model);	  break;
		case model->m_ScaleConfig.Down_Arc:			Arc_Scale(model);	  break;
		case model->m_ScaleConfig.Circle:			Arc_Scale(model);	  break;
		case model->m_ScaleConfig.Three_Four:		Arc_Scale(model);	  break;
		case model->m_ScaleConfig.Horizontal:		Line_Scale(model);    break;
		case model->m_ScaleConfig.Vertical:         Line_Scale(model);    break;
		}
	}
	void GScaleView::draw()
	{
		shared_ptr<GScaleControl> ctrl = BaseView.GetControl<GScaleControl>();
		shared_ptr<GScaleModel> model = BaseView.GetModel<GScaleModel>();
		DrawScale(ctrl.get(), model.get());
	}
}
