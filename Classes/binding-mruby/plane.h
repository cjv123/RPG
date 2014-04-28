#ifndef PLANE_H
#define PLANE_H

#include "disposable.h"
#include "viewport.h"

class Bitmap;
struct Color;
struct Tone;

struct PlanePrivate;

class Plane : public Disposable,public ViewPortDelegate
{
public:
	Plane(Viewport *viewport = 0);
	~Plane();

	DECL_ATTR( Bitmap,    Bitmap* )
	DECL_ATTR( OX,        int     )
	DECL_ATTR( OY,        int     )
	DECL_ATTR(Z  ,        int)
	DECL_ATTR( ZoomX,     float   )
	DECL_ATTR( ZoomY,     float   )
	DECL_ATTR( Opacity,   int     )
	DECL_ATTR( BlendType, int     )
	DECL_ATTR( Color,     Color*  )
	DECL_ATTR( Tone,      Tone*   )
	DECL_ATTR( Viewport,  Viewport*   )
private:
	PlanePrivate *p;

	void draw();
	void aboutToAccess() const;

	void releaseResources();

	virtual void composite();
	
	static int handler_method_set_prop(int ptr1,void* ptr2);
	static int handler_method_set_srcrect(int prt1,void* ptr2);
	static int handler_method_set_bitmap(int ptr1,void* prt2);
	static int handler_method_composite(int ptr1,void* ptr2);
	static int handler_method_release(int ptr1,void* ptr2);

	
};

#endif // PLANE_H
