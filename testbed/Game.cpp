#include "Game.h"

Game::Game()
	:
	m_window("Moving Sprite", sf::Vector2u(1280, 768))
{
	//set members
	m_texture.loadFromFile("assets//textures//skeleton.png");
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(sf::Vector2f(m_sprite.getPosition().x, m_window.GetWindowSize().y / 2.0f));
	m_speed = sf::Vector2f(200.0f, 0.0f); // x pixel per second
}

Game::~Game() {}

void Game::HandleInput()
{
}

void Game::Update()
{
	//handle window events
	m_window.Update();

	
	//fixed timestep at 60x per second
	float frametime = 1.0f / 60.0f;

	if (m_elapsed.asSeconds() >= frametime)
	{
		//do something 60x a second

		//update model
		MoveSprite();

		m_elapsed -= sf::seconds(frametime);
	}
}

void Game::MoveSprite()
{
	sf::Vector2u windowSize(m_window.GetWindowSize());
	sf::Vector2u textureSize(m_texture.getSize());
	//adjust direction
	if ((m_sprite.getPosition().x + (textureSize.x / 2.0f) > windowSize.x && m_speed.x > 0.0f) || //sprite leaves the window on the right side
		(m_sprite.getPosition().x - (textureSize.x / 2.0f) < 0 && m_speed.x < 0.0f))			  //sprite leaves the window on the left side
	{
		//invert the direction
		m_speed.x = -m_speed.x;
	}

	//calculate time passed as seconds since last frame
	float fElapsed = m_elapsed.asSeconds();

	//update position
	m_sprite.setPosition(m_sprite.getPosition() + m_speed * fElapsed);
}

void Game::Render()
{
	//clear the window
	m_window.BeginDraw(); 

	//draw the model
	m_window.Draw(m_sprite);

	//display the result 
	m_window.EndDraw();
}

Window* Game::GetWindow()
{
	return &m_window;
}

sf::Time Game::GetElapsed()
{
	return m_elapsed;
}

void Game::RestartClock()
{
	m_elapsed += m_clock.restart();
}
