#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "StringUtility.h"
#include "RecipeChartView.h"
#include "FL/fl_draw.H"
#include "IDataService.h"
#include "IResourceService.h"
#include "ProjectDataTables.h"
#include "HMIWindow.h"
#include "LocalData.h"
#include "UIData.h"
#include "KeyInputUtility.h"
#include "PinYinPageView.h"
#include "CodeFormatUtility.h"
#include "RecipeStorage.h"
#include "PermUtility.h"
#include "SysSetApi.h"
#include "DataTableView.h"
#include "RecipeUtility.h"
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
namespace UI
{
	RecipeChartView::RecipeChartView(int X, int Y, int W, int H, const char *l = 0) : HMIBaseTable(X, Y, W, H, l)
	{
		box(FL_NO_BOX);
		end();
	}
	RecipeChartView::~RecipeChartView() {}

	void RecipeChartView::InitDraw()
	{
		shared_ptr<RecipeChartControl> ctrl = BaseView.GetControl<RecipeChartControl>();
		shared_ptr<RecipeChartModel> model = BaseView.GetModel<RecipeChartModel>();
		if (!model->RecipeConfig.TitleUsed)
			model->RecipeConfig.TitleHeight = 0;
		if (!model->RecipeConfig.ColTitleShown)
			model->RecipeConfig.EachColTitleHeight = 0;
		//设置是否使用行、列标题
		if (model->RecipeConfig.TitleUsed || model->RecipeConfig.ColTitleShown)
			col_header(1);
		else
			col_header(0);
		col_header_height(model->RecipeConfig.TitleHeight + model->RecipeConfig.EachColTitleHeight);
		row_header(0);

		//设置是否可变宽高
		row_resize(0);
		col_resize(0);

		//设置字体
		fontSize_ = model->RecipeConfig.ListFontStyle.Font.Size;
		fontStyle_ = UI::IResourceService::GetFontIdx(model->RecipeConfig.ListFontStyle.Font.Name);

		//设置列数
		cols(model->RecipeConfig.InfoLst.size());
		datacol_ = RecipeDT::Ins()->GetColNum(model->RecipeConfig.RecipeGroupId);

		//设置行数
		rows(model->RecipeConfig.PerPageRowCount + 1);
		//设置行高
		row_height_all((model->RecipeConfig.Height - model->RecipeConfig.TitleHeight - model->RecipeConfig.EachColTitleHeight) / model->RecipeConfig.PerPageRowCount);
		//设置列宽
		if (IResourceService::Ins()->IsRenderMode())
			Fl::scrollbar_size(0);
		else
			Fl::scrollbar_size(16);
		int allcolwidth = 0;
		for (unsigned i = 0; i < model->RecipeConfig.InfoLst.size(); i++)
		{
			if (i == model->RecipeConfig.InfoLst.size() - 1)
				col_width(i, (model->RecipeConfig.Width - Fl::scrollbar_size() - allcolwidth));
			else
				col_width(i, model->RecipeConfig.InfoLst[i].ColWidth);
			allcolwidth += model->RecipeConfig.InfoLst[i].ColWidth;
		}

		//设置背景颜色
		fontcolor_ = fl_rgb_color(RGBColor(model->RecipeConfig.ListFontStyle.Colors));
		cell_bgcolor_ = fl_rgb_color(RGBColor(model->RecipeConfig.TableBgColor));
		//if (model->RecipeConfig.AppearMode)
		//{
		//	box(FL_FLAT_BOX);
		//	color(cell_bgcolor_);
		//}

		focuscellcolor_ = fl_rgb_color(RGBColor(model->RecipeConfig.FocusCellColor));
		focusfontcolor_ = fl_rgb_color(RGBColor(model->RecipeConfig.FocusFontColor));
		oddbgcolor_ = fl_rgb_color(RGBColor(model->RecipeConfig.OddBackColor));
		evenbgcolor_ = fl_rgb_color(RGBColor(model->RecipeConfig.EvenBackColor));
		focusrowcolor_ = fl_rgb_color(RGBColor(model->RecipeConfig.FocusRowColor));
		//model->RecipeConfig.GridBorderStyle.Weight *= 2;
	}

	
	string RecipeChartView::GetDrawString(string projectname, int row)
	{
		shared_ptr<RecipeChartModel> model = BaseView.GetModel<RecipeChartModel>();
		
		string text = "";
		if (projectname == "序号")
		{
			if (model->RecipeConfig.SerialNumStyle)
			{
				char serial[128] = { 0 };
				snprintf(serial, sizeof(serial), "第%d组", row + 1);
				text = serial;
			}
			else
				text = to_string(row + 1);
		}
		else 
		{
			int col = RecipeDT::Ins()->GetColNum(model->RecipeConfig.RecipeGroupId, projectname);
			if (col < 0)
				return text;
			//if (model->RecipeConfig.SerialNoShown)
			//	col -= 1;
			if (RecipeDatas.size() > (size_t)row && RecipeDatas[row].size() > (size_t)col)
				text = RecipeDatas[row][col];
		}
		//else
		//{
		//	if (DisplayMode)
		//		text = RecipeDT::Ins()->GetRecipeData(model->RecipeConfig.RecipeGroupId, projectname, SearchRows[row]);
		//	else
		//		text = RecipeDT::Ins()->GetRecipeData(model->RecipeConfig.RecipeGroupId, projectname, row);
		//}
		return text;
	}

	bool RecipeChartView::HandleOperatePerm()
	{
		shared_ptr<RecipeChartModel> model = BaseView.GetModel<RecipeChartModel>();
		shared_ptr<RecipeChartControl> ctrl = BaseView.GetControl<RecipeChartControl>();
		if ((model->RecipeConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->RecipeConfig.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			if (model->RecipeConfig.Perm.IsHideElement)			//隐藏原件
			{
				model->RecipeConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->RecipeConfig.Perm.IsPopwin)				//弹出提示窗口
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		return true;
	}
	void RecipeChartView::EnterValue()
	{
		shared_ptr<RecipeChartModel> model = BaseView.GetModel<RecipeChartModel>();
		//string datastr;
		if (selectresinfo_->DataType != Project::VarDataType::DT_String
			&& selectresinfo_->DataType != Project::VarDataType::DT_Bytes) //数值
		{
			if (!inputtext_.empty())
			{
				//double data = stod(inputtext_);
				//XJDataType tmptp;
				//tmptp.Cls = selectresinfo_->DataType;
				//tmptp.Type = selectresinfo_->DataFmt;
				Utility::NumberFmtInfo fmt;
				fmt.IsFillZero = false;
				fmt.Num1 = selectresinfo_->IntegerNum;
				fmt.Num2 = selectresinfo_->DecimalNum;
				int pos = inputtext_.find('.');
				if (pos != string::npos) {
					inputtext_ = inputtext_.erase(pos - 1, 1);
					pos--;
				}
				else {
					pos = inputtext_.length();
					if (fmt.Num2 > 0) {
						inputtext_ = inputtext_ + '.' + string(fmt.Num2, '0');
					}
				}
				// 限制整数位个数
				if (pos > fmt.Num1) {
					inputtext_ = inputtext_.erase(0, pos - fmt.Num1);
				}
				// 是否前导0
				if (fmt.IsFillZero && pos < fmt.Num1) {
					inputtext_ = string(fmt.Num1 - pos, '0') + inputtext_;
				}

				//datastr = Utility::DataFormate::StrFmt(inputtext_, tmptp, fmt);
			}
		}
		//else		//字符串
			//datastr = inputtext_;
		UI::RecipeUtility::Ins().UpdateData(model->RecipeConfig.RecipeGroupId,
			model->RecipeConfig.InfoLst[selectedcol_].ProjectName, selectedrow_, inputtext_);

		PermUtility::QuitLimitPerm(model->RecipeConfig.Perm);
	}
	void RecipeChartView::InitStartInput(int mx, int my)
	{
		shared_ptr<RecipeChartControl> ctrl = BaseView.GetControl<RecipeChartControl>();
		if (!HandleOperatePerm())		//没有权限则返回
			return;
		ctrl->Win()->OpenDialogPage(SYS_FULLKEY_PAGENUM, nullptr,
			mx + col_width(callback_col()) / 2, my + row_height(callback_row()) / 2);
		vector<int>().swap(languageflag_);
		haskeyboard_ = true;
		inputtext_ = "";
	}
	bool RecipeChartView::HandleOperateConfirm()
	{
		shared_ptr<RecipeChartModel> model = BaseView.GetModel<RecipeChartModel>();
		shared_ptr<RecipeChartControl> ctrl = BaseView.GetControl<RecipeChartControl>();
		if (!PermUtility::HandleConfirmPerm(model->RecipeConfig.Perm, ctrl->CtrlId()))
			return false;
		else
		{
			EnterValue();
			return true;
		}

	}
	void RecipeChartView::EndInput()
	{
		shared_ptr<RecipeChartModel> model = BaseView.GetModel<RecipeChartModel>();
		shared_ptr<RecipeChartControl> ctrl = BaseView.GetControl<RecipeChartControl>();
		ctrl->Win()->ClosePage(SYS_FULLKEY_PAGENUM);
		ctrl->Win()->ClosePage(SYS_PINYIN_PAGENUM);
		hasPinYinpage = false;
		haskeyboard_ = false;
		redraw();
	}
	int RecipeChartView::handle(int event) {
		shared_ptr<RecipeChartModel> model = BaseView.GetModel<RecipeChartModel>();
		shared_ptr<RecipeChartControl> ctrl = BaseView.GetControl<RecipeChartControl>();
		int ret = 1;
		if (!haskeyboard_)
			ret = RowTable::handle(event);
		//int ret = Fl_Table_Row::handle(event);
		selectedrow_ = callback_row();
		selectedcol_ = callback_col();
		//TableContext context = cursor2rowcol(R, C, resizeflag);

		switch (event) {
		case FL_FOCUS:
		{
			if (haskeyboard_)
				Fl::focus(this);
			return 1;
		}
		case FL_PUSH:						//控件按下
		{
			Fl::focus(this);

			SysSetApi::TriggerBeep();
			if (!HandleOperatePerm())		//没有权限则返回
				return 1;	
			/*记录当前行列*/
			RecipeUtility::Ins().RecordFocus(model->RecipeConfig.RecipeGroupId, selectedrow_);
			int mx = Fl::event_x();
			int my = Fl::event_y();
			if (!haskeyboard_)
			{
				pushTime_ = high_resolution_clock::now();
				if (std::chrono::duration_cast<milliseconds>(high_resolution_clock::now() - firstTime_).count() > 300) {
					clickcount_ = 0;
				}
				clickcount_++;
				if (clickcount_ == 1) {
					firstTime_ = high_resolution_clock::now();
				}
				else if (clickcount_ == 2) {
					lastTime_ = high_resolution_clock::now();
					// 两次点击小于300ms 也就是连续点击  
					if (std::chrono::duration_cast<milliseconds>(lastTime_ - firstTime_).count() < 300)// 判断是否是执行了双击事件  
					{
						//双击触发，判断是否为序号和名称
						if ((size_t)selectedcol_ < model->RecipeConfig.InfoLst.size())
						{
							if (model->RecipeConfig.InfoLst[selectedcol_].ProjectName != "序号"
								&& model->RecipeConfig.InfoLst[selectedcol_].ProjectName != "名称"
								&& haskeyboard_ == false && selectedcol_ == callback_col() && selectedrow_ == callback_row())
							{
								//弹出键盘
								selectresinfo_ = RecipeDT::Ins()->GetDataType(model->RecipeConfig.RecipeGroupId, model->RecipeConfig.InfoLst[selectedcol_].ProjectName);
								if (selectresinfo_ && selectresinfo_->Editable)
									InitStartInput(mx, my);	//弹出键盘
							}
						}
					}
				}
			}
			redraw();
			if(!haskeyboard_)
				return ret;
			printf("C = %d, R = %d\n", selectedcol_, selectedrow_);
		}
		case FL_RELEASE:			//控件松开
		{
			int R, C;  				// row/column being worked on
			ResizeFlag resizeflag;		// which resizing area are we over? (0=none)
			TableContext context = cursor2rowcol(R, C, resizeflag);
			printf("C = %d, R = %d\n", C, R);
			if (when() & FL_WHEN_RELEASE)
			{
				releaseTime_ = high_resolution_clock::now();	//记录按键松开时间
				milliseconds timeInterval = std::chrono::duration_cast<milliseconds>(releaseTime_ - pushTime_);
			}
			redraw();
			if (!haskeyboard_)
				return ret;
			//return ret;
		}
		case FL_KEYBOARD:
		{
			if (!haskeyboard_ || !selectresinfo_->Editable || !selectresinfo_)
				return 1;
			/*处理键值*/
			XJDataType tp;
			tp.Cls = selectresinfo_->DataType;
			tp.Type = selectresinfo_->DataFmt;
			if (firstopen_)
			{
				inputtext_ = "";
				firstopen_ = false;
			}
			int MaxValue = 65536, MinValue;
			if (tp.Type == Project::NT_Unsigned)
				MinValue = 0;
			else
				MinValue = -65536;
			char ascii = Fl::event_text()[0];		//获取输入键值
			int del;
			if (Fl::compose(del)) {
				if (selectresinfo_->DataType != Project::VarDataType::DT_String
					&& selectresinfo_->DataType != Project::VarDataType::DT_Bytes)
				{
					string tmpstr = inputtext_;
					if (((ascii == '-' && tmpstr.empty())) || (ascii >= '0' && ascii <= '9') || (ascii == '.')
						|| (tp.Type == Project::NT_Hex && ((ascii >= 'A'&& ascii <= 'F') || (ascii >= 'a'&& ascii <= 'f'))))
					{
						if (ascii == '.')
						{
							if (tmpstr.find(".") == -1 && selectresinfo_->DecimalNum != 0) 	//判断当前是否含有小数点
								tmpstr += ascii;
						}
						else
						{
							tmpstr = tmpstr + ascii;
						}
						int pos = tmpstr.find(".");
						int act, bct;
						if (pos == 0)	//小数点在头部
						{
							act = 0;
							bct = (tmpstr.substr(pos + 1, tmpstr.size())).size();
						}
						else if (pos == -1)
						{
							act = tmpstr.size();
							bct = 0;
						}
						else
						{
							act = (tmpstr.substr(0, pos)).size();
							bct = (tmpstr.substr(pos + 1, tmpstr.size())).size();
						}
						if (tmpstr != "-" && tmpstr != "." && tmpstr != "-.")
						{
							double data;
							if (tp.Type == Project::NT_Hex || tp.Type == Project::NT_BCD)
								data = stoi(tmpstr, nullptr, 16);
							else
								data = stod(tmpstr.c_str());
							if (act <= selectresinfo_->IntegerNum && bct <= selectresinfo_->DecimalNum
								&& data <= MaxValue)
								inputtext_ = tmpstr;
						}
						else
						{
							if (((tmpstr == "-" || tmpstr != "-.") && MinValue < 0) || tmpstr == ".")
								inputtext_ = tmpstr;
						}
						return 1;
					}
				}
				else		 //字符串
				{
					/*将字符切换大小写*/
					if ((ascii >= 'A'&& ascii <= 'Z') || (ascii >= 'a'&& ascii <= 'z'))
					{
						if (KeyInputUtility::IsUpper() && (ascii >= 'a'&& ascii <= 'z'))		//大写
							ascii -= 32;
						if (!KeyInputUtility::IsUpper() && (ascii >= 'A'&& ascii <= 'Z'))		//小写
							ascii += 32;
					}

					if (KeyInputUtility::IsChinese())		//中文输入
					{
						if ((ascii >= 'A'&& ascii <= 'Z') || (ascii >= 'a'&& ascii <= 'z'))
						{
							if (!hasPinYinpage)
							{
								ctrl->Win()->OpenDialogPage(SYS_PINYIN_PAGENUM, nullptr, 0, 0);
								hasPinYinpage = true;
								pinyin_.clear();
							}
							pinyin_ += ascii;
							LocalData::SetString(SYS_PSW_PINYIN_CURVAL, pinyin_.c_str());
						}
						else if ((ascii >= '0' && ascii <= '9') && hasPinYinpage)
						{
							if (PinYinPageView::Ins()->ChineseString.size() > (size_t)(ascii - '1'))
							{
								string tmpstr = inputtext_ + PinYinPageView::Ins()->ChineseString[ascii - '1'];
								if (tmpstr.size() < selectresinfo_->RegCount * 2)
								{
									inputtext_ = tmpstr;
									languageflag_.push_back(Chineseinput);
								}
								hasPinYinpage = false;
								ctrl->Win()->ClosePage(SYS_PINYIN_PAGENUM);
								pinyin_.clear();
							}
						}
					}
					else
					{
						string tmpstr = inputtext_ + ascii;
						if (tmpstr.size() <= (size_t)(selectresinfo_->RegCount * 2))
						{
							inputtext_ = tmpstr;
							languageflag_.push_back(Charactinput);
						}
					}
				}
				char *data = new char[33];
				memset(data, '\0', 33);
				memcpy(data, inputtext_.c_str(), inputtext_.size() > 32 ? 32 : inputtext_.size());

				//memcpy(data, inputtext_.c_str(),32);
				LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
				delete[] data;
				return 1;
			}
			
			if (Fl::event_key() == FL_Enter || Fl::event_key() == FL_KP_Enter)
			{
				EndInput();
				HandleOperateConfirm();
				redraw();
				return 1;
			}
			if (Fl::event_key() == FL_BackSpace)	//删除按键
			{
				if (!pinyin_.empty())
				{
					pinyin_.pop_back();
					LocalData::SetString(SYS_PSW_PINYIN_CURVAL, pinyin_.c_str());
					if (pinyin_.empty())
					{
						hasPinYinpage = false;
						ctrl->Win()->ClosePage(SYS_PINYIN_PAGENUM);
					}
				}
				else
				{
					if (!inputtext_.empty() && !languageflag_.empty())
					{
						if (languageflag_[languageflag_.size() - 1] == Chineseinput
							&& inputtext_.size() >= 3)		//中文
						{
							languageflag_.pop_back();
							inputtext_ = inputtext_.substr(0, inputtext_.size() - 3);
						}
						if (languageflag_[languageflag_.size() - 1] == Charactinput)
						{
							languageflag_.pop_back();
							inputtext_.pop_back();
						}
					}
					int len = LocalData::GetLocalVarLen(SYS_PSW_INPUTKEY_CURVAL);
					char *data = new char[len + 1];
					memset(data, '\0', len + 1);
					memcpy(data, inputtext_.c_str(), inputtext_.size() > len ? len : inputtext_.size());
					LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
					delete[] data;
					//LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, inputtext_.c_str());
				}
				return 1;
			}
			if (Fl::event_key() == FL_Escape)	//退出按键
			{
				EndInput();
				return 1;
			}
			if (Fl::event_key() == FL_Shift_L || Fl::event_key() == FL_Shift_R)	//Shift按键，切换中英文
			{
				KeyInputUtility::SetChineseState();
				if (hasPinYinpage)
				{
					hasPinYinpage = false;
					ctrl->Win()->ClosePage(SYS_PINYIN_PAGENUM);
					pinyin_.clear();
				}
				return 1;
			}
			if (Fl::event_key() == FL_Caps_Lock)								//caps按键，切换大小写
			{
				KeyInputUtility::SetUpperState();
				return 1;
			}
		}
		default:
			return HMIBaseTable::handle(event);
		}
			
	}
	void RecipeChartView::GetSearchData()
	{
		shared_ptr<RecipeChartModel> model = BaseView.GetModel<RecipeChartModel>();
		vector<string> colinfo;

		if (model->RecipeConfig.SearchMode == Project::RecipeKey)		//按关键字
		{
			for (size_t i = 0; i < model->RecipeConfig.InfoLst.size(); i++)
			{
				if (model->RecipeConfig.InfoLst[i].ProjectName != "序号")
					colinfo.push_back(model->RecipeConfig.InfoLst[i].ProjectName);
			}
			RecipeDatas = Storage::RecipeStorage::Ins()->QueryByKey(model->RecipeConfig.RecipeGroupId, 
				model->RecipeConfig.SearchKey, colinfo);
		}
		else
		{
			vector<string> coldata;
			for (size_t i = 0; i < model->RecipeConfig.InfoLst.size(); i++)
			{
				if (model->RecipeConfig.InfoLst[i].ProjectName != "序号")
				{
					Project::RecipeInfoRes *info = RecipeDT::Ins()->GetDataType(
						model->RecipeConfig.RecipeGroupId, model->RecipeConfig.InfoLst[i].ProjectName);
					if (info->DataType != Project::DT_String)
					{
						DDWORD da;
						Utility::NumberFmtInfo fmt;
						fmt.Num1 = info->IntegerNum;
						fmt.Num2 = info->DecimalNum;
						fmt.IsFillZero = false;
						memcpy(&da, &model->RecipeConfig.SearchData, sizeof(DOUBLE));
						XJDataType tp;
						tp.Cls = info->DataType;
						tp.Type = info->DataFmt;
						string value = Utility::DataFormate::NumFmt(da, tp, fmt);
						coldata.push_back(value);
						colinfo.push_back(model->RecipeConfig.InfoLst[i].ProjectName);
					}
				}
			}
			RecipeDatas = Storage::RecipeStorage::Ins()->QueryByValue(model->RecipeConfig.RecipeGroupId, coldata, colinfo);
		}
	}

	void RecipeChartView::draw_cell(TableContext context,
		int R, int C, int X, int Y, int W, int H)
	{
		shared_ptr<RecipeChartModel> model = BaseView.GetModel<RecipeChartModel>();
		//sprintf(text, "%d/%d", R + 1, C + 1);		// text for each cell
		string text;
		switch (context)
		{
		case CONTEXT_STARTPAGE:
		{
			//获取数据
			if (IResourceService::Ins()->IsRenderMode())
				rownum = 0;
			else
			{
				if (DisplayMode)	//查询模式
					GetSearchData();
				else
					RecipeDatas = Storage::RecipeStorage::Ins()->QueryByRepiceName(model->RecipeConfig.RecipeGroupId, datacol_);
				rownum = RecipeDatas.size();
			}
			//绘制背景
			if (model->RecipeConfig.AppearMode == 0)
			{
				Fl_Image *rgbImage = IResourceService::Ins()->GetImage(model->RecipeConfig.Key.KeyVal);
				if (!rgbImage) {
					LOG_INFO_("Recipe BackImage is NULL\n");
				}
				else {
					image(rgbImage);
					align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
					box(FL_NO_BOX);
				}
				draw_box();
			}
			if (rownum > model->RecipeConfig.PerPageRowCount)
				rows(rownum);
			else if (rownum > model->RecipeConfig.TotalRowCount)
			{
				rownum = model->RecipeConfig.TotalRowCount;
				rows(rownum);
			}
			return;
		}
		case CONTEXT_COL_HEADER:						//绘制标题及列标题
		{
			/*绘制标题*/
			if (C == 0 && model->RecipeConfig.TitleUsed)
			{
				GraphicDrawHandle::PushClip(X, Y, model->RecipeConfig.Width, model->RecipeConfig.TitleHeight);
				{
					fl_font(UI::IResourceService::GetFontIdx(model->RecipeConfig.TitleStyle.Font.Name),
						model->RecipeConfig.TitleStyle.Font.Size);
					/*设置字体颜色*/
					Fl_Color textcolor = fl_rgb_color(RGBColor(model->RecipeConfig.TitleStyle.Colors));
					if (model->RecipeConfig.AppearMode)
					{
						/*设置背景颜色*/
						fl_color(active() ? fl_rgb_color(RGBColor(model->RecipeConfig.TitleBackColor)) 
							: fl_inactive(fl_rgb_color(RGBColor(model->RecipeConfig.TitleBackColor))));
						fl_rectf(model->RecipeConfig.X + model->RecipeConfig.OffX - 2,
							model->RecipeConfig.Y + model->RecipeConfig.OffY - 2,
							model->RecipeConfig.Width + 6, model->RecipeConfig.TitleHeight + 6);
					}
					/*获取关于字符串*/
					string text = StringUtility::GetDrawString(IResourceService::Ins(), model->RecipeConfig.TxtTitle, 0);
					UI::IResourceService::GB2312toUtf8(text);
					/*绘制文本*/
					fl_color(active() ? textcolor : fl_inactive(textcolor));
					fl_draw(text.data(),
						model->RecipeConfig.X + model->RecipeConfig.OffX,
						model->RecipeConfig.Y + model->RecipeConfig.OffY,
						model->RecipeConfig.Width, model->RecipeConfig.TitleHeight, 
						model->RecipeConfig.TitleStyle.Align);	//FL_ALIGN_CENTER
				}
				fl_pop_clip();
			}
			if (model->RecipeConfig.ColTitleShown)
			{
				GraphicDrawHandle::PushClip(X, Y + model->RecipeConfig.TitleHeight, W, H - model->RecipeConfig.TitleHeight);
				{
					// BG COLOR
					if (model->RecipeConfig.AppearMode)
					{
						fl_color(active() ? fl_rgb_color(RGBColor(model->RecipeConfig.ColTitleBackColor)) : 
							fl_inactive(fl_rgb_color(RGBColor(model->RecipeConfig.ColTitleBackColor))));
						fl_rectf(X - 2, Y + model->RecipeConfig.TitleHeight - 2,
							W + 6, H - model->RecipeConfig.TitleHeight + 6);
					}
					// TEXT
					if (model->RecipeConfig.InfoLst.size() > (size_t)C)
					{
						fl_font(UI::IResourceService::GetFontIdx(model->RecipeConfig.ColTitleStyle.Font.Name),
							model->RecipeConfig.ColTitleStyle.Font.Size);
						fl_color(active() ? fl_rgb_color(RGBColor(model->RecipeConfig.ColTitleStyle.Colors)) 
							: fl_inactive(fl_rgb_color(RGBColor(model->RecipeConfig.ColTitleStyle.Colors))));

						/*获取text*/
						string text = StringUtility::GetDrawString(IResourceService::Ins(),
								model->RecipeConfig.InfoLst[C].TitleDescribe, 0);
						UI::IResourceService::GB2312toUtf8(text);
						fl_draw(text.c_str(), X, Y + model->RecipeConfig.TitleHeight, W, H - model->RecipeConfig.TitleHeight, model->RecipeConfig.ColTitleStyle.Align);
					}
				}
				fl_pop_clip();
			}
			return;
		}
		case CONTEXT_CELL:
		{
			//获取显示字符串
			if (model->RecipeConfig.InfoLst.size() < (size_t)C)
				return;
			if (DisplayMode && SearchRows.size() <= (size_t)R)	//查询模式
				return;
			text = GetDrawString(model->RecipeConfig.InfoLst[C].ProjectName, R);
			if (!UI::CodeFormatUtility::IsStrUtf8(text.c_str()))
				UI::IResourceService::GB2312toUtf8(text);

			if (Y > model->RecipeConfig.Y + model->RecipeConfig.OffY + col_header_height()
				&& Y <= model->RecipeConfig.Y + model->RecipeConfig.OffY + col_header_height() + row_height(0))
				gridstarty_ = Y - model->RecipeConfig.Y - model->RecipeConfig.OffY - col_header_height();

			GraphicDrawHandle::PushClip(X, Y, W, H);
			{
				// BG COLOR
				if (model->RecipeConfig.AppearMode)
				{
					if (selectedrow_ == R)
					{
						if (selectedcol_ == C)
							fl_color(active() ? focuscellcolor_ : fl_inactive(focuscellcolor_));
						else
							fl_color(active() ? focusrowcolor_ : fl_inactive(focusrowcolor_));
					}
					else
					{
						if (model->RecipeConfig.ParityDiffColor)
						{
							if ((R + 1) % 2 == 0)		//偶数行
								fl_color(active() ? oddbgcolor_ : fl_inactive(oddbgcolor_));
							else
								fl_color(active() ? evenbgcolor_ : fl_inactive(evenbgcolor_));
						}
						else
							fl_color(active() ? cell_bgcolor_ : fl_inactive(cell_bgcolor_));
					}
					fl_rectf(X - 2, Y - 2, W + 6, H + 6);
				}
				// TEXT
					fl_font(fontStyle_, fontSize_);
					if (selectedcol_ == C && selectedrow_ == R)
						fl_color(active() ? focusfontcolor_ : fl_inactive(focusfontcolor_));
					else
						fl_color(active() ? fontcolor_ : fl_inactive(fontcolor_));
					fl_draw(text.c_str(), X, Y, W, H, model->RecipeConfig.ListFontStyle.Align);
			}
			fl_pop_clip();
			return;
		}
		case CONTEXT_TABLE:
			fprintf(stderr, "TABLE CONTEXT CALLED\n");
			return;
		case CONTEXT_ENDPAGE:		//画边框
		{
			if (model->RecipeConfig.AppearMode)
			{
				fl_color(active() ? fl_rgb_color(RGBColor(model->RecipeConfig.GridBorderStyle.Color))
					: fl_inactive(fl_rgb_color(RGBColor(model->RecipeConfig.GridBorderStyle.Color))));
				fl_line_style(model->RecipeConfig.GridBorderStyle.Type, model->RecipeConfig.GridBorderStyle.Weight);
				//绘制标题内行边框
				if (model->RecipeConfig.TitleUsed && model->RecipeConfig.IsRowDividLine)
					fl_line(model->RecipeConfig.X + model->RecipeConfig.OffX + model->RecipeConfig.TableBorderStyle.Weight / 2,
						model->RecipeConfig.Y + model->RecipeConfig.OffY + model->RecipeConfig.TitleHeight,
						model->RecipeConfig.X + model->RecipeConfig.OffX + model->RecipeConfig.Width - model->RecipeConfig.TableBorderStyle.Weight - Fl::scrollbar_size(),
						model->RecipeConfig.Y + model->RecipeConfig.OffY + model->RecipeConfig.TitleHeight);
				//绘制列表标题内行边框
				if (model->RecipeConfig.ColTitleShown && model->RecipeConfig.IsRowDividLine)
					fl_line(model->RecipeConfig.X + model->RecipeConfig.OffX + model->RecipeConfig.TableBorderStyle.Weight / 2,
						model->RecipeConfig.Y + model->RecipeConfig.OffY + model->RecipeConfig.TitleHeight + model->RecipeConfig.EachColTitleHeight,
						model->RecipeConfig.X + model->RecipeConfig.OffX + model->RecipeConfig.Width - model->RecipeConfig.TableBorderStyle.Weight - Fl::scrollbar_size(),
						model->RecipeConfig.Y + model->RecipeConfig.OffY + model->RecipeConfig.TitleHeight + model->RecipeConfig.EachColTitleHeight);


				GraphicDrawHandle::PushClip(model->RecipeConfig.X + model->RecipeConfig.OffX + model->RecipeConfig.TableBorderStyle.Weight / 2,
					model->RecipeConfig.Y + model->RecipeConfig.OffY + model->RecipeConfig.TitleHeight,
					model->RecipeConfig.Width - Fl::scrollbar_size() - model->RecipeConfig.TableBorderStyle.Weight,
					model->RecipeConfig.Height - model->RecipeConfig.TitleHeight);
				if (model->RecipeConfig.IsRowDividLine)		//使用行分割线
				{
					for (int rowY = gridstarty_ + col_header_height(); rowY < model->RecipeConfig.Height;)
					{
						fl_line(model->RecipeConfig.X + model->RecipeConfig.OffX,
							model->RecipeConfig.Y + model->RecipeConfig.OffY + rowY,
							model->RecipeConfig.X + model->RecipeConfig.OffX + model->RecipeConfig.Width,
							model->RecipeConfig.Y + model->RecipeConfig.OffY + rowY);
						rowY += row_height(0);
					}
				}
				if (model->RecipeConfig.IsColDividLine)		//使用列分割线
				{
					for (int rowX = 0, i = 0; i < cols() - 1;i++)
					{
						rowX += col_width(i);
						fl_line(model->RecipeConfig.X + model->RecipeConfig.OffX + rowX,
							model->RecipeConfig.Y + model->RecipeConfig.OffY,
							model->RecipeConfig.X + model->RecipeConfig.OffX + rowX,
							model->RecipeConfig.Y + model->RecipeConfig.OffY + model->RecipeConfig.Height);
					}
				}
				fl_pop_clip();
				if (model->RecipeConfig.IsUseBorder)		//画外边框
				{
					fl_color(active() ? fl_rgb_color(RGBColor(model->RecipeConfig.TableBorderStyle.Color))
						: fl_inactive(fl_rgb_color(RGBColor(model->RecipeConfig.TableBorderStyle.Color))));
					fl_line_style(FL_CAP_SQUARE | FL_JOIN_MITER | model->RecipeConfig.TableBorderStyle.Type, model->RecipeConfig.TableBorderStyle.Weight);

					fl_begin_line();
					fl_vertex((double)model->RecipeConfig.X + (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffX,
						(double)model->RecipeConfig.Y + (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffY);
					fl_vertex((double)model->RecipeConfig.X - (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffX + (double)model->RecipeConfig.Width,
						(double)model->RecipeConfig.Y + (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffY);
					fl_end_line();

					fl_begin_line();
					fl_vertex((double)model->RecipeConfig.X + (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffX,
						(double)model->RecipeConfig.Y - (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffY + (double)model->RecipeConfig.Height);
					fl_vertex((double)model->RecipeConfig.X - (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffX + (double)model->RecipeConfig.Width,
						(double)model->RecipeConfig.Y - (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffY + (double)model->RecipeConfig.Height);
					fl_end_line();

					fl_begin_line();
					fl_vertex((double)model->RecipeConfig.X + (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffX,
						(double)model->RecipeConfig.Y + (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffY);
					fl_vertex((double)model->RecipeConfig.X + (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffX,
						(double)model->RecipeConfig.Y - (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffY + (double)model->RecipeConfig.Height);
					fl_end_line();

					fl_begin_line();
					fl_vertex((double)model->RecipeConfig.X - (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffX + (double)model->RecipeConfig.Width,
						(double)model->RecipeConfig.Y + (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffY);
					fl_vertex((double)model->RecipeConfig.X - (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffX + (double)model->RecipeConfig.Width,
						(double)model->RecipeConfig.Y - (double)model->RecipeConfig.TableBorderStyle.Weight / 2.0 + (double)model->RecipeConfig.OffY + (double)model->RecipeConfig.Height);
					fl_end_line();
				}
			}
			return;
		}
		case CONTEXT_RC_RESIZE:
		case CONTEXT_NONE:
			return;
		case CONTEXT_ROW_HEADER:
			return;
		}
	}
}

