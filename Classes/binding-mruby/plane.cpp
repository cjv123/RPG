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

Plane::Plane(Viewport *viewport)
{
	p = new PlanePrivate();
	p->viewport = viewport;
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

extern pthread_mutex_t s_thread_handler_mutex;
int Plane::handler_method_set_bitmap( int ptr1,void* prt2 )
{
	Plane* plane = (Plane*)ptr1;
	Bitmap* bitmap = (Bitmap*)prt2;

	if (!bitmap->getEmuBitmap())
	{
		return -1;
	}

	Viewport* viewport = plane->p->viewport;
	if (NULL==viewport)
		return -1;

	plane->p->bitmap = bitmap;
	if (plane->p->bitmap->getFilename()!="")
	{
		for (int i=0;i<9;i++)
		{
			int x = i%3-1;
			int y = i/3-1;
			CCSprite* sp = CCSprite::createWithTexture(bitmap->getEmuBitmap()->getTexture());
			plane->m_sprites.push_back(sp);
			sp->setAnchorPoint(ccp(0,1));
			sp->setPosition(ccp(x*sp->getContentSize().width*plane->p->zoomX,
				viewport->getRect()->height-y*sp->getContentSize().height*plane->p->zoomY));
			if (plane->p->z)
				sp->setZOrder(plane->p->z);

			sp->setOpacity(plane->p->opacity);
		}
	}

	


	handler_method_composite((int)plane,(void*)NULL);
	


	return 0;
}

void Plane::setBitmap(Bitmap *value)
{
	if (value == p->bitmap)
		return;

	ThreadHandler hander={handler_method_set_bitmap,(int)this,(void*)value};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->bitmap = value;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);

}


int Plane::handler_method_set_prop( int ptr1,void* ptr2 )
{
	Plane* plane = (Plane*)ptr1;
	if (plane->isDisposed())
		return -1;
	SetPropStruct* propstruct = (SetPropStruct*)ptr2;
	int value = propstruct->value;

	if (plane->m_sprites.size())
	{
		for (int i=0;i<plane->m_sprites.size();i++)
		{
			CCSprite* emubitmap = plane->m_sprites[i];
			switch (propstruct->prop_type)
			{
			case SetPropStruct::z:
				emubitmap->setZOrder(value);
				break;
			case SetPropStruct::ox:
			{
				int x = i%3-1;
				int w =emubitmap->getContentSize().width*plane->p->zoomX;
				int ox = value%w;
				
				emubitmap->setPositionX(x*w - ox);
				break;
			}
			case SetPropStruct::oy:
			{
				int y = i/3-1;
				int h =emubitmap->getContentSize().height*plane->p->zoomY;
				int oy = value%h;

				emubitmap->setPositionY(plane->p->viewport->getRect()->height-y*h + oy);
				break;
			}
			case SetPropStruct::zx:
				emubitmap->setScaleX(value);
				break;
			case SetPropStruct::zy:
				emubitmap->setScaleY(value);
				break;
			case SetPropStruct::angle:
				emubitmap->setRotation(value);
				break;
			case SetPropStruct::visible:
				emubitmap->setVisible(value);
				break;
			case SetPropStruct::opacity:
				emubitmap->setOpacity(value);
				break;
			}

		}
	}
	delete propstruct;
	return 0;
}


void Plane::setOX(int value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::ox;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->ox = value;
	p->quadSourceDirty = true;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Plane::setOY(int value)
{

	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::oy;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->oy = value;
	p->quadSourceDirty = true;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Plane::setZ(int value)
{

	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::z;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->z = value;
	p->quadSourceDirty = true;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Plane::setZoomX(float value)
{
	p->zoomX = value;
	p->quadSourceDirty = true;

	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::zx;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->zoomX = value;
	p->quadSourceDirty = true;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Plane::setZoomY(float value)
{

	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::zy;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->zoomY = value;
	p->quadSourceDirty = true;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
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
	vector<CCSprite*>* sps= (vector<CCSprite*>*)ptr1;

	for (int i=0;i<sps->size();i++)
	{
		sps->at(i)->removeFromParentAndCleanup(true);
	}
	delete sps;
	return 0;
}

void Plane::releaseResources()
{
	vector<CCSprite*>* delsps = new vector<CCSprite*>;
	for (int i=0;i<m_sprites.size();i++)
	{
		delsps->push_back(m_sprites[i]);
	}
	ThreadHandler hander={handler_method_release,(int)delsps,(void*)NULL};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandlerRelease(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

extern pthread_mutex_t s_thread_handler_mutex;

int Plane::handler_method_composite( int ptr1,void* ptr2 )
{
	Plane* plane = (Plane*)ptr1;
	Viewport* viewport = plane->p->viewport;

	if (viewport && viewport->getClippingNode() && plane->m_sprites.size())
	{
		for (int i=0;i<plane->m_sprites.size();i++)
		{
			CCSprite* pSprite = plane->m_sprites[i];
			if(!pSprite->getParent())
				viewport->getClippingNode()->addChild(pSprite);
		}
		
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



