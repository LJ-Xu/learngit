#include "stdafx.h"
#include "KeyBoardView.h"
#include "KeyBoardControl.h"
#include "KeyBoardModel.h"
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Shared_Image.H>
#include "HMIWindow.h"
#include "DigitalInputControl.h"
#include "Message.h"
namespace UI
{
	struct keycode_table table[] = {
		{"q","Q"},
		{"w","W"},
		{"e","E"},
		{"r","R"},
		{"t","T"},
		{"y","Y"},
		{"u","U"},
		{"i","I"},
		{"o","O"},
		{"p","P"},
		{"a","A"},
		{"s","S"},
		{"d","D"},
		{"f","F"},
		{"g","G"},
		{"h","H"},
		{"j","J"},
		{"k","K"},
		{"l","L"},
		{"z","Z"},
		{"x","X"},
		{"c","C"},
		{"v","V"},
		{"b","B"},
		{"n","N"},
		{"m","M"},
	};

	void KeyBoardView::changeType(int type)
	{
		int i;
		if (type == MINC) {
			for (i = 0; i < 26; i++)
				btn_char[i]->copy_label(table[i].lower);
		}
		else {
			for (i = 0; i < 26; i++)
				btn_char[i]->copy_label(table[i].upper);
		}
	}
	DigitalInputControl * KeyBoardView::GetInputByID(int num)
	{
		DigitalInputControl *ctrl = new DigitalInputControl();
		return ctrl;
	}

	void  KeyBoardView::KeySendMessage(int message, int lparam)
	{
		shared_ptr<KeyBoardControl> ctrl = BaseView.GetControl<KeyBoardControl>();
		shared_ptr<KeyBoardModel> model = BaseView.GetModel<KeyBoardModel>();
		/*µ÷ÊÔ*/
		struct Key_message * key = (struct Key_message *)lparam;
		printf("char = %s\n", key->KeyName);
		printf("num = %d\n", message);
		//printf("num = %d\n", model->m_KeyBoardConfig.InputNum);
		DigitalInputControl *inputctrl = GetInputByID(model->m_KeyBoardConfig.InputNum);
		ctrl->SendMsg(inputctrl, message, lparam);
	}
	void key_cb(Fl_Widget* b, void* data)
	{
#if 0
		KeyBoardView* k_view = (KeyBoardView *)data;
		Fl_Button *CurrentBtn = (Fl_Button *)b;

		const char *btnlabel = CurrentBtn->label();
		strcpy(k_view->key_msg->KeyName, btnlabel);
		//strcpy(btnlabel,CurrentBtn->label());
		if (strcmp(btnlabel, "Bksp") == 0)    //É¾³ý¼ü
		{
			k_view->KeySendMessage(KEY_EVENT_DEL, (int)k_view->key_msg);
		}
		else if (strcmp(btnlabel, "Caps") == 0)   //ÇÐ»»´óÐ¡Ð´
		{
			if (k_view->currentType == MINC) {
				k_view->currentType = MAXC;
				k_view->changeType(k_view->currentType);
			}
			else {
				k_view->currentType = MINC;
				k_view->changeType(k_view->currentType);
			}
		}
		else if (strcmp(btnlabel, "Esc") == 0)    //ÍË³ö¼ü
		{
			//k_view->KeySendMessage();
			k_view->KeySendMessage(KEY_EVENT_ESC, (int)k_view->key_msg);
		}
		else if (strcmp(btnlabel, "123") == 0)    //×ÖÄ¸¼üÅÌ¼ü
		{
			k_view->KeySendMessage(KEY_EVENT_NUM, (int)k_view->key_msg);
		}
		else if (strcmp(btnlabel, "@4->") == 0)   //Ö¸Õë×óÒÆ¼ü
		{
			k_view->KeySendMessage(KEY_EVENT_LEFT, (int)k_view->key_msg);
		}
		else if (strcmp(btnlabel, "@6->") == 0)   //Ö¸ÕëÓÒÒÆ¼ü
		{
			k_view->KeySendMessage(KEY_EVENT_RIGHT, (int)k_view->key_msg);
		}
		else if (strcmp(btnlabel, "Enter") == 0)  //»Ø³µ¼ü
		{
			k_view->KeySendMessage(KEY_EVENT_ENTER, (int)k_view->key_msg);
		}
		else if (strcmp(btnlabel, ",") == 0)
		{
			k_view->KeySendMessage(KEY_EVENT_SIGN0, (int)k_view->key_msg);
		}
		else if (strcmp(btnlabel, ".") == 0)
		{
			k_view->KeySendMessage(KEY_EVENT_DOT, (int)k_view->key_msg);
		}
		else if (strcmp(btnlabel, "/") == 0)
		{
			k_view->KeySendMessage(KEY_EVENT_SIGN2, (int)k_view->key_msg);
		}
		else if (strcmp(btnlabel, "\\") == 0)
		{
			k_view->KeySendMessage(KEY_EVENT_SIGN1, (int)k_view->key_msg);
		}
		else if (strcmp(btnlabel, " ") == 0)
		{
			k_view->KeySendMessage(KEY_EVENT_BLANK, (int)k_view->key_msg);
		}
		else {
			if (CurrentBtn->value() == 1)
				CurrentBtn->value(0);
			CurrentBtn->redraw();

			if (btnlabel[0] >= 48 & btnlabel[0] < 57)		//Êý×Ö
				k_view->KeySendMessage(KEY_EVENT_VALUE, (int)k_view->key_msg);
			else
				k_view->KeySendMessage(KEY_EVENT_CHAR, (int)k_view->key_msg);	//×Ö·û
		   // k_view->KeySendMessage();
		   // for (int i = 0; i < 26; i++)
		   // {
				//if(strcmp(btnlabel, table[i].lower))
					//m_pWindow->SendUserMessage(ctrl1, table->message, 11);
		   // }
		   //currentLineEdit->insert(btnlabel, 0);

		   //Fl::focus((Fl_Widget *)currentLineEdit);
		}

#endif
	}

	KeyBoardView::KeyBoardView(int W, int H, const char* l) : HMIDoubleWindow(W, H, l)
	{
		shared_ptr<KeyBoardControl> ctrl = BaseView.GetControl<KeyBoardControl>();
		shared_ptr<KeyBoardModel> model = BaseView.GetModel<KeyBoardModel>();
		key_msg = (struct Key_message *)malloc(sizeof(struct Key_message));

		{ Fl_Group* o = new Fl_Group(6, 5, 248, 25);
		{ btn1 = new Fl_Button(7, 5, 19, 25, "1");
		btn1->labelsize(10);
		btn1->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn1
		{ btn2 = new Fl_Button(26, 5, 20, 25, "2");
		btn2->labelsize(10);
		btn2->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn2
		{ btn3 = new Fl_Button(46, 5, 20, 25, "3");
		btn3->labelsize(10);
		btn3->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn3
		{ btn4 = new Fl_Button(66, 5, 20, 25, "4");
		btn4->labelsize(10);
		btn4->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn4
		{ btn5 = new Fl_Button(86, 5, 20, 25, "5");
		btn5->labelsize(10);
		btn5->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn5
		{ btn6 = new Fl_Button(106, 5, 20, 25, "6");
		btn6->labelsize(10);
		btn6->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn6
		{ btn7 = new Fl_Button(126, 5, 20, 25, "7");
		btn7->labelsize(10);
		btn7->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn7
		{ btn8 = new Fl_Button(146, 5, 20, 25, "8");
		btn8->labelsize(10);
		btn8->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn8
		{ btn9 = new Fl_Button(166, 5, 20, 25, "9");
		btn9->labelsize(10);
		btn9->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn9
		{ btn0 = new Fl_Button(186, 5, 20, 25, "0");
		btn0->labelsize(10);
		btn0->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn0
		{ btn_del = new Fl_Button(206, 5, 48, 25, "Bksp");
		//btn_del->labeltype(FL_SHADOW_LABEL);
		btn_del->labelsize(10);
		btn_del->callback((Fl_Callback*)key_cb, (void*)this);
		btn_del->align(Fl_Align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE));
		} // Fl_Button* btn_del
		o->end();
		} // Fl_Group* o
		{ Fl_Group* o = new Fl_Group(6, 30, 248, 22);
		{ btn_char[0] = new Fl_Button(6, 30, 22, 22, "q");
		btn_char[0]->labelsize(10);
		btn_char[0]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[0]
		{ btn_char[1] = new Fl_Button(28, 30, 22, 22, "w");
		btn_char[1]->labelsize(10);
		btn_char[1]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[1]
		{ btn_char[2] = new Fl_Button(50, 30, 22, 22, "e");
		btn_char[2]->labelsize(10);
		btn_char[2]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[2]
		{ btn_char[3] = new Fl_Button(72, 30, 22, 22, "r");
		btn_char[3]->labelsize(10);
		btn_char[3]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[3]
		{ btn_char[4] = new Fl_Button(94, 30, 22, 22, "t");
		btn_char[4]->labelsize(10);
		btn_char[4]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[4]
		{ btn_char[5] = new Fl_Button(116, 30, 22, 22, "y");
		btn_char[5]->labelsize(10);
		btn_char[5]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[5]
		{ btn_char[6] = new Fl_Button(138, 30, 22, 22, "u");
		btn_char[6]->labelsize(10);
		btn_char[6]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[6]
		{ btn_char[7] = new Fl_Button(160, 30, 22, 22, "i");
		btn_char[7]->labelsize(10);
		btn_char[7]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[7]
		{ btn_char[8] = new Fl_Button(182, 30, 22, 22, "o");
		btn_char[8]->labelsize(10);
		btn_char[8]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[8]
		{ btn_char[9] = new Fl_Button(204, 30, 22, 22, "p");
		btn_char[9]->labelsize(10);
		btn_char[9]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[9]
		{ btn_sign0 = new Fl_Button(226, 30, 28, 22, "\\");
		btn_sign0->labelsize(10);
		btn_sign0->callback((Fl_Callback*)key_cb, (void*)this);
		btn_sign0->align(Fl_Align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE));
		} // Fl_Button* btn_siga0
		o->end();
		} // Fl_Group* o
		{ Fl_Group* o = new Fl_Group(6, 52, 248, 22);
		{ btn_char[10] = new Fl_Button(6, 52, 25, 22, "a");
		btn_char[10]->labelsize(10);
		btn_char[10]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[10]
		{ btn_char[11] = new Fl_Button(31, 52, 22, 22, "s");
		btn_char[11]->labelsize(10);
		btn_char[11]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[11]
		{ btn_char[12] = new Fl_Button(53, 52, 22, 22, "d");
		btn_char[12]->labelsize(10);
		btn_char[12]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[12]
		{ btn_char[13] = new Fl_Button(75, 52, 22, 22, "f");
		btn_char[13]->labelsize(10);
		btn_char[13]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[13]
		{ btn_char[14] = new Fl_Button(97, 52, 22, 22, "g");
		btn_char[14]->labelsize(10);
		btn_char[14]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[14]
		{ btn_char[15] = new Fl_Button(119, 52, 22, 22, "h");
		btn_char[15]->labelsize(10);
		btn_char[15]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[15]
		{ btn_char[16] = new Fl_Button(141, 52, 22, 22, "j");
		btn_char[16]->labelsize(10);
		btn_char[16]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[16]
		{ btn_char[17] = new Fl_Button(163, 52, 22, 22, "k");
		btn_char[17]->labelsize(10);
		btn_char[17]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[17]
		{ btn_char[18] = new Fl_Button(185, 52, 22, 22, "l");
		btn_char[18]->labelsize(10);
		btn_char[18]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[18]
		{ btn_enter = new Fl_Button(207, 52, 47, 22, "Enter");
		btn_enter->labelsize(10);
		btn_enter->callback((Fl_Callback*)key_cb, (void*)this);
		btn_enter->align(Fl_Align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE));
		} // Fl_Button* btn_enter
		o->end();
		} // Fl_Group* o
		{ Fl_Group* o = new Fl_Group(6, 74, 248, 22);
		{ btn_char[19] = new Fl_Button(34, 74, 22, 22, "z");
		btn_char[19]->labelsize(10);
		btn_char[19]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[19]
		{ btn_char[20] = new Fl_Button(56, 74, 22, 22, "x");
		btn_char[20]->labelsize(10);
		btn_char[20]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[20]  
		{ btn_char[21] = new Fl_Button(78, 74, 22, 22, "c");
		btn_char[21]->labelsize(10);
		btn_char[21]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[21]	
		{ btn_char[22] = new Fl_Button(100, 74, 22, 22, "v");
		btn_char[22]->labelsize(10);
		btn_char[22]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[22]
		{ btn_char[23] = new Fl_Button(122, 74, 22, 22, "b");
		btn_char[23]->labelsize(10);
		btn_char[23]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[23]
		{ btn_char[24] = new Fl_Button(144, 74, 22, 22, "n");
		btn_char[24]->labelsize(10);
		btn_char[24]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[24]
		{ btn_char[25] = new Fl_Button(166, 74, 22, 22, "m");
		btn_char[25]->labelsize(10);
		btn_char[25]->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_char[25]
		{ btn_sign1 = new Fl_Button(188, 74, 22, 22, ",");
		btn_sign1->labelsize(10);
		btn_sign1->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_sign1
		{ btn_sign2 = new Fl_Button(210, 74, 22, 22, ".");
		btn_sign2->labelsize(10);
		btn_sign2->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_sign2
		{ btn_sign3 = new Fl_Button(232, 74, 22, 22, "/");
		btn_sign3->labelsize(10);
		btn_sign3->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_sign3
		{ btn_caps = new Fl_Button(6, 74, 28, 22, "Caps");
		btn_caps->labelsize(10);
		btn_caps->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_caps
		o->end();
		} // Fl_Group* o
		{ Fl_Group* o = new Fl_Group(6, 96, 248, 22);
		{ btn_esc = new Fl_Button(6, 96, 30, 22, "Esc");
		btn_esc->labelsize(10);
		btn_esc->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_esc
		{ btn_num = new Fl_Button(36, 96, 30, 22, "123");
		btn_num->labelsize(10);
		btn_num->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_num
		{ btn_blank = new Fl_Button(66, 96, 116, 22, " ");
		btn_blank->labelsize(10);
		btn_blank->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_blank
		{ btn_left = new Fl_Button(182, 96, 36, 22, "@4->");
		btn_left->labeltype(FL_SHADOW_LABEL);
		btn_left->labelsize(10);
		btn_left->labelcolor(FL_DARK1);
		btn_left->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_left
		{ btn_right = new Fl_Button(218, 96, 36, 22, "@6->");
		btn_right->labeltype(FL_SHADOW_LABEL);
		btn_right->labelsize(10);
		btn_right->labelcolor(FL_DARK1);
		btn_right->callback((Fl_Callback*)key_cb, (void*)this);
		} // Fl_Button* btn_right
		o->end();
		} // Fl_Group* o  

	}
	KeyBoardView::~KeyBoardView() {}
}