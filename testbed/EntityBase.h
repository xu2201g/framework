#pragma once

#include <SFML/Graphics.hpp>
#include "Map.h"

enum class EntityType
{
	Base,
	Enemy,
	Player,

	Rocket,
	Bolt
};

enum class EntityState
{
	Idle, 
	Walking,
	Jumping,
	Attacking,
	AttackingFast,
	Hurt,
	Dying
};

//used to handle collisions 
struct CollisionElement
{
	CollisionElement(float area, TileInfo* pTile, const sf::FloatRect& bounds)
		:
		m_area(area),
		m_pTile(pTile),
		m_tileBounds(bounds)
	{}

	float m_area;					//area of collision
	TileInfo* m_pTile;				//tile the entity is colliding with
	sf::FloatRect m_tileBounds;		//its boundary
};

//used to store collisions ordered by the collisions area
using Collisions = std::vector<CollisionElement>;

//function used to sort the collisions in the collisions vector
//returns true if collisionarea of a is greater than bs
bool SortCollisions(const CollisionElement& a, const CollisionElement& b);

//entity movement is modeled as follows (position -> velocity -> acceleration derivation by time)
//I. entity is accelerated and its acceleration adjusts its velocity
//II. entity position is recalculated based on its velocity
//III. velocity of an entity is damped by the friction coefficient

class EntityManager;

//notice that entities re able to move continuous over a map screened like a chessboard - think like mario or other sidescroller

class EntityBase
{
	friend class EntityManager; //to let EntityManager class access members of EntityBase

public:

	EntityBase(EntityManager* pEntityManager);
	virtual ~EntityBase();

	void SetPosition(const float& x, const float& y);
	void SetPosition(const sf::Vector2f& pos);
	void SetSize(const float& x, const float& y);
	void SetSize(const sf::Vector2f& size);
	void SetState(const EntityState& state);
	void ScaleVelocity(float scale);

	unsigned int GetId();
	const std::string& GetName();
	EntityType GetType();
	EntityState GetState();
	sf::Vector2f GetPosition();

	void Move(float x, float y);
	void AddVelocity(float x, float y);
	void SetVelocity(float x, float y);
	void SetMaxVelocity(float x, float y);
	void Accelerate(float x, float y);

	void SetAcceleration(float x, float y);
	void ApplyFriction(float x, float y);

	virtual void Update(float dT);
	virtual void Draw(sf::RenderWindow* pWindow) = 0;

protected:

	void UpdateAABB();
	void CheckCollisions();
	void ResolveCollisions();
	virtual void OnEntityCollision(EntityBase* collider, bool attack) = 0;

	//member
	std::string m_name;
	EntityType m_type;
	unsigned int m_id;

	sf::Vector2f m_position;
	sf::Vector2f m_positionOld;
	sf::Vector2f m_velocity;
	sf::Vector2f m_maxVelocity;
	sf::Vector2f m_speed;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_friction;

	TileInfo* m_pTile; //tile underneath entity

	sf::Vector2f m_sizeCB; //collisionbox size
	sf::FloatRect m_AABB; //bounding box
	sf::RectangleShape m_boundingBox; //visual representation 
	EntityState m_state;

	bool m_collidingOnX;
	bool m_collidingOnY;

	bool m_debugOverlay;

	Collisions m_collisions;
	EntityManager* m_pEntityManager;

};