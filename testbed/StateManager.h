#pragma once

#include "SharedContext.h"
#include "BaseState.h"


//used to identify states
enum class StateType
{
	Intro = 1,
	MainMenu,
	Game, 
	Paused,
	GameOver,
	Credits
};

//we store states as key value pair of type and a unique ptr managing memory allocation to the actual state instance into a vector
using StateContainer = std::vector<std::pair<StateType, std::unique_ptr<BaseState>>>;

//used as a container to store states that needs to be removed
using TypeContainer = std::vector<StateType>; 

using StateFactory = std::unordered_map<StateType, std::function<std::unique_ptr<BaseState>(void)>>; //TODO unique ptr instead of raw pointer

class StateManager
{
public:

	StateManager(SharedContext* sharedContext);
	~StateManager();

	void Update(const sf::Time& time);

	void Draw();

	void ProcessRequest();

	SharedContext* GetContext();

	bool HasState(const StateType& type);

	void SwitchTo(const StateType& type);
	void Remove(const StateType& type);

private:

	void CreateState(const StateType& type);
	void RemoveState(const StateType& type);

	template<class T>
	void RegisterState(const StateType& type)
	{
		m_stateFactory[type] = [this] () -> std::unique_ptr<BaseState>
		{
			//return std::unique_ptr<T>(new T(this));
			return std::make_unique<T>(this);
		};
	}

	SharedContext* m_pSharedContext;
	StateContainer m_states;
	TypeContainer m_toRemove;
	StateFactory m_stateFactory;
	
};

