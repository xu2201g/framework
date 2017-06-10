#pragma once

#include "Character.h"

class Enemy : public Character
{

public:

	Enemy(EntityManager* pEntityManager);
	~Enemy();

	void OnEntityCollision(EntityBase* pCollider, bool attack);

	void Update(float dT);

private:

	sf::Vector2f m_destination;
	bool m_hasDestination;

	float m_elapsed;
	float m_fireRatePerSecond;
};