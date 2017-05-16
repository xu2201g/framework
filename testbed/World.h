#pragma once

#include <SFML/Graphics.hpp>

#include "Snake.h"
#include "Textbox.h"

class World
{
public:

	World(sf::Vector2u windowSize, int blockSize, int seed);
	~World();

	int GetBlockSize();

	void RespawnApple();

	void Update(Snake& player);

	void Render(sf::RenderWindow& window);

private:

	sf::Vector2u m_windowSize;
	sf::Vector2i m_item;

	int m_blockSize;

	sf::RectangleShape m_backgroundRect;
	sf::CircleShape m_appleShape;
	sf::RectangleShape m_walls[4];

	Textbox m_textbox;
};