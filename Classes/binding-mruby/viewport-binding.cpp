#include "viewport.h"
#include "disposable-binding.h"
#include "binding-util.h"
#include "binding-types.h"

DEF_TYPE(Viewport);

MRB_METHOD(viewportInitialize)
{
	Viewport *v;

	if (mrb->c->ci->argc == 1)
	{
		/* The rect arg is only used to init the viewport,
		 * and does NOT replace its 'rect' property */
		mrb_value rectObj;
		Rect *rect;

		mrb_get_args(mrb, "o", &rectObj);

		rect = getPrivateDataCheck<Rect>(mrb, rectObj, RectType);

		v = new Viewport(rect);
	}
	else
	{
		mrb_int x, y, width, height;

		mrb_get_args(mrb, "iiii", &x, &y, &width, &height);

		v = new Viewport(x, y, width, height);
	}

	setPrivateData(mrb, self, v, ViewportType);

	/* Wrap property objects */
	v->setRect(new Rect(*v->getRect()));
	v->setColor(new Color);
	v->setTone(new Tone);

	wrapProperty(mrb, self, v->getRect(),  CSrect,  RectType);
	wrapProperty(mrb, self, v->getColor(), CScolor, ColorType);
	wrapProperty(mrb, self, v->getTone(),  CStone,  ToneType);

	return self;
}

MRB_METHOD(viewportUpdate)
{
	Viewport *s = getPrivateData<Viewport>(mrb, self);
	s->update();
	return mrb_nil_value();
}

#define DISP_CLASS_NAME "viewport"

DEF_PROP_OBJ(Viewport, Rect, Rect, CSrect)
DEF_PROP_OBJ(Viewport, Color, Color, CScolor)
DEF_PROP_OBJ(Viewport, Tone, Tone, CStone)

DEF_PROP_I(Viewport, OX)
DEF_PROP_I(Viewport, OY)
DEF_PROP_I(Viewport, Z)


void viewportBindingInit(mrb_state *mrb)
{
	RClass *klass = mrb_define_class(mrb, "Viewport", mrb_class_get(mrb,"Object"));

	disposableBindingInit  <Viewport>(mrb, klass);

	mrb_define_method(mrb, klass, "initialize", viewportInitialize, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(3));

	INIT_PROP_BIND( Viewport, Rect,  "rect"  );
	INIT_PROP_BIND( Viewport, OX,    "ox"    );
	INIT_PROP_BIND( Viewport, OY,    "oy"    );
	INIT_PROP_BIND( Viewport, Z,    "z"    );
	INIT_PROP_BIND( Viewport, Color, "color" );
	INIT_PROP_BIND( Viewport, Tone,  "tone"  );


	mrb_define_method(mrb, klass, "inspect", inspectObject, MRB_ARGS_NONE());
	mrb_define_method(mrb, klass, "update", viewportUpdate, MRB_ARGS_NONE());
}
