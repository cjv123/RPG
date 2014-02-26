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

Viewport::Viewport(int x, int y, int width, int height) : m_clippingNode(NULL)
{
	initViewport(x,y,width,height);
	
}

Viewport::Viewport(Rect *rect) : m_clippingNode(NULL)
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

void Viewport::setRect(Rect *value)
{
	p->rect = value;
	composite();
}


int Viewport::handler_method_composite( int ptr1,void* ptr2 )
{
	Viewport* viewport = (Viewport*)ptr1;
	CCClippingNode* clipper = viewport->m_clippingNode;
	if (clipper==NULL)
	{
		clipper = CCClippingNode::create();
		clipper->retain();
		viewport->m_clippingNode = clipper;
	}
	

	CCSprite* ccsprite = dynamic_cast<CCSprite*>(clipper->getChildByTag(VIEWPORT_SP_TAG));
	if (ccsprite!=NULL)
	{
		Rect* rect = viewport->getRect();
		int ox = viewport->getOX();
		int oy = rgss_y_to_cocos_y(viewport->getOY(),ccsprite->getContentSize().height) - rect->height;

		int x = rect->x - ox;
		int y = rgss_y_to_cocos_y(rect->y,ccsprite->getContentSize().height) - oy + rect->height;

		clipper->setPosition(ccp(x, y));
		clipper->setContentSize(CCSizeMake(rect->width,rect->height));

		CCLayerColor *stencil = CCLayerColor::create(ccc4(255,255,255,255));
		stencil->setPosition(ox,oy);
		stencil->setContentSize(clipper->getContentSize());
		clipper->setStencil(stencil);
	}
		
	

	return 0;
}


extern pthread_mutex_t s_thread_handler_mutex;
void Viewport::composite()
{
	ThreadHandler hander={handler_method_composite,(int)this,(void*)NULL};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}


void Viewport::draw()
{
	
}

/* Disposable */
void Viewport::releaseResources()
{
	delete p;
	if (m_clippingNode)
	{
		m_clippingNode->release();
		m_clippingNode->removeFromParent();
	}
}

CCClippingNode* Viewport::getClippingNode()
{
	return m_clippingNode;
}
