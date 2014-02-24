#include "viewport.h"

#include "etc.h"
#include "util.h"

struct ViewportPrivate
{
	/* Needed for geometry changes */
	Viewport *self;

	Rect *rect;

	Color *color;
	Tone *tone;

	IntRect screenRect;
	int isOnScreen;

	EtcTemps tmp;

	int ox;
	int oy;

	ViewportPrivate(int x, int y, int width, int height, Viewport *self)
	    : self(self),
	      rect(&tmp.rect),
	      color(&tmp.color),
	      tone(&tmp.tone),
	      isOnScreen(false)
	{
		rect->set(x, y, width, height);
		updateRectCon();
	}

	~ViewportPrivate()
	{
	}

	void onRectChange()
	{
		
	}

	void updateRectCon()
	{
		
	}

	void recomputeOnScreen()
	{
		
	}

	bool needsEffectRender(bool flashing)
	{

		return true;
	}
};

Viewport::Viewport(int x, int y, int width, int height)
{
	initViewport(x, y, width, height);
}

Viewport::Viewport(Rect *rect)
{
	initViewport(rect->x, rect->y, rect->width, rect->height);
}

void Viewport::initViewport(int x, int y, int width, int height)
{
	
}

Viewport::~Viewport()
{
	dispose();
}

#define DISP_CLASS_NAME "viewport"

DEF_ATTR_RD_SIMPLE(Viewport, OX,   int,   p->ox)
DEF_ATTR_RD_SIMPLE(Viewport, OY,   int,   p->oy)
DEF_ATTR_RD_SIMPLE(Viewport, Rect, Rect*, p->rect)

DEF_ATTR_SIMPLE(Viewport, Color, Color*, p->color)
DEF_ATTR_SIMPLE(Viewport, Tone, Tone*, p->tone)

void Viewport::setOX(int value)
{
	
}

void Viewport::setOY(int value)
{
	
}

void Viewport::setRect(Rect *value)
{
	
}

/* Scene */
void Viewport::composite()
{
	
}

/* SceneElement */
void Viewport::draw()
{
	composite();
}

/* Disposable */
void Viewport::releaseResources()
{
	delete p;
}

