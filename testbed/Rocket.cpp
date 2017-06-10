#include "Rocket.h"
#include "Character.h"

Rocket::Rocket(EntityManager* pEntityManager)
	:
	Projectile(pEntityManager)
{
	m_type = EntityType::Rocket;
	m_state = EntityState::Idle;
	m_position.x = 64.0f;
	m_position.y = 576.0f;
	
	m_sizeCB.x = 32.0f;
	m_sizeCB.y = 32.0f;

	UpdateAABB();

	m_maxVelocity.x = 100.0f;
	m_maxVelocity.y = 100.0f;

	m_angle = 0.0f;

	sf::Sprite* pSprite = m_spriteSheet.GetSprite();
	sf::Vector2f origin = pSprite->getOrigin();

	pSprite->setOrigin(pSprite->getLocalBounds().width / 2.0f, pSprite->getLocalBounds().height / 2.0f);

}

//Rocket::Rocket(EntityManager* pEntityManager, sf::Vector2f direction, sf::Vector2f position)
//{
//	m_type = EntityType::Rocket;
//	m_position = position;
//
//	m_sizeCB.x = 32.0f;
//	m_sizeCB.y = 32.0f;
//
//	UpdateAABB();
//
//	m_velocity.x = m_speed.x * direction.x;
//	m_velocity.y = m_speed.y * direction.y;
//
//	m_maxVelocity.x = 100.0f;
//	m_maxVelocity.y = 100.0f;
//}

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


	//rotate around the center of the sprite
	m_spriteSheet.GetSprite()->setRotation(m_angle);


	Move();
}

void Rocket::SetRotation(float angle)
{
	m_angle = angle;
}
