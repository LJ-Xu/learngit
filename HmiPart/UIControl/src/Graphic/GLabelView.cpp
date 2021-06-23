#include "stdafx.h"
#include "UIComm.h"
#include "GLabelView.h"
#include "GLabelControl.h"
#include "GLabelModel.h"
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
namespace UI
{
	GLabelView::GLabelView(int X, int Y, int W, int H, const char *L) : HMIBaseLabel(X, Y, W, H, L) {}
	GLabelView::~GLabelView() {}

	void DrawLabel(GLabelControl* ctrl, GLabelModel* model)
	{


	}

	void GLabelView::draw()
	{

		shared_ptr<GLabelControl> ctrl = BaseView.GetControl<GLabelControl>();
		shared_ptr<GLabelModel> model = BaseView.GetModel<GLabelModel>();
		fl_register_images();
		std::string s = model->m_config.image.path;
		Fl_Shared_Image *jpg = Fl_Shared_Image::get(s.c_str(), w(), h());
		align(FL_ALIGN_IMAGE_BACKDROP);
		image(jpg);

		// HMIBaseLabel::draw();

		// 

		 //fl_font(model->m_config.font.GetIndex(), model->m_config.font.size);
		fl_font(1, model->m_config.font.Size);
		//fl_color(model->m_config.font.color);
		color(model->m_config.backColor);
		draw_box();
		fl_draw(label(), x(), y(), w(), h(), align());
		//fl_draw(label(), x()+10, y(), w(), h(), align());
	}
}
