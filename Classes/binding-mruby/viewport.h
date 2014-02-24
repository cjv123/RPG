#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "disposable.h"
#include "util.h"
#include "etc.h"

struct ViewportPrivate;

class Viewport : public Disposable
{
public:
	Viewport(int x, int y, int width, int height);
	Viewport(Rect *rect);
	~Viewport();

	DECL_ATTR( Rect,  Rect*  )
	DECL_ATTR( OX,    int    )
	DECL_ATTR( OY,    int    )
	DECL_ATTR( Color, Color* )
	DECL_ATTR( Tone,  Tone*  )

private:
	void initViewport(int x, int y, int width, int height);
	void geometryChanged();

	void composite();
	void draw();
	bool isEffectiveViewport(Rect *&, Color *&, Tone *&) const;

	void releaseResources();

	ViewportPrivate *p;
	friend struct ViewportPrivate;

};

#endif // VIEWPORT_H
