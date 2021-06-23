#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "StringUtility.h"
#include "OperatorChartView.h"
#include "FL/fl_draw.H"
#include "IDataService.h"
#include "DataInfo.h"
#include "DataFormate.h"
#include "CodeFormatUtility.h"
using namespace std::chrono;

namespace UI
{
	OperatorChartView::OperatorChartView(int X, int Y, int W, int H, const char *l = 0) : HMIBaseTable(X, Y, W, H, l)
	{
		box(FL_NO_BOX);
		end();
	}
	OperatorChartView::~OperatorChartView() {}

	void OperatorChartView::InitDraw()
	{
		shared_ptr<OperatorChartControl> ctrl = BaseView.GetControl<OperatorChartControl>();
		shared_ptr<OperatorChartModel> model = BaseView.GetModel<OperatorChartModel>();
		//�����Ƿ�ʹ���С��б���
		if (!model->OperatorConfig.UsedTitle)
			model->OperatorConfig.TitleHeight = 0;
		col_header(1);
		row_header(0);
		col_header_height(model->OperatorConfig.TitleHeight + model->OperatorConfig.RowHeight);
		//�����Ƿ�ɱ���
		row_resize(0);
		col_resize(0);

		//��������
		fontSize_ = model->OperatorConfig.RecordItemStringStyle.Font.Size;
		fontStyle_ = UI::IResourceService::GetFontIdx(model->OperatorConfig.RecordItemStringStyle.Font.Name);

		//��������
		cols(model->OperatorConfig.DisplayItems.size());
		//��������
		if (model->OperatorConfig.RowHeight != 0)
			rows((model->OperatorConfig.Height - model->OperatorConfig.TitleHeight - model->OperatorConfig.RowHeight) / model->OperatorConfig.RowHeight + 1);
		else
			rows(0);
		//�����и�
		if (model->OperatorConfig.RowHeight == 0)
			model->OperatorConfig.RowHeight = 10;
		row_height_all(model->OperatorConfig.RowHeight);
		//�����п�
		if (IResourceService::Ins()->IsRenderMode())
		{
			Fl::scrollbar_size(0);
			vscrollbar->hide();
			hscrollbar->hide();
		}
		else
			Fl::scrollbar_size(16);
		//if (IResourceService::Ins()->IsRenderMode())
		//{
		//	int allcolwidth = 0;
		//	for (unsigned i = 0; i < model->OperatorConfig.DisplayItems.size(); i++)
		//	{
		//		if(allcolwidth > model->OperatorConfig.Width - Fl::scrollbar_size())
		//			col_width(i,0);
		//		else
		//		{
		//			if (allcolwidth + model->OperatorConfig.DisplayItems[i].ColWidth
		//				> model->OperatorConfig.Width - Fl::scrollbar_size())
		//				col_width(i, allcolwidth + model->OperatorConfig.DisplayItems[i].ColWidth
		//					- model->OperatorConfig.Width + Fl::scrollbar_size());
		//			else
		//				col_width(i, allcolwidth + model->OperatorConfig.DisplayItems[i].ColWidth);
		//			allcolwidth += model->OperatorConfig.DisplayItems[i].ColWidth;
		//		}
		//		//if ((i == model->OperatorConfig.DisplayItems.size() - 1) && model->OperatorConfig.Width - Fl::scrollbar_size() - allcolwidth > 0)
		//		//	col_width(i, (model->OperatorConfig.Width - Fl::scrollbar_size() - allcolwidth));
		//		//else
		//		//	col_width(i, model->OperatorConfig.DisplayItems[i].ColWidth);
		//		//allcolwidth += model->OperatorConfig.DisplayItems[i].ColWidth;
		//	}
		//}
		//else
		//{
			int allcolwidth = 0;
			for (unsigned i = 0; i < model->OperatorConfig.DisplayItems.size(); i++)
			{
				if ((i == model->OperatorConfig.DisplayItems.size() - 1) && model->OperatorConfig.Width - Fl::scrollbar_size() - allcolwidth > 0)
					col_width(i, (model->OperatorConfig.Width - Fl::scrollbar_size() - allcolwidth));
				else
					col_width(i, model->OperatorConfig.DisplayItems[i].ColWidth);
				allcolwidth += model->OperatorConfig.DisplayItems[i].ColWidth;
			}
				//col_width(i, model->OperatorConfig.DisplayItems[i].ColWidth);
		//}

		//���ñ�����ɫ
		cell_bgcolor_ = fl_rgb_color(RGBColor(model->OperatorConfig.TableBgColor));
	}

	string OperatorChartView::GetTimeString(DDWORD time, int mode)
	{
		shared_ptr<OperatorChartModel> model = BaseView.GetModel<OperatorChartModel>();
		char displayTime[100];
		string text;
		time_t tmSec = time / 1000;
		int tmMs = (int)(time % 1000);
		struct tm *local;
		local = localtime(&tmSec);
		if (mode)	//ʱ��
		{
			switch (model->OperatorConfig.TimeFormate)
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
			switch (model->OperatorConfig.DateFormate)
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
		//UI::IResourceService::GB2312toUtf8(text);
		return text;
	}

	string OperatorChartView::GetDrawString(string projectname, int row)
	{
		shared_ptr<OperatorChartModel> model = BaseView.GetModel<OperatorChartModel>();
		string text = "";
		if (RecordData.empty() || (size_t)row >= RecordData.size())
			return text;
		string serialString = "���";
		string dateString = "����";
		string timeString = "ʱ��";
		string partString = "Ԫ������";
		string addrString = "��ַ";
		string actionString = "����";
		string describeString = "����";
		string nameString = "�û���";
		string winString = "����";
		if (projectname == serialString)		//��ȡ���
			text = to_string(row + 1);
		else if (projectname == dateString)		//��ȡ����
			text = GetTimeString(RecordData[row].Date, 0);
		else if (projectname == timeString)		//��ȡʱ��
			text = GetTimeString(RecordData[row].Time, 1);
		else if (projectname == partString)		//Ԫ������
			text = RecordData[row].CtrlName;
		else if (projectname == addrString)		//��ַ
			text = RecordData[row].Address;
		else if (projectname == actionString)	//����
			text = RecordData[row].Action;
		else if (projectname == describeString)	//����
			text = RecordData[row].Information;
		else if (projectname == nameString)		//�û���
			text = RecordData[row].UserName;
		else if (projectname == winString)		//�û���
			text = to_string(RecordData[row].Window);
		return text;
	}

	int OperatorChartView::GetRecordData()
	{
		shared_ptr<OperatorChartModel> model = BaseView.GetModel<OperatorChartModel>();
		//��ȡͨ������
		if (DisplayMode)		//��ѯģʽ
		{
			vector<Storage::OperatorRecord>().swap(RecordData);
			if (model->OperatorConfig.SearchMode == Project::OperatorSeekMode::OperatorByTime)
				Storage::OperatorStorageService::Ins()->SelectOperatorRecordsByTime(model->OperatorConfig.SearchTimeStart,
					model->OperatorConfig.SearchTimeEnd, RecordData);
			else if (model->OperatorConfig.SearchMode == Project::OperatorSeekMode::OperatorByDate)
				Storage::OperatorStorageService::Ins()->SelectOperatorRecordsByDate(model->OperatorConfig.SearchDate,
					model->OperatorConfig.SearchDate + (DDWORD)86400000, RecordData);
			else if (model->OperatorConfig.SearchMode == Project::OperatorSeekMode::OperatorByName)
				Storage::OperatorStorageService::Ins()->SelectOperatorRecordsByUser(model->OperatorConfig.SearchKey.c_str(), RecordData);
			if (model->OperatorConfig.SortMode && !RecordData.empty())		//ʱ������
				std::reverse(RecordData.begin(), RecordData.end());
			return RecordData.size();
		}
		else
		{
			/*�������*/
			vector<Storage::OperatorRecord>().swap(RecordData);
			Storage::OperatorStorageService::Ins()->SelectOperatorRecords(RecordData);
			if (model->OperatorConfig.SortMode && !RecordData.empty())
				std::reverse(RecordData.begin(), RecordData.end());
			return RecordData.size();
		}
		return RecordData.size();
	}
	void OperatorChartView::SetRenderData()
	{
		shared_ptr<OperatorChartModel> model = BaseView.GetModel<OperatorChartModel>();
		vector<Storage::OperatorRecord>().swap(RecordData);
#ifdef WIN32
		for (size_t i = 0; i < 3; i++)
		{
			Storage::OperatorRecord record;
			record.ID = i;			// ��¼���
			record.Date = duration_cast<MilliSecs>(system_clock::now().time_since_epoch()).count();
			record.Time = duration_cast<MilliSecs>(system_clock::now().time_since_epoch()).count();		// ����ʱ��
			record.UserName = "User1";	// �û�����
			record.Class = "2";			// ����Ȩ��
			record.Window = 1;			// ��������
			record.CtrlName = "BU" + to_string(i);	// �ؼ�����
			record.Comment = "#####";				// ��������
			record.Action = "Toggle";				// ��������
			record.Address = "�����豸��PSB" + to_string(i);						// ������ַ
			record.Action = "Toggle";
			record.Information = "Bit Set ON->OFF";
			RecordData.push_back(record);
		}
#endif // WIN32
	}

	void OperatorChartView::draw_cell(TableContext context,
		int R, int C, int X, int Y, int W, int H)
	{
		shared_ptr<OperatorChartModel> model = BaseView.GetModel<OperatorChartModel>();
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
				int num = GetRecordData();
				if (num > rows())
					rows(num);
			}
			return;
		}
		case CONTEXT_COL_HEADER:						//���Ʊ��⼰�б���
		{
			/*���Ʊ���*/
			if (model->OperatorConfig.UsedTitle)
			{
				GraphicDrawHandle::PushClip(X, Y, model->OperatorConfig.Width, model->OperatorConfig.TitleHeight);
				{
					fl_font(UI::IResourceService::GetFontIdx(model->OperatorConfig.TitleStringStyle.Font.Name),
						model->OperatorConfig.TitleStringStyle.Font.Size);
					int h = model->OperatorConfig.TitleHeight;
					/*����������ɫ*/
					Fl_Color textcolor = fl_rgb_color(RGBColor(model->OperatorConfig.TitleStringStyle.Colors));
					
						/*���ñ�����ɫ*/
					fl_color(fl_rgb_color(RGBColor(model->OperatorConfig.TitleBgColor)));
					fl_rectf(model->OperatorConfig.X + model->OperatorConfig.OffX,
						model->OperatorConfig.Y + model->OperatorConfig.OffY, w(), h);
					/*��ȡ�����ַ���*/
					string text = StringUtility::GetDrawString(IResourceService::Ins(), model->OperatorConfig.TitleStringRes, 0);
					UI::IResourceService::GB2312toUtf8(text);
					/*�����ı�*/
					fl_color(textcolor);
					fl_draw(text.data(), model->OperatorConfig.X + model->OperatorConfig.OffX,
						model->OperatorConfig.Y + model->OperatorConfig.OffY,
						w(), h, model->OperatorConfig.TitleStringStyle.Align);	//FL_ALIGN_CENTER
				}
				fl_pop_clip();
			}
			/*�����б���*/
			GraphicDrawHandle::PushClip(X, Y + model->OperatorConfig.TitleHeight, W, H - model->OperatorConfig.TitleHeight);
			{
				if (X <= model->OperatorConfig.X + model->OperatorConfig.OffX)
					//&& X <= model->OperatorConfig.X + model->OperatorConfig.OffX + col_width(C))
				{
					gridstartx_ = X;//X - model->OperatorConfig.X - model->OperatorConfig.OffX;
					gridstartcol_ = C;
				}
				if (IResourceService::Ins()->IsRenderMode() || !hscrollbar->visible())
				{
					gridstartx_ = model->OperatorConfig.X + model->OperatorConfig.OffX;
					gridstartcol_ = 0;
				}
				// BG COLOR
					fl_color(cell_bgcolor_);
					fl_rectf(X, Y + model->OperatorConfig.TitleHeight, W, H - model->OperatorConfig.TitleHeight);
				// TEXT
				if (model->OperatorConfig.DisplayItems.size() > (size_t)C)
				{
					fl_font(UI::IResourceService::GetFontIdx(model->OperatorConfig.RecordItemStringStyle.Font.Name),
						model->OperatorConfig.RecordItemStringStyle.Font.Size);
					fl_color(fl_rgb_color(RGBColor(model->OperatorConfig.RecordItemStringStyle.Colors)));

					/*��ȡtext*/
					string text = StringUtility::GetDrawString(IResourceService::Ins(),
						model->OperatorConfig.DisplayItems[C].TitleDescribe, 0);
					UI::IResourceService::GB2312toUtf8(text);
					fl_draw(text.c_str(), X, Y + model->OperatorConfig.TitleHeight, W,
						H - model->OperatorConfig.TitleHeight, model->OperatorConfig.RecordItemStringStyle.Align);
				}
			}
			fl_pop_clip();
			return;
		}
		case CONTEXT_CELL:
		{
			//��ȡ��ʾ�ַ���
			if (model->OperatorConfig.DisplayItems.size() < (size_t)C)
				return;
			text = GetDrawString(model->OperatorConfig.DisplayItems[C].ProjectName, R);
			if (!UI::CodeFormatUtility::IsStrUtf8(text.c_str()))
				UI::IResourceService::GB2312toUtf8(text);

			if (Y > model->OperatorConfig.Y + model->OperatorConfig.OffY + col_header_height()
				&& Y <= model->OperatorConfig.Y + model->OperatorConfig.OffY + col_header_height() + row_height(0))
				gridstarty_ = Y - model->OperatorConfig.Y - model->OperatorConfig.OffY - col_header_height();
			GraphicDrawHandle::PushClip(X, Y, W, H);
			{
				// BG COLOR
				fl_color(cell_bgcolor_);
				fl_rectf(X, Y, W, H);
				// TEXT
				fl_font(fontStyle_, fontSize_);
				fl_color(fl_rgb_color(RGBColor(model->OperatorConfig.RecordItemStringStyle.Colors)));
				fl_draw(text.c_str(), X, Y, W, H, model->OperatorConfig.RecordItemStringStyle.Align);
			}
			fl_pop_clip();
			return;
		}
		case CONTEXT_TABLE:
			fprintf(stderr, "TABLE CONTEXT CALLED\n");
			return;
		case CONTEXT_ENDPAGE:		//���߿�
		{
			fl_color(active() ? fl_rgb_color(RGBColor(model->OperatorConfig.GridBorderStyle.Color))
				: fl_inactive(fl_rgb_color(RGBColor(model->OperatorConfig.GridBorderStyle.Color))));
			fl_line_style(model->OperatorConfig.GridBorderStyle.Type, model->OperatorConfig.GridBorderStyle.Weight);
			//���Ʊ������б߿�
			if (model->OperatorConfig.UsedTitle && model->OperatorConfig.IsRowDividLine)
				fl_line(model->OperatorConfig.X + model->OperatorConfig.OffX + model->OperatorConfig.TableBorderStyle.Weight / 2,
					model->OperatorConfig.Y + model->OperatorConfig.OffY + model->OperatorConfig.TitleHeight,
					model->OperatorConfig.X + model->OperatorConfig.OffX + model->OperatorConfig.Width - model->OperatorConfig.TableBorderStyle.Weight - Fl::scrollbar_size(),
					model->OperatorConfig.Y + model->OperatorConfig.OffY + model->OperatorConfig.TitleHeight);
			//�����б�������б߿�
			if (model->OperatorConfig.IsRowDividLine)
				fl_line(model->OperatorConfig.X + model->OperatorConfig.OffX + model->OperatorConfig.TableBorderStyle.Weight / 2,
					model->OperatorConfig.Y + model->OperatorConfig.OffY + model->OperatorConfig.TitleHeight + model->OperatorConfig.RowHeight,
					model->OperatorConfig.X + model->OperatorConfig.OffX + model->OperatorConfig.Width - model->OperatorConfig.TableBorderStyle.Weight - Fl::scrollbar_size(),
					model->OperatorConfig.Y + model->OperatorConfig.OffY + model->OperatorConfig.TitleHeight + model->OperatorConfig.RowHeight);
			int vscrollsize, hscrollsize;
			vscrollsize = vscrollbar->visible() ? 16 : 0;
			hscrollsize = hscrollbar->visible() ? 16 : 0;
			if (IResourceService::Ins()->IsRenderMode())
				vscrollsize = 0;
			GraphicDrawHandle::PushClip(model->OperatorConfig.X + model->OperatorConfig.OffX + model->OperatorConfig.TableBorderStyle.Weight / 2,
				model->OperatorConfig.Y + model->OperatorConfig.OffY + model->OperatorConfig.TitleHeight,
				model->OperatorConfig.Width - vscrollsize - model->OperatorConfig.TableBorderStyle.Weight,
				model->OperatorConfig.Height - hscrollsize - model->OperatorConfig.TitleHeight);
			if (model->OperatorConfig.IsRowDividLine)		//ʹ���зָ���
			{
				for (int rowY = gridstarty_ + col_header_height(); rowY < model->OperatorConfig.Height;)
				{
					fl_line(model->OperatorConfig.X + model->OperatorConfig.OffX,
						model->OperatorConfig.Y + model->OperatorConfig.OffY + rowY,
						model->OperatorConfig.X + model->OperatorConfig.OffX + model->OperatorConfig.Width,
						model->OperatorConfig.Y + model->OperatorConfig.OffY + rowY);
					rowY += row_height(0);
				}
			}
			if (model->OperatorConfig.IsColDividLine)		//ʹ���зָ���
			{
				for (int rowX = gridstartx_, i = gridstartcol_; i < cols();i++)
				{
					//rowX += col_width(i);
					fl_line(/*model->OperatorConfig.X + model->OperatorConfig.OffX +*/ rowX,
						model->OperatorConfig.Y + model->OperatorConfig.OffY,
						/*model->OperatorConfig.X + model->OperatorConfig.OffX + */rowX,
						model->OperatorConfig.Y + model->OperatorConfig.OffY + model->OperatorConfig.Height);
					rowX += col_width(i);
				}
			}
			fl_pop_clip();

			if (model->OperatorConfig.IsUseBorder)			//ʹ����߿�
			{
				fl_color(active() ? fl_rgb_color(RGBColor(model->OperatorConfig.TableBorderStyle.Color))
					: fl_inactive(fl_rgb_color(RGBColor(model->OperatorConfig.TableBorderStyle.Color))));
				fl_line_style(FL_CAP_SQUARE | FL_JOIN_MITER | model->OperatorConfig.TableBorderStyle.Type, model->OperatorConfig.TableBorderStyle.Weight);

				fl_begin_line();
				fl_vertex((double)model->OperatorConfig.X + (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffX,
					(double)model->OperatorConfig.Y + (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffY);
				fl_vertex((double)model->OperatorConfig.X - (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffX + (double)model->OperatorConfig.Width,
					(double)model->OperatorConfig.Y + (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffY);
				fl_end_line();

				fl_begin_line();
				fl_vertex((double)model->OperatorConfig.X + (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffX,
					(double)model->OperatorConfig.Y - (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffY + (double)model->OperatorConfig.Height);
				fl_vertex((double)model->OperatorConfig.X - (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffX + (double)model->OperatorConfig.Width,
					(double)model->OperatorConfig.Y - (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffY + (double)model->OperatorConfig.Height);
				fl_end_line();

				fl_begin_line();
				fl_vertex((double)model->OperatorConfig.X + (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffX,
					(double)model->OperatorConfig.Y + (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffY);
				fl_vertex((double)model->OperatorConfig.X + (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffX,
					(double)model->OperatorConfig.Y - (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffY + (double)model->OperatorConfig.Height);
				fl_end_line();

				fl_begin_line();
				fl_vertex((double)model->OperatorConfig.X - (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffX + (double)model->OperatorConfig.Width,
					(double)model->OperatorConfig.Y + (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffY);
				fl_vertex((double)model->OperatorConfig.X - (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffX + (double)model->OperatorConfig.Width,
					(double)model->OperatorConfig.Y - (double)model->OperatorConfig.TableBorderStyle.Weight / 2.0 + (double)model->OperatorConfig.OffY + (double)model->OperatorConfig.Height);
				fl_end_line();
			}
			if(IResourceService::Ins()->IsRenderMode())
				GraphicDrawHandle::Ins()->DrawRenderScroll(1, model->OperatorConfig.X + model->OperatorConfig.OffX + model->OperatorConfig.TableBorderStyle.Weight,
					model->OperatorConfig.Y + model->OperatorConfig.Height + model->OperatorConfig.OffY - 16, 
					model->OperatorConfig.Width - model->OperatorConfig.TableBorderStyle.Weight * 2, 16);
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

