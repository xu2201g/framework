#include "StateManager.h"

#include "State_Intro.h"
#include "State_MainMenu.h"
#include "State_Game.h"
#include "State_GameOver.h"
#include "State_Credits.h"
#include "State_Paused.h"


StateManager::StateManager(SharedContext* sharedContext)
	:
	m_pSharedContext(sharedContext)
{
	RegisterState<State_Intro>(StateType::Intro);
	RegisterState<State_MainMenu>(StateType::MainMenu);
	RegisterState<State_Game>(StateType::Game);
	RegisterState<State_Paused>(StateType::Paused);
//	RegisterState<State_GameOver>(StateType::GameOver);
//	RegisterState<State_Credits>(StateType::Credits);
}

StateManager::~StateManager()
{
	//usually there is no need to deallocate manually heap memory
	//cause we re using smartpointers for resource management
	//but in the future there re may be sideffects lying in the 
	//OnDestroy functions so we call it for each state here
	for (auto &itr : m_states)
	{
		itr.second->OnDestroy();
	}
}

void StateManager::Update(const sf::Time& time)
{
	//no state to update
	if (m_states.empty())
	{
		return;
	}

	//update underlying states if the overlaying state lets it controlled by transcendent flag
	if (m_states.back().second->IsTranscendent() &&
		m_states.size() > 1)
	{
		//iterate through the vector from the end - the last state added - to the beginning the first state added 
		//to get the start state we ll update first

		//its pretty similar to the draw function of this class
		//think of a game running in the game state and u press the esc key to open the mainmenu
		//there is a new state called mainmenu pushed back to the vector and in a singleplayer for example 
		//the game state is not updating cause the mainmenu transcendence flag is false to pause the game actually

		//in a multiplayer game this should be handled not like this so the game is still updating independent of 
		//the players openening the mainmenu or not so the transcendence flag should be true

		auto itr = m_states.end();
		while (itr != m_states.begin())
		{
			//ignore past-the-end element
			if (itr != m_states.end())
			{
				//if the state the itr is pointing to doesnt let the underlying state update we can break
				if (!itr->second->IsTranscendent())
				{
					break;
				}
			}

			//point to the next state
			--itr;
		}
		
		//update the correct states in the correct order
		for (; itr != m_states.end(); ++itr)
		{
			itr->second->Update(time);
		}
	}
	else
	{
		//there is no other state that could be updated 
		m_states.back().second->Update(time);
	}

}

void StateManager::Draw()
{
	//nothing to draw
	if (m_states.empty())
	{
		return;
	}

	//there are at least 2 states to draw 
	if (m_states.back().second->IsTransparent() &&
		m_states.size() > 1)
	{
		//iterate through the vector from the end - the last state added - to the beginning the first state added 
		//to get the start state we ll draw first

		//like the following scenario
		//
		//	VectorEnd()
		//	itr->			MainMenu - transparent == true			last added state
		//		itr->		Game	 - transparent == false 
		//					StateN
		//					...
		//	VectorBegin()	StateM									first added state
		auto itr = m_states.end();
		while (itr != m_states.begin())
		{
			//ignore past-the-end element
			if (itr != m_states.end())
			{
				//if the state is not transparent there is no need to iterate further cause it would be 
				//repainted by this state 
				if (!itr->second->IsTransparent())
				{
					break;
				}
			}

			//iterate into the next state
			--itr;
		}

		//draw all states starting from the state itr is pointing to to the last state pushed to the m_states vector
		//defined by the transparency flags of the states it is controlled what states are drawn and the vector data 
		//structure makes sure that they re drawn in the right order
		for (; itr != m_states.end(); ++itr)
		{
			itr->second->Draw();
		}
	}
	else
	{
		//there is only one state to draw
		m_states.back().second->Draw();
	}
}

void StateManager::ProcessRequest()
{
	//remove marked states
	while (m_toRemove.begin() != m_toRemove.end())
	{
		RemoveState(*m_toRemove.begin());
		m_toRemove.erase(m_toRemove.begin());
	}
}

SharedContext* StateManager::GetContext()
{
	return m_pSharedContext;
}

bool StateManager::HasState(const StateType& type)
{
	//iterates through the states vector and checks for a match 
	//and checks if the state is not marked to be removed soon
	for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
	{
		//match found
		if (itr->first == type)
		{
			//check if state is marked to be removed
			auto removed = std::find(m_toRemove.begin(), m_toRemove.end(), type);
			if (removed == m_toRemove.end())
			{
				return true;	
			}
			return false;
		}
	}
	return false;
}

void StateManager::SwitchTo(const StateType& type)
{
	//update current state to handle events properly
	m_pSharedContext->m_pEventManager->SetCurrentState(type);

	//search for the state we want to switch to in the states vector
	for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
	{
		if (itr->first == type) //state is found
		{
			//deactivate active state
			m_states.back().second->Deactivate();

			//store matching state in tmps
			StateType tmpType = itr->first;
			std::unique_ptr<BaseState> tmpState = std::move(itr->second);

			//remove matching state from the vector
			m_states.erase(itr);

			//activate the state
			tmpState->Activate();

			//add the state at the end of the vector - where the active state belongs to
			m_states.emplace_back(tmpType, std::move(tmpState));		

			//job is done and the itr is invalide cause we used the erased function
			return;
		}
	}

	//state was not found in the states vector
	
	//deactivate active state if there is a state
	if (!m_states.empty())
	{
		m_states.back().second->Deactivate();
	}

	//create the state we want to switch to
	CreateState(type);

	//activate the newly created state
	m_states.back().second->Activate();
}

void StateManager::Remove(const StateType& type)
{
	m_toRemove.push_back(type);
}

void StateManager::CreateState(const StateType& type)
{
	//create the state by our statefactory
	auto newState = m_stateFactory.find(type);

	//statetype was not found in our factory and thatswhy it cant be created
	if (newState == m_stateFactory.end())
	{
		return;
	}

	//std::unique_ptr<BaseState> state = newState->second(); //ive no idea why my code works TODO

	//add the new created state to the states vector
	
	m_states.emplace_back(type, std::move(newState->second()));
	m_states.back().second->OnCreate();
}

void StateManager::RemoveState(const StateType& type)
{
	//search for the state in the states vector
	for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
	{
		//removing state found
		if (itr->first == type)
		{
			itr->second->OnDestroy();
			m_states.erase(itr);
			return;
		}
	}
}
