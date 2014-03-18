#include "window.h"
#include "disposable-binding.h"
#include "binding-util.h"
#include "binding-types.h"

DEF_TYPE(Window);

MRB_METHOD(windowInitialize)
{
	Window *w = viewportElementInitialize<Window>(mrb, self);

	setPrivateData(mrb, self, w, WindowType);

	w->setCursorRect(new Rect);
	wrapNilProperty(mrb, self, CSwindowskin);
	wrapNilProperty(mrb, self, CScontents);
	wrapProperty(mrb, self, w->getCursorRect(), CScursor_rect, RectType);

	return self;
}

MRB_METHOD(windowUpdate)
{
	Window *w = getPrivateData<Window>(mrb, self);

	w->update();

	return mrb_nil_value();
}

#define DISP_CLASS_NAME "window"

DEF_PROP_OBJ_NIL(Window, Bitmap, Windowskin, CSwindowskin)
DEF_PROP_OBJ_NIL(Window, Bitmap, Contents,   CScontents)
DEF_PROP_OBJ(Window, Rect, CursorRect, CScursor_rect)

DEF_PROP_B(Window, Stretch)
DEF_PROP_B(Window, Active)
DEF_PROP_B(Window, Pause)
DEF_PROP_B(Window, Visible)

DEF_PROP_I(Window, X)
DEF_PROP_I(Window, Y)
DEF_PROP_I(Window, Width)
DEF_PROP_I(Window, Height)
DEF_PROP_I(Window, OX)
DEF_PROP_I(Window, OY)
DEF_PROP_I(Window, Opacity)
DEF_PROP_I(Window, BackOpacity)
DEF_PROP_I(Window, ContentsOpacity)
DEF_PROP_I(Window, Z)


void windowBindingInit(mrb_state *mrb)
{
	RClass *klass = mrb_define_class(mrb, "Window", mrb_class_get(mrb,"Object"));

	disposableBindingInit     <Window>(mrb, klass);

	mrb_define_method(mrb, klass, "initialize", windowInitialize, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, klass, "update",     windowUpdate,     MRB_ARGS_NONE());

	INIT_PROP_BIND( Window, Windowskin,      "windowskin"       );
	INIT_PROP_BIND( Window, Contents,        "contents"         );
	INIT_PROP_BIND( Window, Stretch,         "stretch"          );
	INIT_PROP_BIND( Window, CursorRect,      "cursor_rect"      );
	INIT_PROP_BIND( Window, Active,          "active"           );
	INIT_PROP_BIND( Window, Pause,           "pause"            );
	INIT_PROP_BIND( Window, X,               "x"                );
	INIT_PROP_BIND( Window, Y,               "y"                );
	INIT_PROP_BIND( Window, Width,           "width"            );
	INIT_PROP_BIND( Window, Height,          "height"           );
	INIT_PROP_BIND( Window, OX,              "ox"               );
	INIT_PROP_BIND( Window, OY,              "oy"               );
	INIT_PROP_BIND( Window, Opacity,         "opacity"          );
	INIT_PROP_BIND( Window, BackOpacity,     "back_opacity"     );
	INIT_PROP_BIND( Window, ContentsOpacity, "contents_opacity" );
	INIT_PROP_BIND( Window, Z, "z" );
	INIT_PROP_BIND( Window, Visible ,"visible");

	mrb_define_method(mrb, klass, "inspect", inspectObject, MRB_ARGS_NONE());
}
