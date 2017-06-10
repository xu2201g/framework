#include "State_Intro.h"
#include "StateManager.h"

State_Intro::State_Intro(StateManager* pStateManager)
	:
	BaseState(pStateManager)
{
}

State_Intro::~State_Intro()
{
}

void State_Intro::OnCreate()
{
	m_timePassed = 0.0f;

	sf::Vector2u windowSize = m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow().getSize();

	m_introTexture.loadFromFile(Utils::GetWorkingDirectory() + "../../testbed/assets/textures/intro.png");
	m_introSprite.setTexture(m_introTexture);

	m_introSprite.setOrigin(m_introTexture.getSize().x / 2.0f, m_introTexture.getSize().y / 2.0f);
	m_introSprite.setPosition(windowSize.x / 2.0f, 0.0f);

	m_font.loadFromFile(Utils::GetWorkingDirectory() + "../../testbed/assets/fonts/arial.ttf");
	m_text.setFont(m_font);

	m_text.setString({ "Press SPACE to continue" });
	m_text.setCharacterSize(15);

	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	m_text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;
	eventMgr->AddCallback(StateType::Intro, "Key_Space", &State_Intro::Continue, this);
}

void State_Intro::OnDestroy()
{
	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;
	eventMgr->RemoveCallback(StateType::Intro, "Intro_Continue");
}

void State_Intro::Activate()
{
}

void State_Intro::Deactivate()
{
}

void State_Intro::Update(const sf::Time& time)
{
	if (m_timePassed < 5.0f) //in seconds
	{
		m_timePassed += time.asSeconds();
		m_introSprite.getPosition().x;
		m_introSprite.setPosition(m_introSprite.getPosition().x,
			                      m_introSprite.getPosition().y + (48 * time.asSeconds()));
	}
}

void State_Intro::Draw()
{
	sf::RenderWindow* window = &m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow();
	window->draw(m_introSprite);

	if (m_timePassed >= 5.0f)
	{
		window->draw(m_text);
	}
}

void State_Intro::Continue(EventDetails* details)
{
	if (m_timePassed >= 5.0f)
	{
		m_pStateMgr->SwitchTo(StateType::MainMenu);
		m_pStateMgr->Remove(StateType::Intro);
	}
}
