#include "binding-util.h"
#include "util.h"
#include "exception.h"

#define SYMD(symbol) { CS##symbol, #symbol }

struct
{
	CommonSymbol ind;
	const char *str;
} static const symData[] =
{
	SYMD(priv_iv),
	SYMD(font),
	SYMD(viewport),
	SYMD(bitmap),
	SYMD(color),
	SYMD(tone),
	SYMD(rect),
	SYMD(src_rect),
	SYMD(tileset),
	SYMD(autotiles),
	SYMD(map_data),
	SYMD(flash_data),
	SYMD(priorities),
	SYMD(windowskin),
	SYMD(contents),
	SYMD(cursor_rect),
	SYMD(path),
	SYMD(array),
	SYMD(default_color)
};

static elementsN(symData);

struct MrbExcData
{
	MrbException ind;
	const char *str;
};

static const MrbExcData excData[] =
{
	{ Shutdown, "SystemExit"  },
	{ RGSS,     "RGSSError"   },
	{ PHYSFS,   "PHYSFSError" },
	{ SDL,      "SDLError"    },
	{ MKXP,     "MKXPError"   },
	{ IO,       "IOError"     }
};

static elementsN(excData);

#define ENO(id) { Errno##id, #id }

static const MrbExcData enoExcData[] =
{
	ENO(E2BIG),
	ENO(EACCES),
	ENO(EAGAIN),
	ENO(EBADF),
	ENO(ECHILD),
	ENO(EDEADLOCK),
	ENO(EDOM),
	ENO(EEXIST),
	ENO(EINVAL),
	ENO(EMFILE),
	ENO(ENOENT),
	ENO(ENOEXEC),
	ENO(ENOMEM),
	ENO(ENOSPC),
	ENO(ERANGE),
	ENO(EXDEV)
};

static elementsN(enoExcData);

MrbData::MrbData(mrb_state *mrb)
{
	int arena = mrb_gc_arena_save(mrb);

	for (int i = 0; i < excDataN; ++i)
		exc[excData[i].ind] = mrb_define_class(mrb, excData[i].str, mrb->eException_class);

	RClass *errnoMod = mrb_define_module(mrb, "Errno");

	for (int i = 0; i < enoExcDataN; ++i)
		exc[enoExcData[i].ind] =
		        mrb_define_class_under(mrb, errnoMod, enoExcData[i].str, mrb->eStandardError_class);

	exc[TypeError] = mrb_class_get(mrb, "TypeError");
	exc[ArgumentError] = mrb_class_get(mrb, "ArgumentError");

	for (int i = 0; i < symDataN; ++i)
		symbols[symData[i].ind] = mrb_intern_cstr(mrb, symData[i].str);

	mrb_gc_arena_restore(mrb, arena);
}

/* Indexed with Exception::Type */
static const MrbException excToMrbExc[] =
{
	RGSS,        /* RGSSError   */
	ErrnoENOENT, /* NoFileError */
	IO,

	TypeError,
	ArgumentError,

	PHYSFS,      /* PHYSFSError */
	SDL,         /* SDLError    */
	MKXP         /* MKXPError   */
};

void raiseMrbExc(mrb_state *mrb, const Exception &exc)
{
	MrbData *data = getMrbData(mrb);
	RClass *excClass = data->exc[excToMrbExc[exc.type]];

	mrb_raise(mrb, excClass, exc.msg.c_str());
}

MRB_METHOD_PUB(inspectObject)
{
	mrb_value priv = mrb_obj_iv_get(mrb, mrb_obj_ptr(self), getSym(mrb, CSpriv_iv));

	static char buffer[64];
	snprintf(buffer, sizeof(buffer), "#<%s:%p>", DATA_TYPE(priv)->struct_name, DATA_PTR(priv));

	return mrb_str_new_cstr(mrb, buffer);
}



