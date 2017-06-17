#include "PowerUp.h"
#include "EntityManager.h"
#include "Player.h"

PowerUp::PowerUp(EntityManager* pEntityManager)
	:
	EntityBase(pEntityManager),
	m_spriteSheet(pEntityManager->GetContext()->m_pTextureManager)
{
	m_spriteSheet.LoadSheet("../../testbed/assets/spritesheets/PowerUp.SHEET");

	m_type = EntityType::PowerUp;
	m_state = EntityState::Idle; 
	//m_spriteSheet.SetAnimation("Idle", true, true);
	m_position.x = 0.0;
	m_position.y = 0.0f;

	m_sizeCB.x = 32.0f;
	m_sizeCB.y = 32.0f;

	UpdateAABB();

	m_maxVelocity.x = 0.0f;
	m_maxVelocity.y = 0.0f;

	sf::Sprite* pSprite = m_spriteSheet.GetSprite();
	sf::Vector2f origin = pSprite->getOrigin();

	pSprite->setOrigin(pSprite->getLocalBounds().width / 2.0f, pSprite->getLocalBounds().height / 2.0f);
	m_spriteSheet.GetCurrentAnimation()->SetFrame(0);
}

PowerUp::~PowerUp()
{
}

void PowerUp::Load(const std::string& path)
{
}

void PowerUp::OnEntityCollision(EntityBase* pCollider, bool attack)
{
	if (GetState() == EntityState::Dying) //already used powerups ll be ignored
	{
		return;
	}

	if (!pCollider) //no valid collision
	{
		return;
	}
	
	if (pCollider->GetType() == EntityType::Player) //if powerup collides with the player it gets interesting
	{
		Player* pPlayer = (Player*)pCollider;

		if (pPlayer->GetState() != EntityState::Dying)
		{
			pPlayer->IncreaseFireMode();

			SetState(EntityState::Dying);
		}		
	}
}

void PowerUp::Update(float dT)
{
	EntityBase::Update(dT);

	if (GetState() == EntityState::Dying)
	{
		if (!m_spriteSheet.GetCurrentAnimation()->IsPlaying()) //animation is done
		{
			//so remove entity
			m_pEntityManager->Remove(m_id);
		}
	}

	Animate();
	m_spriteSheet.Update(dT);
	m_spriteSheet.SetSpritePosition(m_position);
}

void PowerUp::Draw(sf::RenderWindow* pWindow)
{
	m_spriteSheet.Draw(pWindow);
}

void PowerUp::Animate()
{
	EntityState state = GetState();

	if (state == EntityState::Dying && m_spriteSheet.GetCurrentAnimation()->GetName() != "Death")
	{
		m_spriteSheet.SetAnimation("Death", true, false);
	}
	else
	if (state == EntityState::Idle && m_spriteSheet.GetCurrentAnimation()->GetName() != "Idle")
	{
		m_spriteSheet.SetAnimation("Idle", true, true);
	}
}
