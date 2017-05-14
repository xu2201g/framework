#pragma once

#include <SFML/System.hpp>

class StateManager;

class BaseState
{
	friend class StateManager;

public:

	BaseState(StateManager* pStateManager)
		:
		m_pStateMgr(pStateManager),
		m_transparent(false),
		m_transcendent(false)
	{}

	virtual ~BaseState() 
	{}

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& time) = 0;
	virtual void Draw() = 0;

	void SetTransparent(const bool& transparent)
	{
		m_transparent = transparent;
	}

	void SetTranscendent(const bool& transcendent)
	{
		m_transcendent = transcendent;
	}

	bool IsTransparent()
	{
		return m_transparent;
	}

	bool IsTranscendent()
	{
		return m_transcendent;
	}

	StateManager* GetStateManager()
	{
		return m_pStateMgr;
	}

protected:

	StateManager* m_pStateMgr;

	bool m_transparent; //controls wether the state lets underlying states be drawn 
	bool m_transcendent; //controls wether the state lets underlying states be updated 

};