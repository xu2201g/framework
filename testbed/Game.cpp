#include "Game.h"

#define WINDOW_SIZE sf::Vector2u(1280, 768)
#define MAX_ENTITIES 30

Game::Game()
	:
	m_window("Snake", WINDOW_SIZE),
	m_seed(time(nullptr)),
	m_stateManager(&m_sharedContext),
	m_textureManager(),
	m_entityManager(&m_sharedContext, MAX_ENTITIES)
{

	//shared context
	m_sharedContext.m_pWindow = &m_window;
	m_sharedContext.m_pEventManager = m_window.GetEventManager();
	m_sharedContext.m_pTextureManager = &m_textureManager;
	m_sharedContext.m_pEntityManager = &m_entityManager;
	m_sharedContext.m_pSeed = &m_seed;

	//global events
	m_sharedContext.m_pEventManager->AddCallback(StateType(0), "Window_Close", &Window::Close, m_sharedContext.m_pWindow);
	m_sharedContext.m_pEventManager->AddCallback(StateType(0), "Key_F5", &Window::ToggleFullscreen, m_sharedContext.m_pWindow);

	m_stateManager.SwitchTo(StateType::Intro);
}

Game::~Game() {}

void Game::Update()
{
	//handle window events
	m_window.Update();

	////fixed timestep at 60x per second
	//float frametime = 1.0f /60.0f;

	//if (m_elapsed.asSeconds() >= frametime)
	//{
		//do something 60x a second
		m_stateManager.Update(m_elapsed);

	//	m_elapsed -= sf::seconds(frametime);
	//}
	
	sf::sleep(sf::seconds(0.01f));
}

void Game::LateUpdate()
{
	m_stateManager.ProcessRequest();
	RestartClock();
}

void Game::Render()
{
	//clear the window
	m_window.BeginDraw(); 
	
	//state managed drawing
	m_stateManager.Draw();

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
	m_elapsed = m_clock.restart();
}
