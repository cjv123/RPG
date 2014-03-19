#ifndef BINDINGUTIL_H
#define BINDINGUTIL_H

#include <mruby.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <mruby/class.h>

#include <stdio.h>


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

	MrbData(mrb_state *mrb);
};

struct Exception;

void
raiseMrbExc(mrb_state *mrb, const Exception &exc);

inline MrbData*
getMrbData(mrb_state *mrb)
{
	return static_cast<MrbData*>(mrb->ud);
}

#define GUARD_EXC(exp) \
{ try { exp } catch (Exception &exc) { raiseMrbExc(mrb, exc); } }

#define DECL_TYPE(_Type) \
	extern const mrb_data_type _Type##Type

#define DEF_TYPE(_Type) \
	extern const mrb_data_type _Type##Type = \
	{ \
		#_Type, \
		freeInstance<_Type> \
	}

#define MRB_METHOD_PUB(name) \
	mrb_value name(mrb_state *mrb, mrb_value self)

#define MRB_METHOD(name) static MRB_METHOD_PUB(name)

#define MRB_FUNCTION(name) \
	static mrb_value name(mrb_state *mrb, mrb_value)

#define MRB_UNUSED_PARAM \
	{ (void) mrb; (void) self; }

#define MRB_FUN_UNUSED_PARAM { (void) mrb; }

/* If we're not binding a disposable class,
 * we want to #undef DEF_PROP_CHK_DISP */
#define DEF_PROP_CHK_DISP \
	checkDisposed(mrb, k, DISP_CLASS_NAME);

#define DEF_PROP_OBJ(Klass, PropKlass, PropName, prop_iv) \
	MRB_METHOD(Klass##Get##PropName) \
	{ \
		Klass *k = getPrivateData<Klass>(mrb, self); (void) k; \
		DEF_PROP_CHK_DISP \
		return getProperty(mrb, self, prop_iv); \
	} \
	MRB_METHOD(Klass##Set##PropName) \
	{ \
		Klass *k = getPrivateData<Klass>(mrb, self); \
		mrb_value propObj; \
		PropKlass *prop; \
		mrb_get_args(mrb, "o", &propObj); \
		prop = getPrivateDataCheck<PropKlass>(mrb, propObj, PropKlass##Type); \
		GUARD_EXC( k->set##PropName(prop); ) \
		setProperty(mrb, self, prop_iv, propObj); \
		return propObj; \
	}

/* Object property with allowed NIL */
#define DEF_PROP_OBJ_NIL(Klass, PropKlass, PropName, prop_iv) \
	MRB_METHOD(Klass##Get##PropName) \
	{ \
		Klass *k = getPrivateData<Klass>(mrb, self); (void) k; \
		DEF_PROP_CHK_DISP \
		return getProperty(mrb, self, prop_iv); \
	} \
	MRB_METHOD(Klass##Set##PropName) \
	{ \
		Klass *k = getPrivateData<Klass>(mrb, self); \
		mrb_value propObj; \
		PropKlass *prop; \
		mrb_get_args(mrb, "o", &propObj); \
		if (mrb_nil_p(propObj)) \
			prop = 0; \
		else \
			prop = getPrivateDataCheck<PropKlass>(mrb, propObj, PropKlass##Type); \
		GUARD_EXC( k->set##PropName(prop); ) \
		setProperty(mrb, self, prop_iv, propObj); \
		return mrb_nil_value(); \
	}

#define DEF_PROP(Klass, mrb_type, PropName, arg_type, conv_t) \
	MRB_METHOD(Klass##Get##PropName) \
	{ \
		Klass *k = getPrivateData<Klass>(mrb, self); \
		DEF_PROP_CHK_DISP \
		return mrb_##conv_t##_value(k->get##PropName()); \
	} \
	MRB_METHOD(Klass##Set##PropName) \
	{ \
		Klass *k = getPrivateData<Klass>(mrb, self); \
		mrb_type value; \
		mrb_get_args(mrb, arg_type, &value); \
		GUARD_EXC( k->set##PropName(value); ) \
		return mrb_##conv_t##_value(value); \
	}

#define DEF_PROP_I(Klass, PropName) \
	DEF_PROP(Klass, mrb_int, PropName, "i", fixnum)

#define DEF_PROP_F(Klass, PropName) \
	DEF_PROP(Klass, mrb_float, PropName, "f", _float)

#define DEF_PROP_B(Klass, PropName) \
	DEF_PROP(Klass, mrb_bool, PropName, "b", bool)

#define CLONE_FUN(Klass) \
	MRB_METHOD(Klass##Clone) \
	{ \
		Klass *k = getPrivateData<Klass>(mrb, self); \
		mrb_value dupObj = mrb_obj_clone(mrb, self); \
		Klass *dupK = new Klass(*k); \
		setPrivateData(mrb, dupObj, dupK, Klass##Type); \
		return dupObj; \
	}

#define MARSH_LOAD_FUN(Klass) \
	MRB_METHOD(Klass##Load) \
	{ \
		return objectLoad<Klass>(mrb, self, Klass##Type); \
	}

#define INIT_PROP_BIND(Klass, PropName, prop_name_s) \
{ \
	mrb_define_method(mrb, klass, prop_name_s, Klass##Get##PropName, MRB_ARGS_NONE()); \
	mrb_define_method(mrb, klass, prop_name_s "=", Klass##Set##PropName, MRB_ARGS_REQ(1)); \
}


static inline mrb_value
mrb__float_value(mrb_float f)
{
  mrb_value v;

  MRB_SET_VALUE(v, MRB_TT_FLOAT, value.f, f);
  return v;
}

inline mrb_sym
getSym(mrb_state *mrb, CommonSymbol sym)
{
	return getMrbData(mrb)->symbols[sym];
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




template<typename T>
inline T *
getPrivateDataCheck(mrb_state *mrb, mrb_value obj, const mrb_data_type &type)
{
	static const char mesg[] = "wrong argument type %S (expected %S)";

	if (mrb_type(obj) != MRB_TT_OBJECT)
		mrb_raisef(mrb, E_TYPE_ERROR, mesg,
		           mrb_str_new_cstr(mrb, (mrb_class_name(mrb, mrb_class(mrb, obj)))),
		           mrb_str_new_cstr(mrb, type.struct_name));

	RObject *objP = mrb_obj_ptr(obj);

	if (!mrb_obj_iv_defined(mrb, objP, getSym(mrb, CSpriv_iv)))
		mrb_raisef(mrb, E_TYPE_ERROR, mesg,
		           mrb_str_new_cstr(mrb, (mrb_class_name(mrb, mrb_class(mrb, obj)))),
		           mrb_str_new_cstr(mrb, type.struct_name));

	mrb_value priv = mrb_obj_iv_get(mrb, objP, getSym(mrb, CSpriv_iv));

	void *p = mrb_check_datatype(mrb, priv, &type);

	return static_cast<T*>(p);
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


inline mrb_value
wrapObject(mrb_state *mrb, void *p, const mrb_data_type &type)
{
	RClass *c = mrb_class_get(mrb, type.struct_name);
	RObject *o = (RObject*) mrb_obj_alloc(mrb, MRB_TT_OBJECT, c);
	mrb_value obj = mrb_obj_value(o);

	setPrivateData(mrb, obj, p, type);

	return obj;
}

inline void
wrapProperty(mrb_state *mrb, mrb_value self,
             void *prop, CommonSymbol iv, const mrb_data_type &type)
{
	mrb_value propObj = wrapObject(mrb, prop, type);

	mrb_obj_iv_set(mrb,
	               mrb_obj_ptr(self),
	               getSym(mrb, iv),
		           propObj);
}

inline void
wrapNilProperty(mrb_state *mrb, mrb_value self, CommonSymbol iv)
{
	mrb_obj_iv_set(mrb,
	               mrb_obj_ptr(self),
	               getSym(mrb, iv),
	               mrb_nil_value());
}

inline mrb_value
getProperty(mrb_state *mrb, mrb_value self, CommonSymbol iv)
{
	return mrb_obj_iv_get(mrb,
	                      mrb_obj_ptr(self),
	                      getSym(mrb, iv));
}

inline void
setProperty(mrb_state *mrb, mrb_value self,
            CommonSymbol iv, mrb_value propObject)
{
	mrb_obj_iv_set(mrb,
	               mrb_obj_ptr(self),
	               getSym(mrb, iv),
	               propObject);
}

template<typename T>
void
freeInstance(mrb_state *, void *instance)
{
	delete static_cast<T*>(instance);
}

inline mrb_value
mrb_bool_value(bool value)
{
	return value ? mrb_true_value() : mrb_false_value();
}

inline bool
_mrb_bool(mrb_value o)
{
	return mrb_test(o);
}

template<class C>
inline mrb_value
objectLoad(mrb_state *mrb, mrb_value self, const mrb_data_type &type)
{
	RClass *klass = mrb_class_ptr(self);
	char *data;
	int data_len;
	mrb_get_args(mrb, "s", &data, &data_len);

	RObject *obj = (RObject*) mrb_obj_alloc(mrb, MRB_TT_OBJECT, klass);
	mrb_value obj_value = mrb_obj_value(obj);

	C *c = C::deserialize(data, data_len);

	setPrivateData(mrb, obj_value, c, type);

	return obj_value;
}

template<class C>
static C *
	viewportElementInitialize(mrb_state *mrb, mrb_value self)
{
	/* Get parameters */
	mrb_value viewportObj = mrb_nil_value();
	Viewport *viewport = 0;

	mrb_get_args(mrb, "|o", &viewportObj);

	if (!mrb_nil_p(viewportObj))
		viewport = getPrivateDataCheck<Viewport>(mrb, viewportObj, ViewportType);

	/* Construct object */
	C *ve = new C(viewport);

	/* Set property objects */
	setProperty(mrb, self, CSviewport, viewportObj);

	return ve;
}

inline float rgss_y_to_cocos_y(int y,int h)
{
	return (float)(h - y);
}

MRB_METHOD_PUB(inspectObject);


#endif // BINDINGUTIL_H
