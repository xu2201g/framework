#pragma once

#include "TextureManager.h"
#include "Directions.h"
#include "Animation_Base.h"

#include <unordered_map>
#include <memory>

using Animations = std::unordered_map<std::string, std::unique_ptr<Animation_Base>>;

class SpriteSheet
{
public:

	SpriteSheet(TextureManager* pTextureManager);
	~SpriteSheet();

	void SetSpriteSize(const sf::Vector2i& size);
	void SetSpritePosition(const sf::Vector2f& position);
	void SetDirection(const Direction& direction);

	const sf::Vector2i& GetSpriteSize();
	const sf::Vector2f& GetSpritePosition();
	const Direction& GetDirection();

	void CropSprite(const sf::IntRect& rect);

	bool LoadSheet(const std::string& file);
	void ReleaseSheet();

	Animations* GetAnimations() { return &m_animations; }
	Animation_Base* GetCurrentAnimation();

	bool SetAnimation(const std::string& name, const bool& play = false, const bool& loop = false);

	void Update(const float& dT);
	void Draw(sf::RenderWindow* pWindow);

	sf::Sprite* GetSprite()
	{
		return &m_sprite;
	}

private:

	std::string m_texture;

	sf::Sprite m_sprite;
	sf::Vector2i m_spriteSize;
	sf::Vector2f m_spriteScale;
	
	Direction m_direction;
	
	std::string m_animationType;
	Animations m_animations;

	Animation_Base* m_pAnimationCurrent;
	TextureManager* m_pTextureManager;
};