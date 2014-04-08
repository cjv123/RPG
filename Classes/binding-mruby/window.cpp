#include "window.h"

#include "viewport.h"
#include "bitmap.h"
#include "etc.h"
#include "etc-internal.h"
#include "../ThreadHandlerManager.h"
#include "SceneMain.h"

#define BORDER_TAG 1000

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

static int window_z_base = 100;
static int cursor_z = 200;
static int bgOpacity = 255;

static IntRect backgroundSrc(0, 0, 128, 128);

static IntRect cursorSrc(128, 64, 32, 32);

static IntRect bordersSrc(128,0,64,64);

static IntRect topcurSrc(160,64,16,16);

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
	Rect lastRect;
	bool active;
	bool pause;
	bool visible;
	int z;

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
	      baseTexDirty(true),
		  visible(true),z(0)
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
	window->m_winNode = CCNodeRGBA::create();
	SceneMain::getMainLayer()->addChild(window->m_winNode);

	return 0;
}

Window::Window(Viewport *viewport) : m_winNode(0),m_winsp(0),m_contentNode(0),m_cursorSp(0),m_clipper(0),m_top_cur(0),m_bottom_cur(0)
{
	p = new WindowPrivate(viewport);

	ThreadHandler hander={handler_method_create_winnode,(int)this,(void*)0};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

Window::~Window()
{
	dispose();

	delete p;
}

void Window::update()
{
	if ( !(p->lastRect.x == p->cursorRect->x && p->lastRect.y == p->cursorRect->y && p->lastRect.width == p->cursorRect->width && p->lastRect.height==p->cursorRect->height) )
	{
		ThreadHandler hander={handler_method_set_cursor_rect,(int)this,(void*)p->cursorRect};
		pthread_mutex_lock(&s_thread_handler_mutex);
		ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
		pthread_mutex_unlock(&s_thread_handler_mutex);
		p->lastRect = *p->cursorRect;
	}
	
}

#define DISP_CLASS_NAME "window"

DEF_ATTR_RD_SIMPLE(Window, X,          int,     p->position.x)
DEF_ATTR_RD_SIMPLE(Window, Y,          int,     p->position.y)

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
DEF_ATTR_RD_SIMPLE(Window, Visible, bool,     p->visible)
DEF_ATTR_RD_SIMPLE(Window, Z, int,     p->z)

void Window::setWindowskin(Bitmap *value)
{
	p->windowskin = value;
	drawWindow();
}


int Window::handler_method_set_content( int ptr1,void* ptr2 )
{
	Window* window = (Window*)ptr1;
	Bitmap* content = window->p->contents;
	Bitmap* delcontent = (Bitmap*)ptr2;
	if (delcontent)
	{
		if (delcontent->getEmuBitmap() && delcontent->getEmuBitmap()->getParent())
		{
			delcontent->getEmuBitmap()->removeFromParentAndCleanup(true);
		}
		delcontent->dispose();
	}
	
	if (window->m_contentNode &&
		window->m_contentNode->getContentSize().width &&
		window->m_contentNode->getContentSize().height &&
		content && content->getEmuBitmap())
	{
		CCSprite* contentsp = content->getEmuBitmap();
		if (!contentsp->getParent())
			window->m_contentNode->addChild(contentsp);
		contentsp->setAnchorPoint(ccp(0,1));
		
		if (contentsp->getContentSize().height>window->m_contentNode->getContentSize().height)
		{
			window->m_contentNode->setContentSize(contentsp->getContentSize());
		}
		contentsp->setPosition(ccp(0,rgss_y_to_cocos_y(0,window->m_contentNode->getContentSize().height)));
		handler_method_showtopcur((int)window,(void*)0);
	}
	return 0;
}

void Window::setContents(Bitmap *value)
{
	if (p->contents == value)
		return;

	ThreadHandler hander={handler_method_set_content,(int)this,(void*)p->contents};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->contents = value;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);

}

int Window::handler_method_set_prop( int ptr1,void* ptr2 )
{
	Window* window = (Window*)ptr1;
	SetPropStruct* propstruct = (SetPropStruct*)ptr2;
	int value = propstruct->value;
	switch (propstruct->prop_type)
	{
	case SetPropStruct::x:
		window->m_winNode->setPositionX(value);
		if(window->m_clipper)
			window->m_clipper->setPositionX(value+16);
		break;
	case SetPropStruct::y:
		window->m_winNode->setPositionY(rgss_y_to_cocos_y(value,SceneMain::getMainLayer()->getContentSize().height));
		if(window->m_clipper)
			window->m_clipper->setPositionY(window->m_winNode->getPositionY()-window->m_winNode->getContentSize().height+16);
		break;
	case SetPropStruct::z:
		//window->m_winNode->setZOrder(window_z_base+value);
		window->m_winNode->setZOrder(value);
		window->m_clipper->setZOrder(value+2);
		break;
	case SetPropStruct::visible:
		window->m_winNode->setVisible((bool)value);
		window->m_contentNode->setVisible((bool)value);
		break;
	case SetPropStruct::opacity:
		{
			window->m_winNode->setOpacity(value);
			CCSprite* bordersp = (CCSprite*)window->m_winNode->getChildByTag(BORDER_TAG);
			if (bordersp)
			{
				bordersp->setOpacity(value);
			}
			CCRenderTexture* fontRender = window->p->contents->getRenderTexture();
			if (fontRender)
			{
				CCSprite* sp = fontRender->getSprite();
				sp->setOpacity(value);
				fontRender->clear(0,0,0,0);
				fontRender->begin();
				sp->visit();
				fontRender->end();
			}
			break;
		}
	case SetPropStruct::back_opacity:
		if (window->m_winsp)
		{
			window->m_winsp->setOpacity(value);
		}
		break;
	case SetPropStruct::contents_opacity:
		if (window->m_contentNode)
		{
			window->m_contentNode->setOpacity(value);
		}
		break;
	}

	delete propstruct;
	
	return 0;
}

void Window::setX(int value)
{
	p->position.x = value;
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::x;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Window::setY(int value)
{
	p->position.y = value;
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::y;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Window::setStretch(bool value)
{
	if (value == p->bgStretch)
		return;

	p->bgStretch = value;
}


int Window::handler_method_set_cursor_rect( int ptr1,void* ptr2 )
{
	Window* window = (Window*)ptr1;

	if(window->p->cursorRect 
		&& window->p->windowskin
		&& window->p->windowskin->getEmuBitmap()
		&& window->m_contentNode)
	{
		CCSprite* skipsp = window->p->windowskin->getEmuBitmap();
		CCScale9Sprite* cursorSp = NULL;
		if(window->m_cursorSp && window->m_cursorSp->getParent())
		{
			cursorSp = window->m_cursorSp;
		}
		else
		{
			cursorSp = CCScale9Sprite::createWithSpriteFrame(
				CCSpriteFrame::createWithTexture(skipsp->getTexture(),CCRectMake(cursorSrc.x,cursorSrc.y,cursorSrc.w,cursorSrc.h)));
			window->m_winNode->addChild(cursorSp,cursor_z);
			window->m_cursorSp = cursorSp;
			cursorSp->setAnchorPoint(ccp(0,1));
			cursorSp->setCapInsets(CCRectMake(1,1,cursorSrc.w-2,cursorSrc.h-2));
			
			if (window->p->active)
			{
				CCSequence* seq = CCSequence::create(CCFadeTo::create(0.3f,50),CCFadeTo::create(0.3f,255),NULL);
				cursorSp->runAction(CCRepeatForever::create(seq));
			}
		}

		if (window->p->cursorRect->width == 0 && window->p->cursorRect->height==0)
		{
			if (cursorSp->isVisible())
				cursorSp->setVisible(false);
		}
		else
		{
			if (!cursorSp->isVisible())
				cursorSp->setVisible(true);
		}

		cursorSp->setContentSize(CCSizeMake(window->p->cursorRect->width,window->p->cursorRect->height));
		cursorSp->setPosition(ccp(window->p->cursorRect->x+16,rgss_y_to_cocos_y(window->p->cursorRect->y,window->m_clipper->getContentSize().height)+16));
	}
	return 0;
}


void Window::setCursorRect(Rect *value)
{
	if (p->cursorRect == value)
		return;

	p->cursorRect = value;
	p->lastRect = *value;
	ThreadHandler hander={handler_method_set_cursor_rect,(int)this,(void*)value};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
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

int Window::handler_method_set_ox( int ptr1,void* ptr2 )
{
	Window* window = (Window*)ptr1;
	Vec2i* pos = (Vec2i*)ptr2;
	CCNodeRGBA* contentNode = window->m_contentNode;
	contentNode->setPositionX(- pos->x);
	delete pos;
	return 0;
}

int Window::handler_method_set_oy( int ptr1,void* ptr2 )
{
	Window* window = (Window*)ptr1;
	Vec2i* pos = (Vec2i*)ptr2;
	CCNodeRGBA* contentNode = window->m_contentNode;
	contentNode->setPositionY(rgss_y_to_cocos_y(- pos->y,window->m_clipper->getContentSize().height));
	delete pos;

	handler_method_showtopcur((int)window,(void*)0);
	return 0;
}

void Window::setOX(int value)
{
	if (p->contentsOffset.x == value)
		return;

	p->contentsOffset.x = value;
	ThreadHandler hander={handler_method_set_ox,(int)this,new Vec2i(p->contentsOffset.x,p->contentsOffset.y)};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Window::setOY(int value)
{
	if (p->contentsOffset.y == value)
		return;

	p->contentsOffset.y = value;
	ThreadHandler hander={handler_method_set_oy,(int)this,new Vec2i(p->contentsOffset.x,p->contentsOffset.y)};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Window::setOpacity(int value)
{
	if (p->opacity == value)
		return;

	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::opacity;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->opacity = value;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Window::setBackOpacity(int value)
{
	if (p->backOpacity == value)
		return;

	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::back_opacity;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->backOpacity = value;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Window::setContentsOpacity(int value)
{
	if (p->contentsOpacity == value)
		return;
	
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::contents_opacity;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->contentsOpacity = value;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Window::draw()
{
}


void Window::setZ(int value)
{
	if (value == p->z)
		return;

	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::z;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->z = value;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Window::setVisible(bool value)
{
	p->visible = value;
	SetPropStruct* ptr2 = new SetPropStruct;
	ptr2->prop_type = SetPropStruct::visible;
	ptr2->value = value;
	ThreadHandler hander={handler_method_set_prop,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}


int Window::handler_method_release( int ptr1,void* ptr2 )
{
	CCNodeRGBA* winNode = (CCNodeRGBA*)ptr1;
	CCClippingNode* clipper = (CCClippingNode*)ptr2;
	if (winNode)
	{
		winNode->removeAllChildrenWithCleanup(true);
		winNode->removeFromParentAndCleanup(true);
	}

	if (clipper)
	{
		clipper->removeAllChildrenWithCleanup(true);
		clipper->removeFromParentAndCleanup(true);
	}

	return 0;
}


void Window::releaseResources()
{
	ThreadHandler hander={handler_method_release,(int)m_winNode,(void*)m_clipper};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandlerRelease(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Window::composite()
{
}


int Window::handler_method_draw_window( int par1,void* par2 )
{
	Window* window = (Window*)par1;
	if (NULL==window->m_winNode)
		return -1;

	CCNodeRGBA* winnode = window->m_winNode;
	winnode->removeAllChildrenWithCleanup(true);
	winnode->setContentSize(CCSizeMake(window->p->size.x,window->p->size.y));
	winnode->setAnchorPoint(ccp(0,1));
	winnode->setPosition(ccp(window->p->position.x,
		rgss_y_to_cocos_y(window->p->position.y,SceneMain::getMainLayer()->getContentSize().height)));

	CCSprite* skipsp = window->p->windowskin->getEmuBitmap();

	if(window->m_top_cur==NULL)
	{
		window->m_top_cur = CCSprite::createWithTexture(skipsp->getTexture(),
			CCRectMake(topcurSrc.x,topcurSrc.y,topcurSrc.w,topcurSrc.h));
		window->m_top_cur->setFlipY(true);
		winnode->addChild(window->m_top_cur,cursor_z);
		window->m_top_cur->setPosition(ccp(winnode->getContentSize().width/2,winnode->getContentSize().height - 8));
		window->m_top_cur->setVisible(false);
	}

	if (window->m_bottom_cur==NULL)
	{
		window->m_bottom_cur = CCSprite::createWithTexture(skipsp->getTexture(),
			CCRectMake(topcurSrc.x,topcurSrc.y,topcurSrc.w,topcurSrc.h));
		winnode->addChild(window->m_bottom_cur,cursor_z);
		window->m_bottom_cur->setPosition(ccp(winnode->getContentSize().width/2,8));
		window->m_bottom_cur->setVisible(false);
	}
	
	CCSprite* winsp = CCSprite::createWithTexture(skipsp->getTexture(),
		CCRectMake(backgroundSrc.x,backgroundSrc.y,backgroundSrc.w,backgroundSrc.h));
	winsp->setAnchorPoint(ccp(0,0));
	winnode->addChild(winsp);
	winsp->setScaleX(window->p->size.x/winsp->getContentSize().width);
	winsp->setScaleY(window->p->size.y/winsp->getContentSize().height);
	winsp->setOpacity(bgOpacity);
	window->m_winsp = winsp;

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
	borderf->setTag(BORDER_TAG);
	winnode->addChild(borderf);
	borderf->setAnchorPoint(ccp(0,0));
	winnode->setOpacity(window->p->opacity);

	if (window->m_contentNode==NULL)
	{
		window->m_contentNode = CCNodeRGBA::create();
		window->m_contentNode->setCascadeColorEnabled(true);
	}

	CCNodeRGBA* contentNode = window->m_contentNode;
	contentNode->setContentSize(CCSizeMake(window->p->size.x-16*2,window->p->size.y-16*2));
	contentNode->setOpacity(window->p->contentsOpacity);
	contentNode->setAnchorPoint(ccp(0,1));
	contentNode->setPosition(ccp(0,rgss_y_to_cocos_y(0,contentNode->getContentSize().height)));

	if (window->m_clipper == NULL)
	{
		window->m_clipper = CCClippingNode::create();
		CCClippingNode* clipper = window->m_clipper;
		CCLayerColor* maskLayer = CCLayerColor::create(ccc4(255,255,255,255));
		clipper->setStencil(maskLayer);
		maskLayer->setContentSize(contentNode->getContentSize());
		//maskLayer->setPosition(ccp(0,rgss_y_to_cocos_y(0,clipper->getContentSize().height)-maskLayer->getContentSize().height));
		clipper->addChild(contentNode);
		clipper->setPosition(ccp(winnode->getPositionX()+16,winnode->getPositionY() - winnode->getContentSize().height+16));
		clipper->setContentSize(maskLayer->getContentSize());
		SceneMain::getMainLayer()->addChild(clipper);
	}

	handler_method_set_content( (int)window,(void*)0);
	handler_method_set_cursor_rect((int)window,(void*)0);
	handler_method_showtopcur((int)window,(void*)0);
	return 0;
}


void Window::drawWindow()
{
	if (p->size.x >0 && p->size.y>0 && p->windowskin)
	{
		ThreadHandler hander={handler_method_draw_window,(int)this,(void*)0};
		pthread_mutex_lock(&s_thread_handler_mutex);
		ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
		pthread_mutex_unlock(&s_thread_handler_mutex);
	}
}

int Window::handler_method_showtopcur( int ptr1,void* ptr2 )
{
	Window* window = (Window*)ptr1;

	if (!(window->m_contentNode && window->m_winNode && window->m_top_cur && window->m_bottom_cur && window->m_clipper))
	{
		return -1;
	}
	
	float bottomy =window->m_contentNode->getPositionY() - window->m_contentNode->getContentSize().height;
	if (bottomy<0)
		window->m_bottom_cur->setVisible(true);
	else
		window->m_bottom_cur->setVisible(false);

	float topy = window->m_contentNode->getPositionY();
	if (topy>window->m_clipper->getContentSize().height)
		window->m_top_cur->setVisible(true);
	else
		window->m_top_cur->setVisible(false);

	return 0;
}

