#pragma once
#include "BasicView.h"
#include <FL/Fl_Input.H>
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include "UIComm.h"
#include "DigitalInputControl.h"
namespace UI
{
	//#define HMIBaseInput Fl_Input
#define MINC 0
#define MAXC 1
	struct Key_message
	{
		char KeyName[10];
	};
	class KeyBoardView : public HMIDoubleWindow
	{
	public:
		KeyBoardView(int W, int H, const char* l);
		~KeyBoardView();
		int currentType = MINC;
		void changeType(int type);
		void KeySendMessage(int message, int lparam);
		DigitalInputControl * GetInputByID(int num);
		struct Key_message * key_msg;
		Fl_Button *btn0;
		Fl_Button *btn1;
		Fl_Button *btn2;
		Fl_Button *btn3;
		Fl_Button *btn4;
		Fl_Button *btn5;
		Fl_Button *btn6;
		Fl_Button *btn7;
		Fl_Button *btn8;
		Fl_Button *btn9;

		Fl_Button *btn_del;
		Fl_Button *btn_left;
		Fl_Button *btn_right;
		Fl_Button *btn_num;
		Fl_Button *btn_esc;
		Fl_Button *btn_caps;
		Fl_Button *btn_blank;
		Fl_Button *btn_enter;

		Fl_Button *btn_sign0;
		Fl_Button *btn_sign1;
		Fl_Button *btn_sign2;
		Fl_Button *btn_sign3;

		Fl_Button *btn_char[26] = { (Fl_Button *)0 };
		BasicView BaseView;
	protected:
	};

	void key_cb(Fl_Widget*, void*);
	struct keycode_table { const char *lower; const char *upper; };
}