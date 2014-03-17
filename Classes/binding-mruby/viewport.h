#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "disposable.h"
#include "util.h"
#include "etc.h"

#include <cocos2d.h>
using namespace cocos2d;
#include <vector>

struct ViewportPrivate;

class ViewPortDelegate
{
public:
	virtual void composite() = 0;
};

class Viewport : public Disposable
{
public:
	Viewport(int x, int y, int width, int height);
	Viewport(Rect *rect);
	~Viewport();

	DECL_ATTR( Rect,  Rect*  )
	DECL_ATTR( OX,    int    )
	DECL_ATTR( OY,    int    )
	DECL_ATTR( Z,    int    )
	DECL_ATTR( Color, Color* )
	DECL_ATTR( Tone,  Tone*  )

	void addDelegate(ViewPortDelegate* delegate);
private:
	void initViewport(int x, int y, int width, int height);
	void geometryChanged();

	void composite();
	void draw();
	bool isEffectiveViewport(Rect *&, Color *&, Tone *&) const;

	void releaseResources();

	static int handler_method_composite(int ptr1,void* ptr2);

	ViewportPrivate *p;
	friend struct ViewportPrivate;
	std::vector<ViewPortDelegate*> m_viewPortDelegates;
};

#endif // VIEWPORT_H
