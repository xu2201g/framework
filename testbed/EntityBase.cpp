#include "EntityBase.h"

#include "EntityManager.h"

EntityBase::EntityBase(EntityManager* pEntityManager)
	:
	m_pEntityManager(pEntityManager),
	m_name("BaseEntity"),
	m_type(EntityType::Base),
	m_pTile(nullptr),
	m_state(EntityState::Idle),
	m_id(0),
	m_collidingOnX(false),
	m_collidingOnY(false),
	m_friction(0.8,0.0)
{
}

EntityBase::~EntityBase()
{
}

void EntityBase::SetPosition(const float& x, const float& y)
{
	SetPosition(sf::Vector2f(x, y));
}

void EntityBase::SetPosition(const sf::Vector2f& pos)
{
	m_position = pos;
	UpdateAABB();
}

void EntityBase::SetSize(const float& x, const float& y)
{
	SetSize(sf::Vector2f(x, y));
}

void EntityBase::SetSize(const sf::Vector2f& size)
{
	m_sizeCB = size;
	UpdateAABB();
}

void EntityBase::SetState(const EntityState& state)
{
	if (m_state == EntityState::Dying) //if entity is dying there is no way to change its state
	{
		return;
	}

	m_state = state;
}

unsigned int EntityBase::GetId()
{
	return m_id;
}

const std::string& EntityBase::GetName()
{
	return m_name;
}

EntityType EntityBase::GetType()
{
	return m_type;
}

EntityState EntityBase::GetState()
{
	return m_state;
}

sf::Vector2f EntityBase::GetPosition()
{
	return m_position;
}

void EntityBase::Move(float x, float y)
{
	//store last position
	m_positionOld = m_position;

	//update position
	m_position += sf::Vector2f(x, y);

	sf::Vector2u mapSize;
	mapSize = m_pEntityManager->GetContext()->m_pGameMap->GetMapSize();

	//check wether entity went out of map bounds
	//x
	if (m_position.x < 0)
	{
		m_position.x = 0;
	}
	else
	if (m_position.x > (mapSize.x + 1) * Sheet::Tile_Size)
	{
		m_position.x = (mapSize.x + 1) * Sheet::Tile_Size;
	}

	//y
	if (m_position.y < 0)
	{
		m_position.y = 0;
	}
	else
	if (m_position.y > (mapSize.y + 1) * Sheet::Tile_Size)
	{
		m_position.y = (mapSize.y + 1) * Sheet::Tile_Size;
		SetState(EntityState::Dying);  //entity ll die if its falling onto the maps underground 
	}

	UpdateAABB();
}

void EntityBase::AddVelocity(float x, float y)
{
	//update velocity
	m_velocity += sf::Vector2f(x, y);

	//check if its out of bounds
	//x
	if (abs(m_velocity.x) > m_maxVelocity.x)
	{
		//check direction
		if (m_velocity.x > 0)
		{
			m_velocity.x = m_maxVelocity.x;
		}
		else
		{
			m_velocity.x = -m_maxVelocity.x;
		}
	}

	//y
	if (abs(m_velocity.y) > m_maxVelocity.y)
	{
		//check direction
		if (m_velocity.y > 0)
		{
			m_velocity.y = m_maxVelocity.y;
		}
		else
		{
			m_velocity.y = -m_maxVelocity.y;
		}
	}
}

void EntityBase::Accelerate(float x, float y)
{
	m_acceleration += sf::Vector2f(x, y);
}

void EntityBase::SetAcceleration(float x, float y)
{
	m_acceleration = sf::Vector2f(x, y);
}

void EntityBase::ApplyFriction(float x, float y)
{
	//reduces the velocity related on the friction value (absolute not relative)
	if (m_velocity.x != 0)
	{
		//x
		if (abs(m_velocity.x) - abs(x) < 0)
		{
			m_velocity.x = 0;
		}
		else
		{
			if (m_velocity.x < 0)
			{
				m_velocity.x += x;
			}
			else
			{
				m_velocity.x -= x;
			}
		}

		//y
		if (abs(m_velocity.y) - abs(x) < 0)
		{
			m_velocity.y = 0;
		}
		else
		{
			if (m_velocity.y < 0)
			{
				m_velocity.y += x;
			}
			else
			{
				m_velocity.y -= x;
			}
		}
	}	
}

void EntityBase::Update(float dT)
{
	Map* map = m_pEntityManager->GetContext()->m_pGameMap;

	float gravity = map->GetGravity();

	//apply gravity acceleration
	Accelerate(0.0f, gravity);
	AddVelocity(m_acceleration.x * dT, m_acceleration.y * dT);

	//reset acceleration
	SetAcceleration(0.0f, 0.0f);
	sf::Vector2f frictionValue;

	//load friction value
	if (m_pTile) //if the entity lies on a valid tile the friction of this tile ll be used
	{
		frictionValue = m_pTile->m_friction;
		if (m_pTile->m_deadly)
		{
			SetState(EntityState::Dying); 
		}
	}
	else
	if (map->GetDefaultTile()) //if a defaultTile is setup for the map its friction value ll be applied
	{
		frictionValue = map->GetDefaultTile()->m_friction;
	}
	else //otherwise the standard friction value defined in the EntityBase class ll be used
	{
		frictionValue = m_friction;
	}

	float frictionX = (m_speed.x * frictionValue.x) * dT;
	float frictionY = (m_speed.y * frictionValue.y) * dT;

	ApplyFriction(frictionX, frictionY); //reduces velocity
	sf::Vector2f deltaPos = m_velocity * dT;

	Move(deltaPos.x, deltaPos.y); //update the position

	//reset collision flags
	m_collidingOnX = false;
	m_collidingOnY = false;

	CheckCollisions();
	ResolveCollisions();
}

void EntityBase::UpdateAABB()
{
	//the origin of an entity is on the center in x and the bottom of y			// #####
	m_AABB = sf::FloatRect(m_position.x - (m_sizeCB.x / 2.0f),					// #####
		                   m_position.y - m_sizeCB.y,							// #####
						   m_sizeCB.x,											// #####
						   m_sizeCB.y);											// ##O## O... origin
}

void EntityBase::CheckCollisions()
{
	Map* pGameMap = m_pEntityManager->GetContext()->m_pGameMap;
	unsigned int tileSize = pGameMap->GetTileSize();

	//get the start and the end coordinates the entity is covering by its AABB in the gamemap to identify all tiles in the gamemap we ve to check

	//		xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx			xxxxxxxx
	//		x		x		x		x		x			x	   x
	//		x		x		x		x		x			x	   x
	//		x		x		x		x		x			x	   x
	//		xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx			xxxxxxxx ... tile of the gamemap
	//		x		x	eeeeeee		x		x
	//		x		x	eeeeeee		x		x			eeeeeeee
	//		x		x	eeeeeee		x		x			eeeeeeee
	//		xxxxxxxxxxxxeeeeeeexxxxxxxxxxxxxx			eeeeeeee
	//		x		x	eeeeeee		x		x			eeeeeeee
	//		x		x		x		x		x			eeeeeeee ... entity
	//		x		x		x		x		x
	//		xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx			

	//calculating the x and y coordinates of the tiles colliding the the AABB of the entity from the game map
	int fromX = floor(m_AABB.left / tileSize);
	int toX = floor((m_AABB.left + m_AABB.width) / tileSize);

	int fromY = floor(m_AABB.top / tileSize);
	int toY = floor((m_AABB.top + m_AABB.height) / tileSize);

	//iterating through these coordinates 
	for (int x = fromX; x <= toX; ++x)
	{
		for (int y = fromY; y <= toY; ++y)
		{
			//get the tile from the gamemap at position x y 
			Tile* pTile = pGameMap->GetTile(x, y);
			if (!pTile) //no valid tile found, so no collision
			{
				return;
			}

			//create a rect representing the area the tile at x y is covering
			sf::FloatRect tileBounds(x * tileSize, 
									 y * tileSize,
									 tileSize,
									 tileSize);

			//calculate the intersection
			sf::FloatRect intersection;
			m_AABB.intersects(tileBounds, intersection);

			//calculate the area of the intersection
			float area = intersection.width * intersection.height;

			//create a collision element and store it into the collisions vector
			//it stores the area of intersection, the tile of the map, and the rect the tile is covering
			CollisionElement e(area, pTile->m_properties, tileBounds);
			m_collisions.emplace_back(e);

			//set the flag loadnext map to true if the collision is between the player entity and a warp tile
			if (pTile->m_warp && m_type == EntityType::Player)
			{
				pGameMap->LoadNextMap();
			}
		}
	}
}

void EntityBase::ResolveCollisions()
{
	if (!m_collisions.empty())
	{
		//sort by area
		std::sort(m_collisions.begin(), m_collisions.end(), SortCollisions);

		Map* pGameMap = nullptr;
		//Map* pGameMap = m_pEntityManager->GetContext()->m_pGameMap; TODO

		unsigned int tileSize = pGameMap->GetTileSize();

		for (auto &itr : m_collisions)
		{
			if (!m_AABB.intersects(itr.m_tileBounds))	//collisions may be already resolved as a sideeffect from resolving other collisions before
			{
				continue;
			}

			float xDiff = (m_AABB.left + (m_AABB.width / 2)) - (itr.m_tileBounds.left + (itr.m_tileBounds.width / 2)); //calculate the difference by the origins of each bounds
			float yDiff = (m_AABB.top + (m_AABB.height / 2)) - (itr.m_tileBounds.top + (itr.m_tileBounds.height / 2));

			float resolve = 0.0f;

			if (abs(xDiff) > abs(yDiff))	//if xDiff is greather than yDiff the resolving is done by the x axis otherwise in the y axis
			{
				//calculate the distance in x direction to resolve the collision
				if (xDiff > 0) 
				{
					resolve = (itr.m_tileBounds.left + tileSize) - m_AABB.left; //the amount of pixels to move the entity to the right direction to resolve the collision
				}
				else
				{
					resolve = -((m_AABB.left + m_AABB.width) - itr.m_tileBounds.left); //the amount of pixels to move the entity to the left direction to resolve the collision
				}

				//apply the distance
				Move(resolve, 0.0f);
				m_velocity.x = 0.0f;
				m_collidingOnX = true;
			}
			else
			{
				//calculate the distance in y direction to resolve the collision
				if (yDiff > 0)
				{
					resolve = (itr.m_tileBounds.top + tileSize) - m_AABB.top; //the amount of pixels to move the entity to the right direction to resolve the collision
				}
				else
				{
					resolve = -((m_AABB.top + m_AABB.height) - itr.m_tileBounds.top); //the amount of pixels to move the entity to the left direction to resolve the collision
				}

				//apply the distance
				Move(0.0f, resolve);
				m_velocity.y = 0.0f;

				if (m_collidingOnY)
				{
					continue;
				}
				m_pTile = itr.m_pTile; //update the tile reference the entity is standing on/over
				m_collidingOnY = true;
			}
		}
		m_collisions.clear();
	}

	if (!m_collidingOnY)
	{
		m_pTile = nullptr;
	}
}

bool SortCollisions(const CollisionElement& a, const CollisionElement& b)
{
	return a.m_area > b.m_area;
}
