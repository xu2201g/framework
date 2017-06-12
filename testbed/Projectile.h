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

	virtual void OnImpact(EntityBase* pCollider) = 0;

	virtual void OnEntityCollision(EntityBase* pCollider, bool attack) = 0; //not used in here and too lazy to adjust the inheritance atm
	virtual void Update(float dT);

	void Draw(sf::RenderWindow* pWindow);
	void SetRotation(float angle);
	
protected:

	void Animate();

	SpriteSheet m_spriteSheet;

	sf::Vector2f m_direction;

	float m_angle;
};