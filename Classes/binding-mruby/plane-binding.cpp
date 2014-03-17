#include "plane.h"
#include "disposable-binding.h"
#include "binding-util.h"
#include "binding-types.h"

DEF_TYPE(Plane);

MRB_METHOD(planeInitialize)
{
	Plane *p = viewportElementInitialize<Plane>(mrb, self);

	setPrivateData(mrb, self, p, PlaneType);

	p->setColor(new Color);
	p->setTone(new Tone);

	wrapNilProperty(mrb, self, CSbitmap);
	wrapProperty(mrb, self, p->getColor(), CScolor, ColorType);
	wrapProperty(mrb, self, p->getTone(), CStone, ToneType);

	return self;
}

#define DISP_CLASS_NAME "plane"

DEF_PROP_OBJ(Plane, Bitmap, Bitmap, CSbitmap)
DEF_PROP_OBJ(Plane, Color, Color, CScolor)
DEF_PROP_OBJ(Plane, Tone, Tone, CStone)

DEF_PROP_I(Plane, OX)
DEF_PROP_I(Plane, OY)
DEF_PROP_I(Plane, Z)
DEF_PROP_I(Plane, Opacity)
DEF_PROP_I(Plane, BlendType)

DEF_PROP_F(Plane, ZoomX)
DEF_PROP_F(Plane, ZoomY)


void planeBindingInit(mrb_state *mrb)
{
	RClass *klass = mrb_define_class(mrb, "Plane", mrb_class_get(mrb,"Object"));

	disposableBindingInit<Plane>     (mrb, klass);
	mrb_define_method(mrb, klass, "initialize", planeInitialize, MRB_ARGS_OPT(1));

	INIT_PROP_BIND( Plane, Bitmap,    "bitmap"     );
	INIT_PROP_BIND( Plane, OX,        "ox"         );
	INIT_PROP_BIND( Plane, OY,        "oy"         );
	INIT_PROP_BIND( Plane, Z,        "z"         );
	INIT_PROP_BIND( Plane, ZoomX,     "zoom_x"     );
	INIT_PROP_BIND( Plane, ZoomY,     "zoom_y"     );
	INIT_PROP_BIND( Plane, Opacity,   "opacity"    );
	INIT_PROP_BIND( Plane, BlendType, "blend_type" );
	INIT_PROP_BIND( Plane, Color,     "color"      );
	INIT_PROP_BIND( Plane, Tone,      "tone"       );

	mrb_define_method(mrb, klass, "inspect", inspectObject, MRB_ARGS_NONE());
}
