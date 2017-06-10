#pragma once

#include "Projectile.h"

class Rocket : public Projectile
{

public:

	Rocket(EntityManager* pEntityManager);
	~Rocket();

	void OnEntityCollision(EntityBase* pCollider, bool attack);

	void Update(float dT);

private:

	sf::Vector2f m_destination;
};