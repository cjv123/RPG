#include "binding-util.h"

static int color_prop_r = 0;
static int color_prop_g = 0;
static int color_prop_b = 0;
static int color_prop_a = 0;

MRB_METHOD(colorInitialize)
{
	int r,g,b,a;
	
	if (mrb->c->ci->argc == 3)
	{
		mrb_get_args(mrb, "iii", &r,&g,&b);
		a = 255;
	}
	else
	{
		mrb_get_args(mrb, "iiii", &r,&g,&b,&a);
	}
	color_prop_r = r;
	color_prop_g = g;
	color_prop_b = b;
	color_prop_a = a;

	return self;
}

MRB_METHOD(colorSet)
{
	int r,g,b,a;

	if (mrb->c->ci->argc == 3)
	{
		mrb_get_args(mrb, "iii", &r,&g,&b,&a);
		a = 255;
	}
	else
	{
		mrb_get_args(mrb, "iiii", &r,&g,&b,&a);
	}
	color_prop_r = r;
	color_prop_g = g;
	color_prop_b = b;
	color_prop_a = a;
	return mrb_nil_value();
}

MRB_METHOD(colorRed)
{
	return mrb_fixnum_value(color_prop_r);
}

MRB_METHOD(colorGreen)
{
	return mrb_fixnum_value(color_prop_g);
}

MRB_METHOD(colorBlue)
{
	return mrb_fixnum_value(color_prop_b);
}

MRB_METHOD(colorAlpha)
{
	return mrb_fixnum_value(color_prop_a);
}

void colorBindingInit(mrb_state* mrb)
{
	RClass *klass = mrb_define_class(mrb, "Color", mrb_class_get(mrb,"Object"));
	
	mrb_define_method(mrb, klass, "initialize",  colorInitialize, MRB_ARGS_REQ(3)|MRB_ARGS_OPT(4));
	mrb_define_method(mrb, klass, "set", colorSet,  MRB_ARGS_REQ(3)|MRB_ARGS_OPT(4));
	mrb_define_method(mrb, klass, "red", colorRed,  MRB_ARGS_NONE());
	mrb_define_method(mrb, klass, "green", colorGreen, MRB_ARGS_NONE());
	mrb_define_method(mrb, klass, "blue",colorBlue, MRB_ARGS_NONE());
	mrb_define_method(mrb, klass, "alpha", colorAlpha, MRB_ARGS_NONE());
}

