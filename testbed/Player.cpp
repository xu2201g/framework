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
	pEventMgr->AddCallback<Player>(StateType::Game, "Player_Jump", &Player::React, this);
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
	if (m_state == EntityState::Dying)
	{
		return;
	}

	if (attack)
	{
		if (m_state != EntityState::Attacking)
		{
			return;
		}

		if (!m_spriteSheet.GetCurrentAnimation()->IsInAction())
		{
			return;
		}

		if (pCollider->GetType() != EntityType::Enemy &&
			pCollider->GetType() != EntityType::Player)
		{
			return;
		}

		Character* pOpponent = (Character*)pCollider;

		//apply dmg
		pOpponent->GetHurt(1); 

		//apply knockback
		if (m_position.x > pOpponent->GetPosition().x)
		{
			pOpponent->AddVelocity(-32.0f, 0.0f);
		}
		else
		{
			pOpponent->AddVelocity(32.0f, 0.0f);
		}
	}
}

void Player::React(EventDetails* pDetails)
{
}
