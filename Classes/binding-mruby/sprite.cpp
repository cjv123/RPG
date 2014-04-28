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
	Rect lastRect;

	bool mirrored;
	int bushDepth;
	float efBushDepth;
	NormValue bushOpacity;
	NormValue opacity;
	BlendType blendType;

	CCRect sceneRect;

	bool isVisible;

	Color *color;
	Color lastcolor;
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

	Color* m_flashColor;
	int m_flashDuration;
	CCSprite* m_sprite;

	SpritePrivate()
	    : bitmap(0),
	      srcRect(&tmp.rect),
	      mirrored(false),
	      bushDepth(0),
	      efBushDepth(0),
	      bushOpacity(128),
	      opacity(255),
	      blendType(BlendNormal),
	      isVisible(true),
	      color(&tmp.color),
	      tone(&tmp.tone),
		  viewport(0),x(0),y(0),z(0),ox(0),oy(0),zx(0),zy(0),angle(0),m_flashColor(0),m_flashDuration(0),m_sprite(0)
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
	p->lastRect = *p->srcRect;
	p->srcRect->addDelegate(this);
	if (NULL != viewport)
		setViewport(viewport);
}

Sprite::~Sprite()
{
	dispose();

	delete p;
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
DEF_ATTR_RD_SIMPLE(Sprite, Color,       Color*, p->color)

DEF_ATTR_SIMPLE(Sprite, BushOpacity, int,    p->bushOpacity)
DEF_ATTR_SIMPLE(Sprite, Tone,        Tone*,  p->tone)


int Sprite::handler_method_set_bitmap( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	Bitmap* bitmap = (Bitmap*)ptr2;

	if (!bitmap->getEmuBitmap())
	{
		return -1;
	}

	if (sprite->p->m_sprite)
	{
		sprite->p->m_sprite->removeFromParentAndCleanup(true);
		sprite->p->m_sprite = NULL;
	}

	sprite->p->bitmap = bitmap;
	if (sprite->p->bitmap->getFilename()!="")
		sprite->p->m_sprite = CCSprite::createWithTexture(bitmap->getEmuBitmap()->getTexture());
	else 
	{
		sprite->p->m_sprite = CCSprite::create();
		sprite->p->m_sprite->setContentSize(CCSizeMake(bitmap->width(),bitmap->height()));
	}

	if (bitmap->getRenderTexture())
	{
		CCSprite* fontsp = CCSprite::createWithTexture(bitmap->getRenderTexture()->getSprite()->getTexture());
		fontsp->setAnchorPoint(ccp(0,0));
		fontsp->setFlipY(true);
		sprite->p->m_sprite->addChild(fontsp);
		fontsp->setPosition(ccp(bitmap->getTextRect().x,rgss_y_to_cocos_y(bitmap->getTextRect().y,bitmap->height())));
	}

	sprite->p->m_sprite->setAnchorPoint(ccp(0,1));
	sprite->p->m_sprite->setPosition(ccp(0,SceneMain::getMainLayer()->getContentSize().height));

	if (sprite->p->z)
		sprite->p->m_sprite->setZOrder(sprite->p->z);
	if (!sprite->p->isVisible)
		sprite->p->m_sprite->setVisible(sprite->p->isVisible);
	sprite->p->m_sprite->setOpacity(sprite->p->opacity);
	if (sprite->p->color)
		handler_method_setcolor((int)sprite,NULL);
	
	Viewport* viewport = sprite->p->viewport;
	if (NULL!=viewport)
	{
		handler_method_composite((int)sprite,(void*)NULL);
	}
	else
		SceneMain::getMainLayer()->addChild(sprite->p->m_sprite);

	return 0;
}

extern pthread_mutex_t s_thread_handler_mutex;
void Sprite::setBitmap(Bitmap *bitmap)
{
	if (bitmap==NULL)
		return;
	ThreadHandler hander={handler_method_set_bitmap,(int)this,(void*)bitmap};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->srcRect->width = bitmap->width();
	p->srcRect->height = bitmap->height();
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);

}

int Sprite::handler_method_set_srcrect( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	CCSprite* emuBitmap = sprite->p->m_sprite;
	if (NULL != emuBitmap)
	{
		Rect* rect = (Rect*)ptr2;
		if (rect->getWidth()==0 || rect->getHeight()==0)
			return -1;
		CCRect texturerect = CCRectMake(rect->getX(),
			rect->getY(),
			rect->getWidth(),rect->getHeight());
		if (sprite->p->bitmap->getFilename()!="")
			emuBitmap->setTextureRect(texturerect);
		emuBitmap->setContentSize(CCSizeMake(rect->width,rect->height));
		emuBitmap->setAnchorPoint(ccp(sprite->p->ox/emuBitmap->getContentSize().width,
			1-sprite->p->oy/emuBitmap->getContentSize().height));

	}
	return 0;
}

void Sprite::setSrcRect(Rect *rect)
{
	if (rect == p->srcRect)
		return;
	ThreadHandler hander={handler_method_set_srcrect,(int)this,(void*)rect};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	p->srcRect = rect;
	p->lastRect = *rect;
	rect->addDelegate(this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

int Sprite::handler_method_set_prop( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	if (sprite->isDisposed())
		return -1;
	SetPropStruct* propstruct = (SetPropStruct*)ptr2;
	int value = propstruct->value;

	if (sprite->p->m_sprite)
	{
		CCSprite* emubitmap = sprite->p->m_sprite;
		switch (propstruct->prop_type)
		{
		case SetPropStruct::x:
			emubitmap->setPositionX(value);
			break;
		case SetPropStruct::y:
			if (sprite->p->viewport)
				emubitmap->setPositionY(rgss_y_to_cocos_y(value,sprite->p->viewport->getRect()->height));
			else
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
		case SetPropStruct::opacity:
			emubitmap->setOpacity(value);
			break;
		}
	}
	delete propstruct;
	return 0;
}


void Sprite::setX(int value)
{
	if (value == p->x)
		return;

	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::x;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->x = value;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
	
}

void Sprite::setY(int value)
{
	if (p->y == value)
		return;
	
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::y;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->y = value;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
	
}

void Sprite::setZ(int value)
{
	if (p->z == value)
		return;

	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::z;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	p->z = value;
	pthread_mutex_unlock(&s_thread_handler_mutex);
	
}

void Sprite::setOX(int value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::ox;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->ox = value;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
	
}

void Sprite::setOY(int value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::oy;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->oy = value;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
	
}

void Sprite::setZoomX(float value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::zx;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	p->zx = value;
	pthread_mutex_unlock(&s_thread_handler_mutex);
	
}

void Sprite::setZoomY(float value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::zy;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	p->zy = value;
	pthread_mutex_unlock(&s_thread_handler_mutex);
	
}

void Sprite::setAngle(float value)
{
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::angle;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	p->angle = value;
	pthread_mutex_unlock(&s_thread_handler_mutex);
	
}

void Sprite::setVisible(bool value)
{
	if(value == p->isVisible)
		return;

	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::visible;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->isVisible = value;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
	
}


int Sprite::handler_method_set_mirror( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	CCSprite* emubitmap = sprite->p->m_sprite;
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
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	p->mirrored = mirrored;
	pthread_mutex_unlock(&s_thread_handler_mutex);
	
}

int Sprite::handler_method_set_opacity( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	CCSprite* emubitmap = sprite->p->m_sprite;
	if (NULL != emubitmap)
	{
		emubitmap->setOpacity((int)ptr2);
	}
	return 0;
}


void Sprite::setOpacity(int value)
{
	if(value == p->opacity)
		return;
	int v = value;
	if (v>255)
		v = 255;
	if (v<0)
		v = 0;

	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::opacity;
	ptr2->value = v;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->opacity = v;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
	
}

void Sprite::setViewport(Viewport* value)
{
	p->viewport = value;
}


void Sprite::setBushDepth(int value)
{
	
}

void Sprite::setBlendType(int type)
{
	
}


int Sprite::handler_method_release( int ptr1,void* ptr2 )
{
	CCSprite* sprite = (CCSprite*)ptr1;
	if (sprite)
	{
		sprite->removeFromParentAndCleanup(true);
	}
	return 0;
}


/* Disposable */
void Sprite::releaseResources()
{
	ThreadHandler hander={handler_method_release,(int)p->m_sprite,(void*)NULL};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandlerRelease(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);

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
		CCSprite* emubitmap = sprite->p->m_sprite;
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
		masksp->runAction(CCSequence::create(fadeout,CCRemoveSelf::create(),NULL));

		
	}

	delete flash_ptr;
	return 0;
}

void Sprite::flash( Color* color,int duration )
{
	p->m_flashColor = color;
	p->m_flashDuration = duration;
}

void Sprite::update()
{
	if (p->m_flashDuration!=0)
	{
		Flash_ptr_struct* ptr2 = new Flash_ptr_struct;
		ptr2->color = p->m_flashColor;
		if (p->m_flashColor==NULL)
			ptr2->color = &p->tmp.color;
		ptr2->duration = p->m_flashDuration;
		ThreadHandler hander={handler_method_flash,(int)this,(void*)ptr2};
		pthread_mutex_lock(&s_thread_handler_mutex);
		ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
		pthread_mutex_unlock(&s_thread_handler_mutex);
		p->m_flashDuration = 0;
	}

	if (!(p->color->red == p->lastcolor.red &&
		p->color->green == p->lastcolor.green &&
		p->color->blue == p->lastcolor.blue &&
		p->color->alpha == p->lastcolor.alpha) )
	{
		ThreadHandler hander={handler_method_setcolor,(int)this,(void*)NULL};
		pthread_mutex_lock(&s_thread_handler_mutex);
		ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
		pthread_mutex_unlock(&s_thread_handler_mutex);
		p->lastcolor = *p->color;
	}
}


int Sprite::handler_method_composite( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	Viewport* viewport = sprite->p->viewport;

	if (viewport && viewport->getClippingNode() && sprite->p->m_sprite)
	{
		CCSprite* pSprite = sprite->p->m_sprite;
		if(!pSprite->getParent())
			viewport->getClippingNode()->addChild(pSprite);
		pSprite->setPosition(ccp(sprite->p->x,rgss_y_to_cocos_y(sprite->p->y,viewport->getRect()->height)));
	}

	return 0;
}

void Sprite::onRectChange()
{
	ThreadHandler hander={handler_method_set_srcrect,(int)this,(void*)p->srcRect};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	p->lastRect = *p->srcRect;
	pthread_mutex_unlock(&s_thread_handler_mutex);
	
}

int Sprite::handler_method_setcolor( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;

	if (sprite->p->m_sprite && sprite->p->color)
	{
		double a = (sprite->p->color->alpha<0)?0:sprite->p->color->alpha;
		double ad = 1-a/255;
		double r = sprite->p->color->red*ad;
		double g = sprite->p->color->green*ad;
		double b = sprite->p->color->blue*ad;
		
		if ( r>0 && g>0 && b>0)
		{
			sprite->p->m_sprite->setColor(ccc3(r,g,b));
		}

		if(sprite->p->color->red==0 && 
			sprite->p->color->green==0 && 
			sprite->p->color->blue==0 && 
			sprite->p->color->alpha==0)
		{
			sprite->p->m_sprite->setColor(ccc3(255,255,255));
		}
		
		
	}
	return 0;
}

void Sprite::setColor(Color* value)
{
	if(value == p->color)
		return;

	ThreadHandler hander={handler_method_setcolor,(int)this,(void*)NULL};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->color = value;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}


