#include "Rocket.h"
#include "Character.h"

Rocket::Rocket(EntityManager* pEntityManager)
	:
	Projectile(pEntityManager)
{
	m_type = EntityType::Rocket;
	m_position.x = 64.0f;
	m_position.y = 576.0f;
	
	m_sizeCB.x = 32.0f;
	m_sizeCB.y = 32.0f;

	UpdateAABB();
}

Rocket::~Rocket()
{
}

void Rocket::OnEntityCollision(EntityBase* pCollider, bool attack)
{
	if (m_state == EntityState::Dying)
	{
		return;
	}

	OnImpact();
}

void Rocket::Update(float dT)
{
	Projectile::Update(dT);
}
