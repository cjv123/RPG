#ifndef _FILE_H
#define _FILE_H

class File
{
public:
	File();
	~File();
	FILE* Open(const char* filename,const char* mode);
	int write(unsigned char* data,unsigned long len);
	int read(unsigned char* data,unsigned long len);
	void close();
private:
	FILE* m_fp;
};

DECL_TYPE(File);

#endif
