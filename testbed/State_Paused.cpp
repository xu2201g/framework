#include "State_Paused.h"

#include "StateManager.h"

State_Paused::State_Paused(StateManager* pStateManager)
	:
	BaseState(pStateManager)
{
}

State_Paused::~State_Paused()
{
}

void State_Paused::OnCreate()
{
	//this state is meant to pause the game so the game state ll be rendered before the pause state 
	SetTransparent(true);

	m_font.loadFromFile(Utils::GetWorkingDirectory() + "../../testbed/assets/fonts/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString(sf::String("PAUSED"));
	m_text.setCharacterSize(14);
	m_text.setStyle(sf::Text::Bold);

	sf::Vector2u windowSize = m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow().getSize();
	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	m_text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	//darken the rendered game with a transparent black rect
	m_rect.setSize(sf::Vector2f(windowSize));
	m_rect.setPosition(0, 0);
	m_rect.setFillColor(sf::Color(0, 0, 0, 127));

	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;
	eventMgr->AddCallback(StateType::Paused, "Key_P", &State_Paused::Unpause, this);

}

void State_Paused::OnDestroy()
{
	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;
	eventMgr->RemoveCallback(StateType::Paused, "Key_P");
}

void State_Paused::Activate()
{
}

void State_Paused::Deactivate()
{
}

void State_Paused::Update(const sf::Time& time)
{
	
}

void State_Paused::Draw()
{
	sf::RenderWindow* window = &m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow();

	window->draw(m_rect);
	window->draw(m_text);
}

void State_Paused::Unpause(EventDetails* details)
{
	m_pStateMgr->SwitchTo(StateType::Game);
}
