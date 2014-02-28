#ifndef TILEMAP_H
#define TILEMAP_H

#include "disposable.h"

#include "util.h"
#include <cocos2d.h>
using namespace cocos2d;

class Viewport;
class Bitmap;
class Table;

struct TilemapPrivate;

class Tilemap : public Disposable,public ViewPortDelegate
{
public:
	class Autotiles
	{
	public:
		void set(int i, Bitmap *bitmap);
		Bitmap *get(int i) const;

	private:
		TilemapPrivate *p;
		friend class Tilemap;
	};

	Tilemap(Viewport *viewport = 0);
	~Tilemap();

	void update();

	Autotiles &getAutotiles() const;

#ifndef RGSS2
	Viewport *getViewport() const;
#else
	DECL_ATTR( Viewport,   Viewport* )
#endif
	DECL_ATTR( Tileset,    Bitmap*   )
	DECL_ATTR( MapData,    Table*    )
	DECL_ATTR( FlashData,  Table*    )
	DECL_ATTR( Priorities, Table*    )
	DECL_ATTR( Visible,    bool      )
	DECL_ATTR( OX,         int       )
	DECL_ATTR( OY,         int       )

private:
	TilemapPrivate *p;
	CCLayer* m_mapLayer[3];

	static int handler_method_drawMap(int prt1,void* ptr2);
	static void handleAutotile(Tilemap* tilemap,int x,int y,int z,int tileInd);
	void drawMap();

	void releaseResources();

	virtual void composite();

};

#endif // TILEMAP_H
