/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TableConfig.h
 * Author   : qiaodan
 * Date     : 2020-12-03
 * Descript : ���ڴ�ű��ؼ���ز�������
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "BaseLine.h"
namespace Project
{
	class TableConfig :public MoveableDisplayUnit
	{
	public:
		int Rows;					//����
		int Cols;					//����
		BaseLine OuterBorder;		//�����ʽ
		bool HaveRowBorder;			//�Ƿ���ʾ��������
		BaseLine InnerBorderRow;	//����������ʽ
		bool HaveColBorder;			//�Ƿ���ʾ��������
		BaseLine InnerBorderCol;	//����������ʽ
		bool UseFillin;				//�Ƿ�ʹ�������ɫ
		int FillColor;				//�����ɫ

		vector<int> ColWidth;		//�п�
		vector<int> RowHeight;		//�и�

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Rows, Cols, OuterBorder, HaveRowBorder, InnerBorderRow,
				HaveColBorder, InnerBorderCol, UseFillin, FillColor, ColWidth,
				RowHeight, VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}

