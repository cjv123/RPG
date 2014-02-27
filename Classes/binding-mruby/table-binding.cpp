#include "table.h"
#include "binding-util.h"
#include "binding-types.h"
#include "serializable-binding.h"

DEF_TYPE(Table);

MRB_METHOD(tableInitialize)
{
	mrb_int x, y, z;
	x = y = z = 1;

	mrb_get_args(mrb, "i|ii", &x, &y, &z);

	Table *t = new Table(x, y, z);

	setPrivateData(mrb, self, t, TableType);

	return self;
}

#define TABLE_SIZE(d, D) \
	MRB_METHOD(table##D##Size) \
	{ \
		Table *t = getPrivateData<Table>(mrb, self); \
		return mrb_fixnum_value((mrb_int)t->d##Size()); \
	}

TABLE_SIZE(x, X)
TABLE_SIZE(y, Y)
TABLE_SIZE(z, Z)

MRB_METHOD(tableResize)
{
	Table *t = getPrivateData<Table>(mrb, self);

	mrb_int x, y, z;
	mrb_get_args(mrb, "i|ii", &x, &y, &z);

	switch (mrb->c->ci->argc)
	{
	default:
	case 1:
		t->resize(x);
		return mrb_nil_value();

	case 2:
		t->resize(x, y);
		return mrb_nil_value();

	case 3:
		t->resize(x, y, z);
		return mrb_nil_value();
	}
}

MRB_METHOD(tableGetAt)
{
	Table *t = getPrivateData<Table>(mrb, self);

	mrb_int x, y, z;
	x = y = z = 0;

	mrb_get_args(mrb, "i|ii", &x, &y, &z);

	if (x < 0 || x >= t->xSize()
	||  y < 0 || y >= t->ySize()
	||  z < 0 || z >= t->zSize())
	{
		return mrb_nil_value();
	}

	mrb_int result = t->get(x, y, z);

	return mrb_fixnum_value(result);
}

#define fix(x) mrb_fixnum(x)

MRB_METHOD(tableSetAt)
{
	Table *t = getPrivateData<Table>(mrb, self);

	int argc;
	mrb_value* argv;
	mrb_int x, y, z, value;
	x = y = z = 0;

	mrb_get_args(mrb, "*", &argv, &argc);

	if (argc < 2)
		mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments");

	switch (argc)
	{
	default:
	case 2 :
		x = fix(argv[0]);
		value = fix(argv[1]);

		break;
	case 3 :
		x = fix(argv[0]);
		y = fix(argv[1]);
		value = fix(argv[2]);

		break;
	case 4 :
		x = fix(argv[0]);
		y = fix(argv[1]);
		z = fix(argv[2]);
		value = fix(argv[3]);

		break;
	}

	t->set(value, x, y, z);

	return mrb_fixnum_value(value);
}

MARSH_LOAD_FUN(Table)

void tableBindingInit(mrb_state *mrb)
{
	RClass *klass = mrb_define_class(mrb, "Table", mrb_class_get(mrb,"Object"));

	mrb_define_class_method(mrb, klass, "_load", TableLoad, MRB_ARGS_REQ(1));
	serializableBindingInit<Table>(mrb, klass);

	mrb_define_method(mrb, klass, "initialize", tableInitialize, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(2));
	mrb_define_method(mrb, klass, "resize",     tableResize,     MRB_ARGS_REQ(1) | MRB_ARGS_OPT(2));
	mrb_define_method(mrb, klass, "xsize",      tableXSize,      MRB_ARGS_NONE()                  );
	mrb_define_method(mrb, klass, "ysize",      tableYSize,      MRB_ARGS_NONE()                  );
	mrb_define_method(mrb, klass, "zsize",      tableZSize,      MRB_ARGS_NONE()                  );
	mrb_define_method(mrb, klass, "[]",         tableGetAt,      MRB_ARGS_REQ(1) | MRB_ARGS_OPT(2));
	mrb_define_method(mrb, klass, "[]=",        tableSetAt,      MRB_ARGS_REQ(2) | MRB_ARGS_OPT(2));

	mrb_define_method(mrb, klass, "inspect", inspectObject, MRB_ARGS_NONE());
}
