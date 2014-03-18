#include "plane.h"

#include "bitmap.h"
#include "etc.h"
#include "util.h"


struct PlanePrivate
{
	Bitmap *bitmap;
	NormValue opacity;
	BlendType blendType;
	Color *color;
	Tone *tone;

	int ox, oy;
	float zoomX, zoomY;


	bool quadSourceDirty;

	EtcTemps tmp;

	PlanePrivate()
	    : bitmap(0),
	      opacity(255),
	      blendType(BlendNormal),
	      color(&tmp.color),
	      tone(&tmp.tone),
	      ox(0), oy(0),
	      zoomX(1), zoomY(1),
	      quadSourceDirty(false)
	{
	}

	~PlanePrivate()
	{
	}

	
};

Plane::Plane(Viewport *viewport)
{
	p = new PlanePrivate();

}

#define DISP_CLASS_NAME "plane"

DEF_ATTR_RD_SIMPLE(Plane, Bitmap,    Bitmap*, p->bitmap)
DEF_ATTR_RD_SIMPLE(Plane, OX,        int,     p->ox)
DEF_ATTR_RD_SIMPLE(Plane, OY,        int,     p->oy)
DEF_ATTR_RD_SIMPLE(Plane, ZoomX,     float,   p->zoomX)
DEF_ATTR_RD_SIMPLE(Plane, ZoomY,     float,   p->zoomY)
DEF_ATTR_RD_SIMPLE(Plane, BlendType, int,     p->blendType)

DEF_ATTR_SIMPLE(Plane, Opacity, int,     p->opacity)
DEF_ATTR_SIMPLE(Plane, Color,   Color*,  p->color)
DEF_ATTR_SIMPLE(Plane, Tone,    Tone*,   p->tone)

Plane::~Plane()
{
	dispose();
}

void Plane::setBitmap(Bitmap *value)
{

	p->bitmap = value;

	if (!value)
		return;

}

void Plane::setOX(int value)
{
	p->ox = value;
	p->quadSourceDirty = true;
}

void Plane::setOY(int value)
{
	p->oy = value;
	p->quadSourceDirty = true;
}

void Plane::setZoomX(float value)
{
	p->zoomX = value;
	p->quadSourceDirty = true;
}

void Plane::setZoomY(float value)
{
	p->zoomY = value;
	p->quadSourceDirty = true;
}

void Plane::setBlendType(int value)
{
	switch (value)
	{
	default :
	case BlendNormal :
		p->blendType = BlendNormal;
		return;
	case BlendAddition :
		p->blendType = BlendAddition;
		return;
	case BlendSubstraction :
		p->blendType = BlendSubstraction;
		return;
	}
}


void Plane::draw()
{
	
}


void Plane::aboutToAccess() const
{
	
}


void Plane::releaseResources()
{
	
	delete p;
}
