#include "sprite.h"

#include "bitmap.h"
#include "etc.h"
#include "etc-internal.h"
#include "util.h"
#include "binding-util.h"

#include <cocos2d.h>
using namespace cocos2d;

#include "../ThreadHandlerManager.h"
#include "../SceneMain.h"


struct SpritePrivate
{
	Bitmap *bitmap;
	Viewport* viewport;

	Rect *srcRect;

	bool mirrored;
	int bushDepth;
	float efBushDepth;
	NormValue bushOpacity;
	NormValue opacity;
	BlendType blendType;

	CCRect sceneRect;

	/* Would this sprite be visible on
	 * the screen if drawn? */
	bool isVisible;

	Color *color;
	Tone *tone;

	EtcTemps tmp;


	SpritePrivate()
	    : bitmap(0),
	      srcRect(&tmp.rect),
	      mirrored(false),
	      bushDepth(0),
	      efBushDepth(0),
	      bushOpacity(128),
	      opacity(255),
	      blendType(BlendNormal),
	      isVisible(false),
	      color(&tmp.color),
	      tone(&tmp.tone),
		  viewport(0)
	{

		updateSrcRectCon();

	}

	~SpritePrivate()
	{
		if (bitmap)
		{
			delete bitmap;
		}
	}

	void recomputeBushDepth()
	{
		
	}

	void onSrcRectChange()
	{
		
	}

	void updateSrcRectCon()
	{
		
	}

	void updateVisibility()
	{
		
	}

	void prepare()
	{
		
	}
};

Sprite::Sprite(Viewport *viewport)
{
	p = new SpritePrivate;
	setViewport(viewport);
}

Sprite::~Sprite()
{
	dispose();
}

#define DISP_CLASS_NAME "sprite"
DEF_ATTR_RD_SIMPLE(Sprite, Bitmap,    Bitmap*, p->bitmap)
DEF_ATTR_RD_SIMPLE(Sprite, SrcRect,   Rect*,   p->srcRect)
DEF_ATTR_RD_SIMPLE(Sprite, X,         int,     p->bitmap->getEmuBitmap()->getPositionX())
DEF_ATTR_RD_SIMPLE(Sprite, Y,         int,     p->bitmap->getEmuBitmap()->getPositionY())
DEF_ATTR_RD_SIMPLE(Sprite, OX,        int,     p->bitmap->getEmuBitmap()->getAnchorPoint().x)
DEF_ATTR_RD_SIMPLE(Sprite, OY,        int,     p->bitmap->getEmuBitmap()->getAnchorPoint().y)
DEF_ATTR_RD_SIMPLE(Sprite, ZoomX,     float,   p->bitmap->getEmuBitmap()->getScaleX())
DEF_ATTR_RD_SIMPLE(Sprite, ZoomY,     float,   p->bitmap->getEmuBitmap()->getScaleY())
DEF_ATTR_RD_SIMPLE(Sprite, Angle,     float,   p->bitmap->getEmuBitmap()->getRotation())
DEF_ATTR_RD_SIMPLE(Sprite, Mirror,    bool,    p->mirrored)
DEF_ATTR_RD_SIMPLE(Sprite, BushDepth, int,     p->bushDepth)
DEF_ATTR_RD_SIMPLE(Sprite, BlendType, int,     p->blendType)
DEF_ATTR_RD_SIMPLE(Sprite, Width,     int,     p->srcRect->width)
DEF_ATTR_RD_SIMPLE(Sprite, Height,    int,     p->srcRect->height)
DEF_ATTR_RD_SIMPLE(Sprite, Opacity,    int,    p->opacity)

DEF_ATTR_SIMPLE(Sprite, Viewport,    Viewport*, p->viewport)
DEF_ATTR_SIMPLE(Sprite, BushOpacity, int,    p->bushOpacity)
DEF_ATTR_SIMPLE(Sprite, Color,       Color*, p->color)
DEF_ATTR_SIMPLE(Sprite, Tone,        Tone*,  p->tone)


int Sprite::handler_method_set_bitmap( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	Bitmap* bitmap = (Bitmap*)ptr2;

	if(NULL!=sprite->p->bitmap)
	{
		delete sprite->p->bitmap;
	}

	sprite->p->bitmap = bitmap;
	bitmap->getEmuBitmap()->setAnchorPoint(ccp(0,1));
	bitmap->getEmuBitmap()->setPosition(ccp(0,SceneMain::getMainLayer()->getContentSize().height));
	SceneMain::getMainLayer()->addChild(bitmap->getEmuBitmap());

	return 0;
}

extern pthread_mutex_t s_thread_handler_mutex;
void Sprite::setBitmap(Bitmap *bitmap)
{
	ThreadHandler hander={handler_method_set_bitmap,(int)this,(void*)bitmap};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);

}

int Sprite::handler_method_set_srcrect( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	if (NULL != sprite->p->bitmap->getEmuBitmap())
	{
		Rect* rect = (Rect*)ptr2;
		CCRect texturerect = CCRectMake(rect->getX(),rect->getY(),
			rect->getWidth(),rect->getHeight());
		sprite->getBitmap()->getEmuBitmap()->setTextureRect(texturerect);
	}
	return 0;
}

void Sprite::setSrcRect(Rect *rect)
{
	if (rect->width == 0)
		return;
	ThreadHandler hander={handler_method_set_srcrect,(int)this,(void*)rect};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

struct SetPropStruct
{
	enum type{x=0,y,ox,oy,zx,zy,angle};
	SetPropStruct::type prop_type;
	int value;
};

int Sprite::handler_method_set_prop( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	SetPropStruct* propstruct = (SetPropStruct*)ptr2;
	int value = propstruct->value;
	CCSprite* emubitmap = sprite->p->bitmap->getEmuBitmap();
	if (NULL != emubitmap)
	{
		switch (propstruct->prop_type)
		{
		case SetPropStruct::x:
			emubitmap->setPositionX(value);
			break;
		case SetPropStruct::y:
			emubitmap->setPositionY(rgss_y_to_cocos_y(value,SceneMain::getMainLayer()->getContentSize().height));
			break;
		case SetPropStruct::ox:
			emubitmap->setAnchorPoint(ccp(value/emubitmap->getContentSize().width,emubitmap->getAnchorPoint().y));
			break;
		case SetPropStruct::oy:
			emubitmap->setAnchorPoint(ccp(emubitmap->getAnchorPoint().x,1-value/emubitmap->getContentSize().height));
			break;
		case SetPropStruct::zx:
			emubitmap->setScaleX(value);
			break;
		case SetPropStruct::zy:
			emubitmap->setScaleY(value);
			break;
		case SetPropStruct::angle:
			emubitmap->setRotation(value);
			break;
		}
	}
	delete propstruct;
	return 0;
}


void Sprite::setX(int value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::x;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Sprite::setY(int value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::y;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Sprite::setOX(int value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::ox;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Sprite::setOY(int value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::oy;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Sprite::setZoomX(float value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::zx;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Sprite::setZoomY(float value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::zy;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Sprite::setAngle(float value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::angle;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}


int Sprite::handler_method_set_mirror( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	sprite->getBitmap()->getEmuBitmap()->setFlipX((bool)ptr2);
	return 0;
}

void Sprite::setMirror(bool mirrored)
{
	p->mirrored = mirrored;
	CCSprite* emubitmap = p->bitmap->getEmuBitmap();
	if (NULL != emubitmap)
	{
		ThreadHandler hander={handler_method_set_mirror,(int)this,(void*)mirrored};
		pthread_mutex_lock(&s_thread_handler_mutex);
		ThreadHandlerMananger::getInstance()->pushHandler(hander);
		pthread_mutex_unlock(&s_thread_handler_mutex);
	}
}

int Sprite::handler_method_set_opacity( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	sprite->getBitmap()->getEmuBitmap()->setOpacity((int)ptr2);
	return 0;
}


void Sprite::setOpacity(int value)
{
	CCSprite* emubitmap = p->bitmap->getEmuBitmap();
	if (NULL != emubitmap)
	{
		ThreadHandler hander={handler_method_set_opacity,(int)this,(void*)value};
		pthread_mutex_lock(&s_thread_handler_mutex);
		ThreadHandlerMananger::getInstance()->pushHandler(hander);
		pthread_mutex_unlock(&s_thread_handler_mutex);
	}
}


void Sprite::setBushDepth(int value)
{
	
}

void Sprite::setBlendType(int type)
{
	
}

/* Disposable */
void Sprite::releaseResources()
{
	
	delete p;
}

/* SceneElement */
void Sprite::draw()
{
	
}

void Sprite::flash( Color* color,int duration )
{

}

void Sprite::update()
{

}


