#pragma once

#include "Window.h"
#include "EventManager.h"

//enables a comfortable way to share important objects like managers 

struct SharedContext
{
	SharedContext()
		:
		m_pWindow(nullptr),
		m_pEventManager(nullptr),
		m_pSeed(nullptr)
	{}

	Window* m_pWindow;
	EventManager* m_pEventManager;

	int* m_pSeed;
};