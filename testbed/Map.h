#pragma once

//#include "BaseState.h"
#include "SharedContext.h"

class BaseState;
//constants to load a spritesheet
enum Sheet
{
	Tile_Size = 32,		//pixel width/heights of a tile - a segment represented by a drawn sprite later on
	Sheet_Width = 256,  //pixel 
	Sheet_Height = 256	//pixel 
};

using TileID = unsigned int; //tiles re identified by their tile id

//properties of a type of tile
struct TileInfo
{
	TileInfo(SharedContext* context, const std::string& texture = "", TileID id = 0)
		:
		m_pSharedContext(context),
		m_id(id),
		m_deadly(false)
	{
		TextureManager* textureMgr = m_pSharedContext->m_pTextureManager;
		if (texture == "") 
		{	
			//no texture
			return; 
		} 

		if (!textureMgr->RequireResource(texture))
		{
			//requiring texture from texturemanager failed
			std::cout << "couldnt require texture: " << texture << std::endl;
			return;
		}

		//texture required
		//update members and set the sprite
		m_texture = texture;
		m_sprite.setTexture(*textureMgr->GetResource(m_texture));

		//map the id to an intrect to crop the right sprite from the sheet
		sf::IntRect tileBoundaries( m_id % (Sheet::Sheet_Width / Sheet::Tile_Size) * Sheet::Tile_Size,  //x position (row)
									m_id / (Sheet::Sheet_Height / Sheet::Tile_Size) * Sheet::Tile_Size, //y position (col)
									Sheet::Tile_Size,	//width 
									Sheet::Tile_Size);	//height

		//apply the bounds to crop the right sprite from the sheet
		m_sprite.setTextureRect(tileBoundaries);
	}

	~TileInfo()
	{
		if (m_texture == "")
		{
			return;
		}

		m_pSharedContext->m_pTextureManager->ReleaseResource(m_texture);
	}

	sf::Sprite m_sprite;
	TileID m_id;				//its the an identifier what type of tile/block it is 
	std::string m_name;
	sf::Vector2f m_friction;	//influences entity movement on that tile	
	bool m_deadly;				//entities on that block die

	SharedContext* m_pSharedContext;
	std::string m_texture;

};

//the actual tile which represents a block of the map
struct Tile
{
	TileInfo* m_properties; //it only holds pointer to the related property struct 
	bool m_warp; //character warps (loading next map) when he enters this tile and the flag is true
};

//the actual game map 
//mention that the TileID here represents the position coordinates on the screen 
//this 1d value ll be maped onto 2d coordinates 
using TileMap = std::unordered_map<TileID, std::unique_ptr<Tile>>;

//the set of different tiletypes like a painters palette
//identified by the TileID NOT the same id like the map uses
using TileSet = std::unordered_map<TileID, std::unique_ptr<TileInfo>>;

class Map
{
public:

	Map(SharedContext* pSharedContext, BaseState* pCurrentState);
	~Map();

	Tile* GetTile(unsigned int x, unsigned int y);
	TileInfo* GetDefaultTile();
	float GetGravity() const;
	const sf::Vector2u& GetMapSize() const;
	const sf::Vector2f& GetPlayerStart() const;

	unsigned int GetTileSize() const;

	void LoadMap(const std::string& path);
	void LoadNextMap();
	
	void Update(float dT);
	void Draw();

	unsigned int GetScrollSpeed()
	{
		return m_scrollPixelPerSecond;
	}

	TileSet& GetTileSet()
	{
		return m_tileSet;
	}

	TileMap& GetTIleMap()
	{
		return m_tileMap;
	}

	unsigned int ConvertCoords(unsigned int x, unsigned int y);

	void PurgeMap();

	//quick fix to adjust background TODO
	std::string m_backgroundTexture;
	sf::Sprite m_background;

private:

	

	//loads the different types of tiles
	void LoadTiles(const std::string& path);
	
	
	void PurgeTileSet();

	//member
	TileSet m_tileSet;
	TileMap m_tileMap;
	
	TileInfo m_defaultTile;

	sf::Vector2u m_maxMapSize;
	sf::Vector2f m_playerStart;

	unsigned int m_tileCount;
	unsigned int m_tileSetCount;

	unsigned int m_scrollPixelPerSecond;

	float m_mapGravity;
	std::string m_nextMap;
	bool m_loadNextMap;

	
	BaseState* m_pCurrentState;
	SharedContext* m_pSharedContext;

};