#pragma once

#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"

//enables a comfortable way to share important objects like managers 

struct SharedContext
{
	SharedContext()
		:
		m_pWindow(nullptr),
		m_pEventManager(nullptr),
		m_pSeed(nullptr),
		m_pTextureManager(nullptr)
	{}

	Window* m_pWindow;
	EventManager* m_pEventManager;
	TextureManager* m_pTextureManager;

	int* m_pSeed;
};