#include "tilemap.h"
#include "viewport.h"
#include "bitmap.h"
#include "table.h"
#include "etc-internal.h"
#include <string.h>
#include <stdint.h>
#include <algorithm>
#include <vector>
#include "../ThreadHandlerManager.h"

extern pthread_mutex_t s_thread_handler_mutex;

extern const StaticRect autotileRects[];

typedef std::vector<SVertex> SVVector;
typedef struct { SVVector v[4]; } TileVBuffer;

/* Check if [C]ontainer contains [V]alue */
template<typename C, typename V>
inline bool contains(const C &c, const V &v)
{
	return std::find(c.begin(), c.end(), v) != c.end();
}

static const int tilesetW  = 8 * 32;
static const int autotileW = 3 * 32;
static const int autotileH = 4 * 32;

static const int autotileCount = 7;

static const int atAreaW = autotileW * 4;
static const int atAreaH = autotileH * autotileCount;

static const int tsLaneW = tilesetW / 2;

enum Position
{
	Normal = 1 << 0,

	Left   = 1 << 1,
	Right  = 1 << 2,
	Top    = 1 << 3,
	Bottom = 1 << 4,

	TopLeft     = Top | Left,
	TopRight    = Top | Right,
	BottomLeft  = Bottom | Left,
	BottomRight = Bottom | Right
};

static const Position positions[] =
{
	Normal,
	Left, Right, Top, Bottom,
	TopLeft, TopRight, BottomLeft, BottomRight
};

static elementsN(positions);

/* Autotile animation */
static const uint8_t atAnimation[16*4] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

static elementsN(atAnimation);

/* Flash tiles pulsing opacity */
static const uint8_t flashAlpha[] =
{
	/* Fade in */
	0x3C, 0x3C, 0x3C, 0x3C, 0x4B, 0x4B, 0x4B, 0x4B,
	0x5A, 0x5A, 0x5A, 0x5A, 0x69, 0x69, 0x69, 0x69,
	/* Fade out */
	0x78, 0x78, 0x78, 0x78, 0x69, 0x69, 0x69, 0x69,
	0x5A, 0x5A, 0x5A, 0x5A, 0x4B, 0x4B, 0x4B, 0x4B
};

static elementsN(flashAlpha);

struct TilemapPrivate
{
	Viewport *viewport;

	Tilemap::Autotiles autotilesProxy;
	Bitmap *autotiles[autotileCount];

	Bitmap *tileset;
	Table *mapData;
	Table *flashData;
	Table *priorities;
	bool visible;
	Vec2i offset;

	Vec2i dispPos;

	/* Tile atlas */
	struct {

		Vec2i size;

		/* Effective tileset height,
		 * clamped to a multiple of 32 */
		int efTilesetH;

		/* Indices of usable
		 * (not null, not disposed) autotiles */
		std::vector<uint8_t> usableATs;

		/* Indices of animated autotiles */
		std::vector<uint8_t> animatedATs;
	} atlas;

	/* Map size in tiles */
	int mapWidth;
	int mapHeight;

	/* Ground layer vertices */
	TileVBuffer groundVert;

	/* Scanrow vertices */
	std::vector<TileVBuffer> scanrowVert;

	/* Base quad indices of each scanrow
	 * in the shared buffer */
	std::vector<int> scanrowBases;
	size_t scanrowCount;

	/* Shared buffers for all tiles */
	struct
	{
		bool animated;

		/* Animation state */
		uint8_t frameIdx;
		uint8_t aniIdx;
	} tiles;

	/* Flash buffers */
	struct
	{
		size_t quadCount;
		uint8_t alphaIdx;
	} flash;

	/* Scene elements */
	struct
	{
		Vec2i sceneOffset;

		/* The ground and scanrow elements' creationStamp
		 * should be aquired once (at Tilemap construction)
		 * instead of regenerated everytime the elements are
		 * (re)created. Scanrows can share one stamp because
		 * their z always differs anway */
		unsigned int groundStamp;
		unsigned int scanrowStamp;
	} elem;

	/* Replica bitmask */
	uint8_t replicas;

	/* Affected by: autotiles, tileset */
	bool atlasSizeDirty;
	/* Affected by: autotiles(.changed), tileset(.changed), allocateAtlas */
	bool atlasDirty;
	/* Affected by: mapData(.changed), priorities(.changed) */
	bool buffersDirty;
	/* Affected by: oy */
	bool zOrderDirty;
	/* Affected by: flashData, buffersDirty */
	bool flashDirty;

	/* Resources are sufficient and tilemap is ready to be drawn */
	bool tilemapReady;

	/* Change watches */

	TilemapPrivate(Viewport *viewport)
	    : viewport(viewport),
	      tileset(0),
	      mapData(0),
	      flashData(0),
	      priorities(0),
	      visible(true),
	      mapWidth(0),
	      mapHeight(0),
	      replicas(Normal),
	      atlasSizeDirty(false),
	      atlasDirty(false),
	      buffersDirty(false),
	      zOrderDirty(false),
	      flashDirty(false),
	      tilemapReady(false)
	{
		memset(autotiles, 0, sizeof(autotiles));

		atlas.animatedATs.reserve(autotileCount);
		atlas.efTilesetH = 0;

		tiles.animated = false;
		tiles.frameIdx = 0;
		tiles.aniIdx = 0;

	}

	~TilemapPrivate()
	{
		
	}

	int samplePriority(int tileInd)
	{
		if (!priorities)
			return 0;

		if (tileInd > priorities->xSize()-1)
			return 0;

		int value = priorities->at(tileInd);

		if (value > 5)
			return -1;

		return value;
	}
};


void Tilemap::Autotiles::set(int i, Bitmap *bitmap)
{

}

Bitmap *Tilemap::Autotiles::get(int i) const
{
	if (i < 0 || i > autotileCount-1)
		return 0;

	return p->autotiles[i];
}

Tilemap::Tilemap(Viewport *viewport)
{
	p = new TilemapPrivate(viewport);
	p->autotilesProxy.p = p;
	for (int i=0;i<3;i++)
		m_mapLayer[i] = NULL;
}

Tilemap::~Tilemap()
{
	dispose();
}

void Tilemap::update()
{
	
}

Tilemap::Autotiles &Tilemap::getAutotiles() const
{
	return p->autotilesProxy;
}

#define DISP_CLASS_NAME "tilemap"

DEF_ATTR_RD_SIMPLE(Tilemap, Viewport, Viewport*, p->viewport)
DEF_ATTR_RD_SIMPLE(Tilemap, Tileset, Bitmap*, p->tileset)
DEF_ATTR_RD_SIMPLE(Tilemap, MapData, Table*, p->mapData)
DEF_ATTR_RD_SIMPLE(Tilemap, FlashData, Table*, p->flashData)
DEF_ATTR_RD_SIMPLE(Tilemap, Priorities, Table*, p->priorities)
DEF_ATTR_RD_SIMPLE(Tilemap, Visible, bool, p->visible)
DEF_ATTR_RD_SIMPLE(Tilemap, OX, int, p->offset.x)
DEF_ATTR_RD_SIMPLE(Tilemap, OY, int, p->offset.y)

#ifdef RGSS2

void Tilemap::setViewport(Viewport *value)
{
	GUARD_DISPOSED

	if (p->viewport == value)
		return;

	p->viewport = value;

	if (!p->tilemapReady)
		return;

	p->elem.ground->setViewport(value);

	for (size_t i = 0; i < p->elem.scanrows.size(); ++i)
		p->elem.scanrows[i]->setViewport(value);
}

#endif

void Tilemap::setTileset(Bitmap *value)
{
	p->tileset = value;
}

void Tilemap::setMapData(Table *value)
{
	p->mapData = value;
	p->mapWidth = p->mapData->xSize();
	p->mapHeight = p->mapData->ySize();
}

void Tilemap::setFlashData(Table *value)
{
	p->flashData = value;
}

void Tilemap::setPriorities(Table *value)
{
	p->priorities = value;
}

void Tilemap::setVisible(bool value)
{
	p->visible = value;
}

void Tilemap::setOX(int value)
{
	p->offset.x = value;
}

void Tilemap::setOY(int value)
{
	p->offset.y = value;
}




void Tilemap::releaseResources()
{
	delete p;
}


int Tilemap::handler_method_drawMap( int ptr1,void* ptr2 )
{
	Tilemap* tilemap = (Tilemap*)ptr1;
	CCSprite* tilesetSp = tilemap->p->tileset->getEmuBitmap();
	Bitmap** autotiles = tilemap->p->autotiles;
	Table* mapData = tilemap->p->mapData;
	int mapWidth = tilemap->p->mapWidth;
	int mapHeight = tilemap->p->mapHeight;
	int mapDepth = mapData->zSize();
	CCLayer** mapLayer = tilemap->m_mapLayer;

	for (int i;i<3;i++)
	{
		mapLayer[i] = CCLayer::create();
		mapLayer[i]->setContentSize(CCSizeMake(mapWidth,mapHeight));
	}

	for (int x = 0; x < mapWidth; ++x)
	{	
		for (int y = 0; y < mapHeight; ++y)
		{		
			for (int z = 0; z < mapDepth; ++z)
			{
				int tileInd = mapData->at(x, y, z);

				/* Check for empty space */
				if (tileInd < 48)
					return 0;

				int prio = tilemap->p->samplePriority(tileInd);

				/* Check for faulty data */
				if (prio == -1)
					return 0;

				/* Check for autotile */
				if (tileInd < 48*8)
				{
					//handleAutotile(x, y, tileInd, targetArray);
					return 0;
				}

				int tsInd = tileInd - 48*8;
				int tileX = tsInd % 8;
				int tileY = tsInd / 8;

				//Vec2i texPos = TileAtlas::tileToAtlasCoor(tileX, tileY, atlas.efTilesetH, atlas.size.y);
				//FloatRect texRect((float) texPos.x+.5, (float) texPos.y+.5, 31, 31);
				FloatRect posRect(x*32, y*32, 32, 32);

			}
		}
	}


	return 0;
}


void Tilemap::drawMap()
{
	if (p->tileset && p->autotiles && p->mapData)
	{
		ThreadHandler hander={handler_method_drawMap,(int)this,(void*)NULL};
		pthread_mutex_lock(&s_thread_handler_mutex);
		ThreadHandlerMananger::getInstance()->pushHandler(hander);
		pthread_mutex_unlock(&s_thread_handler_mutex);
	}
}
