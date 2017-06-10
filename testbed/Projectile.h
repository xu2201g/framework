#pragma once

#include "EntityBase.h"
#include "SpriteSheet.h"

class Projectile : public EntityBase
{
	friend class EntityManager;

public:

	Projectile(EntityManager* pEntityManager);
	virtual ~Projectile();

	void Move();

	//void Load(const std::string& path);

	void OnImpact();

	virtual void OnEntityCollision(EntityBase* pCollider, bool attack) = 0; //not used in here and too lazy to adjust the inheritance atm
	virtual void Update(float dT);

	void Draw(sf::RenderWindow* pWindow);

protected:

	void Animate();

	SpriteSheet m_spriteSheet;

	sf::Vector2f m_direction;
};