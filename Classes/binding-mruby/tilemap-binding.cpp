#include "tilemap.h"
#include "viewport.h"
#include "bitmap.h"
#include "table.h"

#include "disposable-binding.h"
#include "binding-util.h"
#include "binding-types.h"

#include <mruby/array.h>

static const mrb_data_type TilemapAutotilesType =
{
    "TilemapAutotiles",
	0
};

MRB_METHOD(tilemapAutotilesSet)
{
	Tilemap::Autotiles *a = getPrivateData<Tilemap::Autotiles>(mrb, self);

	mrb_int i;
	mrb_value bitmapObj;

	mrb_get_args(mrb, "io", &i, &bitmapObj); 

	Bitmap *bitmap = getPrivateDataCheck<Bitmap>(mrb, bitmapObj, BitmapType);

	a->set(i, bitmap);

	mrb_value ary = mrb_iv_get(mrb, self, getMrbData(mrb)->symbols[CSarray]);
	mrb_ary_set(mrb, ary, i, bitmapObj);

	return self;
}

MRB_METHOD(tilemapAutotilesGet)
{
	mrb_int i;

	mrb_get_args (mrb, "i", &i);

	if (i < 0 || i > 6)
		return mrb_nil_value();

	mrb_value ary = mrb_iv_get(mrb, self, getMrbData(mrb)->symbols[CSarray]);

	return mrb_ary_entry(ary, i);
}

DEF_TYPE(Tilemap);

MRB_METHOD(tilemapInitialize)
{
	Tilemap *t;

	/* Get parameters */
	mrb_value viewportObj = mrb_nil_value();
	Viewport *viewport = 0;

	mrb_get_args(mrb, "|o", &viewportObj);

	if (!mrb_nil_p(viewportObj))
		viewport = getPrivateDataCheck<Viewport>(mrb, viewportObj, ViewportType);

	/* Construct object */
	t = new Tilemap(viewport);

	setPrivateData(mrb, self, t, TilemapType);

	setProperty(mrb, self, CSviewport, viewportObj);

	wrapProperty(mrb, self, &t->getAutotiles(), CSautotiles, TilemapAutotilesType);

	mrb_value autotilesObj = mrb_iv_get(mrb, self, getMrbData(mrb)->symbols[CSautotiles]);

	mrb_value ary = mrb_ary_new_capa(mrb, 7);
	for (int i = 0; i < 7; ++i)
		mrb_ary_push(mrb, ary, mrb_nil_value());

	mrb_iv_set(mrb, autotilesObj, getMrbData(mrb)->symbols[CSarray], ary);

	return self;
}

MRB_METHOD(tilemapGetAutotiles)
{
	return getProperty(mrb, self, CSautotiles);
}

MRB_METHOD(tilemapUpdate)
{
	Tilemap *t = getPrivateData<Tilemap>(mrb, self);

	t->update();

	return mrb_nil_value();
}

#define DISP_CLASS_NAME "tilemap"

MRB_METHOD(tilemapGetViewport)
{
	Tilemap *t = getPrivateData<Tilemap>(mrb, self);

	checkDisposed(mrb, t, DISP_CLASS_NAME);

	return getProperty(mrb, self, CSviewport);
}

DEF_PROP_OBJ(Tilemap, Bitmap,   Tileset,    CStileset)
DEF_PROP_OBJ(Tilemap, Table,    MapData,    CSmap_data)
DEF_PROP_OBJ(Tilemap, Table,    FlashData,  CSflash_data)
DEF_PROP_OBJ(Tilemap, Table,    Priorities, CSpriorities)

DEF_PROP_B(Tilemap, Visible)

DEF_PROP_I(Tilemap, OX)
DEF_PROP_I(Tilemap, OY)

void tilemapBindingInit(mrb_state *mrb)
{
	RClass *klass = mrb_define_class(mrb, "TilemapAutotiles", mrb_class_get(mrb,"Object"));

	mrb_define_method(mrb, klass, "[]=", tilemapAutotilesSet, MRB_ARGS_REQ(2));
	mrb_define_method(mrb, klass, "[]", tilemapAutotilesGet, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, klass, "inspect", inspectObject, MRB_ARGS_NONE());

	klass = mrb_define_class(mrb, "Tilemap", mrb_class_get(mrb,"Object"));

	disposableBindingInit<Tilemap>(mrb, klass);

	mrb_define_method(mrb, klass, "initialize", tilemapInitialize, MRB_ARGS_OPT(1));
	mrb_define_method(mrb, klass, "autotiles", tilemapGetAutotiles, MRB_ARGS_NONE());
	mrb_define_method(mrb, klass, "update", tilemapUpdate, MRB_ARGS_NONE());

	mrb_define_method(mrb, klass, "viewport", tilemapGetViewport, MRB_ARGS_NONE());

	INIT_PROP_BIND( Tilemap, Tileset,    "tileset"    );
	INIT_PROP_BIND( Tilemap, MapData,    "map_data"   );
	INIT_PROP_BIND( Tilemap, FlashData,  "flash_data" );
	INIT_PROP_BIND( Tilemap, Priorities, "priorities" );
	INIT_PROP_BIND( Tilemap, Visible,    "visible"    );
	INIT_PROP_BIND( Tilemap, OX,         "ox"         );
	INIT_PROP_BIND( Tilemap, OY,         "oy"         );

	mrb_define_method(mrb, klass, "inspect", inspectObject, MRB_ARGS_NONE());
}
