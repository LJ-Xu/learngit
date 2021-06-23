/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BorderRes.h
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
	class BorderRes
	{
	public:
		int BorderColor;//�߿���ɫ
		int BorderSize; //�߿��С
		int BorderStyle;	//�߿���ʽ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(BorderColor, BorderSize, BorderStyle);
		}
	};
}

