#include "Projectile.h"
#include "EntityManager.h"

Projectile::Projectile(EntityManager* pEntityManager)
	:
	EntityBase(pEntityManager),
	m_spriteSheet(pEntityManager->GetContext()->m_pTextureManager),
	m_direction(-1.0f, 0.0f)
{
	m_spriteSheet.LoadSheet("..//..//testbed//assets//spritesheets//Rocket.SHEET");
}

Projectile::~Projectile()
{
}

void Projectile::Move()
{
	//float speed = sqrt(m_speed.x * m_speed.x + m_speed.y + m_speed.y); //maybe a bit overkill here
	//Accelerate(speed * m_direction.x, speed * m_direction.y);
}

void Projectile::OnImpact()
{
	SetState(EntityState::Dying); //rockets always die/explode on collisions
}

//void Projectile::Load(const std::string& path)			//TODO load these stuff from file
//{
//	std::ifstream file;
//	file.open(Utils::GetWorkingDirectory() + path);
//
//	if (!file.is_open())
//	{
//		std::cout << "! Failed to load projectile: " << path << std::endl;
//		return;
//	}
//
//	std::string line;
//	while (std::getline(file, line))
//	{
//		if (line[0] == '|')
//		{
//			continue;
//		}
//
//		std::stringstream keystream(line);
//		std::string type;
//
//		keystream >> type;
//
//		if (type == "Name")
//		{
//			keystream >> m_name;
//		}
//		else
//		if (type == "SpriteSheet")
//		{
//			std::string spriteFile;
//			keystream >> spriteFile;
//
//			m_spriteSheet.LoadSheet("..//..//testbed//assets//spritesheets//" + spriteFile);
//		}
//		else
//		if (type == "BoundingBox")
//		{
//			sf::Vector2f boundSize;
//			keystream >> boundSize.x >> boundSize.y;
//
//			SetSize(boundSize);
//		}
//		else
//		if (type == "Speed")
//		{
//			keystream >> m_speed.x >> m_speed.y;
//		}
//		else
//		if (type == "MaxVelocity")
//		{
//			keystream >> m_maxVelocity.x >> m_maxVelocity.y;
//		}
//	}
//
//	file.close();
//}

void Projectile::Update(float dT)
{
	EntityBase::Update(dT);

	//check out of bounds
	sf::FloatRect viewSpace = m_pEntityManager->GetContext()->m_pWindow->GetViewSpace();
		
	if (m_position.x <= viewSpace.left || m_position.x >= viewSpace.left + viewSpace.width ||
		m_position.y <= viewSpace.top || m_position.y >= viewSpace.top + viewSpace.height)
	{
		SetState(EntityState::Dying);
	}
	
	if (GetState() == EntityState::Dying)
	{
		if (!m_spriteSheet.GetCurrentAnimation()->IsPlaying()) //animation is done
		{
			//so remove entity
			m_pEntityManager->Remove(m_id);
		}
	}

	Animate(); //sets animations related on the entitystate
	m_spriteSheet.Update(dT);
	m_spriteSheet.SetSpritePosition(m_position);
}

void Projectile::Draw(sf::RenderWindow* pWindow)
{
	m_spriteSheet.Draw(pWindow);
}

void Projectile::Animate()
{
	EntityState state = GetState();

	if (m_name == "ROCKET")
	{
		int x;
	}

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
