#include "State_Game.h"

#include "StateManager.h"

//place to store magic numbers, not the best way handling it but still better than placing them between the code
#define BLOCKSIZE 32

State_Game::State_Game(StateManager* pStateManager)
	:
	BaseState(pStateManager),
	m_world(pStateManager->GetContext()->m_pWindow->GetRenderWindow().getSize(), BLOCKSIZE, *pStateManager->GetContext()->m_pSeed, pStateManager->GetContext()),
	m_snake(m_world.GetBlockSize(), pStateManager->GetContext())
{
}

State_Game::~State_Game()
{
}

void State_Game::OnCreate()
{
	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;
	eventMgr->AddCallback(StateType::Game, "Key_Escape", &State_Game::MainMenu, this);
	eventMgr->AddCallback(StateType::Game, "Key_P", &State_Game::Pause, this);

	eventMgr->AddCallback(StateType::Game, "Key_Left", &Snake::Navigate, &m_snake);
	eventMgr->AddCallback(StateType::Game, "Key_Right", &Snake::Navigate, &m_snake);
	eventMgr->AddCallback(StateType::Game, "Key_Up", &Snake::Navigate, &m_snake);
	eventMgr->AddCallback(StateType::Game, "Key_Down", &Snake::Navigate, &m_snake);
}

void State_Game::OnDestroy()
{
	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;

	eventMgr->RemoveCallback(StateType::Game, "Key_Escape");
	eventMgr->RemoveCallback(StateType::Game, "Key_P");
}

void State_Game::Activate()
{
}

void State_Game::Deactivate()
{
}

void State_Game::Update(const sf::Time& time)
{	
	//storing another elapsed time variable in the game state to update the frames 
	//at a fixed timestep related on the snake speed
	m_elapsed += time;
	float frametime = 1.0f / m_snake.GetSpeed();

	m_snake.UpdateAnimation(time.asSeconds());
	m_snake.UpdateAnimation(time.asSeconds());
	m_world.UpdateAnimation(time);
	if (m_elapsed.asSeconds() >= frametime)
	{		
		m_snake.Tick();
		m_world.Update(m_snake);
		

		m_elapsed -= sf::seconds(frametime);

		if (m_snake.HasLost())
		{
			//game over
			m_snake.Reset();
			m_pStateMgr->SwitchTo(StateType::GameOver);
		}
	}

}

void State_Game::Draw()
{
	m_world.Render(m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow());
	m_snake.Render(m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow());
}

void State_Game::MainMenu(EventDetails* details)
{
	m_pStateMgr->SwitchTo(StateType::MainMenu);
}

void State_Game::Pause(EventDetails* details)
{
	m_pStateMgr->SwitchTo(StateType::Paused);
}
