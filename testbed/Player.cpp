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
	pEventMgr->AddCallback<Player>(StateType::Game, "Player_AttackFast", &Player::React, this);
	pEventMgr->AddCallback<Player>(StateType::Game, "Player_ToggleFireMode", &Player::ToggleFireMode, this);
	
}

Player::~Player()
{
	EventManager* pEventMgr = m_pEntityManager->GetContext()->m_pEventManager;

	pEventMgr->RemoveCallback(StateType::Game, "Player_MoveLeft");
	pEventMgr->RemoveCallback(StateType::Game, "Player_MoveRight");
	pEventMgr->RemoveCallback(StateType::Game, "Player_MoveUp");
	pEventMgr->RemoveCallback(StateType::Game, "Player_MoveDown");
	//pEventMgr->RemoveCallback(StateType::Game, "Player_Jump");
	pEventMgr->RemoveCallback(StateType::Game, "Player_Attack");
	pEventMgr->RemoveCallback(StateType::Game, "Player_AttackFast");
	pEventMgr->RemoveCallback(StateType::Game, "Player_ToggleFireMode");
}

void Player::OnEntityCollision(EntityBase* pCollider, bool attack)
{
	if (m_state == EntityState::Dying || pCollider->GetState() == EntityState::Dying) //apply knockback even if the playership is already exploding
	{
		return; 
	}	

	if (pCollider->GetType() == EntityType::PowerUp)
	{
		return;
	}

	GetHurt(1);//take damage from all collisions for now - ll be adjusted when powerups re implemented

	//if (m_state == EntityState::Dying) //apply knockback even if the playership is already exploding
	//{
	//	//SetVelocity(0.0f, 0.0f);
	//	return;
	//}

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
	else 
	if (pDetails->m_name == "Player_Attack")
	{
		Character::Attack();
	}
	else 
	if (pDetails->m_name == "Player_AttackFast")
	{
		Character::AttackFast();
	}
}

void Player::ToggleFireMode(EventDetails* pDetails)
{	
	int x = (int)m_fireMode;
	++x;
	int y = x % (int)FireMode::Count;

	m_fireMode = (FireMode)y;

	std::cout << "TOGGLE FIREMODE TO " << (int) m_fireMode << std::endl;
}

void Player::IncreaseFireMode()
{
	if ((int)m_fireMode == (int)FireMode::Count - 1)
	{
		return; //already maxed out
	}

	m_fireMode = (FireMode)((int)m_fireMode +1);
}

void Player::ResetFiremode()
{
	m_fireMode = FireMode::Normal;
}
