#pragma once

#include "Window.h"
#include "EventManager.h"

#include "Snake.h"

//enables a comfortable way to share important objects like managers 

struct SharedContext
{
	SharedContext()
		:
		m_pWindow(nullptr),
		m_pEventManager(nullptr)
	{}

	Window* m_pWindow;
	EventManager* m_pEventManager;

	Snake* m_pSnake;
};