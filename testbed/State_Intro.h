#pragma once

#include <SFML/Graphics.hpp>

#include "BaseState.h"
#include "EventManager.h"

class State_Intro : public BaseState
{

public:

	State_Intro(StateManager* pStateManager);
	~State_Intro();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();
	
	void Continue(EventDetails* details);

private:

	sf::Texture m_introTexture;
	sf::Sprite m_introSprite;
	sf::Text m_text;

	sf::Font m_font;

	float m_timePassed;
};