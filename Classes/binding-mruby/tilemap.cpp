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
#include "../SceneMain.h"

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

static const int tileW = 32;
static const int tilesetW  = 8 * tileW;
static const int autotileW = 3 * tileW;
static const int autotileH = 4 * tileW;

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

	Tilemap* tilemap;

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


	CCSpriteBatchNode* m_batchNode;
	bool m_isupdate;
	Vec2i m_screenoxy;


	vector<Tilemap::Tile*> m_tiles;
	vector<Tilemap::Tile*> m_screenTiles;

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
	      tilemapReady(false),
		  m_batchNode(NULL),
		  m_isupdate(false)
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
	if (p->autotiles[i] == bitmap)
		return;

	p->autotiles[i] = bitmap;
	
	p->tilemap->drawMap();
}

Bitmap *Tilemap::Autotiles::get(int i) const
{
	if (i < 0 || i > autotileCount-1)
		return 0;

	return p->autotiles[i];
}

Tilemap::Tilemap(Viewport *viewport) 
{
	if (viewport == 0)
	{
		viewport = new Viewport(0,0,0,0);
	}

	p = new TilemapPrivate(viewport);
	p->autotilesProxy.p = p;
	p->tilemap = this;

}

Tilemap::~Tilemap()
{
	dispose();
}

void Tilemap::update()
{
	if (p->m_isupdate)
	{
		pthread_mutex_lock(&s_thread_handler_mutex);
		p->m_isupdate = false;
		ThreadHandler hander={handler_method_update,(int)this,NULL};
		ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
		pthread_mutex_unlock(&s_thread_handler_mutex);
	}
		
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


void Tilemap::setViewport(Viewport *value)
{

	if (p->viewport == value)
		return;

	p->viewport = value;

}


void Tilemap::setTileset(Bitmap *value)
{
	p->tileset = value;
	drawMap();
}

void Tilemap::setMapData(Table *value)
{
	p->mapData = value;
	p->mapWidth = p->mapData->xSize();
	p->mapHeight = p->mapData->ySize();
	drawMap();
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


int Tilemap::handler_method_setox( int ptr1,void* ptr2 )
{
	Tilemap* tilemap = (Tilemap*)ptr1;
	Vec2i* pos = (Vec2i*)ptr2;

	tilemap->p->m_isupdate = true;
	delete pos;
	return 0;
}

int Tilemap::handler_method_setoy( int ptr1,void* ptr2 )
{
	Tilemap* tilemap = (Tilemap*)ptr1;
	Vec2i* pos = (Vec2i*)ptr2;

	tilemap->p->m_isupdate = true;

	
	delete pos;
	return 0;
}

void Tilemap::setOX(int value)
{
	if (value == p->offset.x)
		return;
	
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->offset.x = value;
	ThreadHandler hander={handler_method_setox,(int)this,new Vec2i(p->offset.x,p->offset.y)};
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Tilemap::setOY(int value)
{
	if (value == p->offset.y)
		return;
	
	pthread_mutex_lock(&s_thread_handler_mutex);
	p->offset.y = value;
	ThreadHandler hander={handler_method_setoy,(int)this,new Vec2i(p->offset.x,p->offset.y)};
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);

}


int Tilemap::handler_method_release( int ptr1,void* ptr2 )
{
	vector<CCNode*>* sps= (vector<CCNode*>*)ptr1;

	for (int i=0;i<sps->size();i++)
	{
		sps->at(i)->removeFromParentAndCleanup(true);
		sps->at(i)->release();
	}
	delete sps;

	CCSpriteBatchNode* spbn = (CCSpriteBatchNode*)ptr2;
	spbn->removeFromParentAndCleanup(true);

	return 0;
}

void Tilemap::releaseResources()
{
	vector<CCNode*>* delsps = new vector<CCNode*>;
	for (int i=0;i<p->m_tiles.size();i++)
	{
		Tile* tile = p->m_tiles[i];
		for (int j=0;j<3;j++)
		{
			if(tile->sp[j])
				delsps->push_back(tile->sp[j]);
		}
		delete tile;
	}
	p->m_tiles.clear();

	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandler hander={handler_method_release,(int)delsps,(void*)p->m_batchNode};
	ThreadHandlerMananger::getInstance()->pushHandlerRelease(hander);
	pthread_mutex_unlock(&s_thread_handler_mutex);

	if(p)
		delete p;
}


static FloatRect getAutotilePieceRect(int x, int y, /* in pixel coords */
	int corner)
{
	switch (corner)
	{
	case 0 : break;
	case 1 : x += 16; break;
	case 2 : x += 16; y += 16; break;
	case 3 : y += 16; break;
	default: abort();
	}

	return FloatRect(x, y, 16, 16);
}

void Tilemap::handleAutotile(Tilemap* tilemap,int x,int y,int z,int tileInd,Tile* tile)
{
	/* Which autotile [0-7] */
	int atInd = tileInd / 48 - 1;
	/* Which tile pattern of the autotile [0-47] */
	int subInd = tileInd % 48;

	CCSprite* autoTilsetSp = tilemap->p->autotiles[atInd]->getEmuBitmap();
	if(NULL == autoTilsetSp)
		return;

	CCClippingNode* clipper = tilemap->p->viewport->getClippingNode();


	Viewport* viewport = tilemap->p->viewport;

	int prio = tilemap->p->samplePriority(tileInd);

	const StaticRect *pieceRect = &autotileRects[subInd*4];
	CCSpriteBatchNode* tileNode = CCSpriteBatchNode::createWithTexture(autoTilsetSp->getTexture());
	tileNode->setContentSize(CCSizeMake(tileW,tileW));
	
	for (int i = 0; i < 4; ++i)
	{
		FloatRect posRect = getAutotilePieceRect(0, 0, i);
		FloatRect texRect = pieceRect[i];

		/* Adjust to atlas coordinates */
		//texRect.y += atInd * autotileH;
		texRect.x -=0.5;texRect.y-=0.5;texRect.w+=1;texRect.h+=1;

		CCSprite* tilesp = CCSprite::createWithTexture(autoTilsetSp->getTexture(),CCRectMake(texRect.x,texRect.y,texRect.w,texRect.h));
		tilesp->setAnchorPoint(ccp(0,1));
		tilesp->setPosition(ccp(posRect.x,rgss_y_to_cocos_y(posRect.y,tileW)));

		tileNode->addChild(tilesp);

		/*is a animte tile*/
		if (autoTilsetSp->getContentSize().width>autotileW)
		{
			CCAnimation* animation= CCAnimation::create();
			for (int j=0;j<4;j++)
			{
				animation->addSpriteFrameWithTexture(autoTilsetSp->getTexture(),CCRectMake(texRect.x,texRect.y,texRect.w,texRect.h));
				texRect.x+=texRect.w*6;
			}
			animation->setDelayPerUnit(0.4f);
			CCAnimate* animate = CCAnimate::create(animation);
			tilesp->runAction(CCRepeatForever::create(animate));
		}
		
		tilemap->orderTileZ(tilesp,x,y,z);
	}
	
	tileNode->retain();
	tileNode->setAnchorPoint(ccp(0,1));
	tile->sp[z]=tileNode;

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
	Viewport* viewport = tilemap->p->viewport;

	CCClippingNode* clipper = viewport->getClippingNode();
	if (!clipper)
		return -1;

	if (tilemap->p->m_batchNode == NULL)
	{
		tilemap->p->m_batchNode = CCSpriteBatchNode::createWithTexture(tilesetSp->getTexture());
		clipper->addChild(tilemap->p->m_batchNode);
	}
	
	for (int x = 0; x < mapWidth; ++x)
	{	
		for (int y = 0; y < mapHeight; ++y)
		{		
			Tile* tile = new Tile;
			tilemap->p->m_tiles.push_back(tile);
			tile->pos = Vec2i(x*tileW,y*tileW);
			tile->sp[0]=NULL;tile->sp[1]=NULL;tile->sp[2]=NULL;
			tile->x = x;tile->y=y;
			for (int z = 0; z < mapDepth; ++z)
			{
				drawTile(tilemap, x,y,z,tile);
			}
		}
	}
	handler_method_drawToScreen(ptr1,ptr2);
	return 0;
}

void Tilemap::drawTile(Tilemap* tilemap, int x,int y,int z,Tile* tile)
{
	Viewport* viewport = tilemap->p->viewport;
	CCClippingNode* clipper = viewport->getClippingNode();
	Table* mapData = tilemap->p->mapData;

	int tileInd = mapData->at(x, y, z);
	CCSprite* tilesetSp = tilemap->p->tileset->getEmuBitmap();

	/* Check for empty space */
	if (tileInd < 48)
	{
		return;
	}

	int prio = tilemap->p->samplePriority(tileInd);

	/* Check for faulty data */
	if (prio == -1)
	{
		return;
	}

	/* Check for autotile */
	if (tileInd < 48*8)
	{
		handleAutotile(tilemap,x,y,z,tileInd,tile);
		return;
	}

	int tsInd = tileInd - 48*8;
	int tileX = tsInd % 8;
	int tileY = tsInd / 8;

	CCSprite* tilesp = CCSprite::createWithTexture(tilesetSp->getTexture(),CCRectMake(tileX*tileW,tileY*tileW,tileW,tileW));
	tilesp->retain();
	tilesp->setAnchorPoint(ccp(0,1));

	tile->sp[z] = tilesp;
}


void Tilemap::drawMap()
{
	for (int i=0;i<autotileCount;i++)
	{
		if (p->autotiles[i] == NULL)
			return;
	}

	if (p->tileset && p->autotiles && p->mapData)
	{
		ThreadHandler hander={handler_method_drawMap,(int)this,(void*)NULL};
		pthread_mutex_lock(&s_thread_handler_mutex);
		ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
		pthread_mutex_unlock(&s_thread_handler_mutex);
	}
}

int Tilemap::handler_method_composite( int ptr1,void* ptr2 )
{
	Tilemap* tilemap = (Tilemap*)ptr1;
	Viewport* viewport = tilemap->p->viewport;
	int mapWidth = tilemap->p->mapWidth;
	int mapHeight = tilemap->p->mapHeight;
	if (viewport && viewport->getClippingNode())
	{
		//CCClippingNode* clipper = viewport->getClippingNode();

	}

	return 0;
}


void Tilemap::composite()
{
	ThreadHandler hander={handler_method_composite,(int)this,(void*)NULL};
	pthread_mutex_lock(&s_thread_handler_mutex);
	ThreadHandlerMananger::getInstance()->pushHandler(hander,this);
	pthread_mutex_unlock(&s_thread_handler_mutex);
}

void Tilemap::orderTileZ(CCNode* tilesp,int x,int y,int z)
{
	Table* mapData = p->mapData;
	int mapWidth = p->mapWidth;
	int mapHeight = p->mapHeight;
	int mapDepth = mapData->zSize();
	
	int tileInd = mapData->at(x, y, z);

	if (tileInd < 48)
		return;

	int prio = p->samplePriority(tileInd);

	if (prio == -1)
		return;

	int zoder = y*tileW - p->offset.y;
	if (prio == 1)
		zoder += 64;
	if (prio>1)
		zoder += 64 + (prio -1)*32;
	tilesp->setZOrder(zoder);
}

int Tilemap::handler_method_update( int ptr1,void* ptr2 )
{
	Tilemap* tilemap = (Tilemap*)ptr1;
	CCClippingNode* clipper = tilemap->p->viewport->getClippingNode();
	Viewport* viewport = tilemap->p->viewport;

	int clipperH = clipper->getContentSize().height;
	int offy = my_abs(tilemap->p->m_screenoxy.y - tilemap->p->offset.y);
	int offx = my_abs(tilemap->p->m_screenoxy.x - tilemap->p->offset.x);

	if (offx>tileW || offy>tileW)
	{
		handler_method_drawToScreen(ptr1,ptr2);
	}
	else
	{
		for (int i=0;i<tilemap->p->m_screenTiles.size();i++)
		{
			Tile* tile = tilemap->p->m_screenTiles[i];
			for (int j=0;j<3;j++)
			{
				CCNode* tilesp = tile->sp[j];
				if (tilesp == NULL)
					continue;
				tilesp->setPositionX(tile->pos.x-tilemap->p->offset.x);
				tilesp->setPositionY( rgss_y_to_cocos_y(tile->pos.y-tilemap->p->offset.y,clipperH));
				tilemap->orderTileZ(tilesp,tile->x,tile->y,j);
			}
		}
	}

	return 0;
}

int Tilemap::handler_method_drawToScreen( int ptr1,void* ptr2 )
{
	Tilemap* tilemap = (Tilemap*)ptr1;
	Table* mapData = tilemap->p->mapData;
	int mapWidth = tilemap->p->mapWidth;
	int mapHeight = tilemap->p->mapHeight;
	int mapDepth = mapData->zSize();
	
	CCClippingNode* clipper = tilemap->p->viewport->getClippingNode();
	Viewport* viewport = tilemap->p->viewport;

	int clipperH = clipper->getContentSize().height;
	
	int col = tilemap->p->offset.x/tileW - 2;
	int row = tilemap->p->offset.y/tileW -2;
	if(col<0) col = 0;
	if(row<0) row = 0;
	int maxcol =col+ clipper->getContentSize().width/tileW + 4;
	int maxrow =row + clipper->getContentSize().height/tileW + 4;
	if(maxcol>mapData->xSize()) maxcol = mapData->xSize();
	if(maxrow>mapData->ySize()) maxrow = mapData->ySize();

	for (int i=0;i<tilemap->p->m_screenTiles.size();i++)
	{
		for (int j=0;j<3;j++)
		{
			if(tilemap->p->m_screenTiles[i]->sp[j])
				tilemap->p->m_screenTiles[i]->sp[j]->removeFromParentAndCleanup(true);
		}
	}
	tilemap->p->m_screenTiles.clear();

	for (int x = col; x < maxcol; ++x)
	{	
		for (int y = row; y < maxrow; ++y)
		{		
			Tile* tile = tilemap->p->m_tiles[x*mapHeight+y];
			for (int z = 0; z < mapDepth; ++z)
			{
				CCNode* tilenode = tile->sp[z];
				if (tilenode)
				{
					tilenode->setPosition(ccp(tile->pos.x - tilemap->p->offset.x,rgss_y_to_cocos_y(tile->pos.y - tilemap->p->offset.y,clipper->getContentSize().height)));
// 					if(tilenode->getChildrenCount()==0)
// 					{
// 						int tileInd = mapData->at(x, y, z);
// 						int prio = tilemap->p->samplePriority(tileInd);
// 						if (z==0 && prio<1)
// 						{
// 							tilemap->p->m_batchNode->addChild(tilenode);
// 						}
// 						else
// 							clipper->addChild(tilenode);
// 					}
// 					else
					clipper->addChild(tilenode);
					tilemap->orderTileZ(tilenode,tile->x,tile->y,z);
					tilemap->p->m_screenTiles.push_back(tile);
				}
				
			}
		}
	}

	tilemap->p->m_screenoxy.x = tilemap->p->offset.x;
	tilemap->p->m_screenoxy.y = tilemap->p->offset.y;

	return 0;
}


