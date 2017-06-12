#pragma once

#include "Projectile.h"

class Bolt : public Projectile
{

public:

	Bolt(EntityManager* pEntityManager);
	~Bolt();

	void OnEntityCollision(EntityBase* pCollider, bool attack);
	void OnImpact(EntityBase* pCollider);

	void Update(float dT);
	
private:

};