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

	string getPath()
	{
		return m_path;
	}
private:
	FILE* m_fp;
	string m_path;
};

DECL_TYPE(File);

#endif
