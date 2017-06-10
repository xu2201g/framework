#include "SpriteSheet.h"
#include "Utilities.h"
#include "Animation_Directional.h"

#include <sstream>
#include <iostream>

SpriteSheet::SpriteSheet(TextureManager* pTextureManager)
	:
	m_pTextureManager(pTextureManager),
	m_pAnimationCurrent(nullptr),
	m_spriteScale(1.0f, 1.0f),
	m_direction(Direction::Right)
{}

SpriteSheet::~SpriteSheet()
{
	ReleaseSheet();
}

void SpriteSheet::SetSpriteSize(const sf::Vector2i& size)
{
	m_spriteSize = size;

	//center origin
	m_sprite.setOrigin(m_spriteSize.x / 2.0f, m_spriteSize.y / 2.0f);
}

void SpriteSheet::SetSpritePosition(const sf::Vector2f& position)
{
	m_sprite.setPosition(position);
}

void SpriteSheet::SetDirection(const Direction& direction)
{
	//nothing to change
	if (m_direction == direction)
	{
		return;
	}

	m_direction = direction;

	//adapt animation
	m_pAnimationCurrent->CropSprite();
}

const sf::Vector2i& SpriteSheet::GetSpriteSize()
{
	return m_spriteSize;
}

const sf::Vector2f& SpriteSheet::GetSpritePosition()
{
	return m_sprite.getPosition();
}

const Direction& SpriteSheet::GetDirection()
{
	return m_direction;
}

void SpriteSheet::CropSprite(const sf::IntRect& rect)
{
	m_sprite.setTextureRect(rect);
}

bool SpriteSheet::LoadSheet(const std::string& file)
{
	std::ifstream sheet;
	sheet.open(Utils::GetWorkingDirectory() + file);

	if (sheet.is_open())
	{
		ReleaseSheet(); //release current sheet resources

		std::string line;
		while (std::getline(sheet, line))
		{
			if (line[0] == '|') { continue; } //'|' indicates comments or not processed lines like // in c++

			std::stringstream keystream(line);
			std::string type;

			keystream >> type;

			if (type == "Texture")
			{
				if (m_texture != "") //already holding a texture
				{
					std::cerr << "! Duplicate texture entries in: " << file << std::endl;
					continue;
				}

				std::string texture;
				keystream >> texture;

				if (!m_pTextureManager->RequireResource(texture)) //setup texture
				{
					std::cerr << "! Could not setup the texture: " << texture << std::endl;
					continue;
				}

				m_texture = texture;
				m_sprite.setTexture(*m_pTextureManager->GetResource(m_texture));
			}
			else
			if (type == "Size")
			{
				keystream >> m_spriteSize.x >> m_spriteSize.y;
				SetSpriteSize(m_spriteSize); //to update the origin
			}
			else
			if (type == "Scale")
			{
				keystream >> m_spriteScale.x >> m_spriteScale.y;
				m_sprite.setScale(m_spriteScale); 
			}
			else
			if (type == "AnimationType")
			{
				keystream >> m_animationType;
			}
			else
			if(type == "Animation")
			{
				std::string name;
				keystream >> name;

				if (m_animations.find(name) != m_animations.end())
				{
					std::cerr << "! Duplicate animation: " << name << " in " << file << std::endl;
					continue;
				}

				std::unique_ptr<Animation_Base> pAnimation(nullptr);
				if (m_animationType == "Directional")
				{
					pAnimation.reset(new Animation_Directional());
				}
				else
				{
					std::cerr << "! Unkown animation type: " << m_animationType << std::endl;
					continue;
				}

				keystream >> *pAnimation.get();

				pAnimation->SetSpriteSheet(this);
				pAnimation->SetName(name);
				pAnimation->Reset();				

				m_animations.emplace(name, std::move(pAnimation));

				//if (m_pAnimationCurrent) //current animation already set //magicly fixed the issue in which the first frame of the animation wasnt the idle one
				//{
				//	continue;
				//}
				
				m_pAnimationCurrent = m_animations.find(name)->second.get(); //sets the current animation to a raw pointer of the new emplaced animation loaded from the file
		
				m_pAnimationCurrent->Play(); //play the animation
			}
		}

		sheet.close();
		return true;
	}

	std::cerr << "! Failed to load spritesheet: " << file << std::endl;
	return false;

}

void SpriteSheet::ReleaseSheet()
{
	m_pTextureManager->ReleaseResource(m_texture);
	m_pAnimationCurrent = nullptr;

	while (m_animations.begin() != m_animations.end())
	{
		m_animations.erase(m_animations.begin());
	}
}

Animation_Base* SpriteSheet::GetCurrentAnimation()
{
	return m_pAnimationCurrent;
}

bool SpriteSheet::SetAnimation(const std::string& name, const bool& play, const bool& loop)
{
	auto itr = m_animations.find(name);
	if (itr == m_animations.end()) //not found
	{
		return false;
	}

	if (itr->second.get() == m_pAnimationCurrent) //no need to set the animaition to itself
	{
		//return false;
	}

	if (m_pAnimationCurrent) //stop current animation if existing
	{
		m_pAnimationCurrent->Stop();
	}

	//set a rawpointer to the animation found
	m_pAnimationCurrent = itr->second.get();
	m_pAnimationCurrent->SetLooping(loop);

	if (play)
	{
		m_pAnimationCurrent->Play();
	}

	m_pAnimationCurrent->CropSprite();

	return true;
}

void SpriteSheet::Update(const float& dT)
{
	m_pAnimationCurrent->Update(dT);
}

void SpriteSheet::Draw(sf::RenderWindow* pWindow)
{
	pWindow->draw(m_sprite);
}
