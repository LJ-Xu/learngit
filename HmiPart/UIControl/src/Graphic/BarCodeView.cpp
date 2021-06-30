#include "stdafx.h"
#include "UIComm.h"
#include "BarCodeView.h"
#include "BarCodeControl.h"
#include "BarCodeModel.h"
#include "ResourceService.h"
#include "StringUtility.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Output.H>
#include "UIData.h"


namespace UI
{

	BarCodeView::BarCodeView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H), PtrBarImage(nullptr)
	{
		BuffValue[0] = 48;
		BuffLen = QRBUFFLEN;
		memset(BuffValue, 0, BuffLen);
	}


	BarCodeView::~BarCodeView()
	{
	}

	void BarCodeView::draw()
	{
		shared_ptr<BarCodeControl> ctrl = BaseView.GetControl<BarCodeControl>();
		shared_ptr<BarCodeModel> model = BaseView.GetModel<BarCodeModel>();
		FinX = model->BarcodeConfig.X + model->BarcodeConfig.OffX;
		FinY = model->BarcodeConfig.Y + model->BarcodeConfig.OffY;
		DrawStickChart(ctrl.get(), model.get());
	}

	void BarCodeView::DrawStickChart(BarCodeControl* ctrl, BarCodeModel* model)
	{
		int drawWidth=model->BarcodeConfig.Width, drawHeight = model->BarcodeConfig.Height;
		struct zint_symbol * symbol;
		symbol = ZBarcode_Create();
		//error
		if (symbol == nullptr)
			return;
		switch (model->BarcodeConfig.Dimension)
		{
			//一维码
		case 0:
			switch (model->BarcodeConfig.OneDimensionType)
			{
			/*case BarCodeConfig::EAN13:
				symbol->symbology = BARCODE_EAN14;
				break;
			case BarCodeConfig::Code39:
				symbol->symbology = BARCODE_CODE39;
				break;*/
			case BarCodeConfig::Code128:
				//drawHeight = -1;
				symbol->symbology = BARCODE_CODE128B;
				break;
			}
			break;
			//二维码
		case 1:
			switch (model->BarcodeConfig.TwoDimensionType)
			{
			case BarCodeConfig::QRCode:
				symbol->symbology = BARCODE_QRCODE;
				symbol->option_1 = model->BarcodeConfig.CorrectErrorLevel; //容错级别
				break;
			case BarCodeConfig::DataMatrix:
				symbol->symbology = BARCODE_DATAMATRIX;
				break;
			case BarCodeConfig::PDF417:
				symbol->symbology = BARCODE_PDF417;
				break;
			}
			break;
		}
		symbol->scale = 2;
		//symbol->option_2 = 10; //版本，决定图片大小
		//symbol->output_options = 0;
		symbol->show_hrt = 0; //可显示信息，如果设置为1，则需要设置text值

		//symbol->input_mode = DATA_MODE;
		int ret = -1;
		if (model->BarcodeConfig.UseConstValue)
		{
			//strcpy(BuffValue, model->BarcodeConfig.ConstString.c_str());
			memcpy(BuffValue, model->BarcodeConfig.ConstString.c_str(), BuffLen);
		}
		//ret = ZBarcode_Encode_and_Buffer(symbol, (unsigned char *)BuffValue, QRBUFFLEN,0); //编码
		ret = ZBarcode_Encode_and_Buffer(symbol, (unsigned char*)BuffValue, BuffLen, 0); //编码
		if (ret != 0)
		{
			//error
			return;
		}
		drawqrcode(model, symbol->bitmap, symbol->bitmap_width, symbol->bitmap_height, drawWidth == -1 ? symbol->bitmap_width : drawWidth, drawHeight==-1? symbol->bitmap_height: drawHeight);

		ZBarcode_Delete(symbol);


	}
	void BarCodeView::drawqrcode(BarCodeModel * model,unsigned char* data, int datawidth, int dataheight, int drawwidth, int drawheight)
	{
		int w = min(model->BarcodeConfig.Width, model->BarcodeConfig.Height);
		Fl_RGB_Image * tempImg= new Fl_RGB_Image(data, datawidth, dataheight);
		if (PtrBarImage != nullptr)
		{
			delete PtrBarImage;
			PtrBarImage = nullptr;
		}
		PtrBarImage = tempImg->copy(drawwidth, drawheight);
		PtrBarImage->draw(FinX, FinY);
		delete tempImg;
		delete PtrBarImage;
		PtrBarImage = nullptr;
		
	}
}

