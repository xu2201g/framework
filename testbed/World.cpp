#include "World.h"

World::World(sf::Vector2u windowSize, int blockSize, int seed, SharedContext* pSharedContext)
	:
	m_blockSize(blockSize),
	m_windowSize(windowSize),
	m_spriteSheetApple(pSharedContext->m_pTextureManager),
	m_textbox(5, 10, 200, sf::Vector2f(blockSize, blockSize)),
	m_pSharedContext(pSharedContext)
{
	m_spriteSheetApple.LoadSheet("..//..//testbed//assets//appleSpriteSheet.sheet");
	m_spriteSheetApple.SetAnimation("Idle", true, true);

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

	m_pSharedContext->m_pTextureManager->RequireResource("SpikesHeadingLeft");
	m_pSharedContext->m_pTextureManager->RequireResource("SpikesHeadingRight");
	m_pSharedContext->m_pTextureManager->RequireResource("SpikesHeadingUp");
	m_pSharedContext->m_pTextureManager->RequireResource("SpikesHeadingDown");

	sf::Texture* pSpikesHeadingLeft = m_pSharedContext->m_pTextureManager->GetResource("SpikesHeadingLeft");
	sf::Texture* pSpikesHeadingRight = m_pSharedContext->m_pTextureManager->GetResource("SpikesHeadingRight");
	sf::Texture* pSpikesHeadingUp = m_pSharedContext->m_pTextureManager->GetResource("SpikesHeadingUp");
	sf::Texture* pSpikesHeadingDown = m_pSharedContext->m_pTextureManager->GetResource("SpikesHeadingDown");
	
	m_walls[0].setTexture(pSpikesHeadingDown);
	m_walls[1].setTexture(pSpikesHeadingUp);
	m_walls[2].setTexture(pSpikesHeadingRight);
	m_walls[3].setTexture(pSpikesHeadingLeft);
	
	//m_walls[0].setFillColor(wallColor);
	//m_walls[1].setFillColor(wallColor);
	//m_walls[2].setFillColor(wallColor);
	//m_walls[3].setFillColor(wallColor);

	pSpikesHeadingLeft->setRepeated(true);
	pSpikesHeadingRight->setRepeated(true);
	pSpikesHeadingUp->setRepeated(true);
	pSpikesHeadingDown->setRepeated(true);

	//north
	m_walls[0].setSize(sf::Vector2f(m_windowSize.x - 2*blockSize, m_blockSize));
	m_walls[0].setPosition(blockSize, 0);

	sf::IntRect rectNorth(sf::Vector2i(m_walls[0].getPosition().x, m_walls[0].getPosition().y), sf::Vector2i(m_walls[0].getSize().x, m_walls[0].getSize().y));

	m_wallSprites[0] = sf::Sprite(*pSpikesHeadingDown, rectNorth);
	m_wallSprites[0].setPosition(m_walls[0].getPosition());

	//south
	m_walls[1].setSize(sf::Vector2f(m_windowSize.x - 2*blockSize, m_blockSize));
	m_walls[1].setPosition(blockSize, m_windowSize.y - m_blockSize);

	sf::IntRect rectSouth(sf::Vector2i(m_walls[1].getPosition().x, m_walls[1].getPosition().y), sf::Vector2i(m_walls[1].getSize().x, m_walls[1].getSize().y));

	m_wallSprites[1] = sf::Sprite(*pSpikesHeadingUp, rectSouth);
	m_wallSprites[1].setPosition(m_walls[1].getPosition());

	//west
	m_walls[2].setSize(sf::Vector2f(m_blockSize, m_windowSize.y - 2*blockSize));
	m_walls[2].setPosition(0, blockSize);

	sf::IntRect rectWest(sf::Vector2i(m_walls[2].getPosition().x, m_walls[2].getPosition().y), sf::Vector2i(m_walls[2].getSize().x, m_walls[2].getSize().y));

	m_wallSprites[2] = sf::Sprite(*pSpikesHeadingRight, rectWest);
	m_wallSprites[2].setPosition(m_walls[2].getPosition());

	//east
	m_walls[3].setSize(sf::Vector2f(m_blockSize, m_windowSize.y - 2 * blockSize));
	m_walls[3].setPosition(m_windowSize.x - m_blockSize, blockSize);
	
	sf::IntRect rectEast(sf::Vector2i(m_walls[2].getPosition().x, m_walls[2].getPosition().y), sf::Vector2i(m_walls[2].getSize().x, m_walls[2].getSize().y));

	m_wallSprites[3] = sf::Sprite(*pSpikesHeadingLeft, rectEast);
	m_wallSprites[3].setPosition(m_walls[3].getPosition());
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

	m_spriteSheetApple.SetSpritePosition(sf::Vector2f(m_item.x * m_blockSize + m_blockSize / 2.0f, m_item.y * m_blockSize + m_blockSize / 2.0f));
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

void World::UpdateAnimation(const sf::Time& dT)
{
	m_spriteSheetApple.Update(dT.asSeconds());
}

void World::Render(sf::RenderWindow& window)
{
	//draw background
	window.draw(m_backgroundRect);

	//draw walls
	for (int i = 0; i < 4; ++i)
	{
		//window.draw(m_walls[i]);
		window.draw(m_wallSprites[i]);
	}

	
	window.draw(m_wallSprites[2]);

	//draw apple - simple shape
	//window.draw(m_appleShape);

	
	m_spriteSheetApple.Draw(&m_pSharedContext->m_pWindow->GetRenderWindow());

	//draw textbox
	m_textbox.Render(window);
}
