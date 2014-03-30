#include "marshal.h"

#include <mruby/array.h>
#include <mruby/hash.h>
#include <mruby/khash.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <mruby/class.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>

#include "binding-util.h"
#include "file.h"
#include "exception.h"
#include "boost-hash.h"


#define MARSHAL_MAJOR 4
#define MARSHAL_MINOR 8

#define TYPE_NIL '0'
#define TYPE_TRUE 'T'
#define TYPE_FALSE 'F'
#define TYPE_FIXNUM 'i'

#define TYPE_OBJECT 'o'
//#define TYPE_DATA 'd'
#define TYPE_USERDEF 'u'
#define TYPE_USRMARSHAL 'U'
#define TYPE_FLOAT 'f'
//#define TYPE_BIGNUM 'l'
#define TYPE_STRING '"'
//#define TYPE_REGEXP '/'
#define TYPE_ARRAY '['
#define TYPE_HASH '{'
#define TYPE_HASH_DEF '}'
#define TYPE_MODULE_OLD 'M'
#define TYPE_IVAR	'I'
#define TYPE_LINK	'@'

#define TYPE_SYMBOL ':'
#define TYPE_SYMLINK ';'

// FIXME make these dynamically allocatd, per MarshalContext
static char gpbuffer[512];

inline size_t hash_value(mrb_value key)
{
	return boost::hash_value(key.value.p);
}

unsigned int g_buf_size;

static unsigned long readBuf(char* buf,char* outbuf,int size,int count)
{
	int s = size*count;
	memcpy(outbuf,buf,s);
	memmove(buf,buf+s,g_buf_size - s);
	g_buf_size -=s;
	return s;
}

static unsigned long writeBuf(char* buf,char* inbuf,int size,int count)
{
	int s = size*count;
	memcpy(buf+g_buf_size,inbuf,s);
	g_buf_size+=s;
	return s;
}



inline bool operator==(mrb_value v1, mrb_value v2)
{
	if (mrb_type(v1) != mrb_type(v2))
		return false;

	switch (mrb_type(v1))
	{
	case MRB_TT_TRUE:
		return true;

	case MRB_TT_FALSE:
	case MRB_TT_FIXNUM:
		return (v1.value.i == v2.value.i);
	case MRB_TT_SYMBOL:
		return (v1.value.sym == v2.value.sym);

	case MRB_TT_FLOAT:
		return (mrb_float(v1) == mrb_float(v2));

	default:
		return (mrb_ptr(v1) == mrb_ptr(v2));
	}
}

template<typename T>
struct LinkBuffer
{
	/* Key: val, Value: idx in vec */
	BoostHash<T, int> hash;
	std::vector<T> vec;

	bool contains(T value)
	{
		return hash.contains(value);
	}

	bool containsIdx(int idx)
	{
		if (vec.empty())
			return false;

		return (idx < vec.size());
	}

	int add(T value)
	{
		int idx = vec.size();

		hash.insert(value, idx);
		vec.push_back(value);

		return idx;
	}

	T lookup(int idx)
	{	
		return vec[idx];
	}

	int lookupIdx(T value)
	{
		return hash[value];
	}
};

struct MarshalContext
{
	char *ops;
	mrb_state *mrb;
	mrb_int limit;
	File* file; 

	MarshalContext()
	{
		ops = NULL;
		file = NULL;
	}

	/* For Symlinks/Links */
	LinkBuffer<mrb_sym> symbols;
	LinkBuffer<mrb_value> objects;

	char readByte()
	{
		char byte;

		int result = 0;
		if(file)
		{
			result = file->read((unsigned char*)&byte,1);
		}
		else
			result = readBuf(ops, &byte, 1, 1);

		if (result < 1)
			CCAssert(false,"dump format error");

		return byte;
	}

	void readData(char *dest, int len)
	{
		int result = 0;
		if(file)
		{
			result = file->read((unsigned char*)dest,len);
		}
		else
			result = readBuf(ops, dest, 1, len);

		if (result < len)
			CCAssert(false,"dump format error");
	}

	void writeByte(char byte)
	{
		int result = writeBuf(ops, &byte, 1, 1);

		if (result < 1) // FIXME not sure what the correct error would be here
			CCAssert(false,"dump writing error");
	}

	void writeData(const char *data, int len)
	{
		int result = writeBuf(ops, (char*)data, 1, len);

		if (result < len) // FIXME not sure what the correct error would be here
			CCAssert(false,"dump writing error");
	}
};


static int
read_fixnum(MarshalContext *ctx)
{
	mrb_int head= static_cast<signed char>(ctx->readByte());
	
	if (head == 0)
		return 0;
	else if (head > 5)
		return head - 5;
	else if (head < -4)
		return head + 5;

	int pos = (head > 0);
	int len = pos ? head : head * -1;

	char n1, n2, n3, n4;

	if (pos)
		n2 = n3 = n4 = 0;
	else
		n2 = n3 = n4 = 0xFF;

	n1 = ctx->readByte();
	if (len >= 2)
		n2 = ctx->readByte();
	if (len >= 3)
		n3 = ctx->readByte();
	if (len >= 4)
		n4 = ctx->readByte();

	int result = ((0xFF << 0x00) & (n1 << 0x00))
	           | ((0xFF << 0x08) & (n2 << 0x08))
	           | ((0xFF << 0x10) & (n3 << 0x10))
	           | ((0xFF << 0x18) & (n4 << 0x18));

//	if (!pos)
//		result = -((result ^ 0xFFFFFFFF) + 1);

	return result;
}

static float
read_float(MarshalContext *ctx)
{
	int len = read_fixnum(ctx);

	ctx->readData(gpbuffer, len);
	gpbuffer[len] = '\0';

	return strtof(gpbuffer, 0);
}

static char *
read_string(MarshalContext *ctx)
{
	int len = read_fixnum(ctx);

	ctx->readData(gpbuffer, len);
	gpbuffer[len] = '\0';

	return gpbuffer;
}

static mrb_value
read_string_value(MarshalContext *ctx)
{
	mrb_state *mrb = ctx->mrb;
	int len = read_fixnum(ctx);

	struct RString *str =
		(struct RString*) mrb_obj_alloc
			(mrb, MRB_TT_STRING, mrb->string_class);

	str->c = mrb->string_class;
	str->len = len;
	str->aux.capa = len;
	str->ptr = (char*) mrb_malloc(mrb, len+1);

	ctx->readData(str->ptr, len);
	str->ptr[len] = '\0';

	mrb_value str_obj = mrb_obj_value(str);

	return str_obj;
}

static mrb_value read_value(MarshalContext *ctx);

static mrb_value
read_array(MarshalContext *ctx)
{
	mrb_state *mrb = ctx->mrb;
	int len = read_fixnum(ctx);
	int i;

	mrb_value array = mrb_ary_new_capa(mrb, len);

	ctx->objects.add(array);

	for (i = 0; i < len; ++i)
	{
		mrb_value val = read_value(ctx);
		mrb_ary_set(mrb, array, i, val);
	}

	return array;
}

static mrb_value
read_hash(MarshalContext *ctx)
{
	mrb_state *mrb = ctx->mrb;
	int len = read_fixnum(ctx);
	int i;

	mrb_value hash = mrb_hash_new_capa(mrb, len);

	ctx->objects.add(hash);

	for (i = 0; i < len; ++i)
	{
		mrb_value key = read_value(ctx);
		mrb_value val = read_value(ctx);

		mrb_hash_set(mrb, hash, key, val);
	}

	return hash;
}

static mrb_sym
read_symbol(MarshalContext *ctx)
{
	mrb_state *mrb = ctx->mrb;

	mrb_sym symbol = mrb_intern_cstr(mrb, read_string(ctx));
	ctx->symbols.add(symbol);

	return symbol;
}

static mrb_sym
read_symlink(MarshalContext *ctx)
{
	int idx = read_fixnum(ctx);

	if (!ctx->symbols.containsIdx(idx))
		throw Exception(Exception::ArgumentError, "bad symlink");

	return ctx->symbols.lookup(idx);
}

static mrb_value
read_link(MarshalContext *ctx)
{
	int idx = read_fixnum(ctx);

	if (!ctx->objects.containsIdx(idx))
		throw Exception(Exception::ArgumentError, "dump format error (unlinked)");

	return ctx->objects.lookup(idx);
}

mrb_value
read_instance_var(MarshalContext *ctx)
{
	mrb_value obj = read_value(ctx);

	int iv_count = read_fixnum(ctx);
	int i;

	ctx->objects.add(obj);

	for (i = 0; i < iv_count; ++i)
	{
		mrb_value iv_name = read_value(ctx);
		mrb_value iv_value = read_value(ctx);
		(void)iv_name;
		(void)iv_value;

		// Instance vars for String/Array not supported yet
//		mrb_iv_set(mrb, obj, mrb_symbol(iv_name), iv_value);
	}

	return obj;
}

static struct RClass *
mrb_class_from_path(mrb_state *mrb, mrb_value path)
{
	char *path_s;
	switch (path.tt)
	{
	case MRB_TT_SYMBOL :
		path_s = (char*) mrb_sym2name(mrb, mrb_symbol(path));
		break;
	case MRB_TT_STRING :
		path_s = (char*) RSTRING_PTR(path);
		break;
	default:
		throw Exception(Exception::ArgumentError, "dump format error for symbol");
	}

	/* If a symbol contains any colons, mrb_sym2name
	 * will return the string with "s around it
	 * (e.g. :Mod::Class => :"Mod::Class"),
	 * so we need to skip those. */
	if (path_s[0] == '\"')
		path_s++;

	char *p, *pbgn;
	mrb_value klass = mrb_obj_value(mrb->object_class);

	p = pbgn = path_s;

	while (*p && *p != '\"')
	{
		while (*p && *p != ':' && *p != '\"') p++;

		mrb_sym sym = mrb_intern(mrb, pbgn, p-pbgn);
		klass = mrb_const_get(mrb, klass, sym);

		if (p[0] == ':')
		{
			if (p[1] != ':')
				return 0;
			p += 2;
			pbgn = p;
		}
	}

	return (struct RClass*) mrb_obj_ptr(klass);
}

static mrb_value
read_object(MarshalContext *ctx)
{
	mrb_state *mrb = ctx->mrb;
	mrb_value class_path = read_value(ctx);

	struct RClass *klass =
		mrb_class_from_path(mrb, class_path);

	mrb_value obj = mrb_obj_value(mrb_obj_alloc(mrb, MRB_TT_OBJECT, klass));

	ctx->objects.add(obj);

	int iv_count = read_fixnum(ctx);
	int i;

	for (i = 0; i < iv_count; ++i)
	{
		mrb_value iv_name = read_value(ctx);
		mrb_value iv_value = read_value(ctx);

		mrb_obj_iv_set(mrb, mrb_obj_ptr(obj),
		               mrb_symbol(iv_name), iv_value);
	}

	return obj;
}

static mrb_value
read_userdef(MarshalContext *ctx)
{
	mrb_state *mrb = ctx->mrb;
	mrb_value class_path = read_value(ctx);

	struct RClass *klass =
		mrb_class_from_path(mrb, class_path);

	/* Should check here if klass implements '_load()' */
	if (!mrb_obj_respond_to(mrb, mrb_class(mrb, mrb_obj_value(klass)), mrb_intern_cstr(mrb, "_load")))
		throw Exception(Exception::TypeError,
	                    "class %s needs to have method '_load'",
		                 RSTRING_PTR(class_path));

	mrb_value data = read_string_value(ctx);
	mrb_value obj = mrb_funcall(mrb, mrb_obj_value(klass), "_load", 1, data);

	return obj;
}

static int maxArena = 0;

static mrb_value
read_value(MarshalContext *ctx)
{
	mrb_state *mrb = ctx->mrb;

	char type = ctx->readByte();

	mrb_value value;
	if (mrb->arena_idx > maxArena)
		maxArena = mrb->arena_idx;

	int arena = mrb_gc_arena_save(mrb);

	switch (type)
	{
	case TYPE_NIL :
		value = mrb_nil_value();
		break;

	case TYPE_TRUE :
		value = mrb_true_value();
		break;

	case TYPE_FALSE :
		value = mrb_false_value();
		break;

	case TYPE_FIXNUM :
		value = mrb_fixnum_value(read_fixnum(ctx));
		break;

	case TYPE_FLOAT :
		value = mrb_float_value(mrb, read_float(ctx));
		ctx->objects.add(value);
		break;

	case TYPE_STRING :
		value = read_string_value(ctx);
		ctx->objects.add(value);
		break;

	case TYPE_ARRAY :
		value = read_array(ctx);
		break;

	case TYPE_HASH :
		value = read_hash(ctx);
		break;

	case TYPE_SYMBOL :
		value = mrb_symbol_value(read_symbol(ctx));
		break;

	case TYPE_SYMLINK :
		value = mrb_symbol_value(read_symlink(ctx));
		break;

	case TYPE_OBJECT :
		value = read_object(ctx);
		break;

	case TYPE_IVAR :
		value = read_instance_var(ctx);
		break;

	case TYPE_LINK :
		value = read_link(ctx);
		break;

	case TYPE_USERDEF :
		value = read_userdef(ctx);
		ctx->objects.add(value);
		break;

	default :
		CCLOG( "Marshal.load: unsupported value type '%c',gbufsize %d",type,g_buf_size);
		CCAssert(false,"fuck");
		exit(0);
	}

	mrb_gc_arena_restore(mrb, arena);

	return value;
}

static void
write_fixnum(MarshalContext *ctx, int value)
{
	if (value == 0)
	{
		ctx->writeByte(0);
		return;
	}
	else if (value > 0 && value < 123)
	{
		ctx->writeByte((char) value + 5);
		return;
	}
	else if (value < 0 && value > -124)
	{
		ctx->writeByte((char) value - 5);
		return;
	}

	char len;

	if (value > 0)
	{
		/* Positive number */
		if (value <= 0x7F)
		{
			/* 1 byte wide */
			len = 1;
		}
		else if (value <= 0x7FFF)
		{
			/* 2 bytes wide */
			len = 2;
		}
		else if (value <= 0x7FFFFF)
		{
			/* 3 bytes wide */
			len = 3;
		}
		else
		{
			/* 4 bytes wide */
			len = 4;
		}
	}
	else
	{
		/* Negative number */
		if (value >= (int)0x80)
		{
			/* 1 byte wide */
			len = -1;
		}
		else if (value >= (int)0x8000)
		{
			/* 2 bytes wide */
			len = -2;
		}
		else if (value <= (int)0x800000)
		{
			/* 3 bytes wide */
			len = -3;
		}
		else
		{
			/* 4 bytes wide */
			len = -4;
		}
	}

	/* Write length */
	ctx->writeByte(len);

	/* Write bytes */
	if (len >= 1 || len <= -1)
	{
		ctx->writeByte((value & 0x000000FF) >> 0x00);
	}
	if (len >= 2 || len <= -2)
	{
		ctx->writeByte((value & 0x0000FF00) >> 0x08);
	}
	if (len >= 3 || len <= -3)
	{
		ctx->writeByte((value & 0x00FF0000) >> 0x10);
	}
	if (len == 4 || len == -4)
	{
		ctx->writeByte((value & 0xFF000000) >> 0x18);
	}
}

static void
write_float(MarshalContext *ctx, float value)
{
	sprintf(gpbuffer, "%.16g", value);

	int len = strlen(gpbuffer);
	write_fixnum(ctx, len);
	ctx->writeData(gpbuffer, len);
}

static void
write_string(MarshalContext *ctx, const char *string)
{
	int len = strlen(string);

	write_fixnum(ctx, len);
	ctx->writeData(string, len);
}

static void
write_string_value(MarshalContext *ctx, mrb_value string)
{
	int len = RSTRING_LEN(string);
	const char *ptr = RSTRING_PTR(string);

	write_fixnum(ctx, len);
	ctx->writeData(ptr, len);
}

static void write_value(MarshalContext *, mrb_value);

static void
write_array(MarshalContext *ctx, mrb_value array)
{
	mrb_state *mrb = ctx->mrb;
	int len = mrb_ary_len(mrb, array);
	write_fixnum(ctx, len);

	int i;
	for (i = 0; i < len; ++i)
	{
		mrb_value value = mrb_ary_entry(array, i);
		write_value(ctx, value);
	}
}

KHASH_DECLARE(ht, mrb_value, mrb_value, 1)

static void
write_hash(MarshalContext *ctx, mrb_value hash)
{
	mrb_state *mrb = ctx->mrb;
	int len = 0;

	kh_ht *h = mrb_hash_tbl(mrb, hash);
	if (h)
		len = kh_size(h);

	write_fixnum(ctx, len);

	for (khiter_t k = kh_begin(h); k != kh_end(h); ++k)
	{
		if (!kh_exist(h, k))
			continue;

		write_value(ctx, kh_key(h, k));
		write_value(ctx, kh_val(h, k));
	}
}

static void
write_symbol(MarshalContext *ctx, mrb_value symbol)
{
	mrb_state *mrb = ctx->mrb;
	mrb_sym sym = mrb_symbol(symbol);
	size_t len;
	const char *p = mrb_sym2name_len(mrb, sym, &len);

	write_string(ctx, p);
}

static void
write_object(MarshalContext *ctx, mrb_value object)
{
	mrb_state *mrb = ctx->mrb;
	struct RObject *o = mrb_obj_ptr(object);

	mrb_value path = mrb_class_path(mrb, o->c);
	write_value(ctx, mrb_str_intern(mrb, path));

	mrb_value iv_ary = mrb_obj_instance_variables(mrb, object);
	int iv_count = mrb_ary_len(mrb, iv_ary);
	write_fixnum(ctx, iv_count);

	int i;
	for (i = 0; i < iv_count; ++i)
	{
		mrb_value iv_name = mrb_ary_entry(iv_ary, i);
		mrb_value iv_value = mrb_obj_iv_get(mrb, o, mrb_symbol(iv_name));

		write_value(ctx, iv_name);
		write_value(ctx, iv_value);
	}
}

static void
write_userdef(MarshalContext *ctx, mrb_value object)
{
	mrb_state *mrb = ctx->mrb;
	struct RObject *o = mrb_obj_ptr(object);

	mrb_value path = mrb_class_path(mrb, o->c);
	write_value(ctx, mrb_str_intern(mrb, path));

	mrb_value dump = mrb_funcall(mrb, object, "_dump", 0);
	write_string_value(ctx, dump);
}

static void
write_value(MarshalContext *ctx, mrb_value value)
{
	mrb_state *mrb = ctx->mrb;

	int arena = mrb_gc_arena_save(mrb);

	if (ctx->objects.contains(value))
	{
		ctx->writeByte(TYPE_LINK);
		write_fixnum(ctx, ctx->objects.lookupIdx(value));
		mrb_gc_arena_restore(mrb, arena);
		return;
	}

	switch (mrb_type(value))
	{
	case MRB_TT_TRUE :
		ctx->writeByte(TYPE_TRUE);
		break;

	case MRB_TT_FALSE :
		if (mrb_fixnum(value))
			ctx->writeByte(TYPE_FALSE);
		else
			ctx->writeByte(TYPE_NIL);

		break;

	case MRB_TT_FIXNUM :
		ctx->writeByte(TYPE_FIXNUM);
		write_fixnum(ctx, mrb_fixnum(value));

		break;

	case MRB_TT_FLOAT :
		ctx->writeByte(TYPE_FLOAT);
		write_float(ctx, mrb_float(value));
		ctx->objects.add(value);

		break;

	case MRB_TT_STRING :
		ctx->objects.add(value);
		ctx->writeByte(TYPE_STRING);
		write_string_value(ctx, value);

		break;

	case MRB_TT_ARRAY :
		ctx->objects.add(value);
		ctx->writeByte(TYPE_ARRAY);
		write_array(ctx, value);

		break;

	case MRB_TT_HASH :
		ctx->objects.add(value);
		ctx->writeByte(TYPE_HASH);
		write_hash(ctx, value);

		break;

	case MRB_TT_SYMBOL :
		if (ctx->symbols.contains(mrb_symbol(value)))
		{
			ctx->writeByte(TYPE_SYMLINK);
			write_fixnum(ctx, ctx->symbols.lookupIdx(mrb_symbol(value)));
			ctx->symbols.add(mrb_symbol(value));
		}
		else
		{
			ctx->writeByte(TYPE_SYMBOL);
			write_symbol(ctx, value);
		}

		break;

	/* Objects seem to have wrong tt */
	case MRB_TT_CLASS :
	case MRB_TT_OBJECT :
		ctx->objects.add(value);

		if (mrb_obj_respond_to(mrb, mrb_obj_ptr(value)->c,
		                       mrb_intern_lit(mrb, "_dump")))
		{
			ctx->writeByte(TYPE_USERDEF);
			write_userdef(ctx, value);
		}
		else
		{
			ctx->writeByte(TYPE_OBJECT);
			write_object(ctx, value);
		}

		break;

	default :
		printf("Warning! Could not write value type '%c'(%d)\n", // FIXME not sure if raise or just print error
		       mrb_type(value), mrb_type(value));
		fflush(stdout);
	}

	mrb_gc_arena_restore(mrb, arena);
}

static void
writeMarshalHeader(MarshalContext *ctx)
{
	ctx->writeByte(MARSHAL_MAJOR);
	ctx->writeByte(MARSHAL_MINOR);
}

static void
verifyMarshalHeader(MarshalContext *ctx)
{
	char maj = ctx->readByte();
	char min = ctx->readByte();

	if (maj != MARSHAL_MAJOR || min != MARSHAL_MINOR)
		CCLOG("incompatible marshal file format (can't be read)");
}

MRB_FUNCTION(marshalDump)
{
	mrb_value val;
	mrb_value port = mrb_nil_value();
	mrb_int limit = 100;

	mrb_get_args(mrb, "o|oi", &val, &port, &limit);

	bool havePort = mrb_type(port) == MRB_TT_OBJECT;
	mrb_value ret = port;

	char *ops = new char[1024*60];
	
	MarshalContext ctx;
	ctx.mrb = mrb;
	ctx.limit = limit;
	ctx.ops = ops;
	g_buf_size = 0;
	writeMarshalHeader(&ctx);
	write_value(&ctx, val);

	int dataSize = g_buf_size;
	ret = mrb_str_new(mrb, ops, dataSize);
	delete [] ops;

	if (havePort)
	{
		File *file = getPrivateDataCheck<File>(mrb, port, FileType);
		file->write((unsigned char*)RSTRING_PTR(ret),dataSize);
		return port;
	}

	

	return ret;
}

MRB_FUNCTION(marshalLoad)
{
	mrb_value port;

	mrb_get_args(mrb, "o", &port);

	char *ops=NULL;

	MarshalContext ctx;
	ctx.mrb = mrb;
	ctx.ops = ops;

	if (mrb_type(port) == MRB_TT_OBJECT)
	{
		File *file = getPrivateDataCheck<File>(mrb, port, FileType);
		ctx.file = file;
		g_buf_size = 0;
	}
	else if (mrb_string_p(port))
	{
		ops = RSTRING_PTR(port);
		ctx.ops = ops;
		g_buf_size = RSTRING_LEN(port);
	}
	else
	{
		CCLOG("FIXME: Marshal.load: generic IO port not implemented");
		return mrb_nil_value();
	}

	mrb_value val;
	verifyMarshalHeader(&ctx);
	val = read_value(&ctx);

	return val;
}

void
marshalBindingInit(mrb_state *mrb)
{
	RClass *module = mrb_define_module(mrb, "Marshal");

	mrb_define_module_function(mrb, module, "dump", marshalDump, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(2));
	mrb_define_module_function(mrb, module, "load", marshalLoad, MRB_ARGS_REQ(1));
}

/* Exceptions from these internal functions will be caught higher up */
void
marshalDumpInt(mrb_state *mrb, char *ops, mrb_value val)
{
	MarshalContext ctx;
	ctx.mrb = mrb;
	ctx.ops = ops;
	ctx.limit = 100;
	g_buf_size = 0;

	writeMarshalHeader(&ctx);
	write_value(&ctx, val);
}

mrb_value
marshalLoadInt(mrb_state *mrb, char *ops)
{
	MarshalContext ctx;
	ctx.mrb = mrb;
	ctx.ops = ops;
	g_buf_size = strlen(ops);

	verifyMarshalHeader(&ctx);

	mrb_value val = read_value(&ctx);

	return val;
}

mrb_value marshalLoadInt( mrb_state* mrb, char* ops, unsigned long len)
{
	MarshalContext ctx;
	ctx.mrb = mrb;
	ctx.ops = ops;
	g_buf_size = len;

	verifyMarshalHeader(&ctx);
	mrb_value val = read_value(&ctx);

	return val;
}

