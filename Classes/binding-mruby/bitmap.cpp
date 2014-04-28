#include "bitmap.h"
#include "font.h"
#include "../ThreadHandlerManager.h"
#include "binding-util.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRIGONOMETRY_H
#include FT_GLYPH_H
#include FT_STROKER_H
#include FT_SYNTHESIS_H

#define DISP_CLASS_NAME "bitmap"

struct BitmapPrivate
{
	Font *font;

	CCSprite* m_emuBitmap;
	std::string m_filename;
	float m_width;
	float m_height;
	IntRect m_TextRect;
	CCRenderTexture* m_fontRender;

	BitmapPrivate() : m_emuBitmap(NULL),m_fontRender(0)
	{
		font = new Font();
	}

	~BitmapPrivate()
	{

	}
};

int Bitmap::handler_method_create_sprite(int bitmap_instance ,void* image)
{
	Bitmap* bitmap = (Bitmap*)bitmap_instance;
	CCSprite* sp = NULL;
	CCImage* ccimage = (CCImage*)image;
	if (image)
	{
		CCTexture2D* texture = new CCTexture2D;
		texture->autorelease();
		texture->initWithImage(ccimage);
		sp = CCSprite::createWithTexture(texture);
		
		CCAssert(sp,"fuck! image file error!");

		bitmap->p->m_width = sp->getContentSize().width;
		bitmap->p->m_height = sp->getContentSize().height;
	}
	else
	{
		sp = CCSprite::create();
		sp->setContentSize(CCSizeMake(bitmap->p->m_width,bitmap->p->m_height));
	}
	//sp->getTexture()->setAliasTexParameters();
	sp->retain();
	bitmap->p->m_emuBitmap = sp;
	
	delete ccimage;
	return 0;
}

extern pthread_mutex_t s_thread_handler_mutex;
Bitmap::Bitmap(const char *filename) : p(0)
{
	p = new BitmapPrivate;

	string* path = new string(filename);

	CCImage* image = new CCImage;
	string filename_c=*path;
	if (path->find(".png") == string::npos)
	{
		filename_c=*path + ".png";
		if (filename_c.find("Characters") != string::npos)
		{
			int pos = filename_c.find('-');
			if(pos!=string::npos)
			{
				char c = filename_c[pos+1];
				filename_c[pos+1]=toupper(c);
			}
		}
	}

	bool ret =image->initWithImageFile(filename_c.c_str());

	if (!ret)
	{
		filename_c = *path + ".jpg";
		ret = image->initWithImageFile(filename_c.c_str(),CCImage::kFmtJpg);
	}

	if (ret)
	{
		ThreadHandler hander={handler_method_create_sprite,(int)this,(void*)image};
		pthread_mutex_lock(&s_thread_handler_mutex);
		p->m_filename = filename_c;
		p->m_width = image->getWidth();
		p->m_height = image->getHeight();
		ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
		pthread_mutex_unlock(&s_thread_handler_mutex);
	}


}

Bitmap::Bitmap(int width, int height)  : p(0)
{
	p = new BitmapPrivate;
	p->m_width = width;
	p->m_height = height;

	ThreadHandler hander={handler_method_create_sprite,(int)this,(void*)0};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);

}

Bitmap::Bitmap(const Bitmap &other) : p(0)
{
	p = new BitmapPrivate;
	p->m_emuBitmap = other.p->m_emuBitmap;
}

Bitmap::~Bitmap()
{
	dispose();

}

int Bitmap::width() const
{
	return p->m_width;
}

int Bitmap::height() const
{
	return p->m_height;
}

IntRect Bitmap::rect() const
{
	int x=0;int y=0;
	if (NULL!=p->m_emuBitmap)
	{
		x = p->m_emuBitmap->getPositionX();
		y = p->m_emuBitmap->getPositionY();
	}
	return IntRect(x, y, width(), height());
}

void Bitmap::blt(int x, int y,
                   Bitmap *source, const IntRect &rect,
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
	int opacity;
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
		sSprite->setAnchorPoint(ccp(0,1));
		sSprite->setPosition(ccp(ptrstr->desRect.x,rgss_y_to_cocos_y(ptrstr->desRect.y,desbitmap->p->m_height)));
		sSprite->setOpacity(ptrstr->opacity);
	}

	delete ptrstr;
	return 0;
}

void Bitmap::stretchBlt(const IntRect &destRect,
                        Bitmap *source, const IntRect &sourceRect,
                        int opacity)
{
	Blt_handler_ptr_struct* ptr2= new Blt_handler_ptr_struct;
	ptr2->desRect=destRect;ptr2->sourceRect=sourceRect;ptr2->source = source;ptr2->opacity = opacity;
	ThreadHandler hander={handler_method_blt,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
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
	Vec4 color;
};

int Bitmap::handler_method_fill( int ptr1, void* ptr2 )
{
	FillRectStruct* ptr2str = (FillRectStruct*)ptr2;
	Bitmap* bitmap =(Bitmap*)ptr1;
	
	if (NULL!=bitmap->p->m_emuBitmap)
	{
		Vec4 color = ptr2str->color;
		CCLayerColor* layerColor = CCLayerColor::create(ccc4(color.x*255,color.y*255,color.z*255,color.w*255));
		layerColor->setContentSize(CCSizeMake(ptr2str->rect.w,ptr2str->rect.h));
		layerColor->setAnchorPoint(ccp(0,1));
		layerColor->ignoreAnchorPointForPosition(false);
		layerColor->setPosition(ccp(ptr2str->rect.x,rgss_y_to_cocos_y(ptr2str->rect.y,bitmap->p->m_height)));
		bitmap->p->m_emuBitmap->addChild(layerColor);
	}

	delete ptr2str;
	return 0;
}


void Bitmap::fillRect(const IntRect &rect, const Vec4 &color)
{
	FillRectStruct* ptr2 = new FillRectStruct;
	ptr2->rect = rect;
	ptr2->color = color;
	if(p->m_filename=="")
		p->m_filename = "rect";
	ThreadHandler hander={handler_method_fill,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
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
		bitmap->p->m_emuBitmap->removeAllChildrenWithCleanup(true);
		if (bitmap->p->m_fontRender)
		{
			bitmap->p->m_fontRender->release();
			bitmap->p->m_fontRender = NULL;
		}
	
	}

	return 0;
}


void Bitmap::clear()
{
	if (NULL != p->m_emuBitmap)
	{
		ThreadHandler hander={handler_method_clear,(int)this,(void*)NULL};
		pthread_mutex_lock(&s_thread_handler_mutex);
		ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
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
	Font* font;

	~DrawtextStruct()
	{
		delete font;
	}
};

int Bitmap::handler_method_drawtext( int ptr1,void* ptr2 )
{
 	Bitmap* bitmap = (Bitmap*)ptr1;
	if (NULL==bitmap->p->m_emuBitmap)
		return -1;

	bool firstdraw = false;
	CCRenderTexture* fontRender = (CCRenderTexture*)bitmap->p->m_fontRender;
	if (NULL==fontRender)
	{
		fontRender = CCRenderTexture::create(bitmap->p->m_width,bitmap->p->m_height);
		bitmap->getEmuBitmap()->addChild(fontRender);
		fontRender->setPosition(ccp(bitmap->p->m_width/2,bitmap->p->m_height/2));
		bitmap->p->m_fontRender = fontRender;
		fontRender->retain();
		firstdraw = true;
		//fontRender->getSprite()->getTexture()->setAliasTexParameters();
	}

	DrawtextStruct* ptr2struct = (DrawtextStruct*)ptr2;
	string tmpdrawchar = ptr2struct->str;
// 	int checknum = atoi(tmpdrawchar.c_str());
// 	if (checknum!=0)
// 	{
// 		char tmp[20]={0};
// 		sprintf(tmp,"%d",checknum);
// 		tmpdrawchar = tmp;
// 	}

	CCLabelTTF* label = CCLabelTTF::create(tmpdrawchar.c_str(),ptr2struct->font->getName(),ptr2struct->font->getSize());
	//label->getTexture()->setAliasTexParameters();
	if (ptr2struct->font)
	{
		Font* f = ptr2struct->font; 
		label->setFontName(f->getName());
		label->setFontSize(f->getSize());
		label->setColor(ccc3(f->getColor()->red*f->getColor()->alpha/255,
			f->getColor()->green*f->getColor()->alpha/255,
			f->getColor()->blue*f->getColor()->alpha/255));
		//label->setOpacity(f->getColor()->alpha);
	}
	label->setAnchorPoint(ccp(0,1));
	label->setDimensions(CCSizeMake(ptr2struct->rect.w,ptr2struct->rect.h));
	label->setPosition(ccp(ptr2struct->rect.x,rgss_y_to_cocos_y(ptr2struct->rect.y,bitmap->p->m_height)));
	label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);

	if (ptr2struct->align == Bitmap::Center)
		label->setHorizontalAlignment(kCCTextAlignmentCenter);
	else if(ptr2struct->align == Bitmap::Right)
		label->setHorizontalAlignment(kCCTextAlignmentRight);
	else if (ptr2struct->align == Bitmap::Left)
		label->setHorizontalAlignment(kCCTextAlignmentLeft);

// 	CCLayerColor* masklayer = CCLayerColor::create(ccc4(255,255,255,255));
// 	masklayer->setContentSize(label->getContentSize());
// 	masklayer->setPosition(ccp(ptr2struct->rect.x,rgss_y_to_cocos_y(ptr2struct->rect.y,bitmap->m_height)-masklayer->getContentSize().height));
// 	ccBlendFunc fun = {GL_ZERO,GL_ZERO};
// 	masklayer->setBlendFunc(fun);

	fontRender->begin();
	//masklayer->visit();
	label->visit();
	fontRender->end();
	label->release();

	delete ptr2struct;
	return 0;
}


void Bitmap::drawText(const IntRect &rect, const char *str, int align)
{

	DrawtextStruct* ptr2 = new DrawtextStruct;
	ptr2->rect = rect;ptr2->str = str;ptr2->align = align;ptr2->font = new Font(*p->font);
	ThreadHandler hander={handler_method_drawtext,(int)this,(void*)ptr2};
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->m_TextRect = rect;
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
	
}

DEF_ATTR_RD_SIMPLE(Bitmap, Font, Font*, p->font)

void Bitmap::setFont(Font* value)
{
	p->font = value;
}

typedef struct{
	char*       data;
	int         datalen;
} workbuf;

typedef struct
{
	void* procs;	/* font-specific rendering routines*/
	int	size;	/* font height in pixels*/
	int	rotation;	/* font rotation*/
	uint32_t disp;	/* diplaymode*/
	int flags;
	workbuf	encodingBuf;
	/* freetype stuff */
	//color
	uint8_t  r;
	uint8_t  g;
	uint8_t  b;
	uint8_t  a;
	int   fix_width;
	FT_Face face;
	FT_Matrix matrix;
	FT_Library library;

	/* custom stuff */
	uint8_t  alpha_table[256];
}FontFreetype,*PFontFreetype;

typedef uint32_t ucs4_t;
/* Return code if invalid. (xxx_mbtowc) */
#define RET_ILSEQ      -1
/* Return code if only a shift sequence of n bytes was read. (xxx_mbtowc) */
#define RET_TOOFEW(n)  (-2-(n))
/* Return code if invalid. (xxx_wctomb) */
#define RET_ILUNI      -1
/* Return code if output buffer is too small. (xxx_wctomb) */
#define RET_TOOSMALL   -2

static int utf8_mbtowc (ucs4_t *pwc, const unsigned char *s, int n)
{
	unsigned char c = s[0];

	if (c < 0x80) {
		*pwc = c;
		return 1;
	} else if (c < 0xc2) {
		return RET_ILSEQ;
	} else if (c < 0xe0) {
		if (n < 2)
			return RET_TOOFEW(0);
		if (!((s[1] ^ 0x80) < 0x40))
			return RET_ILSEQ;
		*pwc = ((ucs4_t) (c & 0x1f) << 6)
			| (ucs4_t) (s[1] ^ 0x80);
		return 2;
	} else if (c < 0xf0) {
		if (n < 3)
			return RET_TOOFEW(0);
		if (!((s[1] ^ 0x80) < 0x40 && (s[2] ^ 0x80) < 0x40
			&& (c >= 0xe1 || s[1] >= 0xa0)))
			return RET_ILSEQ;
		*pwc = ((ucs4_t) (c & 0x0f) << 12)
			| ((ucs4_t) (s[1] ^ 0x80) << 6)
			| (ucs4_t) (s[2] ^ 0x80);
		return 3;
	} else if (c < 0xf8 && sizeof(ucs4_t)*8 >= 32) {
		if (n < 4)
			return RET_TOOFEW(0);
		if (!((s[1] ^ 0x80) < 0x40 && (s[2] ^ 0x80) < 0x40
			&& (s[3] ^ 0x80) < 0x40
			&& (c >= 0xf1 || s[1] >= 0x90)))
			return RET_ILSEQ;
		*pwc = ((ucs4_t) (c & 0x07) << 18)
			| ((ucs4_t) (s[1] ^ 0x80) << 12)
			| ((ucs4_t) (s[2] ^ 0x80) << 6)
			| (ucs4_t) (s[3] ^ 0x80);
		return 4;
	} else if (c < 0xfc && sizeof(ucs4_t)*8 >= 32) {
		if (n < 5)
			return RET_TOOFEW(0);
		if (!((s[1] ^ 0x80) < 0x40 && (s[2] ^ 0x80) < 0x40
			&& (s[3] ^ 0x80) < 0x40 && (s[4] ^ 0x80) < 0x40
			&& (c >= 0xf9 || s[1] >= 0x88)))
			return RET_ILSEQ;
		*pwc = ((ucs4_t) (c & 0x03) << 24)
			| ((ucs4_t) (s[1] ^ 0x80) << 18)
			| ((ucs4_t) (s[2] ^ 0x80) << 12)
			| ((ucs4_t) (s[3] ^ 0x80) << 6)
			| (ucs4_t) (s[4] ^ 0x80);
		return 5;
	} else if (c < 0xfe && sizeof(ucs4_t)*8 >= 32) {
		if (n < 6)
			return RET_TOOFEW(0);
		if (!((s[1] ^ 0x80) < 0x40 && (s[2] ^ 0x80) < 0x40
			&& (s[3] ^ 0x80) < 0x40 && (s[4] ^ 0x80) < 0x40
			&& (s[5] ^ 0x80) < 0x40
			&& (c >= 0xfd || s[1] >= 0x84)))
			return RET_ILSEQ;
		*pwc = ((ucs4_t) (c & 0x01) << 30)
			| ((ucs4_t) (s[1] ^ 0x80) << 24)
			| ((ucs4_t) (s[2] ^ 0x80) << 18)
			| ((ucs4_t) (s[3] ^ 0x80) << 12)
			| ((ucs4_t) (s[4] ^ 0x80) << 6)
			| (ucs4_t) (s[5] ^ 0x80);
		return 6;
	} else
		return RET_ILSEQ;
}


int nge_charset_utf8_to_ucs2(const uint8_t* in, uint16_t* out, int len, int n) {
	ucs4_t wc = 0;
	int cur = 0, ret;
	uint16_t *pOut = out, *pEnd = out+n/2;

	while (cur < len) {
		ret = utf8_mbtowc(&wc, in+cur, len-cur);
		if (ret < 0 || wc > 0xffff)
			return 0;
		*pOut = (uint16_t)wc;
		cur += ret;
		pOut++;
		if (pOut > pEnd)
			return -1;
	}
	*pOut = 0x0;
	return pOut - (uint16_t*)out;
}

inline static uint16_t* _nge_ft_conv_encoding(PFontFreetype pf, const void *text, int * pCC) {
	uint16_t *value;
	int len = *pCC;

	if( len > pf->encodingBuf.datalen){
		pf->encodingBuf.datalen = len*2;
		free(pf->encodingBuf.data);
		pf->encodingBuf.data = (char*)malloc(pf->encodingBuf.datalen);
	}
	value = (uint16_t*)pf->encodingBuf.data;

	*pCC = nge_charset_utf8_to_ucs2((uint8_t*)text, value, len, pf->encodingBuf.datalen);
	
	return value;
}

PFontFreetype create_font_freetype_buf(const char* buf,int bsize, int height,int disp)
{
	PFontFreetype pf;
	//FT_Error error;

	/* allocate font structure */
	pf = (PFontFreetype) malloc(sizeof(FontFreetype));
	memset(pf,0,sizeof(FontFreetype));
	if (!pf) {
		return NULL;
	}
	FT_Init_FreeType( &pf->library );
	FT_New_Memory_Face( pf->library, (const unsigned char *)buf,bsize, 0, &pf->face );
	FT_Set_Char_Size( pf->face,height<< 6, height << 6, 96, 96);
	pf->size = height;
	pf->disp = 0;
	pf->r = 0;
	pf->g = 0;
	pf->a = 0;

	pf->encodingBuf.datalen = 2048;
	pf->encodingBuf.data = (char*)malloc(pf->encodingBuf.datalen);
	memset(pf->encodingBuf.data,0,pf->encodingBuf.datalen);
	memset(pf->alpha_table,0,256);
	return pf;
}

#define ROUND_26_6_TO_INT(valuetoround) (((valuetoround) + 63) >> 6)
static FT_Error freetype2_get_glyph_size(FT_Face face,
	int glyph_index,
	int *padvance,
	int *pascent,
	int *pdescent)
{
	FT_Error error;
	error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
	if (error)
		return error;

	if (padvance)
		*padvance = ROUND_26_6_TO_INT(face->glyph->advance.x);
	if (pascent || pdescent)
	{
		FT_Glyph glyph;
		FT_BBox bbox;

		error = FT_Get_Glyph(face->glyph, &glyph);
		if (error)
			return error;

		FT_Glyph_Get_CBox(glyph, ft_glyph_bbox_pixels, &bbox);

		FT_Done_Glyph(glyph);

		if (pascent)
			*pascent = bbox.yMax;
		if (pdescent)
			*pdescent = -bbox.yMin;
	}

	return 0;
}

static void freetype2_gettextsize(PFontFreetype pfont, const void *text, int cc,
								  int flags, int *pwidth, int *pheight,
								  int *pbase)
{
	FT_Face face;
	uint16_t* value;
	int char_index;
	int total_advance;
	int max_ascent;
	int max_descent;
	int advance;
	int ascent;
	int descent;
	FT_Error error;
	int cur_glyph_code;
	PFontFreetype pf = pfont;

	value = _nge_ft_conv_encoding(pfont, text, &cc);
	if (cc <= 0)
		return;

	face = pf->face;
	/*
	* Starting point
	*/
	total_advance = 0;
	max_ascent  = 0;
	max_descent = 0;

	for (char_index = 0; char_index < cc; char_index++) {
		cur_glyph_code = FT_Get_Char_Index( pf->face, value[char_index] );//LOOKUP_CHAR(pf, face, str[char_index]);

		error = freetype2_get_glyph_size(face, cur_glyph_code, &advance, &ascent, &descent);
		if (error)
			continue;

		total_advance += advance;
		if (max_ascent < ascent)
			max_ascent = ascent;
		if (max_descent < descent)
			max_descent = descent;
	}

	if(pwidth) *pwidth = total_advance;
	if(pheight) *pheight = max_ascent + max_descent;
	if(pbase) *pbase = max_ascent;
}


static void freetype2_destroyfont(PFontFreetype pfont)
{
	PFontFreetype pf = (PFontFreetype) pfont;
	FT_Done_Face(pf->face);
	FT_Done_FreeType(pf->library);
	CC_SAFE_DELETE(pf->encodingBuf.data);
	CC_SAFE_DELETE(pf);
}

IntRect Bitmap::textSize(const char *str)
{
	int interval = 0;
	int w = 0;
	int h = 0;
	
	unsigned long s=0;
	unsigned char* fpbuf = CCFileUtils::sharedFileUtils()->getFileData("fonts/arial.ttf","rb",&s);
	CCAssert(s,"fuck");
	PFontFreetype fp = create_font_freetype_buf((const char*)fpbuf,s,p->font->getSize(),0);
	freetype2_gettextsize(fp,str,strlen(str),0,&w,&h,&interval);
	IntRect rect(0,0,w,h);
	freetype2_destroyfont(fp);
	delete [] fpbuf;

	return rect;
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

int Bitmap::handler_method_release( int ptr1,void* ptr2 )
{
	CCSprite* emubitmap = (CCSprite*)ptr1;
	CCRenderTexture* fontRender = (CCRenderTexture*)ptr2;

	if (NULL!=emubitmap)
	{
		CCTexture2D* releaseTexture = emubitmap->getTexture();
		emubitmap->release();
		CCTextureCache::sharedTextureCache()->removeTexture(releaseTexture);
	}

	if (NULL!=fontRender)
	{
		fontRender->release();
	}

	return 0;
}

void Bitmap::releaseResources()
{
	ThreadHandler hander={handler_method_release,(int)p->m_emuBitmap,(void*)p->m_fontRender};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandlerRelease(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);

	if (p)
		delete p;
}

CCSprite* Bitmap::getEmuBitmap()
{
	return p->m_emuBitmap;
}

std::string Bitmap::getFilename()
{
	return p->m_filename;
}

CCRenderTexture* Bitmap::getRenderTexture()
{
	return p->m_fontRender;
}

IntRect Bitmap::getTextRect()
{
	return p->m_TextRect;
}



