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

	bool isVisible;

	Color *color;
	Tone *tone;

	EtcTemps tmp;

	int x;
	int y;
	int z;
	int ox;
	int oy;
	int zx;
	int zy;
	int angle;

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
		  viewport(0),x(0),y(0),z(0),ox(0),oy(0),zx(0),zy(0),angle(0)
	{

		updateSrcRectCon();

	}

	~SpritePrivate()
	{

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
DEF_ATTR_RD_SIMPLE(Sprite, X,         int,     p->x)
DEF_ATTR_RD_SIMPLE(Sprite, Y,         int,     p->y)
DEF_ATTR_RD_SIMPLE(Sprite, Z,         int,     p->z)
DEF_ATTR_RD_SIMPLE(Sprite, OX,        int,     p->ox)
DEF_ATTR_RD_SIMPLE(Sprite, OY,        int,     p->oy)
DEF_ATTR_RD_SIMPLE(Sprite, ZoomX,     float,   p->zx)
DEF_ATTR_RD_SIMPLE(Sprite, ZoomY,     float,   p->zy)
DEF_ATTR_RD_SIMPLE(Sprite, Angle,     float,   p->angle)
DEF_ATTR_RD_SIMPLE(Sprite, Mirror,    bool,    p->mirrored)
DEF_ATTR_RD_SIMPLE(Sprite, BushDepth, int,     p->bushDepth)
DEF_ATTR_RD_SIMPLE(Sprite, BlendType, int,     p->blendType)
DEF_ATTR_RD_SIMPLE(Sprite, Width,     int,     p->srcRect->width)
DEF_ATTR_RD_SIMPLE(Sprite, Height,    int,     p->srcRect->height)
DEF_ATTR_RD_SIMPLE(Sprite, Opacity,    int,    p->opacity)
DEF_ATTR_RD_SIMPLE(Sprite, Visible,    bool,    p->isVisible)
DEF_ATTR_RD_SIMPLE(Sprite, Viewport,    Viewport*, p->viewport)

DEF_ATTR_SIMPLE(Sprite, BushOpacity, int,    p->bushOpacity)
DEF_ATTR_SIMPLE(Sprite, Color,       Color*, p->color)
DEF_ATTR_SIMPLE(Sprite, Tone,        Tone*,  p->tone)


int Sprite::handler_method_set_bitmap( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	Bitmap* bitmap = (Bitmap*)ptr2;

	sprite->p->bitmap = bitmap;
	bitmap->getEmuBitmap()->setAnchorPoint(ccp(0,1));
	bitmap->getEmuBitmap()->setPosition(ccp(0,SceneMain::getMainLayer()->getContentSize().height));

	Viewport* viewport = sprite->p->viewport;
	if (NULL!=viewport)
	{
		handler_method_composite((int)sprite,(void*)NULL);
	}

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
	CCSprite* emuBitmap = sprite->p->bitmap->getEmuBitmap();
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

void Sprite::setSrcRect(Rect *rect)
{
	if (rect->width == 0)
		return;
	ThreadHandler hander={handler_method_set_srcrect,(int)this,(void*)rect};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
	p->srcRect = rect;
}

struct SetPropStruct
{
	enum type{x=0,y,z,ox,oy,zx,zy,angle,visible};
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
		case SetPropStruct::z:
			emubitmap->setZOrder(value);
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
		case SetPropStruct::visible:
			emubitmap->setVisible(value);
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
	p->x = value;
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
	p->y = value;
}

void Sprite::setZ(int value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::z;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
	p->z = value;
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
	p->ox = value;
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
	p->oy = value;
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
	p->zx = value;
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
	p->zy = value;
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
	p->angle = value;
}

void Sprite::setVisible(bool value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::visible;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
	p->isVisible = value;
}


int Sprite::handler_method_set_mirror( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	CCSprite* emubitmap = sprite->p->bitmap->getEmuBitmap();
	if (NULL != emubitmap)
	{
		emubitmap->setFlipX((bool)ptr2);
	}
	return 0;
}

void Sprite::setMirror(bool mirrored)
{
	ThreadHandler hander={handler_method_set_mirror,(int)this,(void*)mirrored};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
	p->mirrored = mirrored;
}

int Sprite::handler_method_set_opacity( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	CCSprite* emubitmap = sprite->p->bitmap->getEmuBitmap();
	if (NULL != emubitmap)
	{
		emubitmap->setOpacity((int)ptr2);
	}
	return 0;
}


void Sprite::setOpacity(int value)
{
	ThreadHandler hander={handler_method_set_opacity,(int)this,(void*)value};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
	p->opacity = value;
}

void Sprite::setViewport(Viewport* value)
{
	value->addDelegate(this);
	p->viewport = value;
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

struct Flash_ptr_struct 
{
	Color* color;
	int duration;
};

int Sprite::handler_method_flash( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	Flash_ptr_struct* flash_ptr = (Flash_ptr_struct*)ptr2;
	Color* color = flash_ptr->color;
	float duration = flash_ptr->duration*1.0f/60.0f;
	if (sprite->p->bitmap)
	{
		CCSprite* emubitmap = sprite->p->bitmap->getEmuBitmap();
		CCSprite* pSprite = CCSprite::createWithTexture(emubitmap->getTexture(),emubitmap->getTextureRect());
		pSprite->setAnchorPoint(ccp(0,0));

		CCLayerColor* blendlayer = CCLayerColor::create(ccc4(color->getRed(),color->getGreen(),color->getBlue(),color->getAlpha()));
		blendlayer->setContentSize(pSprite->getContentSize());
		ccBlendFunc blendFunc1 = { GL_DST_ALPHA, GL_ZERO};
		blendlayer->setBlendFunc(blendFunc1);

		CCRenderTexture* rt = CCRenderTexture::create(pSprite->getContentSize().width,pSprite->getContentSize().height);
		rt->begin();
		pSprite->visit();
		blendlayer->visit();
		rt->end();

		CCImage* image = rt->newCCImage();
		CCTexture2D* texture = new CCTexture2D;
		texture->initWithImage(image);
		CCSprite* masksp = CCSprite::createWithTexture(texture,CCRectMake(0,0,texture->getContentSize().width,texture->getContentSize().height));
		masksp->setAnchorPoint(ccp(0,0));
		
		emubitmap->addChild(masksp);

		CCFadeOut* fadeout = CCFadeOut::create(duration);
		masksp->runAction(fadeout);

		
	}

	delete flash_ptr;
	return 0;
}

void Sprite::flash( Color* color,int duration )
{
	m_flashColor = color;
	m_flashDuration = duration;
}

void Sprite::update()
{
	if (m_flashDuration!=0)
	{
		Flash_ptr_struct* ptr2 = new Flash_ptr_struct;
		ptr2->color = m_flashColor;
		ptr2->duration = m_flashDuration;
		ThreadHandler hander={handler_method_flash,(int)this,(void*)ptr2};
		pthread_mutex_lock(&s_thread_handler_mutex);
		ThreadHandlerMananger::getInstance()->pushHandler(hander);
		pthread_mutex_unlock(&s_thread_handler_mutex);
		m_flashDuration = 0;
	}
}


int Sprite::handler_method_composite( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	Viewport* viewport = sprite->p->viewport;

	if (viewport)
	{
		CCSprite* pSprite = sprite->p->bitmap->getEmuBitmap();
		pSprite->setPosition(ccp(viewport->getRect()->x,rgss_y_to_cocos_y(viewport->getRect()->y,SceneMain::getMainLayer()->getContentSize().height)));

		Rect rect(viewport->getOX(),viewport->getOY(),viewport->getRect()->width,viewport->getRect()->height);

		Rect* prect= &rect;
		handler_method_set_srcrect((int)sprite,(void*)(prect));
	}

	return 0;
}


void Sprite::composite()
{
	ThreadHandler hander={handler_method_composite,(int)this,(void*)NULL};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}




