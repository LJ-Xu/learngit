#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "StringUtility.h"
#include "DataTableView.h"
#include "FL/fl_draw.H"
#include "IDataService.h"
#include "IResourceService.h"
#include "DataInfo.h"
#include "DataFormate.h"
#include "KeyInputUtility.h"
#include "UIData.h"
#include "CodeFormatUtility.h"
#include "HMIWindow.h"
#include "PinYinPageView.h"
#include "PermUtility.h"
#include "SysSetApi.h"
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
namespace UI
{
	DataTableView::DataTableView(int X, int Y, int W, int H, const char *l = 0) : HMIBaseTable(X, Y, W, H, l)
	{
		box(FL_NO_BOX);
		end();
	}
	DataTableView::~DataTableView() {}

	void DataTableView::InitDraw()
	{
		shared_ptr<DataTableControl> ctrl = BaseView.GetControl<DataTableControl>();
		shared_ptr<DataTableModel> model = BaseView.GetModel<DataTableModel>();
		//设置是否使用行、列标题
		if (model->DataConfig.UseTitleBar || model->DataConfig.ColTitleUsed)
			col_header(1);
		else
		{
			col_header(0);
			model->DataConfig.ColTitleHeight = 0;
			model->DataConfig.TitleHeight = 0;
		}
		row_header(0);
		col_header_height(model->DataConfig.ColTitleHeight + model->DataConfig.TitleHeight);
		//设置是否可变宽高
		row_resize(0);
		col_resize(0);

		//设置字体
		if (model->DataConfig.UseSameStyle)
		{
			fontSize_ = model->DataConfig.SameFontStyle.Font.Size;
			fontStyle_ = UI::IResourceService::GetFontIdx(model->DataConfig.SameFontStyle.Font.Name);
		}
		else
		{
			fontSize_ = model->DataConfig.ListFontStyle.Font.Size;
			fontStyle_ = UI::IResourceService::GetFontIdx(model->DataConfig.ListFontStyle.Font.Name);
		}
	
		//设置列数
		cols(model->DataConfig.ColInfoLst.size());
		//设置行数
		rows(model->DataConfig.RowsNum);

		//设置行高 
		if (model->DataConfig.UseContineAddr)	//使用连续地址
		{
			if (model->DataConfig.PerPageRows != 0)
			{
				if(model->DataConfig.RowsNum > model->DataConfig.PerPageRows)
					row_height_all((model->DataConfig.Height - model->DataConfig.TitleHeight -
						model->DataConfig.ColTitleHeight) / model->DataConfig.PerPageRows);
				else
				{
					int rowh = (model->DataConfig.Height - model->DataConfig.TitleHeight -
						model->DataConfig.ColTitleHeight) / model->DataConfig.PerPageRows;
					int allrowheight = 0;
					for (unsigned i = 0; i < model->DataConfig.RowsNum; i++)
					{
						if (i == model->DataConfig.RowsNum - 1)
							row_height(i, (model->DataConfig.Height - col_header_height() - allrowheight));
						else
							row_height(i, rowh);
						allrowheight += rowh;
					}
				}
			}
		}
		else
		{
			if (model->DataConfig.RowsNum != 0)
			{
				int rowh = (model->DataConfig.Height - model->DataConfig.TitleHeight -
					model->DataConfig.ColTitleHeight) / model->DataConfig.RowsNum;
				int allrowheight = 0;
				for (unsigned i = 0; i < model->DataConfig.RowsNum; i++)
				{
					if (i == model->DataConfig.RowsNum - 1)
						row_height(i, (model->DataConfig.Height - col_header_height() - allrowheight));
					else
						row_height(i, rowh);
					allrowheight += rowh;
				}
			}
				//row_height_all((model->DataConfig.Height - model->DataConfig.TitleHeight -
				//	model->DataConfig.ColTitleHeight) / model->DataConfig.RowsNum);
		}
		//设置列宽
		if (!model->DataConfig.UseContineAddr
			|| model->DataConfig.RowsNum == model->DataConfig.PerPageRows)
			scrollsize_ = 0;
		else
			scrollsize_ = 16;

		datacolnum_ = cols();
		for (unsigned i = 0; i < model->DataConfig.ColInfoLst.size(); i++)
		{
			if (model->DataConfig.ColInfoLst[i].ProjectName == "序号")
			{
				datacolnum_ -= 1;
				useserial_ = true;
			}
			if (model->DataConfig.ColInfoLst[i].ProjectName == "名称")
			{
				datacolnum_ -= 1;
				usestaticcol_ = true;
			}
			//if (i == model->DataConfig.ColInfoLst.size() - 1)
			//	col_width(i, (model->DataConfig.Width - scrollsize_ - allcolwidth));
			//else
			//	col_width(i, model->DataConfig.ColInfoLst[i].ColWidth);
			//allcolwidth += model->DataConfig.ColInfoLst[i].ColWidth;
		}

		//设置背景颜色
		cell_bgcolor_ = fl_rgb_color(RGBColor(model->DataConfig.TableBgColor));
		if (model->DataConfig.ColInfoLst.empty())
			return;
		int colw = (model->DataConfig.Width - scrollsize_) / model->DataConfig.ColInfoLst.size();
		int allcolwidth = 0;
		for (unsigned i = 0; i < model->DataConfig.ColInfoLst.size(); i++)
		{
			//col_width(i, model->DataConfig.AlarmOptions[i].ColWidth);
			if (i == model->DataConfig.ColInfoLst.size() - 1)
				col_width(i, model->DataConfig.Width - scrollsize_ - allcolwidth);
			else
				col_width(i, colw);
			allcolwidth += colw;
		}
		//col_width_all((model->DataConfig.Width - scrollsize_) / model->DataConfig.ColInfoLst.size());
	}
	void DataTableView::EnterValue()
	{
		shared_ptr<DataTableModel> model = BaseView.GetModel<DataTableModel>();
		if (selectcell_.AddrType == 0)			//数值
		{
			if (!inputtext_.empty())
			{
				double data = stod(inputtext_);
				UI::UIData::Number(selectcell_.DataAddrVar, data);
			}
		}
		else		//字符串
		{
			int count = UIDataService::Ins().GetDataCounts(selectcell_.DataAddrVar);
			char *buf = new char[count + 1];
			memset(buf, '\0', count + 1);
			memcpy(buf, inputtext_.c_str(), inputtext_.size() > count ? count : inputtext_.size());

			//memcpy(buf, inputtext_.c_str(), count);

			/*编码转换*/
			CodeFormatUtility::Utf8ToCode((CodeFormatUtility::CodeRule)selectcell_.Rule, buf, count);
			DataApi::SetDataBytes(selectcell_.DataAddrVar, buf, count);
			//UI::UIDataService::Ins().SetString(selectcell_.DataAddrVar, inputtext_.c_str());
		}
		PermUtility::QuitLimitPerm(model->DataConfig.Perm);
	}
	bool DataTableView::HandleOperateConfirm()
	{
		shared_ptr<DataTableModel> model = BaseView.GetModel<DataTableModel>();
		shared_ptr<DataTableControl> ctrl = BaseView.GetControl<DataTableControl>();
		if (!PermUtility::HandleConfirmPerm(model->DataConfig.Perm, ctrl->CtrlId()))
			return false;
		else
		{
			EnterValue();
			return true;
		}

	}
	void DataTableView::EndInput()
	{
		shared_ptr<DataTableModel> model = BaseView.GetModel<DataTableModel>();
		shared_ptr<DataTableControl> ctrl = BaseView.GetControl<DataTableControl>();
		ctrl->Win()->ClosePage(SYS_FULLKEY_PAGENUM);
		ctrl->Win()->ClosePage(SYS_PINYIN_PAGENUM);
		hasPinYinpage = false;
		haskeyboard_ = false;
		redraw();
	}
	bool DataTableView::HandleOperatePerm()
	{
		shared_ptr<DataTableModel> model = BaseView.GetModel<DataTableModel>();
		shared_ptr<DataTableControl> ctrl = BaseView.GetControl<DataTableControl>();

		if ((model->DataConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->DataConfig.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			if (model->DataConfig.Perm.IsHideElement)			//隐藏原件
			{
				model->DataConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->DataConfig.Perm.IsPopwin)				//弹出提示窗口
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		return true;
	}
	void DataTableView::InitStartInput(int mx, int my)
	{
		shared_ptr<DataTableControl> ctrl = BaseView.GetControl<DataTableControl>();
		if (!HandleOperatePerm())		//没有权限则返回
			return;
		ctrl->Win()->OpenDialogPage(SYS_FULLKEY_PAGENUM, nullptr,
			mx + col_width(callback_col()) / 2, my + row_height(callback_row()) / 2);
		vector<int>().swap(languageflag_);
		haskeyboard_ = true;
		inputtext_ = "";
	}
	int DataTableView::handle(int event) {
		shared_ptr<DataTableModel> model = BaseView.GetModel<DataTableModel>();
		shared_ptr<DataTableControl> ctrl = BaseView.GetControl<DataTableControl>();
		int ret = 1;
		if (!haskeyboard_)
			ret = RowTable::handle(event);
		//int ret = Fl_Table_Row::handle(event);
		//TableContext context = cursor2rowcol(R, C, resizeflag);

		switch (event) {
		case FL_PUSH:				//控件按下
		{
			SysSetApi::TriggerBeep();
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
					selectedrow_ = callback_row();
					selectedcol_ = callback_col();
					firstTime_ = high_resolution_clock::now();
				}
				else if (clickcount_ == 2) {
					lastTime_ = high_resolution_clock::now();
					// 两次点击小于300ms 也就是连续点击  
					if (std::chrono::duration_cast<milliseconds>(lastTime_ - firstTime_).count() < 300
						&& (selectedrow_ == callback_row() && selectedcol_ == callback_col()))// 判断是否是执行了双击事件  
					{
						if (usestaticcol_) selectedcol_ -= 1;
						if (useserial_) selectedcol_ -= 1;
						int num = datacolnum_ * selectedrow_ + selectedcol_;
						if (model->DataConfig.DataCellInfoLst.size() > (size_t)num)
						{
							selectcell_ = model->DataConfig.DataCellInfoLst[num];
							//双击触发，判断是否为序号和名称
							if ((size_t)selectedcol_ < model->DataConfig.ColInfoLst.size())
							{
								if (model->DataConfig.ColInfoLst[callback_col()].ProjectName != "序号"
									&& model->DataConfig.ColInfoLst[callback_col()].ProjectName != "名称"
									&& haskeyboard_ == false && selectcell_.CellType == 0 
									&& selectcell_.IsEditable && !selectcell_.IsNull)
									InitStartInput(mx,my);	//弹出键盘
							}
						}
						
					}
				}
			}
			redraw();
			if (!haskeyboard_)
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
			if (!haskeyboard_)
				return 1;
			/*处理键值*/
			XJDataType tp = UI::UIDataService::Ins().GetDataType(selectcell_.DataAddrVar);
			int MaxValue = 65536,MinValue;
			if (tp.Type == Project::NT_Unsigned)
				MinValue = 0;
			else
				MinValue = -65536;
			char ascii = Fl::event_text()[0];		//获取输入键值
			int del;
			if (Fl::compose(del)) {
				if (selectcell_.AddrType == 0)			//数值
				{
					string tmpstr = inputtext_;
					if (((ascii == '-' && tmpstr.empty())) || (ascii >= '0' && ascii <= '9') || (ascii == '.')
						|| (tp.Type == Project::NT_Hex && ((ascii >= 'A'&& ascii <= 'F') || (ascii >= 'a'&& ascii <= 'f'))))
					{
						if (ascii == '.')
						{
							if (tmpstr.find(".") == -1 && selectcell_.DecimalNum != 0) 	//判断当前是否含有小数点
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
							if (act <= selectcell_.IntegerNum && bct <= selectcell_.DecimalNum
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
								if (tmpstr.size() < selectcell_.RegCount * 2)
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
						if (tmpstr.size() <= (size_t)(selectcell_.RegCount * 2))
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
				//LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, inputtext_.c_str());
			}
			if (Fl::event_key() == FL_Enter || Fl::event_key() == FL_KP_Enter)
			{
				//EnterValue();
				EndInput();
				HandleOperateConfirm();
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
			return HMIBaseTable::handle(event);
		}
		default:
			return HMIBaseTable::handle(event);
		}
	}

	string DataTableView::GetDrawString(int row, int col)
	{
		shared_ptr<DataTableModel> model = BaseView.GetModel<DataTableModel>();

		string serialString = "序号";
		string staticcolString = "名称";
		string text = "";
		if (model->DataConfig.ColInfoLst[col].ProjectName == serialString)
			text = to_string(row + 1);
		else if (model->DataConfig.ColInfoLst[col].ProjectName == staticcolString)
		{
			if(model->DataConfig.StaticColName.size() > (size_t)row)
				text = StringUtility::GetDrawString(IResourceService::Ins(), model->DataConfig.StaticColName[row], 0);
		}
		else
		{
			if (!IResourceService::Ins()->IsRenderMode())
			{
				//int colnum = cols();
				if (usestaticcol_) col -= 1;
				if (useserial_) col -= 1;
				if (col == selectedcol_ && row == selectedrow_ && haskeyboard_)
					return inputtext_;
				int num = datacolnum_ * row + col;
				if (model->DataConfig.DataCellInfoLst.size() > (size_t)num)
				{
					Project::DataCell cellinfo = model->DataConfig.DataCellInfoLst[num];
					if (cellinfo.IsNull)
						return text;
					if (cellinfo.CellType)	//文本
						text = cellinfo.Text;
					else		//地址监控
					{
						if (cellinfo.AddrType)		//字符
						{
							int count = UIDataService::Ins().GetDataCounts(cellinfo.DataAddrVar);
							char *buf = new char[count + 1];
							memset(buf, '\0', count + 1);
							UIDataService::Ins().GetString(cellinfo.DataAddrVar, buf, count);
							text = buf;
							delete[] buf;
						}
						else //数值
						{
							double value = UI::UIData::Number<double>(cellinfo.DataAddrVar);
							DDWORD da;
							memcpy(&da, &value, sizeof(DOUBLE));
							//DDWORD da = UIDataService::Ins().GetNumber(cellinfo.DataAddrVar);

							XJDataType tp = UIDataService::Ins().GetDataType(cellinfo.DataAddrVar);
							Utility::NumberFmtInfo fmt;
							fmt.IsFillZero = cellinfo.IsFillZero;
							fmt.Num1 = cellinfo.IntegerNum;
							fmt.Num2 = cellinfo.DecimalNum;
							text = Utility::DataFormate::NumFmt(da, tp, fmt);
						}
					}
				}
			}
		}
		return text;
	}



	void DataTableView::draw_cell(TableContext context,
		int R, int C, int X, int Y, int W, int H)
	{
		shared_ptr<DataTableModel> model = BaseView.GetModel<DataTableModel>();
		string text;
		//int srow = callback_row();
		//int scol = callback_col();

		switch (context)
		{
		case CONTEXT_STARTPAGE:
		{
			//绘制背景
			if (model->DataConfig.Appearance == 0)
			{
				Fl_Image *rgbImage = IResourceService::Ins()->GetImage(model->DataConfig.Key.KeyVal);
				if (!rgbImage) {
					printf("Can't get image\n");
				}
				else {
					image(rgbImage);
					align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
					box(FL_NO_BOX);
				}
				draw_box();
			}
			return;
		}
		case CONTEXT_COL_HEADER:						//绘制标题及列标题
		{
			/*绘制标题*/
			if (C == 0 && model->DataConfig.UseTitleBar)
			{
				GraphicDrawHandle::PushClip(X, Y, model->DataConfig.Width, model->DataConfig.TitleHeight);
				{
					fl_font(UI::IResourceService::GetFontIdx(model->DataConfig.TitleBarStyle.Font.Name),
						model->DataConfig.TitleBarStyle.Font.Size);
					int h = model->DataConfig.TitleHeight;
					Fl_Color textcolor = fl_rgb_color(RGBColor(model->DataConfig.TitleBarStyle.Colors));
					if (model->DataConfig.Appearance)
					{
						/*设置背景颜色*/
						fl_color(fl_rgb_color(RGBColor(model->DataConfig.TitleBgColor)));
						fl_rectf(model->DataConfig.X + model->DataConfig.OffX,
							model->DataConfig.Y + model->DataConfig.OffY, w(), h);
					}
					/*获取关于字符串*/
					string text = StringUtility::GetDrawString(IResourceService::Ins(), model->DataConfig.TitleRes, 0);
					UI::IResourceService::GB2312toUtf8(text);
					/*绘制文本*/
					fl_color(textcolor);
					fl_draw(text.data(), model->DataConfig.X + model->DataConfig.OffX,
						model->DataConfig.Y + model->DataConfig.OffY, w(), 
						h, model->DataConfig.TitleBarStyle.Align);	//FL_ALIGN_CENTER

					
				}
				fl_pop_clip();
			}
			if (model->DataConfig.ColTitleUsed)
			{
				GraphicDrawHandle::PushClip(X, Y + model->DataConfig.TitleHeight, W, H - model->DataConfig.TitleHeight);
				{
					// BG COLOR
					if (model->DataConfig.Appearance)
					{
						fl_color(cell_bgcolor_);
						fl_rectf(X, Y, W, H);
					}
					// TEXT
					if (model->DataConfig.ColInfoLst.size() > (size_t)C)
					{
						fl_font(UI::IResourceService::GetFontIdx(model->DataConfig.TitleBarStyle.Font.Name),
							model->DataConfig.TitleBarStyle.Font.Size);
						fl_color(fl_rgb_color(RGBColor(model->DataConfig.TitleBarStyle.Colors)));

						/*获取text*/
						string text = StringUtility::GetDrawString(IResourceService::Ins(),
							model->DataConfig.ColInfoLst[C].TitleDescribe, 0);
						UI::IResourceService::GB2312toUtf8(text);
						fl_draw(text.c_str(), X, Y + model->DataConfig.TitleHeight,
							W, H - model->DataConfig.TitleHeight, model->DataConfig.TitleBarStyle.Align);
					}
				}
				fl_pop_clip();
			}
			return;
		}
		case CONTEXT_CELL:
		{
			//获取显示字符串
			if (model->DataConfig.ColInfoLst.size() < (size_t)C)
				return;
			text = GetDrawString(R, C);
			if (!UI::CodeFormatUtility::IsStrUtf8(text.c_str()))
				UI::IResourceService::GB2312toUtf8(text);
			if (Y > model->DataConfig.Y + model->DataConfig.OffY + col_header_height()
				&& Y <= model->DataConfig.Y + model->DataConfig.OffY + col_header_height() + row_height(0))
			{
				gridstartrow_ = R;
				gridstarty_ = Y - model->DataConfig.Y - model->DataConfig.OffY - col_header_height();
			}
			GraphicDrawHandle::PushClip(X, Y, W, H);
			{
				// BG COLOR
				if (model->DataConfig.Appearance)
				{
					fl_color(cell_bgcolor_);
					fl_rectf(X, Y, W, H);
				}
				// TEXT'
				int align = 0;
				if (model->DataConfig.UseSameStyle)
				{
					fl_font(UI::IResourceService::GetFontIdx(model->DataConfig.SameFontStyle.Font.Name),
						model->DataConfig.SameFontStyle.Font.Size);
					fl_color(fl_rgb_color(RGBColor(model->DataConfig.SameFontStyle.Colors)));
					align = model->DataConfig.SameFontStyle.Align;
				}
				else
				{
					fl_font(fontStyle_, fontSize_);
					fl_color(fl_rgb_color(RGBColor(model->DataConfig.ListFontStyle.Colors)));
					align = model->DataConfig.ListFontStyle.Align;
				}
				fl_draw(text.c_str(), X, Y, W, H, align);
			}
			fl_pop_clip();
			return;
		}
		case CONTEXT_TABLE:
			fprintf(stderr, "TABLE CONTEXT CALLED\n");
			return;
		case CONTEXT_ENDPAGE:		//画边框
		{
			if (model->DataConfig.Appearance)
			{
				fl_color(active() ? fl_rgb_color(RGBColor(model->DataConfig.GridBorderStyle.Color))
					: fl_inactive(fl_rgb_color(RGBColor(model->DataConfig.GridBorderStyle.Color))));
				fl_line_style(model->DataConfig.GridBorderStyle.Type, model->DataConfig.GridBorderStyle.Weight);
				//绘制标题内行边框
				if (model->DataConfig.UseTitleBar && model->DataConfig.IsRowDividLine)
					fl_line(model->DataConfig.X + model->DataConfig.OffX + model->DataConfig.TableBorderStyle.Weight / 2,
						model->DataConfig.Y + model->DataConfig.OffY + model->DataConfig.TitleHeight,
						model->DataConfig.X + model->DataConfig.OffX + model->DataConfig.Width - model->DataConfig.TableBorderStyle.Weight - scrollsize_,
						model->DataConfig.Y + model->DataConfig.OffY + model->DataConfig.TitleHeight);
				//绘制列表标题内行边框
				if (model->DataConfig.ColTitleUsed && model->DataConfig.IsRowDividLine)
					fl_line(model->DataConfig.X + model->DataConfig.OffX + model->DataConfig.TableBorderStyle.Weight / 2,
						model->DataConfig.Y + model->DataConfig.OffY + model->DataConfig.TitleHeight + model->DataConfig.ColTitleHeight,
						model->DataConfig.X + model->DataConfig.OffX + model->DataConfig.Width - model->DataConfig.TableBorderStyle.Weight - scrollsize_,
						model->DataConfig.Y + model->DataConfig.OffY + model->DataConfig.TitleHeight + model->DataConfig.ColTitleHeight);


				GraphicDrawHandle::PushClip(model->DataConfig.X + model->DataConfig.OffX + model->DataConfig.TableBorderStyle.Weight / 2,
					model->DataConfig.Y + model->DataConfig.OffY + model->DataConfig.TitleHeight,
					model->DataConfig.Width - scrollsize_ - model->DataConfig.TableBorderStyle.Weight,
					model->DataConfig.Height - model->DataConfig.TitleHeight);
				if (model->DataConfig.IsRowDividLine)		//使用行分割线
				{
					for (int i = 0, rowY = gridstarty_ + col_header_height(); rowY < model->DataConfig.Height; i ++)
					{
						fl_line(model->DataConfig.X + model->DataConfig.OffX,
							model->DataConfig.Y + model->DataConfig.OffY + rowY,
							model->DataConfig.X + model->DataConfig.OffX + model->DataConfig.Width,
							model->DataConfig.Y + model->DataConfig.OffY + rowY);
						rowY += row_height(gridstartrow_ + i);
					}
				}
				if (model->DataConfig.IsColDividLine)		//使用列分割线
				{
					for (int rowX = 0, i = 0; i < cols() - 1;i++)
					{
						rowX += col_width(i);
						fl_line(model->DataConfig.X + model->DataConfig.OffX + rowX,
							model->DataConfig.Y + model->DataConfig.OffY,
							model->DataConfig.X + model->DataConfig.OffX + rowX,
							model->DataConfig.Y + model->DataConfig.OffY + model->DataConfig.Height);
					}
				}
				fl_pop_clip();

				if (model->DataConfig.IsUseBorder)			//使用外边框
				{
					fl_color(active() ? fl_rgb_color(RGBColor(model->DataConfig.TableBorderStyle.Color))
						: fl_inactive(fl_rgb_color(RGBColor(model->DataConfig.TableBorderStyle.Color))));
					fl_line_style(FL_CAP_SQUARE | FL_JOIN_MITER | model->DataConfig.TableBorderStyle.Type, model->DataConfig.TableBorderStyle.Weight);

					fl_begin_line();
					fl_vertex((double)model->DataConfig.X + (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffX,
						(double)model->DataConfig.Y + (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffY);
					fl_vertex((double)model->DataConfig.X - (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffX + (double)model->DataConfig.Width,
						(double)model->DataConfig.Y + (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffY);
					fl_end_line();

					fl_begin_line();
					fl_vertex((double)model->DataConfig.X + (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffX,
						(double)model->DataConfig.Y - (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffY + (double)model->DataConfig.Height);
					fl_vertex((double)model->DataConfig.X - (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffX + (double)model->DataConfig.Width,
						(double)model->DataConfig.Y - (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffY + (double)model->DataConfig.Height);
					fl_end_line();

					fl_begin_line();
					fl_vertex((double)model->DataConfig.X + (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffX,
						(double)model->DataConfig.Y + (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffY);
					fl_vertex((double)model->DataConfig.X + (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffX,
						(double)model->DataConfig.Y - (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffY + (double)model->DataConfig.Height);
					fl_end_line();

					fl_begin_line();
					fl_vertex((double)model->DataConfig.X - (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffX + (double)model->DataConfig.Width,
						(double)model->DataConfig.Y + (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffY);
					fl_vertex((double)model->DataConfig.X - (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffX + (double)model->DataConfig.Width,
						(double)model->DataConfig.Y - (double)model->DataConfig.TableBorderStyle.Weight / 2.0 + (double)model->DataConfig.OffY + (double)model->DataConfig.Height);
					fl_end_line();
				}
			}
			if(IResourceService::Ins()->IsRenderMode() && scrollsize_ != 0)
				GraphicDrawHandle::Ins()->DrawRenderScroll(0, model->DataConfig.X + model->DataConfig.OffX + model->DataConfig.Width - scrollsize_ - model->DataConfig.TableBorderStyle.Weight,
					model->DataConfig.Y - model->DataConfig.TableBorderStyle.Weight + model->DataConfig.OffY,
					scrollsize_, model->DataConfig.Height - model->DataConfig.TableBorderStyle.Weight * 2);

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

