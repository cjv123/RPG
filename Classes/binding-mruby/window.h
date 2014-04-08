#ifndef WINDOW_H
#define WINDOW_H

#include "viewport.h"
#include "disposable.h"

#include "util.h"
#include <cocos-ext.h>
using namespace cocos2d::extension;

class Bitmap;
struct Rect;

struct WindowPrivate;

class Window : public Disposable,public ViewPortDelegate
{
public:
	Window(Viewport *viewport = 0);
	~Window();

	void update();

	DECL_ATTR( Windowskin,      Bitmap* )
	DECL_ATTR( Contents,        Bitmap* )
	DECL_ATTR( Stretch,         bool    )
	DECL_ATTR( CursorRect,      Rect*   )
	DECL_ATTR( Active,          bool    )
	DECL_ATTR( Pause,           bool    )
	DECL_ATTR( X,               int     )
	DECL_ATTR( Y,               int     )
	DECL_ATTR( Width,           int     )
	DECL_ATTR( Height,          int     )
	DECL_ATTR( OX,              int     )
	DECL_ATTR( OY,              int     )
	DECL_ATTR( Opacity,         int     )
	DECL_ATTR( BackOpacity,     int     )
	DECL_ATTR( ContentsOpacity, int     )
	DECL_ATTR( Visible, bool     )
	DECL_ATTR( Z, int    )
private:
	WindowPrivate *p;

	void draw();

	void releaseResources();

	virtual void composite();

	void drawWindow();

	static int handler_method_create_winnode(int par1,void* par2);
	static int handler_method_draw_window(int par1,void* par2);
	static int handler_method_set_prop(int ptr1,void* ptr2);
	static int handler_method_set_cursor_rect(int ptr1,void* ptr2);
	static int handler_method_set_content(int ptr1,void* ptr2);
	static int handler_method_set_ox(int ptr1,void* ptr2);
	static int handler_method_set_oy(int ptr1,void* ptr2);
	static int handler_method_release(int ptr1,void* ptr2);
	static int handler_method_showtopcur(int ptr1,void* ptr2);

	CCNodeRGBA* m_winNode;
	CCSprite* m_winsp;
	CCNodeRGBA* m_contentNode;
	CCScale9Sprite* m_cursorSp;
	CCClippingNode* m_clipper;
	CCSprite* m_top_cur;
	CCSprite* m_bottom_cur;
};

#endif // WINDOW_H
