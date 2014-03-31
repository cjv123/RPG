#include "plane.h"

#include "bitmap.h"
#include "etc.h"
#include "util.h"
#include "ThreadHandlerManager.h"
#include "sprite.h"
#include "SceneMain.h"

struct PlanePrivate
{
	Bitmap *bitmap;
	NormValue opacity;
	BlendType blendType;
	Color *color;
	Tone *tone;
	Viewport* viewport;

	int ox, oy,z;
	float zoomX, zoomY;


	bool quadSourceDirty;

	EtcTemps tmp;

	PlanePrivate()
	    : bitmap(0),
	      opacity(255),
	      blendType(BlendNormal),
	      color(&tmp.color),
	      tone(&tmp.tone),
	      ox(0), oy(0),
	      zoomX(1), zoomY(1),
	      quadSourceDirty(false),
		  viewport(NULL)
	{
	}

	~PlanePrivate()
	{
	}

	
};

Plane::Plane(Viewport *viewport) : m_clippingNode(0)
{
	p = new PlanePrivate();
}

#define DISP_CLASS_NAME "plane"

DEF_ATTR_RD_SIMPLE(Plane, Bitmap,    Bitmap*, p->bitmap)
DEF_ATTR_RD_SIMPLE(Plane, OX,        int,     p->ox)
DEF_ATTR_RD_SIMPLE(Plane, OY,        int,     p->oy)
DEF_ATTR_RD_SIMPLE(Plane, Z,        int,     p->z)
DEF_ATTR_RD_SIMPLE(Plane, ZoomX,     float,   p->zoomX)
DEF_ATTR_RD_SIMPLE(Plane, ZoomY,     float,   p->zoomY)
DEF_ATTR_RD_SIMPLE(Plane, BlendType, int,     p->blendType)
DEF_ATTR_RD_SIMPLE(Plane, Viewport, Viewport*,   p->viewport)

DEF_ATTR_SIMPLE(Plane, Opacity, int,     p->opacity)
DEF_ATTR_SIMPLE(Plane, Color,   Color*,  p->color)
DEF_ATTR_SIMPLE(Plane, Tone,    Tone*,   p->tone)

Plane::~Plane()
{
	dispose();
	delete p;
}

int Plane::handler_method_set_bitmap( int ptr1,void* prt2 )
{
	Plane* plane = (Plane*)ptr1;
	Bitmap* bitmap = (Bitmap*)prt2;

	return 0;
}

void Plane::setBitmap(Bitmap *value)
{
	p->bitmap = value;

	if (!value)
		return;
}

void Plane::setOX(int value)
{
	p->ox = value;
	p->quadSourceDirty = true;
}

void Plane::setOY(int value)
{
	p->oy = value;
	p->quadSourceDirty = true;
}

void Plane::setZ(int value)
{
	p->z = value;
}

void Plane::setZoomX(float value)
{
	p->zoomX = value;
	p->quadSourceDirty = true;
}

void Plane::setZoomY(float value)
{
	p->zoomY = value;
	p->quadSourceDirty = true;
}

void Plane::setViewport(Viewport* value)
{
	p->viewport = value;
	value->addDelegate(this);
}

void Plane::setBlendType(int value)
{
	switch (value)
	{
	default :
	case BlendNormal :
		p->blendType = BlendNormal;
		return;
	case BlendAddition :
		p->blendType = BlendAddition;
		return;
	case BlendSubstraction :
		p->blendType = BlendSubstraction;
		return;
	}
}


void Plane::draw()
{
	
}


void Plane::aboutToAccess() const
{
	
}

extern pthread_mutex_t s_thread_handler_mutex;

int Plane::handler_method_release( int ptr1,void* ptr2 )
{
	CCClippingNode* clippingNode = (CCClippingNode*)ptr1;
	if (clippingNode)
	{
		clippingNode->removeFromParentAndCleanup(true);
	}
	return 0;
}

void Plane::releaseResources()
{
	ThreadHandler hander={handler_method_release,(int)m_clippingNode,(void*)NULL};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandlerRelease(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

extern pthread_mutex_t s_thread_handler_mutex;

int Plane::handler_method_composite( int ptr1,void* ptr2 )
{
	Plane* plane = (Plane*)ptr1;

	return 0;
}

void Plane::composite()
{
	ThreadHandler hander={handler_method_composite,(int)this,(void*)NULL};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

int Plane::handler_method_set_srcrect( int prt1,void* ptr2 )
{
	Plane* plane = (Plane*)prt1;
	CCSprite* emuBitmap = plane->p->bitmap->getEmuBitmap();
	if (NULL != emuBitmap)
	{
		Rect* rect = (Rect*)ptr2;
		CCRect texturerect = CCRectMake(rect->getX(),
			rect->getY(),
			rect->getWidth(),rect->getHeight());
		emuBitmap->setTextureRect(texturerect);
	}
	return 0;
}



