#ifndef PLANE_H
#define PLANE_H

#include "disposable.h"
#include "viewport.h"

class Bitmap;
struct Color;
struct Tone;

struct PlanePrivate;

class Plane : public Disposable
{
public:
	Plane(Viewport *viewport = 0);
	~Plane();

	DECL_ATTR( Bitmap,    Bitmap* )
	DECL_ATTR( OX,        int     )
	DECL_ATTR( OY,        int     )
	DECL_ATTR( ZoomX,     float   )
	DECL_ATTR( ZoomY,     float   )
	DECL_ATTR( Opacity,   int     )
	DECL_ATTR( BlendType, int     )
	DECL_ATTR( Color,     Color*  )
	DECL_ATTR( Tone,      Tone*   )

private:
	PlanePrivate *p;

	void draw();
	void aboutToAccess() const;

	void releaseResources();
};

#endif // PLANE_H
