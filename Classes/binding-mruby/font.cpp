#include "font.h"

#include "exception.h"
#include "boost-hash.h"
#include "util.h"

#include <string>
#include <utility>

static void strToLower(std::string &str)
{
	for (size_t i = 0; i < str.size(); ++i)
		str[i] = tolower(str[i]);
}



std::string FontPrivate::defaultName   = "Arial";
int         FontPrivate::defaultSize   = 22;
bool        FontPrivate::defaultBold   = false;
bool        FontPrivate::defaultItalic = false;
Color      *FontPrivate::defaultColor  = &FontPrivate::defaultColorTmp;

Color FontPrivate::defaultColorTmp(255, 255, 255, 255);

bool Font::doesExist(const char *name)
{
	FILE* fp =fopen(name,"r");
	if (NULL==fp)
	{
		return false;
	}
	fclose(fp);
	return true;
}

Font::Font(const char *name,
           int size)
{
	p = new FontPrivate(name, size);
}

Font::Font(const Font &other)
{
	p = new FontPrivate(*other.p);
}

Font::~Font()
{
	delete p;
}

const char *Font::getName() const
{
	return p->name.c_str();
}

void Font::setName(const char *value)
{
	if (p->name == value)
		return;

	p->name = value;
}

void Font::setSize(int value)
{
	if (p->size == value)
		return;

	p->size = value;
}

#undef CHK_DISP
#define CHK_DISP

DEF_ATTR_RD_SIMPLE(Font, Size, int, p->size)
DEF_ATTR_SIMPLE(Font, Bold, bool, p->bold)
DEF_ATTR_SIMPLE(Font, Italic, bool, p->italic)
DEF_ATTR_SIMPLE(Font, Color, Color*, p->color)

DEF_ATTR_SIMPLE_STATIC(Font, DefaultSize, int, FontPrivate::defaultSize)
DEF_ATTR_SIMPLE_STATIC(Font, DefaultBold, bool, FontPrivate::defaultBold)
DEF_ATTR_SIMPLE_STATIC(Font, DefaultItalic, bool, FontPrivate::defaultItalic)
DEF_ATTR_SIMPLE_STATIC(Font, DefaultColor, Color*, FontPrivate::defaultColor)

const char *Font::getDefaultName()
{
	return FontPrivate::defaultName.c_str();
}

void Font::setDefaultName(const char *value)
{
	FontPrivate::defaultName = value;
}