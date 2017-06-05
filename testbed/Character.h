#pragma once

#include "EntityBase.h"
#include "Directions.h"
#include "SpriteSheet.h"

class Character : public EntityBase
{
	friend class EntityManager;

public:

	Character(EntityManager* pEntityManager);
	virtual ~Character();

	void Move(const Direction& direction);
	void Jump();
	void Attack();
	void GetHurt(const int& damage);

	void Load(const std::string& path);

	virtual void OnEntityCollision(EntityBase* pCollider, bool attack) = 0;

	virtual void Update(float dT);

	void Draw(sf::RenderWindow* pWindow);

protected:

	void UpdateAttackAABB();
	void Animate();

	//member
	SpriteSheet m_spriteSheet;
	float m_jumpVelocity;
	int m_hitpoints;

	sf::FloatRect m_attackAABB;
	sf::Vector2f m_attackAABBOffset;

};