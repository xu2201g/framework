#pragma once

#include "BaseState.h"
#include "EventManager.h"

class State_GameOver : public BaseState
{

public:

	State_GameOver(StateManager* pStateManager);
	~State_GameOver();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	void RestartGame(EventDetails* details);

private:

	sf::Texture m_texture;
	sf::Sprite m_sprite;

	sf::Font m_font;
	sf::Text m_text;
	sf::RectangleShape m_rect;
};