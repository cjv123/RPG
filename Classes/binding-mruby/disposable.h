#ifndef DISPOSABLE_H
#define DISPOSABLE_H

#include "exception.h"

class Disposable
{
public:
	Disposable()
		: disposed(false)
	{}

	virtual ~Disposable() 
	{

	}

	void dispose();

	bool isDisposed() const 
	{ 
		return disposed; 
	}


protected:
	virtual void releaseResources() = 0;

private:
	bool disposed;
};

/* Every cpp needs to define DISP_CLASS_NAME for itself (lowercase) */
#define GUARD_DISPOSED \
{ if (isDisposed()) throw Exception(Exception::RGSSError, "disposed %s", DISP_CLASS_NAME); }

#endif // DISPOSABLE_H
