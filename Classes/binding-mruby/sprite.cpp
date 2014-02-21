#include "sprite.h"

#include "bitmap.h"
#include "etc.h"
#include "etc-internal.h"
#include "util.h"

#include <cocos2d.h>
using namespace cocos2d;

#include "../ThreadHandlerManager.h"
#include "../SceneMain.h"


struct SpritePrivate
{
	Bitmap *bitmap;


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
	      tone(&tmp.tone)

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

Sprite::Sprite()
{
	p = new SpritePrivate;
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

DEF_ATTR_SIMPLE(Sprite, BushOpacity, int,    p->bushOpacity)
DEF_ATTR_SIMPLE(Sprite, Opacity,     int,    p->opacity)
DEF_ATTR_SIMPLE(Sprite, Color,       Color*, p->color)
DEF_ATTR_SIMPLE(Sprite, Tone,        Tone*,  p->tone)


int Sprite::handler_method_set_bitmap( int ptr1,void* ptr2 )
{
	Sprite* sprite = (Sprite*)ptr1;
	Bitmap* bitmap = (Bitmap*)ptr2;

	if(NULL!=sprite->p->bitmap)
		delete sprite->p->bitmap;

	sprite->p->bitmap = bitmap;
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

void Sprite::setSrcRect(Rect *rect)
{
	
}

void Sprite::setX(int value)
{
	
}

void Sprite::setY(int value)
{
	
}

void Sprite::setOX(int value)
{
	
}

void Sprite::setOY(int value)
{
	
}

void Sprite::setZoomX(float value)
{
	
}

void Sprite::setZoomY(float value)
{
	
}

void Sprite::setAngle(float value)
{
	
}

void Sprite::setMirror(bool mirrored)
{
	
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
