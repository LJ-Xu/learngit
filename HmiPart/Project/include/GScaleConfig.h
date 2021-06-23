#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"


namespace Project
{


	class GScaleConfig : public MoveableDisplayUnit
	{
	public:

		int Scale_style;          //�̶���ʽ

		int Maj_scale;            //���̶���
		int Second_scale;         //�ο̶���

		int Scale_length;         //�̶ȳ���

		bool isText;              //�Ƿ��ע����
		int Start_Seg;			  //��ʼ��
		int End_Seg;		      //������
		int Font_Size;            //�����С

		enum { Up_Arc, Down_Arc, Circle, Three_Four, Horizontal, Vertical };

		BaseLine Scale_line;      //��  ��ɫ ��ʽ ��ϸ ͸����
	};



}