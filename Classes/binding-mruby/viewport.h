#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "disposable.h"
#include "util.h"
#include "etc.h"

#include <cocos2d.h>
using namespace cocos2d;

struct ViewportPrivate;

class Viewport : public Disposable
{
public:
	Viewport(int x, int y, int width, int height);
	Viewport(Rect *rect);
	~Viewport();

	DECL_ATTR( Rect,  Rect*  )
	DECL_ATTR( OX,    int    )
	DECL_ATTR( OY,    int    )
	DECL_ATTR( Color, Color* )
	DECL_ATTR( Tone,  Tone*  )

	CCClippingNode* getClippingNode();
private:
	void initViewport(int x, int y, int width, int height);
	void geometryChanged();

	void composite();
	void draw();
	bool isEffectiveViewport(Rect *&, Color *&, Tone *&) const;

	void releaseResources();

	static int handler_method_composite(int ptr1,void* ptr2);

	ViewportPrivate *p;
	CCClippingNode* m_clippingNode;
	friend struct ViewportPrivate;

};

#endif // VIEWPORT_H
