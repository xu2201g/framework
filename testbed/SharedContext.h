#pragma once

#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"


//enables a comfortable way to share important objects like managers 
class Map;
struct SharedContext
{
	SharedContext()
		:
		m_pWindow(nullptr),
		m_pEventManager(nullptr),
		m_pSeed(nullptr),
		m_pTextureManager(nullptr),
		m_pGameMap(nullptr)
	{}

	Window* m_pWindow;
	EventManager* m_pEventManager;
	TextureManager* m_pTextureManager;

	Map* m_pGameMap;

	int* m_pSeed;
};