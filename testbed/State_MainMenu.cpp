#include "State_MainMenu.h"

#include "StateManager.h"

#include <iostream>

State_MainMenu::State_MainMenu(StateManager* pStateManager)
	:
	BaseState(pStateManager)
{
}

State_MainMenu::~State_MainMenu()
{
}

void State_MainMenu::OnCreate()
{
	m_font.loadFromFile("assets//fonts//arial.ttf");
	m_text.setFont(m_font);
	m_text.setString(sf::String("MAIN MENU:"));
	m_text.setCharacterSize(18);

	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,
				     textRect.top + textRect.height / 2.0f);
	m_text.setPosition(sf::Vector2f(640, 200));

	m_buttonSize = sf::Vector2f(300.0f, 32.0f);
	m_buttonPosition = sf::Vector2f(640, 250);
	m_buttonPadding = 4; //as pixel

	std::string str[3];

	str[0] = "PLAY";
	str[1] = "CREDITS";
	str[2] = "EXIT";

	for (int i = 0; i < 3; ++i)
	{
		sf::Vector2f buttonPosition(m_buttonPosition.x, 
								    m_buttonPosition.y + (i * (m_buttonSize.y + m_buttonPadding)));
		m_rects[i].setSize(m_buttonSize);
		m_rects[i].setFillColor(sf::Color::Green);
		m_rects[i].setOrigin(m_buttonSize.x / 2.0f, m_buttonSize.y / 2.0f);
		m_rects[i].setPosition(buttonPosition);

		m_labels[i].setFont(m_font);
		m_labels[i].setString(str[i]);
		m_labels[i].setCharacterSize(12);

		sf::FloatRect rect = m_labels[i].getLocalBounds();
		m_labels[i].setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
		m_labels[i].setPosition(buttonPosition);
	}

	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;
	eventMgr->AddCallback(StateType::MainMenu, "Mouse_Left", &State_MainMenu::MouseClick, this);
}

void State_MainMenu::OnDestroy()
{
	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;
	eventMgr->RemoveCallback(StateType::MainMenu, "Mouse_Left");
}

void State_MainMenu::Activate()
{
	if (m_pStateMgr->HasState(StateType::Game) &&
		m_labels[0].getString() == "PLAY")
	{
		m_labels[0].setString(sf::String("RESUME"));
		sf::FloatRect rect = m_labels[0].getLocalBounds();
		m_labels[0].setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);

	}
}

void State_MainMenu::Deactivate()
{
}

void State_MainMenu::Update(const sf::Time& time)
{
}

void State_MainMenu::Draw()
{
	sf::RenderWindow* window = &m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow();
	window->draw(m_text);

	for (int i = 0; i < 3; ++i)
	{
		window->draw(m_rects[i]);
		window->draw(m_labels[i]);
	}
}

void State_MainMenu::MouseClick(EventDetails* details)
{
	sf::Vector2i mousePosition = details->m_mouse;

	float halfX = m_buttonSize.x / 2.0f;
	float halfY = m_buttonSize.y / 2.0f;

	for (int i = 0; i < 3; ++i)
	{
		if (mousePosition.x >= m_rects[i].getPosition().x - halfX &&
			mousePosition.x <= m_rects[i].getPosition().x + halfX &&
			mousePosition.y >= m_rects[i].getPosition().y - halfY &&
			mousePosition.y <= m_rects[i].getPosition().y + halfY)
		{
			if (i == 0)
			{
				//play/resume
				m_pStateMgr->SwitchTo(StateType::Game);
			}
			else
			if (i == 1)
			{
				//credits
				m_pStateMgr->SwitchTo(StateType::Credits);
			}
			else
			if (i == 2)
			{
				//exit
				m_pStateMgr->GetContext()->m_pWindow->Close();
			}
		}
	}

}
