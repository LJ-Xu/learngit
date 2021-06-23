#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "StringUtility.h"
#include "ReportFormView.h"
#include "FL/fl_draw.H"
#include "IDataService.h"
#include "DataInfo.h"
#include "DataFormate.h"
#include "CodeFormatUtility.h"
namespace UI
{
	ReportFormView::ReportFormView(int X, int Y, int W, int H, const char *l = 0) : HMIBaseTable(X, Y, W, H, l)
	{
		box(FL_NO_BOX);
		end();
	}
	ReportFormView::~ReportFormView() {}

	void ReportFormView::InitDraw()
	{
		shared_ptr<ReportFormControl> ctrl = BaseView.GetControl<ReportFormControl>();
		shared_ptr<ReportFormModel> model = BaseView.GetModel<ReportFormModel>();
		//�����Ƿ�ʹ���С��б���
		if (!model->ReportConfig.IsViewEachColTitle)
			model->ReportConfig.EachColTitleHeight = 0;
		if (!model->ReportConfig.IsViewTitle)
			model->ReportConfig.TitleHeight = 0;
		if (model->ReportConfig.IsViewEachColTitle || model->ReportConfig.IsViewTitle)
			col_header(1);
		else
			col_header(0);
		row_header(0);
		col_header_height(model->ReportConfig.TitleHeight + model->ReportConfig.EachColTitleHeight);
		//�����Ƿ�ɱ���
		row_resize(0);
		col_resize(0);

		//��������
		fontSize_ = model->ReportConfig.ListFontStyle.Font.Size;
		fontStyle_ = UI::IResourceService::GetFontIdx(model->ReportConfig.ListFontStyle.Font.Name);

		//��������
		cols(model->ReportConfig.InfoLst.size());
		//��������
		rows(model->ReportConfig.PerPageRows + 1);
		//�����и�
		row_height_all((model->ReportConfig.Height - model->ReportConfig.TitleHeight - model->ReportConfig.EachColTitleHeight)/model->ReportConfig.PerPageRows);
		//�����п�
		if(IResourceService::Ins()->IsRenderMode())
			Fl::scrollbar_size(0);
		else
			Fl::scrollbar_size(16);

		int allcolwidth = 0;
		for (unsigned i = 0; i < model->ReportConfig.InfoLst.size(); i++)
		{
			if (i == model->ReportConfig.InfoLst.size() - 1)
				col_width(i, (model->ReportConfig.Width - Fl::scrollbar_size()- allcolwidth));
			else
				col_width(i, model->ReportConfig.InfoLst[i].ColWidth);
			allcolwidth += model->ReportConfig.InfoLst[i].ColWidth;
		}

		//���ñ�����ɫ
		cell_bgcolor_ = fl_rgb_color(RGBColor(model->ReportConfig.TableBgColor));
		//if (model->ReportConfig.Appearance)
		//{
		//	box(FL_FLAT_BOX);
		//	color(cell_bgcolor_);
		//}
		//��������ɫ�����뷽ʽ
		serialcolor_ = fl_rgb_color(RGBColor(model->ReportConfig.SerialColor));
		datecolor_ = fl_rgb_color(RGBColor(model->ReportConfig.DateColor));
		timecolor_ = fl_rgb_color(RGBColor(model->ReportConfig.TimeColor));

		std::vector<Fl_Color>().swap(colcolor_);
		std::vector<Fl_Align>().swap(colalignstyle_);

		string serialString = "���";
		string dateString = "����";
		string timeString = "ʱ��";
		//UI::IResourceService::GB2312toUtf8(serialString);
		//UI::IResourceService::GB2312toUtf8(dateString);
		//UI::IResourceService::GB2312toUtf8(timeString);
		string text;
		Fl_Color textColor;
		Fl_Align alignstyle = FL_ALIGN_CENTER;
		for (unsigned i = 0; i < model->ReportConfig.InfoLst.size(); i++)
		{
			if (model->ReportConfig.InfoLst[i].ProjectName == serialString)
			{
				//���
				text = model->ReportConfig.InfoLst[i].DescribeName;
				textColor = serialcolor_;
			}
			else if (model->ReportConfig.InfoLst[i].ProjectName == dateString)
			{
				text = model->ReportConfig.InfoLst[i].DescribeName;
				//UI::IResourceService::GB2312toUtf8(text);
				textColor = datecolor_;
			}
			else if (model->ReportConfig.InfoLst[i].ProjectName == timeString)
			{
				text = model->ReportConfig.InfoLst[i].DescribeName;
				//UI::IResourceService::GB2312toUtf8(text);
				textColor = timecolor_;
			}
			else
			{
				text = model->ReportConfig.InfoLst[i].DescribeName;
				//UI::IResourceService::GB2312toUtf8(text);
				int channelNo = atoi(&model->ReportConfig.InfoLst[i].ProjectName.back());
				channelno_.push_back(channelNo);
				for (unsigned j = 0; j < model->ReportConfig.ReportChannelLst.size(); j++)
				{
					if (channelNo == model->ReportConfig.ReportChannelLst[j].Channel)
					{
						alignstyle = model->ReportConfig.ReportChannelLst[j].AlignStyle;
						textColor = fl_rgb_color(RGBColor(model->ReportConfig.ReportChannelLst[j].ViewColor));
						break;
					}
				}
			}
			colcolor_.push_back(textColor);
			colalignstyle_.push_back(alignstyle);
		}
	}
		
	string ReportFormView::GetTimeString(DDWORD time, int mode)
	{
		shared_ptr<ReportFormModel> model = BaseView.GetModel<ReportFormModel>();
		char displayTime[100];
		string text;
		time_t tmSec = time / 1000;
		int tmMs = (int)(time % 1000);
		struct tm *local;
		local = localtime(&tmSec);
		if (mode)	//ʱ��
		{
			switch (model->ReportConfig.TimeStyle)
			{
			case Project::HMS:
				sprintf(displayTime, "%d:%d:%d", local->tm_hour, local->tm_min, local->tm_sec);
				break;
			case Project::HM:
				sprintf(displayTime, "%d:%d", local->tm_hour, local->tm_min);
				break;
			case Project::HMSMS:
				sprintf(displayTime, "%d:%d:%d:%d", local->tm_hour, local->tm_min, local->tm_sec, tmMs);
				break;
			case Project::HMSChinese:
				sprintf(displayTime, "%dʱ%d��%d��", local->tm_hour, local->tm_min, local->tm_sec);
				break;
			default:
				break;
			}
		}
		else    //����
		{
			switch (model->ReportConfig.DateStyle)
			{
			case Project::MDYSlash:
				sprintf(displayTime, "%d\\%d\\%d", local->tm_mon + 1, local->tm_mday, local->tm_year + 1900);
				break;
			case Project::DMYSlash:
				sprintf(displayTime, "%d\\%d\\%d", local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
				break;
			case Project::YMDChinese:
				sprintf(displayTime, "%d��%d��%d��", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
				break;
			case Project::YMDSlash:
				sprintf(displayTime, "%d\\%d\\%d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
				break;
			default:
				break;
			}
		}
		text = displayTime;
		return text;
	}

	string ReportFormView::GetDrawString(string projectname, int row)
	{
		shared_ptr<ReportFormModel> model = BaseView.GetModel<ReportFormModel>();

		string serialString = "���";
		string dateString = "����";
		string timeString = "ʱ��";
		//UI::IResourceService::GB2312toUtf8(serialString);
		//UI::IResourceService::GB2312toUtf8(dateString);
		//UI::IResourceService::GB2312toUtf8(timeString);
		string text = "";
		if (ChannelData.empty())
			return text;
		bool hasData = false;
		for (size_t i = 0; i < ChannelData.size(); i++)
		{
			if(!ChannelData[i].empty() && (size_t)row < ChannelData[i].size())
				hasData = true;
		}
		if (!hasData)
			return text;
		if (projectname == serialString)		//��ȡ���
			text = to_string(row + 1);
		else if (projectname == dateString)		//��ȡ����
		{
			for (size_t i = 0; i < ChannelData.size(); i++)
			{
				if (ChannelData[i].size() > (size_t)row)
					text = GetTimeString(ChannelData[i][row].Date, 0);
			}
		}
		else if (projectname == timeString)		//��ȡʱ��
		{
			for (size_t i = 0; i < ChannelData.size(); i++)
			{
				if (ChannelData[i].size() > (size_t)row)
					text = GetTimeString(ChannelData[i][row].Date, 1);
			}
		}
		else	//��ȡͨ������
		{
			int channelNo = atoi(&projectname.back());
			for (size_t i = 0; i < ChannelData.size(); i++)
			{
				if (ChannelData[i].empty() || (size_t)row >= ChannelData[i].size())
					return text;
				if (ChannelData[i][row].Channel == channelNo)
				{
					for (size_t j = 0; j < model->ReportConfig.ReportChannelLst.size(); j++)
					{
						if (model->ReportConfig.ReportChannelLst[j].Channel == channelNo)
						{
							Utility::NumberFmtInfo fmt;
							fmt.IsFillZero = model->ReportConfig.ReportChannelLst[j].IsLead;
							fmt.Num1 = model->ReportConfig.ReportChannelLst[j].IntegerNum;
							fmt.Num2 = model->ReportConfig.ReportChannelLst[j].DecimalNum;
							//text = Utility::DataFormate::NumFmt(ChannelData[i][row].Data, ChannelData[i][row].Type, fmt);
							double db = Utility::DataFormate::GetNumber<double>(ChannelData[i][row].Data, ChannelData[i][row].Type);
							DDWORD da;
							memcpy(&da, &db, sizeof(DDWORD));
							text = Utility::DataFormate::NumFmt(da, ChannelData[i][row].Type, fmt);
							return text;
						}
					}
					
				}
			}
		}
		return text;
	}

	int ReportFormView::GetSampleDataNum()
	{
		shared_ptr<ReportFormModel> model = BaseView.GetModel<ReportFormModel>();
		size_t num = 0;
		//��ȡͨ������
		if (DisplayMode)		//��ѯģʽ
		{
			vector<vector<Storage::SampleRecord>>().swap(ChannelData);
			if (model->ReportConfig.SearchMode == Project::ReportSeekMode::ReportTIME)
			{
				for (size_t i = 0; i < channelno_.size(); i++)
				{
					vector<Storage::SampleRecord> tmpdata=Storage::SampleStorage::Ins()->QueryByTime(channelno_[i], model->ReportConfig.SearchTimeStart,
						model->ReportConfig.SearchTimeEnd);
					if (model->ReportConfig.ViewSort && !tmpdata.empty())
						std::reverse(tmpdata.begin(), tmpdata.end());
					if (num < tmpdata.size())
						num = tmpdata.size();
					ChannelData.push_back(tmpdata);
				}
			}
			else if (model->ReportConfig.SearchMode == Project::ReportSeekMode::ReportDATE)
			{
				for (size_t i = 0; i < channelno_.size(); i++)
				{
					vector<Storage::SampleRecord> tmpdata=Storage::SampleStorage::Ins()->QueryByDate(channelno_[i], model->ReportConfig.SearchDate,
						model->ReportConfig.SearchDate + (DDWORD)86400000);
					if (model->ReportConfig.ViewSort && !tmpdata.empty())
						std::reverse(tmpdata.begin(), tmpdata.end());
					if (num < tmpdata.size())
						num = tmpdata.size();
					ChannelData.push_back(tmpdata);
				}
			}
		}
		else
		{
			/*�������*/
			vector<vector<Storage::SampleRecord>>().swap(ChannelData);
			for (size_t i = 0; i < channelno_.size(); i++)
			{
				vector<Storage::SampleRecord> tmpdata;
				Storage::SampleStorage::Ins()->QueryLastRecords(channelno_[i], model->ReportConfig.MaxRows,tmpdata);
				if (model->ReportConfig.ViewSort && !tmpdata.empty())
					std::reverse(tmpdata.begin(), tmpdata.end());
					//tmpdata.reserve(tmpdata.size());
				if (num < tmpdata.size())
					num = tmpdata.size();
				ChannelData.push_back(tmpdata);
			}
		}
		return num;
	}
	void ReportFormView::SetRenderData()
	{
		shared_ptr<ReportFormModel> model = BaseView.GetModel<ReportFormModel>();
		vector<vector<Storage::SampleRecord>>().swap(ChannelData);
		int num = model->ReportConfig.InfoLst.size();
		for (size_t i = 0; i < model->ReportConfig.InfoLst.size(); i++)
		{
			string serialString = "���";
			string dateString = "����";
			string timeString = "ʱ��";
			if (model->ReportConfig.InfoLst[i].ProjectName == serialString ||
				model->ReportConfig.InfoLst[i].ProjectName == dateString ||
				model->ReportConfig.InfoLst[i].ProjectName == timeString)
				num -= 1;
		}
#ifdef WIN32
		XJDataType tp;
		tp.Cls = Project::DT_WORD;
		tp.Type = Project::NT_Unsigned;
		for (size_t j = 0; j < (size_t)num; j++)
		{
			vector<Storage::SampleRecord> tmprecords;
			for (size_t i = 0; i < 3; i++)
			{
				Storage::SampleRecord record;
				record.Channel = j + 1;		// �ɼ�ͨ�����
				record.Data = i * j;		// �ɼ���������
				record.Type = tp;			// �ɼ��������ݸ�ʽ
				record.Date = duration_cast<MilliSecs>(system_clock::now().time_since_epoch()).count();		// �ɼ�����ʱ��
				tmprecords.push_back(record);
			}
			ChannelData.push_back(tmprecords);
		}
#endif // WIN32
		
	}
	void ReportFormView::draw_cell(TableContext context,
		int R, int C, int X, int Y, int W, int H)
	{
		shared_ptr<ReportFormModel> model = BaseView.GetModel<ReportFormModel>();
		string text;
		switch (context)
		{
		case CONTEXT_STARTPAGE:
		{
			//��ȡ����
			if (IResourceService::Ins()->IsRenderMode())
			{
				InitDraw();
				SetRenderData();
			}
			else
			{
				int num = GetSampleDataNum();
				if (num > model->ReportConfig.PerPageRows)
					rows(num);
			}
			
			//���Ʊ���
			if (model->ReportConfig.Appearance == 0)
			{
				Fl_Image *rgbImage = IResourceService::Ins()->GetImage(model->ReportConfig.Key.KeyVal);
				if (!rgbImage) {
					LOG_INFO_("Report BackImage is NULL\n");
				}
				else {
					//rgbImage->draw(model->ReportConfig.X + model->ReportConfig.OffX,
					//	model->ReportConfig.Y + model->ReportConfig.OffY,
					//	rgbImage->w(), rgbImage->h());
					image(rgbImage);
					align(FL_ALIGN_IMAGE_BACKDROP);		//���ñ���ͼƬ
					box(FL_NO_BOX);
				}
				draw_box();
			}
			return;
		}
		case CONTEXT_COL_HEADER:						//���Ʊ��⼰�б���
		{
			/*���Ʊ���*/
			if (C == 0 && model->ReportConfig.IsViewTitle)
			{
				GraphicDrawHandle::PushClip(X, Y, model->ReportConfig.Width, model->ReportConfig.TitleHeight);
				{
					fl_font(UI::IResourceService::GetFontIdx(model->ReportConfig.ElementTitleStyle.Font.Name),
						model->ReportConfig.ElementTitleStyle.Font.Size);
					int h = model->ReportConfig.TitleHeight;
					/*����������ɫ*/
					Fl_Color textcolor = fl_rgb_color(RGBColor(model->ReportConfig.ElementTitleStyle.Colors));
					if (model->ReportConfig.Appearance)
					{
						/*���ñ�����ɫ*/
						fl_color(fl_rgb_color(RGBColor(model->ReportConfig.TitleBgColor)));
						fl_rectf(model->ReportConfig.X + model->ReportConfig.OffX,
							model->ReportConfig.Y + model->ReportConfig.OffY,
							w(), h);
					}
					/*��ȡ�����ַ���*/
					string text = StringUtility::GetDrawString(IResourceService::Ins(), model->ReportConfig.TitleRes, 0);
					UI::IResourceService::GB2312toUtf8(text);
					/*�����ı�*/
					fl_color(textcolor);
					fl_draw(text.data(), model->ReportConfig.X + model->ReportConfig.OffX,
						model->ReportConfig.Y + model->ReportConfig.OffY,
						w(), h, model->ReportConfig.ElementTitleStyle.Align);	//FL_ALIGN_CENTER
				}
				fl_pop_clip();
			}
			if (model->ReportConfig.IsViewEachColTitle)
			{
				GraphicDrawHandle::PushClip(X, Y + model->ReportConfig.TitleHeight, W, H - model->ReportConfig.TitleHeight);
				{
					// BG COLOR
					if (model->ReportConfig.Appearance)
					{
						fl_color(cell_bgcolor_);
						fl_rectf(X, Y + model->ReportConfig.TitleHeight, W, H - model->ReportConfig.TitleHeight);
					}
					// TEXT
					if (model->ReportConfig.InfoLst.size() > (size_t)C)
					{
						fl_font(UI::IResourceService::GetFontIdx(model->ReportConfig.TitleBarStyle.Font.Name),
							model->ReportConfig.TitleBarStyle.Font.Size);
						fl_color(fl_rgb_color(RGBColor(model->ReportConfig.TitleBarStyle.Colors)));

						/*��ȡtext*/
						string text = StringUtility::GetDrawString(IResourceService::Ins(),
								model->ReportConfig.InfoLst[C].TitleDescribe, 0);
						UI::IResourceService::GB2312toUtf8(text);
						fl_draw(text.c_str(), X, Y + model->ReportConfig.TitleHeight, W, H - model->ReportConfig.TitleHeight, colalignstyle_[C]);
					}
				}
				fl_pop_clip();
			}
			return;
		}
		case CONTEXT_CELL:
		{
			//��ȡ��ʾ�ַ���
			if (model->ReportConfig.InfoLst.size() < (size_t)C)
				return;
			text = GetDrawString(model->ReportConfig.InfoLst[C].ProjectName, R);
			if (!UI::CodeFormatUtility::IsStrUtf8(text.c_str()))
				UI::IResourceService::GB2312toUtf8(text);

			if (Y > model->ReportConfig.Y + model->ReportConfig.OffY + col_header_height()
				&& Y <= model->ReportConfig.Y + model->ReportConfig.OffY + col_header_height() + row_height(0))
				gridstarty_ = Y - model->ReportConfig.Y - model->ReportConfig.OffY - col_header_height();
			GraphicDrawHandle::PushClip(X, Y, W, H);
			{
				// BG COLOR
				if (model->ReportConfig.Appearance)
				{
					fl_color(cell_bgcolor_);
					fl_rectf(X, Y, W, H);
				}
				// TEXT
				if (model->ReportConfig.InfoLst.size() > (size_t)C)
				{
					fl_font(fontStyle_, fontSize_);
					fl_color(colcolor_[C]);
					fl_draw(text.c_str(), X, Y, W, H, colalignstyle_[C]);
				}
			}
			fl_pop_clip();
			return;
		}
		case CONTEXT_TABLE:
			fprintf(stderr, "TABLE CONTEXT CALLED\n");
			return;
		case CONTEXT_ENDPAGE:		//���߿�
		{
			if (model->ReportConfig.Appearance)
			{
				fl_color(active() ? fl_rgb_color(RGBColor(model->ReportConfig.GridBorderStyle.Color))
					: fl_inactive(fl_rgb_color(RGBColor(model->ReportConfig.GridBorderStyle.Color))));
				fl_line_style(model->ReportConfig.GridBorderStyle.Type, model->ReportConfig.GridBorderStyle.Weight);
				//���Ʊ������б߿�
				if (model->ReportConfig.IsViewTitle && model->ReportConfig.IsRowDividLine)
					fl_line(model->ReportConfig.X + model->ReportConfig.OffX + model->ReportConfig.TableBorderStyle.Weight / 2,
						model->ReportConfig.Y + model->ReportConfig.OffY + model->ReportConfig.TitleHeight,
						model->ReportConfig.X + model->ReportConfig.OffX + model->ReportConfig.Width - model->ReportConfig.TableBorderStyle.Weight -Fl::scrollbar_size(),
						model->ReportConfig.Y + model->ReportConfig.OffY + model->ReportConfig.TitleHeight);
				//�����б�������б߿�
				if (model->ReportConfig.IsViewEachColTitle && model->ReportConfig.IsRowDividLine)
					fl_line(model->ReportConfig.X + model->ReportConfig.OffX + model->ReportConfig.TableBorderStyle.Weight / 2,
						model->ReportConfig.Y + model->ReportConfig.OffY + model->ReportConfig.TitleHeight + model->ReportConfig.EachColTitleHeight,
						model->ReportConfig.X + model->ReportConfig.OffX + model->ReportConfig.Width - model->ReportConfig.TableBorderStyle.Weight - Fl::scrollbar_size(),
						model->ReportConfig.Y + model->ReportConfig.OffY + model->ReportConfig.TitleHeight + model->ReportConfig.EachColTitleHeight);


				GraphicDrawHandle::PushClip(model->ReportConfig.X + model->ReportConfig.OffX + model->ReportConfig.TableBorderStyle.Weight / 2,
					model->ReportConfig.Y + model->ReportConfig.OffY + model->ReportConfig.TitleHeight,
					model->ReportConfig.Width - Fl::scrollbar_size() - model->ReportConfig.TableBorderStyle.Weight,
					model->ReportConfig.Height - model->ReportConfig.TitleHeight);
				if (model->ReportConfig.IsRowDividLine)		//ʹ���зָ���
				{
					for (int rowY = gridstarty_ + col_header_height(); rowY < model->ReportConfig.Height;)
					{
						fl_line(model->ReportConfig.X + model->ReportConfig.OffX, 
							model->ReportConfig.Y + model->ReportConfig.OffY + rowY,
							model->ReportConfig.X + model->ReportConfig.OffX + model->ReportConfig.Width,
							model->ReportConfig.Y + model->ReportConfig.OffY + rowY);
						rowY += row_height(0);
					}
				}
				if (model->ReportConfig.IsColDividLine)		//ʹ���зָ���
				{
					for (int rowX = 0,i = 0; i < cols() - 1;i ++)
					{
						rowX += col_width(i);
						fl_line(model->ReportConfig.X + model->ReportConfig.OffX + rowX,
							model->ReportConfig.Y + model->ReportConfig.OffY,
							model->ReportConfig.X + model->ReportConfig.OffX + rowX,
							model->ReportConfig.Y + model->ReportConfig.OffY + model->ReportConfig.Height);
					}
				}
				fl_pop_clip();

				if (model->ReportConfig.IsUseBorder)			//ʹ����߿�
				{
					fl_color(active() ? fl_rgb_color(RGBColor(model->ReportConfig.TableBorderStyle.Color)) 
						: fl_inactive(fl_rgb_color(RGBColor(model->ReportConfig.TableBorderStyle.Color))));
					fl_line_style(FL_CAP_SQUARE | FL_JOIN_MITER | model->ReportConfig.TableBorderStyle.Type, model->ReportConfig.TableBorderStyle.Weight);

					fl_begin_line();
					fl_vertex((double)model->ReportConfig.X + (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffX,
						(double)model->ReportConfig.Y + (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffY);
					fl_vertex((double)model->ReportConfig.X - (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffX + (double)model->ReportConfig.Width,
						(double)model->ReportConfig.Y + (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffY);
					fl_end_line();

					fl_begin_line();
					fl_vertex((double)model->ReportConfig.X + (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffX,
						(double)model->ReportConfig.Y - (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffY + (double)model->ReportConfig.Height);
					fl_vertex((double)model->ReportConfig.X - (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffX + (double)model->ReportConfig.Width,
						(double)model->ReportConfig.Y - (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffY + (double)model->ReportConfig.Height);
					fl_end_line();

					fl_begin_line();
					fl_vertex((double)model->ReportConfig.X + (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffX,
						(double)model->ReportConfig.Y + (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffY);
					fl_vertex((double)model->ReportConfig.X + (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffX,
						(double)model->ReportConfig.Y - (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffY + (double)model->ReportConfig.Height);
					fl_end_line();

					fl_begin_line();
					fl_vertex((double)model->ReportConfig.X - (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffX + (double)model->ReportConfig.Width,
						(double)model->ReportConfig.Y + (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffY);
					fl_vertex((double)model->ReportConfig.X - (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffX + (double)model->ReportConfig.Width,
						(double)model->ReportConfig.Y - (double)model->ReportConfig.TableBorderStyle.Weight / 2.0 + (double)model->ReportConfig.OffY + (double)model->ReportConfig.Height);
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

