#include "Character.h"
#include "EntityManager.h"
#include "Rocket.h"
#include "Bolt.h"

Character::Character(EntityManager* pEntityManager)
	:
	EntityBase(pEntityManager),
	m_spriteSheet(pEntityManager->GetContext()->m_pTextureManager),
	//m_jumpVelocity(250),
	m_hitpoints(5),
	m_fireMode(FireMode::Insane)
{
	m_name = "Character";
}

Character::~Character()
{
}

void Character::Move(const Direction& direction)
{
	if (GetState() == EntityState::Dying) //cant move dying entities
	{
		return;
	}

	//apply direction to the spritesheet so that the right animation ll be loaded
	//m_spriteSheet.SetDirection(direction);		//there is only 1 direction for now

	//apply the acceleration
	switch (direction)
	{
		case Direction::Left:
			Accelerate(-m_speed.x, 0.0f);
			break;
		case Direction::Right:
			Accelerate(m_speed.x, 0.0f);
			break;
		case Direction::Up:
			Accelerate(0.0f, -m_speed.y);
			break;
		case Direction::Down:
			Accelerate(0.0f, m_speed.y);
			break;
	}
	
	//if the character is not moving already update its state
	if (GetState() == EntityState::Idle)
	{
		SetState(EntityState::Walking);
	}
}

//void Character::Jump() //no need to jump in a space shooter
//{
//	if (GetState() == EntityState::Dying ||		//cant jump while dying, jumping or getting hurt
//		GetState() == EntityState::Jumping ||
//		GetState() == EntityState::Hurt)
//	{
//		return;
//	}
//
//	//update state
//	SetState(EntityState::Jumping);
//
//	//add velocity so that the character ll be moved
//	AddVelocity(0, -m_jumpVelocity);
//}

void Character::Attack() 
{
	if (GetState() == EntityState::Dying ||		//cant attack while dying, jumping, getting hurt or attacking
		//GetState() == EntityState::Jumping ||
		//GetState() == EntityState::Hurt ||
		GetState() == EntityState::Attacking)
	{
		return;
	}

	//update the state
	SetState(EntityState::Attacking); //this is enough cause this "flag" ll be checked in the onentitycollision functions to perform actually an attack 

	//fire projectiles
	EntityManager* pEntityMgr = m_pEntityManager->GetContext()->m_pEntityManager;
	int id = pEntityMgr->Add(EntityType::Rocket, "ROCKET");

	EntityBase* pEntity = pEntityMgr->Find(id);
	if (pEntity)
	{
		Rocket* pRocket = (Rocket*)pEntity;

		//check for player or enemy firing the rocket
		if (m_type == EntityType::Player)
		{
			pRocket->SetRotation(180.0f);
			pRocket->SetPosition(m_position.x + (float)Sheet::Tile_Size, m_position.y);
			pRocket->AddVelocity(200.0f, 0.0f);
		}
		else
		if (m_type == EntityType::Enemy)
		{
			pRocket->SetPosition(m_position.x - (float)Sheet::Tile_Size, m_position.y);
			pRocket->AddVelocity(-200.0f, 0.0f);
		}
	}
}

void Character::AttackFast()
{
	if (GetState() == EntityState::Dying ||		//cant attack while dying, jumping, getting hurt or attacking
												//GetState() == EntityState::Jumping ||
												//GetState() == EntityState::Hurt ||
		GetState() == EntityState::AttackingFast ||
		GetState() == EntityState::Attacking)
	{
		return;
	}

	//update the state
	SetState(EntityState::AttackingFast); //this is enough cause this "flag" ll be checked in the onentitycollision functions to perform actually an attack 

	
	//fire projectiles
	EntityManager* pEntityMgr = m_pEntityManager->GetContext()->m_pEntityManager;
	int i = 0;
	do
	{ 
		int id = pEntityMgr->Add(EntityType::Bolt, "BOLT");

		EntityBase* pEntity = pEntityMgr->Find(id);
		if (pEntity)
		{
			Bolt* pBolt = (Bolt*)pEntity;

			//check for player or enemy firing the rocket
			if (m_type == EntityType::Player)
			{
				switch(i)
				{ 
				case 0:
					pBolt->SetRotation(180.0f);
					pBolt->SetPosition(m_position.x + (float)Sheet::Tile_Size, m_position.y);
					pBolt->AddVelocity(400.0f, 0.0f);
					break;
				case 1:
					pBolt->SetRotation(180.0f);
					pBolt->SetPosition(m_position.x + (float)Sheet::Tile_Size, m_position.y);
					pBolt->AddVelocity(300.0f, 100.0f);
					break;
				case 2:
					pBolt->SetRotation(180.0f);
					pBolt->SetPosition(m_position.x + (float)Sheet::Tile_Size, m_position.y);
					pBolt->AddVelocity(300.0f, -100.0f);
					break;
				}
				
			}
			else
			if (m_type == EntityType::Enemy)
			{
				pBolt->SetPosition(m_position.x - (float)Sheet::Tile_Size, m_position.y);
				pBolt->AddVelocity(-400.0f, 0.0f);
			}
		}
		++i;
	} while (i <= (int)m_fireMode);
}

void Character::GetHurt(const int& damage) //ll be adjusted soon
{
	if (GetState() == EntityState::Dying ||
		GetState() == EntityState::Hurt)
	{
		return;
	}

	//reduce hitpoints to at least 0
	m_hitpoints = (m_hitpoints - damage > 0 ? m_hitpoints - damage : 0);

	if (m_hitpoints)
	{
		SetState(EntityState::Hurt);
	}
	else
	{
		SetState(EntityState::Dying);
	}
}

void Character::Load(const std::string& path)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + path);

	if (!file.is_open())
	{
		std::cout << "! Failed to load character: " << path << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|')
		{
			continue;
		}

		std::stringstream keystream(line);
		std::string type;

		keystream >> type;

		if (type == "Name")
		{
			keystream >> m_name;
		}
		else
		if (type == "SpriteSheet")
		{
			std::string spriteFile;
			keystream >> spriteFile;

			m_spriteSheet.LoadSheet("..//..//testbed//assets//spritesheets//" + spriteFile);
		}
		else
		if (type == "Hitpoints")
		{
			keystream >> m_hitpoints;
		}
		else
		if (type == "BoundingBox")
		{
			sf::Vector2f boundSize;
			keystream >> boundSize.x >> boundSize.y;

			SetSize(boundSize);
		}
		else
		//if (type == "DamageBox") //damage ll be handled by projectiles
		//{
		//	keystream >> m_attackAABBOffset.x >> m_attackAABBOffset.y >> m_attackAABB.width >> m_attackAABB.height;
		//}
		//else
		if (type == "Speed")
		{
			keystream >> m_speed.x >> m_speed.y;
		}
		else
		//if (type == "JumpVelocity") //there is no jumping anymore
		//{
		//	keystream >> m_jumpVelocity;
		//}
		//else
		if (type == "MaxVelocity")
		{
			keystream >> m_maxVelocity.x >> m_maxVelocity.y;
		}
		else
		{
			std::cout << "! Unknown type: \"" << type <<"\" in character file: " << path << std::endl;
		}
	}

	file.close();
}

void Character::Update(float dT)
{
	EntityBase::Update(dT);

	//if (m_attackAABB.width != 0 && m_attackAABB.height != 0)
	//{
	//	UpdateAttackAABB();
	//}

	//update enitystate
	if (GetState() != EntityState::Dying && GetState() != EntityState::Attacking && GetState() != EntityState::AttackingFast && GetState() != EntityState::Hurt)
	{
		
		//if (abs(m_velocity.y) > 0.001f) //jumping 
		//{
		//	SetState(EntityState::Jumping);
		//}
		//else
		//if (abs(m_velocity.x) > 0.1f || abs(m_velocity.y)) //walking
		//{
		//	SetState(EntityState::Walking);
		//}
		//else
		{
			SetState(EntityState::Idle); //idle
		}
	}
	else
	if (GetState() == EntityState::Attacking || GetState() == EntityState::Hurt || GetState() == EntityState::AttackingFast)
	{
		if (!m_spriteSheet.GetCurrentAnimation()->IsPlaying()) //animation is done
		{
			//so set state back to idle
			SetState(EntityState::Idle);
		}
	}
	else
	if (GetState() == EntityState::Dying)
	{
		if (!m_spriteSheet.GetCurrentAnimation()->IsPlaying()) //animation is done
		{
			//so remove entity
			m_pEntityManager->Remove(m_id);
		}
	}
	//else
	//if(GetState() == EntityState::Jumping)
	//{
	//	if (!m_spriteSheet.GetCurrentAnimation()->IsPlaying())
	//	{
	//		SetState(EntityState::Idle);
	//	}
	//}

	Animate(); //sets animations related on the entitystate
	m_spriteSheet.Update(dT);
	m_spriteSheet.SetSpritePosition(m_position);
}

void Character::Draw(sf::RenderWindow* pWindow)
{
	//sf::RectangleShape AABB(sf::Vector2f(m_AABB.width, m_AABB.height));
	//AABB.setPosition(m_AABB.left, m_AABB.top);
	//
	//sf::RectangleShape attackAABB(sf::Vector2f(m_attackAABB.width, m_attackAABB.height));
	//attackAABB.setPosition(m_spriteSheet.GetSpritePosition());

	m_spriteSheet.Draw(pWindow);
	//if (m_pTile)
	//{
	//	sf::Sprite s = m_pTile->m_sprite;
	//	s.setPosition(m_position);
	//	m_pEntityManager->GetContext()->m_pWindow->GetRenderWindow().draw(s);
	//}
	//pWindow->draw(AABB);
	//pWindow->draw(attackAABB);
}

//void Character::UpdateAttackAABB()
//{
//	//x
//	m_attackAABB.left = m_spriteSheet.GetDirection() == Direction::Left ? 
//						(m_AABB.left - m_attackAABB.width) - m_attackAABBOffset.x :
//						(m_AABB.left + m_AABB.width) + m_attackAABBOffset.x;
//	//y
//	m_attackAABB.top = m_AABB.top + m_attackAABBOffset.y;
//}

void Character::Animate()
{
	EntityState state = GetState();

	if (state == EntityState::Walking && m_spriteSheet.GetCurrentAnimation()->GetName() != "Walk")
	{
		m_spriteSheet.SetAnimation("Walk", true, true);
	}
	else
	//if (state == EntityState::Jumping && m_spriteSheet.GetCurrentAnimation()->GetName() != "Jump")
	//{
	//	m_spriteSheet.SetAnimation("Jump", true, false);
	//}
	//else
	if (state == EntityState::Attacking && m_spriteSheet.GetCurrentAnimation()->GetName() != "Attack")
	{
		m_spriteSheet.SetAnimation("Attack", true, false);
	}
	else
	if (state == EntityState::AttackingFast && m_spriteSheet.GetCurrentAnimation()->GetName() != "AttackFast")
	{
		m_spriteSheet.SetAnimation("AttackFast", true, false);
	}
	else
	if (state == EntityState::Hurt && m_spriteSheet.GetCurrentAnimation()->GetName() != "Hurt")
	{
		m_spriteSheet.SetAnimation("Hurt", true, false);
	}
	else
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
