#include "Rocket.h"
#include "Character.h"
#include "Enemy.h"
#include "Player.h"

Rocket::Rocket(EntityManager* pEntityManager)
	:
	Projectile(pEntityManager)
{
	m_spriteSheet.LoadSheet("../../testbed/assets/spritesheets/Rocket.SHEET");

	m_type = EntityType::Rocket;
	m_state = EntityState::Idle;
	m_position.x = 64.0f;
	m_position.y = 576.0f;
	
	m_sizeCB.x = 26.0f;
	m_sizeCB.y =  9.0f;

	UpdateAABB();

	m_maxVelocity.x = 200.0f;
	m_maxVelocity.y = 200.0f;

	m_angle = 0.0f;

	sf::Sprite* pSprite = m_spriteSheet.GetSprite();
	sf::Vector2f origin = pSprite->getOrigin();

	pSprite->setOrigin(pSprite->getLocalBounds().width / 2.0f, pSprite->getLocalBounds().height / 2.0f);
	m_spriteSheet.GetCurrentAnimation()->SetFrame(0);
	
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
	if (m_state == EntityState::Dying || pCollider->GetState() == EntityState::Dying)
	{
		return;
	}

	OnImpact(pCollider);
}

void Rocket::OnImpact(EntityBase* pCollider)
{
	//check what is colliding with what in here
	if (pCollider->GetType() == EntityType::Rocket)
	{
		pCollider->ScaleVelocity(0.2f);
		pCollider->SetState(EntityState::Dying);

		ScaleVelocity(0.2f); //let the explosion flow a bit more in the origin direction
	}
	else
	if (pCollider->GetType() == EntityType::Enemy)
	{
		Enemy* pEnemy = (Enemy*)pCollider;

		pEnemy->SetState(EntityState::Dying);
		ScaleVelocity(0.0f);
	}
	else
	if (pCollider->GetType() == EntityType::Player)
	{
		Player* pPlayer = (Player*)pCollider;
		pPlayer->SetState(EntityState::Dying);
		ScaleVelocity(0.0f);
	}

	SetState(EntityState::Dying); //rockets always die/explode on collisions	
}

void Rocket::Update(float dT)
{
	Projectile::Update(dT);


	//rotate around the center of the sprite
	m_spriteSheet.GetSprite()->setRotation(m_angle);


	Move();
}


