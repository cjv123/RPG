#include "sprite.h"
#include "disposable-binding.h"
#include "binding-util.h"
#include "etc.h"
#include "binding-types.h"

DEF_TYPE(Sprite);

MRB_METHOD(spriteInitialize)
{
	Sprite *s = viewportElementInitialize<Sprite>(mrb, self);

	setPrivateData(mrb, self, s, SpriteType);

	/* Wrap property objects */
	s->setSrcRect(new Rect);
	s->setColor(new Color);
	s->setTone(new Tone);

	wrapNilProperty(mrb, self, CSbitmap);
	wrapProperty(mrb, self, s->getSrcRect(), CSsrc_rect, RectType);
	wrapProperty(mrb, self, s->getColor(), CScolor, ColorType);
	wrapProperty(mrb, self, s->getTone(), CStone, ToneType);

	return self;
}

MRB_METHOD(flashableFlash)
{
	Sprite *s = getPrivateData<Sprite>(mrb, self);

	mrb_value colorObj;
	Color *color;
	int duration;

	mrb_get_args(mrb, "oi", &colorObj,&duration);
	color = getPrivateDataCheck<Color>(mrb, colorObj, ColorType);

	GUARD_EXC( s->flash(color,duration); )

	return mrb_nil_value();
}

MRB_METHOD(flashableUpdate)
{
	Sprite *s = getPrivateData<Sprite>(mrb, self);
	GUARD_EXC( s->update(); )
	return mrb_nil_value();
}

#define DISP_CLASS_NAME "sprite"

DEF_PROP_OBJ_NIL(Sprite, Bitmap, Bitmap,  CSbitmap)
DEF_PROP_OBJ(Sprite, Rect,   SrcRect, CSsrc_rect)
DEF_PROP_OBJ(Sprite, Color,  Color,   CScolor)
DEF_PROP_OBJ(Sprite, Tone,   Tone,    CStone)
DEF_PROP_OBJ(Sprite,Viewport,Viewport,CSviewport)

DEF_PROP_I(Sprite, X)
DEF_PROP_I(Sprite, Y)
DEF_PROP_I(Sprite, Z)
DEF_PROP_I(Sprite, OX)
DEF_PROP_I(Sprite, OY)
DEF_PROP_I(Sprite, BushDepth)
DEF_PROP_I(Sprite, Opacity)
DEF_PROP_I(Sprite, BlendType)

DEF_PROP_F(Sprite, ZoomX)
DEF_PROP_F(Sprite, ZoomY)
DEF_PROP_F(Sprite, Angle)

DEF_PROP_B(Sprite, Mirror)
DEF_PROP_B(Sprite,Visible)

void spriteBindingInit(mrb_state *mrb)
{
	RClass *klass = mrb_define_class(mrb, "Sprite", mrb_class_get(mrb,"Object"));

	disposableBindingInit  <Sprite>(mrb, klass);

	mrb_define_method(mrb, klass, "initialize", spriteInitialize, MRB_ARGS_OPT(1));

	INIT_PROP_BIND( Sprite, Bitmap,    "bitmap"     );
	INIT_PROP_BIND( Sprite, SrcRect,   "src_rect"   );
	INIT_PROP_BIND( Sprite, X,         "x"          );
	INIT_PROP_BIND( Sprite, Y,         "y"          );
	INIT_PROP_BIND( Sprite, Z,         "z"          );
	INIT_PROP_BIND( Sprite, OX,        "ox"         );
	INIT_PROP_BIND( Sprite, OY,        "oy"         );
	INIT_PROP_BIND( Sprite, ZoomX,     "zoom_x"     );
	INIT_PROP_BIND( Sprite, ZoomY,     "zoom_y"     );
	INIT_PROP_BIND( Sprite, Angle,     "angle"      );
	INIT_PROP_BIND( Sprite, Mirror,    "mirror"     );
	INIT_PROP_BIND( Sprite, BushDepth, "bush_depth" );
	INIT_PROP_BIND( Sprite, Opacity,   "opacity"    );
	INIT_PROP_BIND( Sprite, BlendType, "blend_type" );
	INIT_PROP_BIND( Sprite, Color,     "color"      );
	INIT_PROP_BIND( Sprite, Tone,      "tone"       );
	INIT_PROP_BIND(Sprite,Viewport,"viewport");
	INIT_PROP_BIND(Sprite,Visible,"visible");
	
	mrb_define_method(mrb, klass, "flash", flashableFlash, MRB_ARGS_REQ(2));
	mrb_define_method(mrb, klass, "update", flashableUpdate, MRB_ARGS_NONE());

	mrb_define_method(mrb, klass, "inspect", inspectObject, MRB_ARGS_NONE());
}
