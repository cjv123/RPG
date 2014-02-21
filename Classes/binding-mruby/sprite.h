#ifndef SPRITE_H
#define SPRITE_H

#include "disposable.h"
#include "util.h"

class Bitmap;
struct Color;
struct Tone;
struct Rect;

struct SpritePrivate;

class Sprite : public Disposable
{
public:
	Sprite();
	~Sprite();

	int getWidth()  const;
	int getHeight() const;

	DECL_ATTR( Bitmap,      Bitmap* )
	DECL_ATTR( SrcRect,     Rect*   )
	DECL_ATTR( X,           int     )
	DECL_ATTR( Y,           int     )
	DECL_ATTR( OX,          int     )
	DECL_ATTR( OY,          int     )
	DECL_ATTR( ZoomX,       float   )
	DECL_ATTR( ZoomY,       float   )
	DECL_ATTR( Angle,       float   )
	DECL_ATTR( Mirror,      bool    )
	DECL_ATTR( BushDepth,   int     )
	DECL_ATTR( BushOpacity, int     )
	DECL_ATTR( Opacity,     int     )
	DECL_ATTR( BlendType,   int     )
	DECL_ATTR( Color,       Color*  )
	DECL_ATTR( Tone,        Tone*   )

private:
	SpritePrivate *p;

	void draw();

	void releaseResources();

	static int handler_method_set_bitmap(int ptr1,void* prt2);
};

#endif // SPRITE_H
