#include "exception.h"
#include "binding-util.h"
#include "input.h"

MRB_FUNCTION(inputUpdate)
{
	MRB_FUN_UNUSED_PARAM;

	Input::getInstance()->update();
	return mrb_nil_value();
}

MRB_FUNCTION(inputPress)
{
	mrb_int num;
	mrb_get_args(mrb, "i", &num);
	bool ret = Input::getInstance()->isPressed(num);

	return mrb_bool_value(ret);
}

MRB_FUNCTION(inputTrigger)
{
	mrb_int num;
	mrb_get_args(mrb, "i", &num);
	bool ret =Input::getInstance()->isTriggered(num);

	return mrb_bool_value(ret);
}

MRB_FUNCTION(inputRepeat)
{
	mrb_int num;
	mrb_get_args(mrb, "i", &num);
	bool ret =Input::getInstance()->isRepeated(num);

	return mrb_bool_value(ret);
}

MRB_FUNCTION(inputDir4)
{
	MRB_FUN_UNUSED_PARAM
	mrb_int ret =Input::getInstance()->dir4Value();

	return mrb_fixnum_value(ret);
}

MRB_FUNCTION(inputDir8)
{
	MRB_FUN_UNUSED_PARAM
	mrb_int ret =Input::getInstance()->dir8Value();

	return mrb_fixnum_value(ret);
}

/* Non-standard extensions */
MRB_FUNCTION(inputMouseX)
{
	MRB_FUN_UNUSED_PARAM;

	return mrb_nil_value();
}

MRB_FUNCTION(inputMouseY)
{
	MRB_FUN_UNUSED_PARAM;

	return mrb_nil_value();
}

#define DEF_CONST_I(name, value) \
	mrb_const_set(mrb, mrb_obj_value(module), mrb_intern_lit(mrb, name), mrb_fixnum_value(value))

void inputBindingInit(mrb_state *mrb)
{
	RClass *module = mrb_define_module(mrb, "Input");

	mrb_define_module_function(mrb, module, "update", inputUpdate, MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module, "press?", inputPress, MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module, "trigger?", inputTrigger, MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module, "repeat?", inputRepeat, MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module, "dir4", inputDir4, MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module, "dir8", inputDir8, MRB_ARGS_NONE());

	DEF_CONST_I("DOWN",  Input::Down );
	DEF_CONST_I("LEFT",  Input::Left );
	DEF_CONST_I("RIGHT", Input::Right);
	DEF_CONST_I("UP",    Input::Up   );

	DEF_CONST_I("A",     Input::A    );
	DEF_CONST_I("B",     Input::B    );
	DEF_CONST_I("C",     Input::C    );
	DEF_CONST_I("X",     Input::X    );
	DEF_CONST_I("Y",     Input::Y    );
	DEF_CONST_I("Z",     Input::Z    );
	DEF_CONST_I("L",     Input::L    );
	DEF_CONST_I("R",     Input::R    );

	DEF_CONST_I("SHIFT", Input::Shift);
	DEF_CONST_I("CTRL",  Input::Ctrl );
	DEF_CONST_I("ALT",   Input::Alt  );

	DEF_CONST_I("F5",    Input::F5   );
	DEF_CONST_I("F6",    Input::F6   );
	DEF_CONST_I("F7",    Input::F7   );
	DEF_CONST_I("F8",    Input::F8   );
	DEF_CONST_I("F9",    Input::F9   );

	mrb_define_module_function(mrb, module, "mouse_x", inputMouseX, MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module, "mouse_y", inputMouseY, MRB_ARGS_NONE());

	DEF_CONST_I("MOUSELEFT",   Input::MouseLeft  );
	DEF_CONST_I("MOUSEMIDDLE", Input::MouseMiddle);
	DEF_CONST_I("MOUSERIGHT",  Input::MouseRight );
}
