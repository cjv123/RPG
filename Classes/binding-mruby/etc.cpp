#include "etc.h"

#include "serial-util.h"
#include "exception.h"


Color::Color(double red, double green, double blue, double alpha)
	: red(red), green(green), blue(blue), alpha(alpha)
{
	updateInternal();
}

Color::Color(const Vec4 &norm)
	: norm(norm)
{
	updateExternal();
}

Color::Color(const Color &o)
    : red(o.red), green(o.green), blue(o.blue), alpha(o.alpha),
      norm(o.norm)
{}

bool Color::operator==(const Color &o) const
{
	return red   == o.red   &&
	       green == o.green &&
	       blue  == o.blue  &&
	       alpha == o.alpha;
}

void Color::updateInternal()
{
	norm.x = red   / 255;
	norm.y = green / 255;
	norm.z = blue  / 255;
	norm.w = alpha / 255;
}

void Color::updateExternal()
{
	red   = norm.x * 255;
	green = norm.y * 255;
	blue  = norm.z * 255;
	alpha = norm.w * 255;
}

void Color::set(double red, double green, double blue, double alpha)
{
	this->red   = red;
	this->green = green;
	this->blue  = blue;
	this->alpha = alpha;

	updateInternal();
}

void Color::setRed(double value)
{
	red = value;
	norm.x = clamp<double>(value, 0, 255) / 255;
}

void Color::setGreen(double value)
{
	green = value;
	norm.y = clamp<double>(value, 0, 255) / 255;
}

void Color::setBlue(double value)
{
	blue = value;
	norm.z = clamp<double>(value, 0, 255) / 255;
}

void Color::setAlpha(double value)
{
	alpha = value;
	norm.w = clamp<double>(value, 0, 255) / 255;
}

/* Serializable */
int Color::serialSize() const
{
	return 4 * 8;
}

void Color::serialize(char *buffer) const
{
	char *buf = buffer;

	write_double(&buf, red);
	write_double(&buf, green);
	write_double(&buf, blue);
	write_double(&buf, alpha);
}

Color *Color::deserialize(const char *data, int len)
{
	if (len != 32)
		throw Exception(Exception::ArgumentError, "Color: Serialized data invalid");

	Color *c = new Color();
	uint i = 0;

	c->red   = read_double(data, i);
	c->green = read_double(data, i);
	c->blue  = read_double(data, i);
	c->alpha = read_double(data, i);
	c->updateInternal();

	return c;
}


Tone::Tone(double red, double green, double blue, double gray)
	: red(red), green(green), blue(blue), gray(gray)
{
	updateInternal();
}

Tone::Tone(const Tone &o)
    : red(o.red), green(o.green), blue(o.blue), gray(o.gray),
      norm(o.norm)
{}

bool Tone::operator==(const Tone &o) const
{
	return red   == o.red   &&
	       green == o.green &&
	       blue  == o.blue  &&
	       gray  == o.gray;
}

void Tone::updateInternal()
{
	norm.x = (float) clamp<double>(red,   -255, 255) / 255;
	norm.y = (float) clamp<double>(green, -255, 255) / 255;
	norm.z = (float) clamp<double>(blue,  -255, 255) / 255;
	norm.w = (float) clamp<double>(gray,     0, 255) / 255;
}

void Tone::set(double red, double green, double blue, double gray)
{
	this->red   = red;
	this->green = green;
	this->blue  = blue;
	this->gray  = gray;

	updateInternal();
}

void Tone::setRed(double value)
{
	red = value;
	norm.x = (float) clamp<double>(value, -255, 255) / 255;
}

void Tone::setGreen(double value)
{
	green = value;
	norm.y = (float) clamp<double>(value, -255, 255) / 255;
}

void Tone::setBlue(double value)
{
	blue = value;
	norm.z = (float) clamp<double>(value, -255, 255) / 255;
}

void Tone::setGray(double value)
{
	gray = value;
	norm.w = (float) clamp<double>(value, 0, 255) / 255;
}

/* Serializable */
int Tone::serialSize() const
{
	return 4 * 8;
}

void Tone::serialize(char *buffer) const
{
	char *buf = buffer;

	write_double(&buf, red);
	write_double(&buf, green);
	write_double(&buf, blue);
	write_double(&buf, gray);
}

Tone *Tone::deserialize(const char *data, int len)
{
	if (len != 32)
		throw Exception(Exception::ArgumentError, "Tone: Serialized data invalid");

	Tone *t = new Tone();
	uint i = 0;

	t->red   = read_double(data, i);
	t->green = read_double(data, i);
	t->blue  = read_double(data, i);
	t->gray  = read_double(data, i);
	t->updateInternal();

	return t;
}


Rect::Rect(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height)
{}

Rect::Rect(const Rect &o)
    : x(o.x), y(o.y),
      width(o.width), height(o.height)
{}

Rect::Rect(const IntRect &r)
    : x(r.x), y(r.y), width(r.w), height(r.h)
{}

bool Rect::operator==(const Rect &o) const
{
	return x      == o.x     &&
	       y      == o.y     &&
	       width  == o.width &&
	       height == o.height;
}

void Rect::operator=(const IntRect &rect)
{
	x = rect.x;
	y = rect.y;
	width = rect.w;
	height = rect.h;
}

void Rect::set(int x, int y, int w, int h)
{
	if (this->x == x &&
	    this->y == y &&
	    width   == w &&
	    height  == h)
	{
		return;
	}

	this->x = x;
	this->y = y;
	width = w;
	height = h;

	onChange();
}

void Rect::empty()
{
	if (!(x || y || width || height))
		return;

	x = y = width = height = 0;
}

bool Rect::isEmpty() const
{
	return !(width && height);
}

void Rect::setX(int value)
{
	if (x == value)
		return;

	x = value;
}

void Rect::setY(int value)
{
	if (y == value)
		return;

	y = value;
}

void Rect::setWidth(int value)
{
	if (width == value)
		return;

	width = value;
}

void Rect::setHeight(int value)
{
	if (height == value)
		return;

	height = value;
}

int Rect::serialSize() const
{
	return 4 * 4;
}

void Rect::serialize(char *buffer) const
{
	char *buf = buffer;

	write_int32(&buf, x);
	write_int32(&buf, y);
	write_int32(&buf, width);
	write_int32(&buf, height);
}

Rect *Rect::deserialize(const char *data, int len)
{
	if (len != 16)
		throw Exception(Exception::ArgumentError, "Rect: Serialized data invalid");

	Rect *r = new Rect();
	uint i = 0;

	r->x      = read_int32(data, i);
	r->y      = read_int32(data, i);
	r->width  = read_int32(data, i);
	r->height = read_int32(data, i);

	return r;
}

void Rect::addDelegate( Delegate* delegate )
{
	m_delegates.push_back(delegate);
}

void Rect::onChange()
{
	for (int i=0;i<m_delegates.size();i++)
	{
		m_delegates[i]->onRectChange();
	}
}

Rect::~Rect()
{
	//m_delegates.clear();
}
