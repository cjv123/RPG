#include "window.h"

#include "viewport.h"
#include "bitmap.h"
#include "etc.h"
#include "etc-internal.h"
#include "../ThreadHandlerManager.h"
#include "SceneMain.h"
#include <cocos-ext.h>
using namespace cocos2d::extension;

template<typename T>
struct Sides
{
	T l, r, t, b;
};

template<typename T>
struct Corners
{
	T tl, tr, bl, br;
};

static IntRect backgroundSrc(0, 0, 128, 128);

static IntRect cursorSrc(128, 64, 32, 32);

static IntRect bordersSrc(128,0,64,64);

static IntRect pauseAniSrc[] =
{
	IntRect(160, 64, 16, 16),
	IntRect(176, 64, 16, 16),
	IntRect(160, 80, 16, 16),
	IntRect(176, 80, 16, 16)
};

static Corners<IntRect> cornersSrc =
{
	IntRect(128,  0, 16, 16),
	IntRect(176,  0, 16, 16),
	IntRect(128, 48, 16, 16),
	IntRect(176, 48, 16, 16)
};

static Sides<IntRect> scrollArrowSrc =
{
	IntRect(144, 24,  8, 16),
	IntRect(168, 24,  8, 16),
	IntRect(152, 16, 16,  8),
	IntRect(152, 40, 16,  8)
};

/* Cycling */
static unsigned char cursorAniAlpha[] =
{
	/* Fade out */
	0xFF, 0xF7, 0xEF, 0xE7, 0xDF, 0xD7, 0xCF, 0xC7,
	0xBF, 0xB7, 0xAF, 0xA7, 0x9F, 0x97, 0x8F, 0x87,
	/* Fade in */
	0x7F, 0x87, 0x8F, 0x97, 0x9F, 0xA7, 0xAF, 0xB7,
	0xBF, 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7
};

static elementsN(cursorAniAlpha);

/* Cycling */
static unsigned char pauseAniQuad[] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3
};

static elementsN(pauseAniQuad);

/* No cycle */
static unsigned char pauseAniAlpha[] =
{
	0x00, 0x20, 0x40, 0x60,
	0x80, 0xA0, 0xC0, 0xE0,
	0xFF
};

static elementsN(pauseAniAlpha);


struct WindowPrivate
{
	Bitmap *windowskin;
	Bitmap *contents;
	bool bgStretch;
	Rect *cursorRect;
	bool active;
	bool pause;

	Vec2i sceneOffset;

	Vec2i position;
	Vec2i size;
	Vec2i contentsOffset;

	NormValue opacity;
	NormValue backOpacity;
	NormValue contentsOpacity;

	bool baseVertDirty;
	bool opacityDirty;
	bool baseTexDirty;

	EtcTemps tmp;

	WindowPrivate(Viewport *viewport = 0)
	    : windowskin(0),
	      contents(0),
	      bgStretch(true),
	      cursorRect(&tmp.rect),
	      active(true),
	      pause(false),
	      opacity(255),
	      backOpacity(255),
	      contentsOpacity(255),
	      baseVertDirty(true),
	      opacityDirty(true),
	      baseTexDirty(true)
	{
		
	}

	~WindowPrivate()
	{
		
	}
};

extern pthread_mutex_t s_thread_handler_mutex;
int Window::handler_method_create_winnode( int par1,void* par2 )
{
	Window* window = (Window*)par1;
	window->m_winNode = CCNode::create();
	return 0;
}

Window::Window(Viewport *viewport) : m_winNode(0),m_winsp(0)
{
	p = new WindowPrivate(viewport);

	ThreadHandler hander={handler_method_create_winnode,(int)this,(void*)0};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

Window::~Window()
{
	dispose();
}

void Window::update()
{
	
}

#define DISP_CLASS_NAME "window"

DEF_ATTR_SIMPLE(Window, X,          int,     p->position.x)
DEF_ATTR_SIMPLE(Window, Y,          int,     p->position.y)

DEF_ATTR_RD_SIMPLE(Window, Windowskin,      Bitmap*, p->windowskin)
DEF_ATTR_RD_SIMPLE(Window, Contents,        Bitmap*, p->contents)
DEF_ATTR_RD_SIMPLE(Window, Stretch,         bool,    p->bgStretch)
DEF_ATTR_RD_SIMPLE(Window, CursorRect,      Rect*,   p->cursorRect)
DEF_ATTR_RD_SIMPLE(Window, Active,          bool,    p->active)
DEF_ATTR_RD_SIMPLE(Window, Pause,           bool,    p->pause)
DEF_ATTR_RD_SIMPLE(Window, Width,           int,     p->size.x)
DEF_ATTR_RD_SIMPLE(Window, Height,          int,     p->size.y)
DEF_ATTR_RD_SIMPLE(Window, OX,              int,     p->contentsOffset.x)
DEF_ATTR_RD_SIMPLE(Window, OY,              int,     p->contentsOffset.y)
DEF_ATTR_RD_SIMPLE(Window, Opacity,         int,     p->opacity)
DEF_ATTR_RD_SIMPLE(Window, BackOpacity,     int,     p->backOpacity)
DEF_ATTR_RD_SIMPLE(Window, ContentsOpacity, int,     p->contentsOpacity)

void Window::setWindowskin(Bitmap *value)
{
	p->windowskin = value;
	drawWindow();
}

void Window::setContents(Bitmap *value)
{
	if (p->contents == value)
		return;

	p->contents = value;
}


int Window::handler_method_set_prop( int ptr1,void* ptr2 )
{

	return 0;
}


void Window::setStretch(bool value)
{
	if (value == p->bgStretch)
		return;

	p->bgStretch = value;
}

void Window::setCursorRect(Rect *value)
{
	if (p->cursorRect == value)
		return;

	p->cursorRect = value;

}

void Window::setActive(bool value)
{
	if (p->active == value)
		return;

	p->active = value;
}

void Window::setPause(bool value)
{
	if (p->pause == value)
		return;

	p->pause = value;
}

void Window::setWidth(int value)
{
	if (p->size.x == value)
		return;
	
	p->size.x = value;
	drawWindow();
}

void Window::setHeight(int value)
{
	if (p->size.y == value)
		return;

	p->size.y = value;
	drawWindow();
}

void Window::setOX(int value)
{
	if (p->contentsOffset.x == value)
		return;

	p->contentsOffset.x = value;
}

void Window::setOY(int value)
{
	if (p->contentsOffset.y == value)
		return;

	p->contentsOffset.y = value;
}

void Window::setOpacity(int value)
{
	if (p->opacity == value)
		return;

	p->opacity = value;
}

void Window::setBackOpacity(int value)
{
	if (p->backOpacity == value)
		return;

	p->backOpacity = value;
}

void Window::setContentsOpacity(int value)
{
	if (p->contentsOpacity == value)
		return;

	p->contentsOpacity = value;
}

void Window::draw()
{
}


void Window::setZ(int value)
{
	
}

void Window::setVisible(bool value)
{
	
}

void Window::releaseResources()
{

	delete p;
}

void Window::composite()
{
}


int Window::handler_method_draw_window( int par1,void* par2 )
{
	Window* window = (Window*)par1;
	if (NULL==window->m_winNode)
		return -1;

	CCNode* winnode = window->m_winNode;
	winnode->setContentSize(CCSizeMake(window->p->size.x,window->p->size.y));
	winnode->setAnchorPoint(ccp(0,1));
	winnode->setPosition(ccp(window->p->position.x,
		rgss_y_to_cocos_y(window->p->position.y,SceneMain::getMainLayer()->getContentSize().height)));
	SceneMain::getMainLayer()->addChild(winnode);
	CCSprite* skipsp = window->p->windowskin->getEmuBitmap();
	CCSprite* winsp = CCSprite::createWithTexture(skipsp->getTexture(),
		CCRectMake(backgroundSrc.x,backgroundSrc.y,backgroundSrc.w,backgroundSrc.h));
	winsp->setAnchorPoint(ccp(0,0));
	winnode->addChild(winsp);
	winsp->setScaleX(window->p->size.x/winsp->getContentSize().width);
	winsp->setScaleY(window->p->size.y/winsp->getContentSize().height);

	CCScale9Sprite* border = CCScale9Sprite::createWithSpriteFrame(
		CCSpriteFrame::createWithTexture(skipsp->getTexture(),CCRectMake(bordersSrc.x,bordersSrc.y,bordersSrc.w,bordersSrc.h)));
	border->setCapInsets(CCRectMake(3,3,58,58));
	border->setAnchorPoint(ccp(0,0));
	border->setContentSize(winnode->getContentSize());

	CCLayerColor* masklayer = CCLayerColor::create(ccc4(255,255,255,255));
	masklayer->setContentSize(CCSizeMake(border->getContentSize().width-8,border->getContentSize().height-8));
	masklayer->setPosition(ccp(4,4));
	ccBlendFunc fun = {GL_ZERO,GL_ZERO};
	masklayer->setBlendFunc(fun);
	CCRenderTexture* renderTexture = CCRenderTexture::create(border->getContentSize().width,border->getContentSize().height);
	renderTexture->begin();
	border->visit();
	masklayer->visit();
	renderTexture->end();
	CCSprite* borderf = CCSprite::createWithTexture(renderTexture->getSprite()->getTexture());
	winnode->addChild(borderf);
	borderf->setAnchorPoint(ccp(0,0));
	return 0;
}


void Window::drawWindow()
{
	if (p->size.x >0 && p->size.y>0 && p->windowskin)
	{
		ThreadHandler hander={handler_method_draw_window,(int)this,(void*)0};
		pthread_mutex_lock(&s_thread_handler_mutex);
		ThreadHandlerMananger::getInstance()->pushHandler(hander);
		pthread_mutex_unlock(&s_thread_handler_mutex);
	}
}
