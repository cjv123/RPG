#ifndef BITMAP_H
#define BITMAP_H

#include "disposable.h"
#include "etc-internal.h"
#include "etc.h"

#include <cocos2d.h>
using namespace cocos2d;

class Font;
class ShaderBase;
struct BitmapPrivate;

// FIXME make this class use proper RGSS classes again
class Bitmap : public Disposable
{
public:
	Bitmap(const char *filename);
	Bitmap(int width, int height);
	/* Clone constructor */
	Bitmap(const Bitmap &other);
	~Bitmap();

	int width()  const;
	int height() const;
	IntRect rect() const;

	void blt(int x, int y,
	          Bitmap *source, const IntRect &rect,
	         int opacity = 255);

	void stretchBlt(const IntRect &destRect,
	                 Bitmap *source, const IntRect &sourceRect,
	                int opacity = 255);

	void fillRect(int x, int y,
	              int width, int height,
	              const Vec4 &color);
	void fillRect(const IntRect &rect, const Vec4 &color);

#ifdef RGSS2
	void gradientFillRect(int x, int y,
	                      int width, int height,
	                      const Vec4 &color1, const Vec4 &color2,
	                      bool vertical = false);
	void gradientFillRect(const IntRect &rect,
	                      const Vec4 &color1, const Vec4 &color2,
	                      bool vertical = false);

	void clearRect(int x, int y,
	               int width, int height);
	void clearRect(const IntRect &rect);

	void blur();

	void radialBlur(int angle, int divisions);
#endif

	void clear();

	Vec4 getPixel(int x, int y) const;
	void setPixel(int x, int y, const Vec4 &color);

	void hueChange(int hue);

	enum TextAlign
	{
		Left = 0,
		Center = 1,
		Right = 2
	};

	void drawText(int x, int y,
	              int width, int height,
	              const char *str, int align = Left);

	void drawText(const IntRect &rect,
	              const char *str, int align = Left);

	IntRect textSize(const char *str);

	DECL_ATTR(Font, Font*)
	

	/* <internal> */
	/* Warning: Flushing might change the current
	 * FBO binding (so don't call it during 'draw()' routines */
	void flush() const;
	void ensureNonMega() const;

	/* Binds the backing texture and sets the correct
	 * texture size uniform in shader */
	void bindTex(ShaderBase &shader);

	CCSprite* getEmuBitmap();
	CCRenderTexture* getRenderTexture();
	std::string getFilename();
	IntRect getTextRect();

private:
	static int handler_method_create_sprite(int bitmap_instance ,void* image);
	static int handler_method_blt(int ptr1,void* ptr2);
	static int handler_method_clear(int ptr1,void* ptr2);
	static int handler_method_fill(int prt1, void* ptr2);
	static int handler_method_drawtext(int ptr1,void* ptr2);
	static int handler_method_release(int ptr1,void* ptr2);

	virtual void releaseResources();

	BitmapPrivate *p;
};

#endif // BITMAP_H
