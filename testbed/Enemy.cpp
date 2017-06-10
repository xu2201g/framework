#include "Enemy.h"

Enemy::Enemy(EntityManager* pEntityManager)
	:
	Character(pEntityManager)
{
	m_type = EntityType::Enemy;
	m_fireRatePerSecond = 0.3f;
	m_elapsed = 0.0f;
}

Enemy::~Enemy()
{
}

void Enemy::OnEntityCollision(EntityBase* pCollider, bool attack)
{
	if (m_state == EntityState::Dying)
	{
		return;
	}

	//if (attack)
	//{
	//	return;
	//}

	//if (pCollider->GetType() != EntityType::Player)
	//{
	//	return;
	//}

	//Character* pPlayer = (Character*) pCollider;

	//pPlayer->GetHurt(1);

	//if (m_position.x > pPlayer->GetPosition().x)
	//{
	//	pPlayer->AddVelocity(-m_speed.x, 0.0f);
	//	m_spriteSheet.SetDirection(Direction::Left);
	//}
	//else
	//{
	//	pPlayer->AddVelocity(m_speed.x, 0.0f);
	//	m_spriteSheet.SetDirection(Direction::Right);
	//}
}

void Enemy::Update(float dT) //TODO
{
	Character::Update(dT);

	if (m_state == EntityState::Dying)
	{
		return;
	}
	m_elapsed += dT;

	if (m_elapsed >= 1.0f / m_fireRatePerSecond)
	{
		Attack();

		m_elapsed = 0.0f;
	}



	//if (m_hasDestination)
	//{
	//	if (abs(m_destination.x - m_position.x) < 16)
	//	{
	//		m_hasDestination = false; //reached destination by a threshold of 16 px
	//		return;
	//	}
	//	if (m_destination.x - m_position.x > 0)
	//	{
	//		Move(Direction::Right);
	//	}
	//	else
	//	{
	//		Move(Direction::Left);
	//	}

	//	if (m_collidingOnX)
	//	{
	//		m_hasDestination = false; //cant reach destination cause of collision within the game map
	//	}
	//	return;
	//}

	//int random = rand() % 1000 + 1;

	////if (random != 1000) //?????
	////{
	////	return;
	////}

	//int newX = rand() % 65 + 0;
	//if (rand() % 2) 
	//{ 
	//	newX = -newX; 
	//}

	//m_destination.x = m_position.x + newX;

	//if (m_destination.x < 0)
	//{
	//	m_destination.x = 0;
	//}

	//m_hasDestination = true;
}
