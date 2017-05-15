#pragma once

#include <SFML/Graphics.hpp>

#include "BaseState.h"
#include "EventManager.h"

class State_MainMenu : public BaseState
{
public:
	
	State_MainMenu(StateManager* pStateManager);
	~State_MainMenu();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	void MouseClick(EventDetails* details);

private:

	sf::Font m_font;
	sf::Text m_text;

	sf::Vector2f m_buttonSize;
	sf::Vector2f m_buttonPosition;
	unsigned int m_buttonPadding;

	sf::RectangleShape m_rects[3]; //3 buttons ll be created 
	sf::Text m_labels[3];
};