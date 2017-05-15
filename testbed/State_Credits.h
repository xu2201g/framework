#pragma once

#include "BaseState.h"

class State_Credits : public BaseState
{

public:

	State_Credits(StateManager* pStateManager);
	~State_Credits();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

private:

};