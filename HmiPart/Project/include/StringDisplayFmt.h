/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StringDisplayFmt.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
namespace Project
{
	class StringDisplayFmt
	{
	public:
		int mode;//������,��ֵ������ ������
		int encode;//���뷽ʽ ��UNICODE,GB2312 ,UTF8
		string format;//��ֵ��%02x��;  ͨ���ḻ��������Զ����ʽ
	};
}
