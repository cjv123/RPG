#include "binding-util.h"
#include <mruby/string.h>
#include <mruby/array.h>
#include <mruby/class.h>

#include <stdio.h>

#include <vector>


/* File class methods */
MRB_FUNCTION(fileBasename)
{
	mrb_value filename;
	const char *suffix = 0;

	mrb_get_args(mrb, "S|z", &filename, &suffix);


	mrb_value str = mrb_str_new_cstr(mrb, "bb");


	return str;
}

MRB_FUNCTION(fileDelete)
{
	mrb_int argc;
	mrb_value *argv;

	mrb_get_args(mrb, "*", &argv, &argc);

	for (int i = 0; i < argc; ++i)
	{
		mrb_value &v = argv[i];

		if (!mrb_string_p(v))
			continue;

		const char *filename = RSTRING_PTR(v);
	}

	return mrb_nil_value();
}

MRB_FUNCTION(fileDirname)
{
	mrb_value filename;
	mrb_get_args(mrb, "S", &filename);

	char *_filename = RSTRING_PTR(filename);

	return mrb_str_new_cstr(mrb, "");
}

MRB_FUNCTION(fileExpandPath)
{
	const char *path;
	const char *defDir = 0;

	mrb_get_args(mrb, "z|z", &path, &defDir);

	
	char buffer[512];


	return mrb_str_new_cstr(mrb, buffer);
}

MRB_FUNCTION(fileExtname)
{
	mrb_value filename;
	mrb_get_args(mrb, "S", &filename);

	return mrb_str_new_cstr(mrb, "");
}

MRB_FUNCTION(fileOpen)
{
	mrb_value path;
	const char *mode = "r";
	mrb_value block = mrb_nil_value();

	mrb_get_args(mrb, "S|z&", &path, &mode, &block);


	mrb_value obj;
	setProperty(mrb, obj, CSpath, path);


	return obj;
}

MRB_FUNCTION(fileRename)
{
	const char *from, *to;
	mrb_get_args(mrb, "zz", &from, &to);

	return mrb_fixnum_value(0);
}


MRB_FUNCTION(mrbNoop)
{
	MRB_FUN_UNUSED_PARAM;

	return mrb_nil_value();
}

/* File instance methods */
MRB_METHOD(fileClose)
{
	return self;
}

static void
readLine(FILE *f, std::vector<char> &buffer)
{
	buffer.clear();

	while (true)
	{
		if (feof(f))
			break;

		char c = fgetc(f);

		if (c == '\n' || c == EOF)
			break;

		buffer.push_back(c);
	}
}

MRB_METHOD(fileEachLine)
{

	mrb_value block;
	mrb_get_args(mrb, "&", &block);


	return self;
}

MRB_METHOD(fileEachByte)
{


	mrb_value block;
	mrb_get_args(mrb, "&", &block);



	return self;
}

MRB_METHOD(fileIsEof)
{


	bool isEof;

	return mrb_bool_value(isEof);
}

MRB_METHOD(fileSetPos)
{

	mrb_int pos;
	mrb_get_args(mrb, "i", &pos);

	return self;
}

MRB_METHOD(fileGetPos)
{

	long pos;

	return mrb_fixnum_value(pos);
}

MRB_METHOD(fileRead)
{


	long cur, size;

	mrb_int length = size - cur;
	mrb_get_args(mrb, "|i", &length);

	mrb_value str = mrb_str_new(mrb, 0, 0);
	mrb_str_resize(mrb, str, length);

	return str;
}

// FIXME this function always splits on newline right now,
// to make rs fully work, I'll have to use some strrstr magic I guess
MRB_METHOD(fileReadLines)
{
	mrb_value arg;
	mrb_get_args(mrb, "|o", &arg);

	mrb_value ary = mrb_ary_new(mrb);
	std::vector<char> buffer;


	return ary;
}

MRB_METHOD(fileWrite)
{

	mrb_value str;
	mrb_get_args(mrb, "S", &str);

	size_t count;

	return mrb_fixnum_value(count);
}

MRB_METHOD(fileGetPath)
{

	return getProperty(mrb, self, CSpath);
}

static void
getFileStat(mrb_state *mrb, struct stat &fileStat)
{
	const char *filename;
	mrb_get_args(mrb, "z", &filename);

}

MRB_METHOD(fileGetMtime)
{
	mrb_value path = getProperty(mrb, self, CSpath);

	struct stat fileStat;

	return mrb_nil_value();
}

/* FileTest module functions */
MRB_FUNCTION(fileTestDoesExist)
{
	const char *filename;
	mrb_get_args(mrb, "z", &filename);

	int result=-1;

	return mrb_bool_value(result == 0);
}

MRB_FUNCTION(fileTestIsFile)
{
	struct stat fileStat;
	getFileStat(mrb, fileStat);

	return mrb_bool_value(true);
}

MRB_FUNCTION(fileTestIsDirectory)
{
	struct stat fileStat;
	getFileStat(mrb, fileStat);

	return mrb_bool_value(true);
}

MRB_FUNCTION(fileTestSize)
{
	struct stat fileStat;
	getFileStat(mrb, fileStat);

	return mrb_fixnum_value(fileStat.st_size);
}

void fileBindingInit(mrb_state *mrb)
{
	mrb_define_method(mrb, mrb->kernel_module, "open", fileOpen, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1) | MRB_ARGS_BLOCK());

	RClass *klass = mrb_define_class(mrb, "IO", mrb_class_get(mrb, "Object"));
	klass = mrb_define_class(mrb, "File", klass);

	mrb_define_class_method(mrb, klass, "basename", fileBasename, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
	mrb_define_class_method(mrb, klass, "delete", fileDelete, MRB_ARGS_REQ(1) | MRB_ARGS_ANY());
	mrb_define_class_method(mrb, klass, "dirname", fileDirname, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, klass, "expand_path", fileExpandPath, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
	mrb_define_class_method(mrb, klass, "extname", fileExtname, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, klass, "open", fileOpen, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1) | MRB_ARGS_BLOCK());
	mrb_define_class_method(mrb, klass, "rename", fileRename, MRB_ARGS_REQ(2));

	/* IO */
	mrb_define_method(mrb, klass, "binmode", mrbNoop, MRB_ARGS_NONE());
	mrb_define_method(mrb, klass, "close", fileClose, MRB_ARGS_NONE());
	mrb_define_method(mrb, klass, "each_line", fileEachLine, MRB_ARGS_BLOCK());
	mrb_define_method(mrb, klass, "each_byte", fileEachByte, MRB_ARGS_BLOCK());
	mrb_define_method(mrb, klass, "eof?", fileIsEof, MRB_ARGS_NONE());
	mrb_define_method(mrb, klass, "pos", fileGetPos, MRB_ARGS_NONE());
	mrb_define_method(mrb, klass, "pos=", fileSetPos, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, klass, "read", fileRead, MRB_ARGS_OPT(1));
	mrb_define_method(mrb, klass, "readlines", fileReadLines, MRB_ARGS_OPT(1));
	mrb_define_method(mrb, klass, "write", fileWrite, MRB_ARGS_REQ(1));

	/* File */
	mrb_define_method(mrb, klass, "mtime", fileGetMtime, MRB_ARGS_NONE());
	mrb_define_method(mrb, klass, "path", fileGetPath, MRB_ARGS_NONE());

	/* FileTest */
	RClass *module = mrb_define_module(mrb, "FileTest");
	mrb_define_module_function(mrb, module, "exist?", fileTestDoesExist, MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module, "directory?", fileTestIsDirectory, MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module, "file?", fileTestIsFile, MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module, "size", fileTestSize, MRB_ARGS_REQ(1));
}
