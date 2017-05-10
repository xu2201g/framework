#pragma once

#include "Window.h"

class Game
{
public:

	Game();
	~Game();

	void HandleInput();
	void Update();
	void Render();

	Window* GetWindow();

	sf::Time GetElapsed();
	void RestartClock();

private:

	void MoveSprite();
	Window m_window;

	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Vector2f m_speed;

	sf::Clock m_clock;
	sf::Time m_elapsed;
};