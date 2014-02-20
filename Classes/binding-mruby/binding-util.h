#ifndef _BINDING_UTIL
#define _BINDING_UTIL

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


#define MRB_FUNCTION(name) \
	static mrb_value name(mrb_state *mrb, mrb_value)

#define MRB_FUN_UNUSED_PARAM { (void) mrb; }

#endif
