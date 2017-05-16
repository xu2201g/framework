#pragma once

#include "Window.h"
#include "World.h"
#include "Textbox.h"

#include "StateManager.h"

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
	SharedContext m_sharedContext;
	StateManager m_stateManager;
};