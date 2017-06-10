#include "Player.h"
#include "EntityManager.h"
#include "StateManager.h"

Player::Player(EntityManager* pEntityManager)
	:
	Character(pEntityManager)
{
	Load("../../testbed/assets/characters/player.char");

	m_type = EntityType::Player;
	
	EventManager* pEventMgr = m_pEntityManager->GetContext()->m_pEventManager;

	pEventMgr->AddCallback<Player>(StateType::Game, "Player_MoveLeft", &Player::React, this);
	pEventMgr->AddCallback<Player>(StateType::Game, "Player_MoveRight", &Player::React, this);
	pEventMgr->AddCallback<Player>(StateType::Game, "Player_MoveUp", &Player::React, this);
	pEventMgr->AddCallback<Player>(StateType::Game, "Player_MoveDown", &Player::React, this);

	//pEventMgr->AddCallback<Player>(StateType::Game, "Player_Jump", &Player::React, this);
	pEventMgr->AddCallback<Player>(StateType::Game, "Player_Attack", &Player::React, this);
}

Player::~Player()
{
	EventManager* pEventMgr = m_pEntityManager->GetContext()->m_pEventManager;

	pEventMgr->RemoveCallback(StateType::Game, "Player_MoveLeft");
	pEventMgr->RemoveCallback(StateType::Game, "Player_MoveRight");
	pEventMgr->RemoveCallback(StateType::Game, "Player_Jump");
	pEventMgr->RemoveCallback(StateType::Game, "Player_Attack");
}

void Player::OnEntityCollision(EntityBase* pCollider, bool attack)
{
	if (m_state != EntityState::Dying) //apply knockback even if the playership is already exploding
	{
		GetHurt(1); //take damage from all collisions for now - ll be adjusted when powerups re implemented
	}	

	//apply knockback 	
	float knockbackVelocity = 8.0f;
	//x
	if (m_position.x > pCollider->GetPosition().x)
	{
		AddVelocity(knockbackVelocity, 0.0f);
	}
	else
	{
		AddVelocity(-knockbackVelocity, 0.0f);
	}

	//y
	if (m_position.y > pCollider->GetPosition().y)
	{
		AddVelocity(0.0f, knockbackVelocity);
	}
	else
	{
		AddVelocity(0.0f, -knockbackVelocity);
	}

	//if (attack)
	//{
	//	if (m_state != EntityState::Attacking)
	//	{
	//		return;
	//	}

	//	if (!m_spriteSheet.GetCurrentAnimation()->IsInAction())
	//	{
	//		return;
	//	}

	//	if (pCollider->GetType() != EntityType::Enemy &&
	//		pCollider->GetType() != EntityType::Player)
	//	{
	//		return;
	//	}

	//	Character* pOpponent = (Character*)pCollider;

	//	//apply dmg
	//	pOpponent->GetHurt(1); 

	//	//apply knockback
	//	if (m_position.x > pOpponent->GetPosition().x)
	//	{
	//		pOpponent->AddVelocity(-32.0f, 0.0f);
	//	}
	//	else
	//	{
	//		pOpponent->AddVelocity(32.0f, 0.0f);
	//	}
	//}
}

void Player::React(EventDetails* pDetails)
{
	if (pDetails->m_name == "Player_MoveLeft") 
	{
		Character::Move(Direction::Left);
	}
	else 
	if (pDetails->m_name == "Player_MoveRight") 
	{
		Character::Move(Direction::Right);
	}
	else 
	if (pDetails->m_name == "Player_MoveUp") 
	{
		Character::Move(Direction::Up);
	}
	else
	if (pDetails->m_name == "Player_MoveDown") 
	{
		Character::Move(Direction::Down);
	}
	//if (pDetails->m_name == "Player_Jump") 
	//{
	//	Character::Jump();
	//}
	//else 
	if (pDetails->m_name == "Player_Attack")
	{
		Character::Attack();
	}
}
