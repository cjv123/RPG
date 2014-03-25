#ifndef SPRITE_H
#define SPRITE_H

#include "disposable.h"
#include "util.h"
#include "viewport.h"

#define VIEWPORT_SP_TAG 2001

class Bitmap;
struct Color;
struct Tone;
struct Rect;

struct SpritePrivate;

class Sprite : public Disposable,public Rect::Delegate
{
public:
	Sprite(Viewport *viewport = 0);
	~Sprite();

	int getWidth()  const;
	int getHeight() const;

	DECL_ATTR( Bitmap,      Bitmap* )
	DECL_ATTR( SrcRect,     Rect*   )
	DECL_ATTR( X,           int     )
	DECL_ATTR( Y,           int     )
	DECL_ATTR( Z,           int     )
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
	DECL_ATTR(Viewport,     Viewport*)
	DECL_ATTR(Visible,     bool)

	void flash(Color* color,int duration);
	void update();
	
private:
	static int handler_method_set_mirror(int ptr1,void* ptr2);
	static int handler_method_set_opacity(int ptr1,void* ptr2);
	static int handler_method_set_srcrect(int prt1,void* ptr2);
	static int handler_method_set_prop(int ptr1,void* ptr2);
	static int handler_method_flash(int ptr1,void* ptr2);
	static int handler_method_composite(int ptr1,void* ptr2);
	static int handler_method_release(int ptr1,void* ptr2);
	static int handler_method_setcolor(int ptr1,void* ptr2);

	SpritePrivate *p;

	void draw();

	void releaseResources();

	static int handler_method_set_bitmap(int ptr1,void* prt2);

	virtual void onRectChange();

	Color* m_flashColor;
	int m_flashDuration;
	CCSprite* m_sprite;
};

#endif // SPRITE_H
