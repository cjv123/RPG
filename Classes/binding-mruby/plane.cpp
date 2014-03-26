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
	setViewport(new Viewport(0,0,SceneMain::getMainLayer()->getContentSize().width,SceneMain::getMainLayer()->getContentSize().height));
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

	plane->p->bitmap = bitmap;
	bitmap->getEmuBitmap()->setAnchorPoint(ccp(0,1));
	bitmap->getEmuBitmap()->setPosition(ccp(0,SceneMain::getMainLayer()->getContentSize().height));

	Viewport* viewport = plane->p->viewport;
	CCClippingNode* clippingNode = CCClippingNode::create();
	plane->m_clippingNode = clippingNode;
	CCLayerColor* maskLayer = CCLayerColor::create(ccc4(255,255,255,255));
	clippingNode->setStencil(maskLayer);

	if (NULL!=viewport)
	{
		handler_method_composite((int)plane,(void*)NULL);
	}
		
	clippingNode->addChild(bitmap->getEmuBitmap());

	SceneMain::getMainLayer()->addChild(clippingNode);
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
	p->viewport->setOX(value);
}

void Plane::setOY(int value)
{
	p->oy = value;
	p->quadSourceDirty = true;
	p->viewport->setOY(value);
}

void Plane::setZ(int value)
{
	p->z = value;
	p->viewport->setZ(value);
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


void Plane::releaseResources()
{
	
}

extern pthread_mutex_t s_thread_handler_mutex;

int Plane::handler_method_composite( int ptr1,void* ptr2 )
{
	Plane* plane = (Plane*)ptr1;
	Viewport* viewport = plane->p->viewport;
	float height = viewport->getRect()->getHeight();
	CCClippingNode* clipper = plane->m_clippingNode;
	if (viewport && clipper)
	{
		CCLayerColor* maskLayer = (CCLayerColor*)clipper->getStencil();
		if(!viewport)
			maskLayer->setContentSize(CCSizeMake(SceneMain::getMainLayer()->getContentSize().width,SceneMain::getMainLayer()->getContentSize().height));
		else
			maskLayer->setContentSize(CCSizeMake(viewport->getRect()->getWidth(),viewport->getRect()->getHeight()));

		maskLayer->setPosition(ccp(viewport->getOX(),
			rgss_y_to_cocos_y(viewport->getOY(),height)-maskLayer->getContentSize().height));

		clipper->setPosition(ccp(-maskLayer->getPositionX()+viewport->getRect()->getX(),
			-maskLayer->getPositionY()+rgss_y_to_cocos_y(viewport->getRect()->getY(),SceneMain::getMainLayer()->getContentSize().height) - maskLayer->getContentSize().height));

		clipper->setZOrder(viewport->getZ());
	}

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



