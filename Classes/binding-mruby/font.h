#ifndef FONT_H
#define FONT_H

#include "etc.h"
#include "util.h"

struct FontPrivate
{
	std::string name;
	int size;
	bool bold;
	bool italic;
	Color *color;

	Color colorTmp;

	static std::string defaultName;
	static int defaultSize;
	static bool defaultBold;
	static bool defaultItalic;
	static Color *defaultColor;

	static Color defaultColorTmp;

	FontPrivate(const char *name = 0,
		int size = 0)
		: name(name ? std::string(name) : defaultName),
		size(size ? size : defaultSize),
		bold(defaultBold),
		italic(defaultItalic),
		color(&colorTmp),
		colorTmp(*defaultColor)
	{}

	FontPrivate(const FontPrivate &other)
		: name(other.name),
		size(other.size),
		bold(other.bold),
		italic(other.italic),
		color(&colorTmp),
		colorTmp(*other.color)
	{}
};

class Font
{
public:
	static bool doesExist(const char *name);

	Font(const char *name = 0,
	     int size = 0);
	/* Clone constructor */
	Font(const Font &other);
	~Font();

	const char *getName() const;
	void setName(const char *value);

	DECL_ATTR( Size,   int )
	DECL_ATTR( Bold,   bool )
	DECL_ATTR( Italic, bool )
	DECL_ATTR( Color,  Color* )

	DECL_ATTR_STATIC( DefaultName,   const char* )
	DECL_ATTR_STATIC( DefaultSize,   int         )
	DECL_ATTR_STATIC( DefaultBold,   bool        )
	DECL_ATTR_STATIC( DefaultItalic, bool        )
	DECL_ATTR_STATIC( DefaultColor,  Color*      )

private:
	static int handler_method_setcolor(int ptr1,void* ptr2);
	FontPrivate *p;
};

#endif // FONT_H
