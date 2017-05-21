#pragma once

#include <SFML/Graphics.hpp>

#include "SharedContext.h"

#include "Snake.h"
#include "Textbox.h"

class World
{
public:

	World(sf::Vector2u windowSize, int blockSize, int seed, SharedContext* pSharedContext);
	~World();

	int GetBlockSize();

	void RespawnApple();

	void Update(Snake& player);
	void UpdateAnimation(const sf::Time& dT);

	void Render(sf::RenderWindow& window);

private:

	sf::Vector2u m_windowSize;
	sf::Vector2i m_item;

	int m_blockSize;

	sf::RectangleShape m_backgroundRect;
	sf::CircleShape m_appleShape;


	SpriteSheet m_spriteSheetApple;
	sf::RectangleShape m_walls[4];
	sf::Sprite m_wallSprites[4];

	Textbox m_textbox;

	SharedContext* m_pSharedContext;
};