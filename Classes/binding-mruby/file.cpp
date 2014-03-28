#include "binding-util.h"
#include "file.h"
#include <mruby/string.h>
#include <mruby/array.h>
#include <mruby/class.h>

#include <stdio.h>

#include <vector>
#include <string>
using namespace std;
#include <cocos2d.h>
using namespace cocos2d;

#ifndef WIN32
#include <sys/stat.h>
#include <unistd.h>
#endif
#include "../SceneMain.h"

DEF_TYPE(File);

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
	File* p = new File;
	p->Open(RSTRING_PTR(path),mode);
	mrb_value obj = wrapObject(mrb, p, FileType);

	if (mrb_type(block) == MRB_TT_PROC)
	{
		mrb_value ret = mrb_yield(mrb, block, obj);
		p->close();
	}

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
	File *fileimpl = getPrivateData<File>(mrb, self);
	fileimpl->close();
	return mrb_nil_value();
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
	File *fileimpl = getPrivateData<File>(mrb, self);

	mrb_int length = 0;
	mrb_get_args(mrb, "|i", &length);

	mrb_value str = mrb_str_new(mrb, 0, 0);
	mrb_str_resize(mrb, str, length);

	char* data = RSTRING_PTR(str);
	size_t count = fileimpl->read((unsigned char*)data,(length==0)?RSTRING_LEN(str):length);

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
	File *fileimpl = getPrivateData<File>(mrb, self);

	mrb_value str;
	mrb_get_args(mrb, "S", &str);

	char* data = RSTRING_PTR(str);
	size_t count = fileimpl->write((unsigned char*)data,RSTRING_LEN(str));

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

extern mrb_value timeFromSecondsInt(mrb_state *mrb, time_t seconds);
MRB_METHOD(fileGetMtime)
{
	File *fileimpl = getPrivateData<File>(mrb, self);

#ifdef _WIN32
	WIN32_FIND_DATA ffd ;
	HANDLE hFind = FindFirstFile(fileimpl->getPath().c_str(),&ffd);
	SYSTEMTIME st,stLocal;
	FileTimeToSystemTime(&(ffd.ftLastWriteTime), &st);
	SystemTimeToTzSpecificLocalTime(NULL, &st, &stLocal);
	struct tm gm = {stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, stLocal.wMonth-1, stLocal.wYear-1900, stLocal.wDayOfWeek, 0, 0};
	time_t sec = mktime(&gm);

	return timeFromSecondsInt(mrb, sec);
#else
	struct stat fileStat;
	stat(fileimpl->getPath().c_str(), &fileStat);

	return timeFromSecondsInt(mrb, fileStat.st_mtime);
#endif
}

/* FileTest module functions */
MRB_FUNCTION(fileTestDoesExist)
{
	const char *filename;
	mrb_get_args(mrb, "z", &filename);
	string writepath = SceneMain::writeablePath;
	string filepath = writepath + filename;

	bool ret = false;
	FILE* f = fopen(filepath.c_str(),"r");
	if (f)
	{
		fclose(f);
		ret = true;
	}

	return mrb_bool_value(ret);
}

MRB_FUNCTION(fileTestIsFile)
{
	//struct stat fileStat;
	//getFileStat(mrb, fileStat);

	return mrb_bool_value(true);
}

MRB_FUNCTION(fileTestIsDirectory)
{
//	struct stat fileStat;
	//getFileStat(mrb, fileStat);

	return mrb_bool_value(true);
}

MRB_FUNCTION(fileTestSize)
{
	//struct stat fileStat;
	//getFileStat(mrb, fileStat);

	return mrb_fixnum_value(0);
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

File::File() : m_fp(NULL)
{

}

File::~File()
{

}

FILE* File::Open( const char* filename,const char* mode )
{
	string path = SceneMain::writeablePath + filename;
	m_fp = fopen(path.c_str(),mode);
	m_path = path;
	return m_fp;
}

int File::write( unsigned char* data,unsigned long len )
{
	if (!m_fp)
		return -1;

	return fwrite(data,sizeof(char),len,m_fp);
}

int File::read( unsigned char* data,unsigned long len )
{
	if (!m_fp)
		return -1;

	return fread(data,sizeof(char),len,m_fp);
}

void File::close()
{
	if (m_fp)
	{
		fclose(m_fp);
	}
}


