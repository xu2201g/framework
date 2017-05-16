#include "World.h"

World::World(sf::Vector2u windowSize, int blockSize, int seed)
	:
	m_blockSize(blockSize),
	m_windowSize(windowSize),
	m_textbox(5, 10, 200, sf::Vector2f(blockSize, blockSize))
{
	//seed random number generator
	srand(seed);

	//background color
	m_backgroundRect.setSize(sf::Vector2f(windowSize.x, windowSize.y));
	m_backgroundRect.setPosition(0.0f, 0.f);
	m_backgroundRect.setFillColor(sf::Color(0, 0, 0, 255));

	//apple
	RespawnApple();
	m_appleShape.setFillColor(sf::Color::Red);
	m_appleShape.setRadius(m_blockSize / 2.0f);

	//walls
	sf::Color wallColor = sf::Color(127, 127, 127, 255);

	m_walls[0].setFillColor(wallColor);
	m_walls[1].setFillColor(wallColor);
	m_walls[2].setFillColor(wallColor);
	m_walls[3].setFillColor(wallColor);

	//north
	m_walls[0].setSize(sf::Vector2f(m_windowSize.x, m_blockSize));
	m_walls[0].setPosition(0, 0);

	//south
	m_walls[1].setSize(sf::Vector2f(m_windowSize.x, m_blockSize));
	m_walls[1].setPosition(0, m_windowSize.y - m_blockSize);

	//west
	m_walls[2].setSize(sf::Vector2f(m_blockSize, m_windowSize.y));
	m_walls[2].setPosition(0, 0);

	//east
	m_walls[3].setSize(sf::Vector2f(m_blockSize, m_windowSize.y));
	m_walls[3].setPosition(m_windowSize.x - m_blockSize, 0);
}

World::~World()
{
}

int World::GetBlockSize()
{
	return m_blockSize;
}

void World::RespawnApple()
{
	//TODO fix apple respawn collision with the snake body
	int maxX = (m_windowSize.x / m_blockSize) - 2;
	int maxY = (m_windowSize.y / m_blockSize) - 2;

	m_item = sf::Vector2i(rand() % maxX + 1, rand() % maxY + 1);
	m_appleShape.setPosition(m_item.x * m_blockSize, m_item.y * m_blockSize);
}

void World::Update(Snake& player)
{
	//snake head collides with apple
	if (player.GetPosition() == m_item)
	{
		player.Extend();
		player.IncreaseScore();
		player.IncreaseSpeed();
		RespawnApple();
	}

	//check snake wall collision
	int gridSize_x = m_windowSize.x / m_blockSize;
	int gridSize_y = m_windowSize.y / m_blockSize;

	if (player.GetPosition().x <= 0 ||
		player.GetPosition().y <= 0 ||
		player.GetPosition().x >= gridSize_x - 1 ||
		player.GetPosition().y >= gridSize_y - 1)
	{
		player.Lose();
	}

	//update textbox
	m_textbox.Clear();

	m_textbox.Add("Lives: " + std::to_string(player.GetLives()));
	m_textbox.Add("Score: " + std::to_string(player.GetScore()));
	m_textbox.Add("Speed: " + std::to_string(player.GetSpeed()));
}

void World::Render(sf::RenderWindow& window)
{
	//draw background
	window.draw(m_backgroundRect);

	//draw walls
	for (int i = 0; i < 4; ++i)
	{
		window.draw(m_walls[i]);
	}

	//draw apple
	window.draw(m_appleShape);

	//draw textbox
	m_textbox.Render(window);
}
