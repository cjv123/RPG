#include "viewport.h"

#include "etc.h"
#include "util.h"
#include "../ThreadHandlerManager.h"
#include "binding-util.h"
#include "../SceneMain.h"

#include "sprite.h"

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
	int z;

	ViewportPrivate(int x, int y, int width, int height, Viewport *self)
	    : self(self),
		rect(&tmp.rect),
		color(&tmp.color),
		tone(&tmp.tone),
		isOnScreen(false),ox(0),oy(0)
	{
		rect->set(x,y,width,height);
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
	initViewport(x,y,width,height);
	
}

Viewport::Viewport(Rect *rect)
{
	initViewport(rect->x,rect->y,rect->width,rect->height);
}

void Viewport::initViewport(int x, int y, int width, int height)
{
	p = new ViewportPrivate(x,y,width,height,this);
	composite();
}

Viewport::~Viewport()
{
	dispose();
}

#define DISP_CLASS_NAME "viewport"

DEF_ATTR_RD_SIMPLE(Viewport, OX,   int,   p->ox)
DEF_ATTR_RD_SIMPLE(Viewport, OY,   int,   p->oy)
DEF_ATTR_RD_SIMPLE(Viewport, Z,   int,   p->z)

DEF_ATTR_RD_SIMPLE(Viewport, Rect, Rect*, p->rect)

DEF_ATTR_SIMPLE(Viewport, Color, Color*, p->color)
DEF_ATTR_SIMPLE(Viewport, Tone, Tone*, p->tone)


void Viewport::setOX(int value)
{
	p->ox = value;
	composite();
}

void Viewport::setOY(int value)
{
	p->oy = value;
	composite();
}

void Viewport::setZ(int value)
{
	p->z = value;
	composite();
}

void Viewport::setRect(Rect *value)
{
	p->rect = value;
	composite();
}


int Viewport::handler_method_composite( int ptr1,void* ptr2 )
{
	Viewport* viewport = (Viewport*)ptr1;
	
	

	return 0;
}


extern pthread_mutex_t s_thread_handler_mutex;
void Viewport::composite()
{
// 	ThreadHandler hander={handler_method_composite,(int)this,(void*)NULL};
// 	pthread_mutex_lock(&s_thread_handler_mutex);
// 	ThreadHandlerMananger::getInstance()->pushHandler(hander);
// 	pthread_mutex_unlock(&s_thread_handler_mutex);
	if (m_viewPortDelegates.size()>0)
	{
		for (int i=0;i<m_viewPortDelegates.size();i++)
		{
			m_viewPortDelegates[i]->composite();
		}
	}
}


void Viewport::draw()
{
	
}

/* Disposable */
void Viewport::releaseResources()
{
	delete p;
	
}



void Viewport::addDelegate( ViewPortDelegate* delegate )
{
	m_viewPortDelegates.push_back(delegate);
}
