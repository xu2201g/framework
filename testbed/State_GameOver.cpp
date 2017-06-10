#include "State_GameOver.h"
#include "StateManager.h"

State_GameOver::State_GameOver(StateManager* pStateManager)
	:
	BaseState(pStateManager)
{
}

State_GameOver::~State_GameOver()
{
}

void State_GameOver::OnCreate()
{
	SetTransparent(true);

	sf::Vector2u windowSize = m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow().getSize();

	m_texture.loadFromFile(Utils::GetWorkingDirectory() + "../../testbed/assets/textures/gameover.png");
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_texture.getSize().x / 2.0f, m_texture.getSize().y / 2.0f);
	m_sprite.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	m_font.loadFromFile(Utils::GetWorkingDirectory() + "../../testbed/assets/fonts/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString(sf::String("press SPACE to try again"));
	m_text.setCharacterSize(14);
	m_text.setStyle(sf::Text::Bold);

	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	m_text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f + 75);

	//darken the rendered game with a transparent black rect
	m_rect.setSize(sf::Vector2f(windowSize));
	m_rect.setPosition(0, 0);
	m_rect.setFillColor(sf::Color(0, 0, 0, 127));

	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;
	eventMgr->AddCallback(StateType::GameOver, "Key_Space", &State_GameOver::RestartGame, this);
}

void State_GameOver::OnDestroy()
{
	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;
	eventMgr->RemoveCallback(StateType::GameOver, "Key_Space");
}

void State_GameOver::Activate()
{
}

void State_GameOver::Deactivate()
{
}

void State_GameOver::Update(const sf::Time& time)
{
}

void State_GameOver::Draw()
{
	sf::RenderWindow* window = &m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow();

	window->draw(m_rect);
	window->draw(m_text);
	window->draw(m_sprite);
}

void State_GameOver::RestartGame(EventDetails* details)
{
	m_pStateMgr->SwitchTo(StateType::Game);
}
