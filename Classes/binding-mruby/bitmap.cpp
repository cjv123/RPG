#include "bitmap.h"
#include "font.h"
#include "../ThreadHandlerManager.h"

#define DISP_CLASS_NAME "bitmap"

struct BitmapPrivate
{
	Font *font;

	BitmapPrivate()
	{
		font = new Font(FontPrivate::defaultName.c_str(),FontPrivate::defaultSize);
	}

	~BitmapPrivate()
	{
		delete font;
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

Bitmap::Bitmap(int width, int height)
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
	
	return 0;
}

int Bitmap::height() const
{
	

	return 0;
}

IntRect Bitmap::rect() const
{
	return IntRect(0, 0, width(), height());
}

void Bitmap::blt(int x, int y,
                  const Bitmap &source, const IntRect &rect,
                  int opacity)
{
	stretchBlt(IntRect(x, y, rect.w, rect.h),
	           source, rect, opacity);
}

void Bitmap::stretchBlt(const IntRect &destRect,
                        const Bitmap &source, const IntRect &sourceRect,
                        int opacity)
{
	
}

void Bitmap::fillRect(int x, int y,
                      int width, int height,
                      const Vec4 &color)
{
	fillRect(IntRect(x, y, width, height), color);
}

void Bitmap::fillRect(const IntRect &rect, const Vec4 &color)
{
	
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

void Bitmap::clear()
{
	
}

Vec4 Bitmap::getPixel(int x, int y) const
{
	Vec4 pixel;

	return pixel;
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

void Bitmap::drawText(const IntRect &rect, const char *str, int align)
{
	
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
	return IntRect(0, 0, 0, 0);
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
	m_emuBitmap->release();
}

CCSprite* Bitmap::getEmuBitmap()
{
	return m_emuBitmap;
}
