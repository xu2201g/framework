#include "Map.h"
#include "BaseState.h"
#include "StateManager.h"
Map::Map(SharedContext* pSharedContext, BaseState* pCurrentState)
	:
	m_pSharedContext(pSharedContext),
	m_pCurrentState(pCurrentState),
	m_defaultTile(pSharedContext),
	m_maxMapSize(32,32),
	m_tileCount(0),
	m_tileSetCount(0),
	m_mapGravity(512.0f),
	m_loadNextMap(false)
{
	m_pSharedContext->m_pGameMap = this;
	LoadTiles("tiles.cfg");
}

Map::~Map()
{
	PurgeMap();
	PurgeTileSet();
	m_pSharedContext->m_pGameMap = nullptr;
}

Tile* Map::GetTile(unsigned int x, unsigned int y)
{
	auto itr = m_tileMap.find(ConvertCoords(x, y));
	return itr != m_tileMap.end() ? itr->second.get() : nullptr;
}

TileInfo* Map::GetDefaultTile()
{
	return &m_defaultTile;
}

float Map::GetGravity() const
{
	return m_mapGravity;
}

const sf::Vector2u& Map::GetMapSize() const
{
	return m_maxMapSize;
}

const sf::Vector2f& Map::GetPlayerStart() const
{
	return m_playerStart;
}

unsigned int Map::GetTileSize() const 
{
	return Sheet::Tile_Size;
}

void Map::LoadNextMap()
{
	m_loadNextMap = true;
}

void Map::LoadMap(const std::string& path)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + path);

	if (!file.is_open())
	{
		std::cout << "! Failed loading next map: " << m_nextMap << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|')
		{
			continue;
		}

		std::stringstream keystream(line);
		std::string type;
		keystream >> type;

		if (type == "TILE")
		{
			int tileId;
			keystream >> tileId;

			if (tileId < 0)
			{
				std::cout << "! Bad tile id: " << tileId << std::endl;
				continue;
			}

			auto itr = m_tileSet.find(tileId);
			if (itr == m_tileSet.end())
			{
				std::cout << "! Tile id(" << tileId << ") was not found in tileset." << std::endl;
				continue;
			}

			sf::Vector2i tileCoords;
			keystream >> tileCoords.x >> tileCoords.y;

			if (tileCoords.x > m_maxMapSize.x || tileCoords.y > m_maxMapSize.y)
			{
				std::cout << "! Tile is out of map range: " << tileCoords.x << " " << tileCoords.y << std::endl;
				continue;
			}

			Tile tile;

			//bind properties from the related tileset
			tile.m_properties = itr->second.get();
			if (!m_tileMap.emplace(ConvertCoords(tileCoords.x, tileCoords.y), std::make_unique<Tile>(tile)).second)
			{
				//duplicate
				std::cout << "! Duplicate tile coordinates: " << tileCoords.x << " " << tileCoords.y << std::endl;
				continue;
			}

			Tile* pTile = m_tileMap.find(ConvertCoords(tileCoords.x, tileCoords.y))->second.get();

			std::string warp;
			keystream >> warp;
			pTile->m_warp = false;

			if (warp == "WARP")
			{
				pTile->m_warp = true;
			}
		}
		else
		if (type == "BACKGROUND")
		{
			if (m_backgroundTexture != "")
			{
				continue;
			}

			keystream >> m_backgroundTexture;

			if (!m_pSharedContext->m_pTextureManager->RequireResource(m_backgroundTexture))
			{
				std::cout << "! Failed to load backgroundtexture: " << m_backgroundTexture << std::endl;
				m_backgroundTexture = "";
				continue;
			}

			sf::Texture* pTexture = m_pSharedContext->m_pTextureManager->GetResource(m_backgroundTexture);
			m_background.setTexture(*pTexture);

			sf::Vector2f viewSize = m_pCurrentState->GetView().getSize();
			sf::Vector2u textureSize = pTexture->getSize();
			sf::Vector2f scaleFactors;

			scaleFactors.x = viewSize.x / textureSize.x;
			scaleFactors.y = viewSize.y / textureSize.y;

			m_background.setScale(scaleFactors);
		}
		else
		if (type == "SIZE")
		{
			keystream >> m_maxMapSize.x >> m_maxMapSize.y;
		}
		else
		if (type == "GRAVITY")
		{
			keystream >> m_mapGravity;
		}
		else
		if (type == "DEFAULT_FRICTION")
		{
			keystream >> m_defaultTile.m_friction.x >> m_defaultTile.m_friction.y;
		}
		else
		if (type == "NEXTMAP")
		{
			keystream >> m_nextMap;
		}

	}
	file.close();

}

void Map::Update(float dT)
{
	if (m_loadNextMap)
	{
		PurgeMap();
		m_loadNextMap = false;
		if (m_nextMap != "") //next map
		{
			LoadMap("asstets//maps//" + m_nextMap);
		}
		else //no next map - game is over
		{
			m_pCurrentState->GetStateManager()->SwitchTo(StateType::GameOver);
		}
		m_nextMap = "";
	}

	sf::FloatRect viewSpace = m_pSharedContext->m_pWindow->GetViewSpace();
	m_background.setPosition(viewSpace.left, viewSpace.top);
}

void Map::Draw()
{
	sf::RenderWindow* pWindow = &m_pSharedContext->m_pWindow->GetRenderWindow();
	pWindow->draw(m_background);

	//culling
	sf::FloatRect viewSpace = m_pSharedContext->m_pWindow->GetViewSpace();

	//gets the coords of the start tiles in the map
	sf::Vector2i tileBegin( floor(viewSpace.left / Sheet::Tile_Size),
						    floor(viewSpace.top / Sheet::Tile_Size));

	//gets the coords of the end tiles in the map
	sf::Vector2i tileEnd( ceil((viewSpace.left + viewSpace.width) / Sheet::Tile_Size),
					      ceil((viewSpace.top + viewSpace.height) / Sheet::Tile_Size));

	//		#########Svvvvvvvvvvv############		v... viewspace rect
	//		#########v##########v############		#... tile
	//		#########v##########v############		S... tile begin
	//		#########v####C#####v############		E... tile end
	//		#########v##########v############		C... character/focused entity (so the viewspace changes related on its movement)
	//		#########vvvvvvvvvvvE############		the whole thing is the gamemap


	//unsigned int count = 0;

	for (int x = tileBegin.x; x <= tileEnd.x; ++x)
	{
		for (int y = tileBegin.y; y <= tileEnd.y; ++y)
		{
			if (x < 0 || y < 0)
			{
				continue;
			}

			Tile* pTile = GetTile(x, y);
			if (!pTile) 
			{
				continue;
			}

			sf::Sprite& sprite = pTile->m_properties->m_sprite;
			sprite.setPosition(x * Sheet::Tile_Size, y * Sheet::Tile_Size);
			pWindow->draw(sprite);

			//++count;
		}
	}



}

unsigned int Map::ConvertCoords(unsigned int x, unsigned int y)
{
	return x * m_maxMapSize.x * y; //TODO
}

void Map::LoadTiles(const std::string& path)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + path);

	if (!file.is_open())
	{
		std::cout << "! Failed loading tile set file: " << path << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|')
		{
			continue;
		}

		std::stringstream keystream(line);
		int tileId;
		keystream >> tileId;
		if (tileId < 0)
		{
			continue;
		}

		TileInfo tileInfo(m_pSharedContext, "TileSheet", tileId);
		keystream >> tileInfo.m_name >> tileInfo.m_friction.x >> tileInfo.m_friction.y >> tileInfo.m_deadly;

		if (!m_tileSet.emplace(tileId, std::make_unique<TileInfo>(tileInfo)).second)
		{
			//duplicate tile
			std::cout << "! Duplicate tile type: " << tileInfo.m_name << std::endl;
		}
	}
	file.close();
}

void Map::PurgeMap()
{
	m_tileCount = 0;
	m_tileMap.clear();
	//m_pSharedContext->pEntityManager->Purge(); TODO

	if (m_backgroundTexture == "")
	{
		return;
	}
	m_pSharedContext->m_pTextureManager->ReleaseResource(m_backgroundTexture);
	m_backgroundTexture = "";
}

void Map::PurgeTileSet()
{
	m_tileSet.clear();
	m_tileSetCount = 0;
}
