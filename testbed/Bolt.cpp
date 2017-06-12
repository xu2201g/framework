#include "Bolt.h"

#include "Enemy.h"
#include "Player.h"

Bolt::Bolt(EntityManager* pEntityManager)
	:
	Projectile(pEntityManager)
{
	m_spriteSheet.LoadSheet("../../testbed/assets/spritesheets/Bolt.SHEET");

	m_type = EntityType::Bolt;
	m_state = EntityState::Idle;
	m_position.x = 64.0f;
	m_position.y = 576.0f;

	m_sizeCB.x = 14.0f;
	m_sizeCB.y =  8.0f;

	UpdateAABB();

	m_maxVelocity.x = 400.0f;
	m_maxVelocity.y = 400.0f;

	m_angle = 0.0f;

	sf::Sprite* pSprite = m_spriteSheet.GetSprite();
	sf::Vector2f origin = pSprite->getOrigin();

	pSprite->setOrigin(pSprite->getLocalBounds().width / 2.0f, pSprite->getLocalBounds().height / 2.0f);
	m_spriteSheet.GetCurrentAnimation()->SetFrame(0);
	
}

Bolt::~Bolt()
{
}

void Bolt::OnEntityCollision(EntityBase* pCollider, bool attack)
{
	if (m_state == EntityState::Dying || pCollider->GetState() == EntityState::Dying)
	{
		return;
	}

	if (pCollider->GetType() == EntityType::Bolt)
	{
		return;
	}

	OnImpact(pCollider);
}

void Bolt::OnImpact(EntityBase* pCollider)
{
	//check what is colliding with what in here
	if (pCollider->GetType() == EntityType::Rocket)
	{
		pCollider->ScaleVelocity(0.2f);
		pCollider->SetState(EntityState::Dying);		
	}
	else
	if (pCollider->GetType() == EntityType::Enemy)
	{
		Enemy* pEnemy = (Enemy*)pCollider;

		pEnemy->GetHurt(1);
		
	}
	else
	if (pCollider->GetType() == EntityType::Player)
	{
		Player* pPlayer = (Player*)pCollider;
		pPlayer->GetHurt(1);
	}

	ScaleVelocity(0.0f);
	SetState(EntityState::Dying); //rockets always die/explode on collisions
}

void Bolt::Update(float dT)
{
	Projectile::Update(dT);

	m_spriteSheet.GetSprite()->setRotation(m_angle);


	Move();
}
