#pragma once

#include "Window.h"
#include "Textbox.h"

#include "StateManager.h"
#include "TextureManager.h"
#include "EntityManager.h"

#include <iostream>

class Game
{
public:

	Game();
	~Game();

	void Update();
	void LateUpdate();
	void Render();

	Window* GetWindow();

	sf::Time GetElapsed();
	void RestartClock();

private:

	Window m_window;

	sf::Clock m_clock;
	sf::Time m_elapsed;

	int m_seed;
	
	//managers
	EntityManager m_entityManager;
	TextureManager m_textureManager;
	SharedContext m_sharedContext;
	StateManager m_stateManager;
	
};