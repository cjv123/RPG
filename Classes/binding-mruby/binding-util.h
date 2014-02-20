#ifndef _BINDING_UTIL
#define _BINDING_UTIL

#include <mruby.h>
#include <mruby/string.h>
#include <mruby/compile.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <mruby/class.h>


enum CommonSymbol
{
	CSpriv_iv = 0, /* private data */

	/* From here on out all symbols
	 * have implicit '@' prefix */
	CSfont,
	CSviewport,
	CSbitmap,
	CScolor,
	CStone,
	CSrect,
	CSsrc_rect,
	CStileset,
	CSautotiles,
	CSmap_data,
	CSflash_data,
	CSpriorities,
	CSwindowskin,
	CScontents,
	CScursor_rect,
	CSpath,
	CSarray,
	CSdefault_color,

	CommonSymbolsMax
};

enum MrbException
{
	Shutdown = 0,
	RGSS,
	PHYSFS,
	SDL,
	MKXP,

	ErrnoE2BIG,
	ErrnoEACCES,
	ErrnoEAGAIN,
	ErrnoEBADF,
	ErrnoECHILD,
	ErrnoEDEADLOCK,
	ErrnoEDOM,
	ErrnoEEXIST,
	ErrnoEINVAL,
	ErrnoEMFILE,
	ErrnoENOENT,
	ErrnoENOEXEC,
	ErrnoENOMEM,
	ErrnoENOSPC,
	ErrnoERANGE,
	ErrnoEXDEV,

	IO,

	TypeError,
	ArgumentError,

	MrbExceptionsMax
};

struct MrbData
{
	RClass *exc[MrbExceptionsMax];
	/* I'll leave the usage of these syms to later,
	 * so I can measure how much of a speed difference they make */
	mrb_sym symbols[CommonSymbolsMax];

	MrbData(mrb_state *mrb){};
};

#define MRB_METHOD_PUB(name) \
	mrb_value name(mrb_state *mrb, mrb_value self)

#define MRB_METHOD(name) static MRB_METHOD_PUB(name)

#define MRB_FUNCTION(name) \
	static mrb_value name(mrb_state *mrb, mrb_value)

#define MRB_FUN_UNUSED_PARAM { (void) mrb; }

template<typename T>
void freeInstance(mrb_state *, void *instance)
{
	delete static_cast<T*>(instance);
}

#define DECL_TYPE(_Type) \
	extern const mrb_data_type _Type##Type

#define DEF_TYPE(_Type) \
	extern const mrb_data_type _Type##Type = \
{ \
	#_Type, \
	freeInstance<_Type> \
}

inline MrbData*
	getMrbData(mrb_state *mrb)
{
	return static_cast<MrbData*>(mrb->ud);
}


inline mrb_sym
	getSym(mrb_state *mrb, CommonSymbol sym)
{
	return getMrbData(mrb)->symbols[sym];
}

inline void
	setPrivateData(mrb_state *mrb, mrb_value self, void *p, const mrb_data_type &type)
{
	RData *data =
		mrb_data_object_alloc(mrb,
		mrb_obj_class(mrb, self),
		p,
		&type);

	mrb_obj_iv_set(mrb,
		mrb_obj_ptr(self),
		getSym(mrb, CSpriv_iv),
		mrb_obj_value(data));
}

template<typename T>
inline T *
	getPrivateData(mrb_state *mrb, mrb_value self)
{
	mrb_value priv = mrb_obj_iv_get(mrb,
		mrb_obj_ptr(self),
		getSym(mrb, CSpriv_iv));

	return static_cast<T*>(DATA_PTR(priv));
}

#endif
