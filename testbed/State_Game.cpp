#include "State_Game.h"

#include "StateManager.h"

State_Game::State_Game(StateManager* pStateManager)
	:
	BaseState(pStateManager)
{
}

State_Game::~State_Game()
{
}

void State_Game::OnCreate()
{
	m_texture.loadFromFile("assets//textures//skeleton.png");
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(0, 0);
	m_increment = sf::Vector2f(400.0f, 400.0f);

	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;
	eventMgr->AddCallback(StateType::Game, "Key_Escape", &State_Game::MainMenu, this);
	eventMgr->AddCallback(StateType::Game, "Key_P", &State_Game::Pause, this);

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
	sf::Vector2u windowSize = m_pStateMgr->GetContext()->m_pWindow->GetWindowSize();
	sf::Vector2u textureSize = m_texture.getSize();

	//let the sprite bounce within the window

	//update x direction
	if ((m_sprite.getPosition().x > windowSize.x - textureSize.x  && m_increment.x > 0.0f) ||
		(m_sprite.getPosition().x < 0 && m_increment.x < 0.0f))
	{
		m_increment.x = -m_increment.x;
	}

	//update y direction
	if ((m_sprite.getPosition().y > windowSize.y - textureSize.y  && m_increment.y > 0.0f) ||
		(m_sprite.getPosition().y < 0 && m_increment.y < 0.0f))
	{
		m_increment.y = -m_increment.y;
	}

	//update position
	m_sprite.setPosition(m_sprite.getPosition().x + (m_increment.x * time.asSeconds()),
		                 m_sprite.getPosition().y + (m_increment.y * time.asSeconds()));
}

void State_Game::Draw()
{
	m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow().draw(m_sprite);
}

void State_Game::MainMenu(EventDetails* details)
{
	m_pStateMgr->SwitchTo(StateType::MainMenu);
}

void State_Game::Pause(EventDetails* details)
{
	m_pStateMgr->SwitchTo(StateType::Paused);
}
