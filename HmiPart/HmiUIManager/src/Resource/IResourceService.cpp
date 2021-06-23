/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name		: ResourceService.cs
 * Author   : zhanglinbo
 * Date     : 2020/07/12
 * Descript	: 资源接口服务，这里提供了图片文字字符串等资源获取
 * Version	: 1.0
 * modify	:
 *			:
 *******************************************************************************/
#include "stdafx.h"
#include <FL/Fl.H>
#include <map>
#include <istream>
#include "UIComm.h"
#include "BinBitmap.h"
#include "HMIProject.h"
#include "FL/Fl_BMP_Image.H"
#include "IResourceService.h"
#include "CodeFormatUtility.h"
#ifndef WIN32
#include <iconv.h> 
#define OUTLEN 1024 
#endif // !WIN32
namespace UI
{
	IResourceService* IResourceService::ins_;
	/*******************************************************************************
	 * Name	    : GetFontIdx
	 * Descript : 根据字体的名称获取FLTK识别的字体索引
	 * Input	: name - 字体名称
	 * Output	: int - FLTK中对应的字体索引
	 * Note	    :
	*******************************************************************************/
	int  IResourceService::GetFontIdx(std::string name)
	{
		if (!UI::CodeFormatUtility::IsStrUtf8(name.c_str()))
			UI::IResourceService::GB2312toUtf8(name);
		int i = 0, k;
		k = Fl::set_fonts(i ? (i > 1 ? "*" : 0) : "-*");
		for (i = 0; i < k; i++)
		{
			int t;
			const char *s = Fl::get_font_name((Fl_Font)i, &t);
			//printf("%s\n",s);
			if (name.empty())
				return 0;
			if (strstr(s, name.c_str()))
				return i;
		}
		return 0;

	}

#ifndef WIN32

	//代码转换:从一种编码转为另一种编码 
	int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen)
	{
		iconv_t cd;
		int rc;
		char **pin = &inbuf;
		char **pout = &outbuf;

		cd = iconv_open(to_charset, from_charset);
		if (cd == 0) return -1;
		memset(outbuf, 0, outlen);
		if (iconv(cd, pin, &inlen, pout, &outlen) == -1) return -1;
		iconv_close(cd);
		return 0;
	}
	//UNICODE码转为GB2312码 
	int u2g(char *inbuf, int inlen, char *outbuf, int outlen)
	{
		return code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, outlen);
	}
	//GB2312码转为UNICODE码 
	int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen)
	{
		return code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, outlen);
	}
#endif
	void IResourceService::GB2312toUtf8(string &gb2312)
	{

#ifdef WIN32
		/*中文转宽字符*/
		size_t len = strlen(gb2312.c_str()) + 1;
		size_t wlen = MultiByteToWideChar(CP_ACP, 0, (const char*)gb2312.c_str(), int(len), NULL, 0);
		std::unique_ptr<wchar_t> wc{ new wchar_t[wlen + 1] };
		std::unique_ptr<char> utf8{ new char[(wlen + 1) * 3] };
		memset(wc.get(), 0, wlen + 1);
		MultiByteToWideChar(CP_ACP, 0, (const char*)gb2312.c_str(), int(len), wc.get(), int(wlen));
		fl_utf8fromwc(utf8.get(), (wlen + 1) * 3, wc.get(), wcslen(wc.get()));
		gb2312 = utf8.get();
#else		
		//unicode码转为gb2312码 
		char *src = new char[gb2312.size() + 1];
		int srclen = gb2312.size() + 1;
		int dstlen = gb2312.size() * 3 / 2 > 1024 ? gb2312.size() * 3 / 2 : 1024 + 1;
		char *dst = new char[dstlen];
		memset(src, '\0', srclen);
		memcpy(src, gb2312.c_str(), gb2312.size());
		memset(dst, '\0', dstlen);
		CodeFormatUtility::GB2312ToUtf8(dst, dstlen, src, srclen);
		gb2312 = dst;
		delete[] src;
		delete[] dst;
		//char in_utf8[1024] = { 0 };
		//char *ps = in_utf8;
		//memcpy(in_utf8, gb2312.c_str(), gb2312.size());
		//char out[OUTLEN];
		//g2u(in_utf8, strlen(in_utf8), out, OUTLEN);
		//gb2312 = out;
#endif

	}


	IResourceService* IResourceService::Ins()
	{
		return ins_;
	}

    void  IResourceService::SetIns(IResourceService* ins)
	{
		ins_ = ins;
	}
}
