#include "bitmap.h"
#include "font.h"
#include "../ThreadHandlerManager.h"

#define DISP_CLASS_NAME "bitmap"

struct BitmapPrivate
{
	Font *font;

	BitmapPrivate() : font(0)
	{

	}

	~BitmapPrivate()
	{
		
	}
};

int Bitmap::handler_method_create_sprite(int bitmap_instance ,void* filename)
{
	Bitmap* bitmap = (Bitmap*)bitmap_instance;
	char* filename_c = (char*)filename;
	CCSprite* sp = CCSprite::create(filename_c);
	bitmap->m_emuBitmap = sp;
	sp->retain();
	return 0;
}

extern pthread_mutex_t s_thread_handler_mutex;
Bitmap::Bitmap(const char *filename) : m_emuBitmap(NULL)
{
	ThreadHandler hander={handler_method_create_sprite,(int)this,(void*)filename};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
	m_filename = filename;

	p = new BitmapPrivate;
}

Bitmap::Bitmap(int width, int height) : m_emuBitmap(NULL)
{
	m_width = width;
	m_height = height;
	p = new BitmapPrivate;
}

Bitmap::Bitmap(const Bitmap &other)
{
	m_emuBitmap = other.m_emuBitmap;
	p = new BitmapPrivate;
}

Bitmap::~Bitmap()
{
	delete p;
	dispose();
}

int Bitmap::width() const
{
	if (NULL!=m_emuBitmap)
	{
		return m_emuBitmap->getContentSize().width;
	}
	return 0;
}

int Bitmap::height() const
{
	if (NULL!=m_emuBitmap)
	{
		return m_emuBitmap->getContentSize().height;
	}

	return 0;
}

IntRect Bitmap::rect() const
{
	int x=0;int y=0;
	if (NULL!=m_emuBitmap)
	{
		x = m_emuBitmap->getPositionX();
		y = m_emuBitmap->getPositionY();
	}
	return IntRect(x, y, width(), height());
}

void Bitmap::blt(int x, int y,
                  const Bitmap &source, const IntRect &rect,
                  int opacity)
{
	stretchBlt(IntRect(x, y, rect.w, rect.h),
	           source, rect, opacity);
}

struct Blt_handler_ptr_struct
{
	IntRect desRect;
	IntRect sourceRect;
	Bitmap* source;
};

int Bitmap::handler_method_blt( int ptr1,void* ptr2 )
{
	Bitmap* desbitmap = (Bitmap*)ptr1;
	Blt_handler_ptr_struct* ptrstr = (Blt_handler_ptr_struct*)ptr2;
	CCSprite* desSprite = desbitmap->getEmuBitmap();
	CCSprite* sourceSprite = ptrstr->source->getEmuBitmap();
	if (NULL!=desSprite && NULL!=sourceSprite)
	{
		CCTexture2D* srouceTexture =sourceSprite->getTexture();
		CCSprite* sSprite = CCSprite::createWithTexture(srouceTexture,CCRectMake(ptrstr->sourceRect.x,ptrstr->sourceRect.y,ptrstr->sourceRect.w,ptrstr->sourceRect.h));
		desSprite->addChild(sSprite);
		sSprite->setPosition(ccp(ptrstr->desRect.x,ptrstr->desRect.y));
	}

	delete ptrstr;
	return 0;
}

void Bitmap::stretchBlt(const IntRect &destRect,
                        const Bitmap &source, const IntRect &sourceRect,
                        int opacity)
{
	Bitmap& s = (Bitmap)source;
	Blt_handler_ptr_struct* ptr2= new Blt_handler_ptr_struct;
	ptr2->desRect=destRect;ptr2->sourceRect=sourceRect;ptr2->source = &s;
	ThreadHandler hander={handler_method_blt,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Bitmap::fillRect(int x, int y,
                      int width, int height,
                      const Vec4 &color)
{
	fillRect(IntRect(x, y, width, height), color);
}

struct FillRectStruct
{
	IntRect rect;
	ccColor4B color;
};

int Bitmap::handler_method_fill( int ptr1, void* ptr2 )
{
	FillRectStruct* ptr2str = (FillRectStruct*)ptr2;
	Bitmap* bitmap =(Bitmap*)ptr1;

	if (NULL!=bitmap->m_emuBitmap)
	{
		CCLayerColor* layerColor = CCLayerColor::create(ptr2str->color);
		layerColor->setContentSize(CCSizeMake(ptr2str->rect.w,ptr2str->rect.h));
		layerColor->setPosition(ccp(ptr2str->rect.x,ptr2str->rect.y));
		bitmap->m_emuBitmap->addChild(layerColor);
	}

	delete ptr2str;
	return 0;
}


void Bitmap::fillRect(const IntRect &rect, const Vec4 &color)
{
	FillRectStruct* ptr2 = new FillRectStruct;
	ptr2->rect = rect;
	ptr2->color = ccc4(color.x,color.y,color.z,color.w);
	ThreadHandler hander={handler_method_fill,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

#ifdef RGSS2

void Bitmap::gradientFillRect(int x, int y,
                              int width, int height,
                              const Vec4 &color1, const Vec4 &color2,
                              bool vertical)
{
	gradientFillRect(IntRect(x, y, width, height), color1, color2, vertical);
}

void Bitmap::gradientFillRect(const IntRect &rect,
                              const Vec4 &color1, const Vec4 &color2,
                              bool vertical)
{
	
}

void Bitmap::clearRect(int x, int y, int width, int height)
{
	clearRect(IntRect(x, y, width, height));
}

void Bitmap::clearRect(const IntRect &rect)
{
	
}

void Bitmap::blur()
{
	
}

void Bitmap::radialBlur(int angle, int divisions)
{
	
}

#endif

int Bitmap::handler_method_clear( int ptr1,void* ptr2 )
{
	Bitmap* bitmap = (Bitmap*)ptr1;
	if (NULL!= bitmap->getEmuBitmap())
	{
		bitmap->m_emuBitmap->removeAllChildrenWithCleanup(true);
		bitmap->m_emuBitmap = NULL;
	}

	return 0;
}


void Bitmap::clear()
{
	if (NULL != m_emuBitmap)
	{
		ThreadHandler hander={handler_method_clear,(int)this,(void*)NULL};
		pthread_mutex_lock(&s_thread_handler_mutex);
		ThreadHandlerMananger::getInstance()->pushHandler(hander);
		pthread_mutex_unlock(&s_thread_handler_mutex);
	}
}

Vec4 Bitmap::getPixel(int x, int y) const
{
	Vec4 v;
	return v;
}

void Bitmap::setPixel(int x, int y, const Vec4 &color)
{

}

void Bitmap::hueChange(int hue)
{
	
}

void Bitmap::drawText(int x, int y,
                      int width, int height,
                      const char *str, int align)
{
	drawText(IntRect(x, y, width, height), str, align);
}

struct DrawtextStruct
{
	IntRect rect;
	string str;
	int align;
};

int Bitmap::handler_method_drawtext( int ptr1,void* ptr2 )
{
	Bitmap* bitmap = (Bitmap*)ptr1;
	DrawtextStruct* ptr2struct = (DrawtextStruct*)ptr2;

	CCLabelTTF* label = CCLabelTTF::create(ptr2struct->str.c_str(),FontPrivate::defaultName.c_str(),FontPrivate::defaultSize);
	if (bitmap->p->font)
	{
		label->setFontName(bitmap->p->font->getName());
		label->setFontSize(bitmap->p->font->getSize());
	}
	label->setDimensions(CCSizeMake(ptr2struct->rect.w,ptr2struct->rect.h));
	label->setPosition(ccp(ptr2struct->rect.x,ptr2struct->rect.y));

	if (ptr2struct->align == 1)
		label->setHorizontalAlignment(kCCTextAlignmentCenter);
	else if(ptr2struct->align == 2)
		label->setHorizontalAlignment(kCCTextAlignmentRight);
	bitmap->m_emuBitmap->addChild(label);
	return 0;
}

void Bitmap::drawText(const IntRect &rect, const char *str, int align)
{
	if (NULL==m_emuBitmap)
		return;
	DrawtextStruct* ptr2 = new DrawtextStruct;
	ptr2->rect = rect;ptr2->str = str;ptr2->align = align;
	ThreadHandler hander={handler_method_drawtext,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);
	m_TextRect = rect;
}

/* http://www.lemoda.net/c/utf8-to-ucs2/index.html */
static uint16_t utf8_to_ucs2(const char *_input,
                             const char **end_ptr)
{
	const unsigned char *input =
	        reinterpret_cast<const unsigned char*>(_input);
	*end_ptr = _input;

	if (input[0] == 0)
		return -1;

	if (input[0] < 0x80)
	{
		*end_ptr = _input + 1;

		return input[0];
	}

	if ((input[0] & 0xE0) == 0xE0)
	{
		if (input[1] == 0 || input[2] == 0)
			return -1;

		*end_ptr = _input + 3;

		return (input[0] & 0x0F)<<12 |
		       (input[1] & 0x3F)<<6  |
		       (input[2] & 0x3F);
	}

	if ((input[0] & 0xC0) == 0xC0)
	{
		if (input[1] == 0)
			return -1;

		*end_ptr = _input + 2;

		return (input[0] & 0x1F)<<6  |
		       (input[1] & 0x3F);
	}

	return -1;
}

DEF_ATTR_SIMPLE(Bitmap, Font, Font*, p->font)

IntRect Bitmap::textSize(const char *str)
{
	return m_TextRect;
}

void Bitmap::flush() const
{

}

void Bitmap::ensureNonMega() const
{
	
}

void Bitmap::bindTex(ShaderBase &shader)
{
	
}

void Bitmap::releaseResources()
{
	if (NULL!=m_emuBitmap)
	{
		m_emuBitmap->release();
		m_emuBitmap->removeFromParent();
		m_emuBitmap = NULL;
	}
}

CCSprite* Bitmap::getEmuBitmap()
{
	return m_emuBitmap;
}

std::string Bitmap::getFilename()
{
	return m_filename;
}

