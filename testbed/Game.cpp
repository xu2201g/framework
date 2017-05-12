#include "Game.h"

#define WINDOW_SIZE sf::Vector2u(1280, 768)

Game::Game()
	:
	m_window("Snake", WINDOW_SIZE),
	m_seed(time(nullptr)),
	m_world(WINDOW_SIZE, m_seed),
	m_snake(m_world.GetBlockSize()),
	m_textbox(5,14,350,sf::Vector2f(m_world.GetBlockSize(), m_world.GetBlockSize()))
{
	//set members
	m_texture.loadFromFile("assets//textures//skeleton.png");
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_texture.getSize().x / 2.0f, m_texture.getSize().y / 2.0f);
	m_sprite.setPosition(sf::Vector2f(m_texture.getSize().x / 2.0f, m_window.GetWindowSize().y / 2.0f));
	m_speed = sf::Vector2f(200.0f, 0.0f); // x pixel per second

	m_textbox.Add("RNG seed: " + std::to_string(m_seed));
}

Game::~Game() {}

void Game::HandleInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_snake.GetPhysicalDirection() != Direction::Down)
	{
		m_snake.SetDirection(Direction::Up);
	}
	else
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_snake.GetPhysicalDirection() != Direction::Up)
	{
		m_snake.SetDirection(Direction::Down);
	}
	else
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_snake.GetPhysicalDirection() != Direction::Right)
	{
		m_snake.SetDirection(Direction::Left);
	}
	else
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_snake.GetPhysicalDirection() != Direction::Left)
	{
		m_snake.SetDirection(Direction::Right);
	}
}

void Game::Update()
{
	//handle window events
	m_window.Update();
	
	//fixed timestep at 60x per second
	float frametime = 1.0f / m_snake.GetSpeed();

	if (m_elapsed.asSeconds() >= frametime)
	{
		//do something 60x a second

		//update model
		//MoveSprite();

		m_snake.Tick();
		m_world.Update(m_snake);

		m_elapsed -= sf::seconds(frametime);

		if (m_snake.HasLost())
		{
			m_snake.Reset();
		}
	}

	//sf::sleep(sf::Time::asSeconds(0.01));
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
	//m_window.Draw(m_sprite);
	m_world.Render(m_window.GetRenderWindow());
	m_snake.Render(m_window.GetRenderWindow());
	m_textbox.Renderer(m_window.GetRenderWindow());

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
