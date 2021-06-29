/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PinYinPageControl.cpp
 * Author   : qiaodan
 * Date     : 2021-01-21
 * Descript : 创建拼音界面，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "PinYinPageControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
#include "pinyinime.h"

#ifdef WIN32
#include "../ThirdPart/googlepinyin/spellingtrie.h"
#include <direct.h>
#else
#include "spellingtrie.h"
#include <unistd.h>
#endif

#include "Logger.h"

#ifdef WIN32
#define getcwd _getcwd
#endif

using namespace ime_pinyin;
namespace UI
{
	PinYinPageControl::PinYinPageControl(HMIPage* w) : BaseControl(w)
	{
		//创建CharacterInputModel
		mode_ = std::shared_ptr <PinYinPageModel>(new PinYinPageModel());
		InitMVCModel(mode_);
		
	}
	void PinYinPageControl::OnReady()
	{
        	char buf[1024];
		memset(buf,'\0',1024);
		string path,syspath,usrpath;
#ifdef WIN32
		//path = path.substr(0, path.rfind('\\'));
		path = getcwd(buf, 1024);
		syspath = path + "\\dict_pinyin.dat";
		usrpath = path + "\\dict_pinyin_user.dat";
		LOG_INFO("PinYin Data Get From : %s\n", path.c_str());

#else
		readlink("/proc/self/exe", buf, 1024 - 1);
		path = buf;
		//LOG_ERROR("Can't Open DataBase: %s\n", _pgmptr);
		printf("PinYin Data Get From : %s\n", path.c_str());
		path = path.substr(0, path.rfind('/'));
		syspath = path + "/dict_pinyin.dat";
		usrpath = path + "/dict_pinyin_user.dat";
		syspath = "/data/dict_pinyin.dat";
		usrpath = "/data/dict_pinyin_user.dat";
#endif
		LOG_INFO("syspath is : %s\n", syspath.c_str());
		LOG_INFO("usrpath is : %s\n", usrpath.c_str());
		bool ret = im_open_decoder(syspath.c_str(), usrpath.c_str());
		if(!ret)
			printf("open pinyin data fail\n");

	}
	void PinYinPageControl::CreateView()
	{
		//初始化VIEW
		mode_->PinYinConfig.Width = Page()->w();
		mode_->PinYinConfig.Height = Page()->h();
		PinYinPageView* view = new PinYinPageView(
			mode_->PinYinConfig.X + mode_->PinYinConfig.OffX,
			mode_->PinYinConfig.Y + mode_->PinYinConfig.OffY,
			mode_->PinYinConfig.Width,
			mode_->PinYinConfig.Height, nullptr);
		InitMVCView(view);
		view->clear_visible_focus();
		PinYinPageView::Ins()->SetIns(view);
	}
	void PinYinPageControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->PinYinConfig.VOffX != Project::DataVarId::NullId)
			mode_->PinYinConfig.OffX = UI::UIData::Number<int>(mode_->PinYinConfig.VOffX) + pageoffx;
		else
			mode_->PinYinConfig.OffX = pageoffx;


		if (mode_->PinYinConfig.VOffY != Project::DataVarId::NullId)
			mode_->PinYinConfig.OffY = UI::UIData::Number<int>(mode_->PinYinConfig.VOffY) + pageoffy;
		else
			mode_->PinYinConfig.OffY = pageoffy;
	}
	void PinYinPageControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			switch (mode_->PinYinConfig.Perm.Mode)
			{
			case '0':
			{
				/*弹出窗体*/
				break;
			}
			case '1':
			{
				/*隐藏*/
				pView_->hide();
				pView_->redraw();
				break;
			}
			default:
				break;
			}
			break;
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
	void PinYinPageControl::SearchChineseCharacters(const int &currentPage, string pinyin)
	{
        char buf[512];
		PinYinPageView* pView = static_cast<PinYinPageView*>(pView_);
		vector<string>().swap(pView->ChineseString);

		const int max_spelling_length = 32;
		const int max_decoded_length = 32;
		const int max_single_hanzi = 20;
		static unsigned int page_change_times = 0;
        //getcwd(buf, 512);
		//string path = _pgmptr;
        string path= getcwd(buf, 512);
		//cout << _pgmptr << endl;
		//LOG_ERROR("Can't Open DataBase: %s\n", _pgmptr);

		//path = path.substr(0, path.rfind('\\'));
		//string syspath = path + "\\dict_pinyin.dat";
		//string usrpath = path + "\\dict_pinyin_user.dat";
		//im_open_decoder(syspath.c_str(), usrpath.c_str());

		//const char* syspath = "C:\\Users\\PC\\source\\repos\\pd-hmi-cpp3\\ThirdPart\\googlepinyin\\dict\\dict_pinyin.dat";
		//const char* usrpath = "C:\\Users\\PC\\source\\repos\\pd-hmi-cpp3\\ThirdPart\\googlepinyin\\dict\\dict_pinyin_user.dat";
		//im_open_decoder(syspath, usrpath);
		im_set_max_lens(max_spelling_length, max_decoded_length);
		im_reset_search();
		size_t cand_num = im_search(pinyin.c_str(), pinyin.size());

		size_t decode_len;
		im_get_sps_str(&decode_len);

		switch (currentPage)
		{
		case 1:
			if (cand_num > (size_t)ChineseNum && page_change_times <= cand_num / (size_t)ChineseNum)
				page_change_times++;
			break;
		case -1:
			if (page_change_times > 0) page_change_times--;
			break;
		default:
			page_change_times = 0;
			break;
		}
		if (0 == page_change_times)
			pView->EnablePreBtn = false;
		else
			pView->EnablePreBtn = true;

		if (page_change_times == cand_num / ChineseNum)
			pView->EnableNextBtn = false;
		else
			pView->EnableNextBtn = true;

		char16 *cand_buf = new char16[max_decoded_length];
		char16 *cand;
		char *cand_char = new char[max_decoded_length];
		memset(cand_char, 0, max_decoded_length);
		string cand_str;
		for (size_t i = 0; i < (size_t)ChineseNum; i++)
		{
			if (page_change_times * ChineseNum + i <= cand_num)
			{
				cand = im_get_candidate(page_change_times * ChineseNum + i, cand_buf, max_decoded_length);
				if (cand)
				{
					string chinesetext;
					for (size_t j = 0; j < strlen((char *)cand_buf) / 2; j++)
					{
						fl_utf8encode(cand_buf[j], cand_char);	//转换为utf8编码
						chinesetext += cand_char;
					}
					if (chinesetext.size() > 6)
						chinesetext = chinesetext.substr(0, 6);
					pView->ChineseString.push_back(chinesetext);
					if (i == 0) cand_str.erase(0, im_get_fixed_len());
				}
				else
				{
					cand_str = "";
				}
			}
		}
		delete cand_buf;
		delete cand_char;
	//	im_close_decoder();
		//ime_pinyin::SpellingTrie s = ime_pinyin::SpellingTrie::get_instance();
		//delete &s;
		im_flush_cache();
	}
	void PinYinPageControl::HandleDataVar(Project::DataVarId &varId)
	{
		PinYinPageView* pView = static_cast<PinYinPageView*>(pView_);
		if (mode_->PinYinConfig.VOffX.Cmp(varId))
		{
			mode_->PinYinConfig.OffX = UI::UIData::Number<int>(mode_->PinYinConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->PinYinConfig.Perm, mode_->PinYinConfig.X + mode_->PinYinConfig.OffX, mode_->PinYinConfig.Y + mode_->PinYinConfig.OffY);
		}
		if (mode_->PinYinConfig.VOffY.Cmp(varId))
		{
			mode_->PinYinConfig.OffY = UI::UIData::Number<int>(mode_->PinYinConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->PinYinConfig.Perm, mode_->PinYinConfig.X + mode_->PinYinConfig.OffX, mode_->PinYinConfig.Y + mode_->PinYinConfig.OffY);
		}
		if (mode_->PinYinConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->PinYinConfig.Perm, pView_);
		if (mode_->PinYinConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->PinYinConfig.Perm, pView_);
		if (mode_->PinYinConfig.ReadVar.Cmp(varId))
		{
			char buf[32];
			pView->hide();
			UIDataService::Ins().GetString(mode_->PinYinConfig.ReadVar, buf, sizeof(buf));
			pView->PinYin = buf;
			/*根据拼音获取中文字符*/
			SearchChineseCharacters(0, pView->PinYin);
			//if (!pView->ChineseString.empty())
			//{
			//	mode_->PinYinConfig.Width = (int)(9.0 * (fl_width(pView->ChineseString[0].c_str(), pView->ChineseString[0].size()) + 3));
			//	pView->resize(mode_->PinYinConfig.X + mode_->PinYinConfig.OffX,
			//		mode_->PinYinConfig.Y + mode_->PinYinConfig.OffY, 
			//		mode_->PinYinConfig.Width,mode_->PinYinConfig.Height);
			//}
			pView->show();
			UI::ViewShowUtility::ShowView(pView, mode_->PinYinConfig.Perm,
				mode_->PinYinConfig.X + mode_->PinYinConfig.OffX,
				mode_->PinYinConfig.Y + mode_->PinYinConfig.OffY);
		}
	}

	int PinYinPageControl::PeekHMIMessage(Message::Msg* msg)
	{
		PinYinPageView* pView = static_cast<PinYinPageView*>(pView_);
		Project::DataVarId varId;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}
