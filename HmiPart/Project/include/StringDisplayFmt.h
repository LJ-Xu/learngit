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
		int mode;//二进制,数值，文字 ，编码
		int encode;//编码方式 ，UNICODE,GB2312 ,UTF8
		string format;//数值“%02x”;  通过丰富可以添加自定义格式
	};
}
