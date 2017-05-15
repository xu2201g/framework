#pragma once
#include "BaseState.h"
#include "EventManager.h"

class State_Game : public BaseState
{

public:

	State_Game(StateManager* pStateManager);
	~State_Game();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();


	void MainMenu(EventDetails* details);
	void Pause(EventDetails* details);

private:

	sf::Texture m_texture;
	sf::Sprite m_sprite;

	sf::Vector2f m_increment;

};