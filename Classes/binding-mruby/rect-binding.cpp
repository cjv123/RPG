#include "binding-util.h"

static mrb_int rect_prop_x=0;
static mrb_int rect_prop_y=0;
static mrb_int rect_prop_width=0;
static mrb_int rect_prop_height=0;

MRB_METHOD(rectInitialize)
{
	mrb_int x,y,width,height;
	mrb_get_args(mrb, "iiii", &x,&y,&width,&height);
	rect_prop_x = x;
	rect_prop_y = y;
	rect_prop_width = width;
	rect_prop_height = height;

	return self;
}

MRB_METHOD(rectX)
{
	return mrb_fixnum_value(rect_prop_x);
}

MRB_METHOD(rectY)
{
	return mrb_fixnum_value(rect_prop_y);
}

MRB_METHOD(rectWidth)
{
	return mrb_fixnum_value(rect_prop_width);
}

MRB_METHOD(rectHeight)
{
	return mrb_fixnum_value(rect_prop_height);
}


void rectBindingInit(mrb_state* mrb)
{
	RClass *klass = mrb_define_class(mrb, "Rect", mrb_class_get(mrb,"Object"));

	mrb_define_method(mrb, klass, "initialize",  rectInitialize, MRB_ARGS_OPT(4));
	mrb_define_method(mrb, klass, "x", rectX,  MRB_ARGS_NONE());
	mrb_define_method(mrb, klass, "y", rectY, MRB_ARGS_NONE());
	mrb_define_method(mrb, klass, "width",rectWidth, MRB_ARGS_NONE());
	mrb_define_method(mrb, klass, "height", rectHeight, MRB_ARGS_NONE());
}

