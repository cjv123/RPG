#ifndef TILEMAP_H
#define TILEMAP_H

#include "disposable.h"

#include "util.h"
#include <cocos2d.h>
#include "viewport.h"
using namespace cocos2d;

class Viewport;
class Bitmap;
class Table;

struct TilemapPrivate;

class Tilemap : public Disposable
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


	DECL_ATTR( Viewport,   Viewport* )
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
	CCClippingNode* m_clippingNode;

	static int handler_method_drawMap(int prt1,void* ptr2);
	static void handleAutotile(Tilemap* tilemap,int x,int y,int z,int tileInd);
	static int Tilemap::handler_method_composite( int ptr1,void* ptr2 );
	void drawMap();

	void releaseResources();

	virtual void composite();

};

#endif // TILEMAP_H
