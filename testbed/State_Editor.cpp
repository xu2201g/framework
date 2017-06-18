#include "State_Editor.h"

#include "StateManager.h"
#include "EntityManager.h"
#include "State_Game.h"

State_Editor::State_Editor(StateManager* pStateManager)
	:
	BaseState(pStateManager),
	m_pGameMap(nullptr),
	m_scrollSpeed(2.0f),
	m_selectedTileMap(0,0)
{
}

State_Editor::~State_Editor()
{
}

void State_Editor::OnCreate()
{
	//set callbacks
	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;
	eventMgr->AddCallback(StateType::Editor, "Key_Escape", &State_Editor::MainMenu, this);
	//eventMgr->AddCallback(StateType::Game, "Key_P", &State_Editor::Pause, this);

	eventMgr->AddCallback(StateType::Editor, "Player_MoveLeft", &State_Editor::Scroll, this);
	eventMgr->AddCallback(StateType::Editor, "Player_MoveRight", &State_Editor::Scroll, this);
	eventMgr->AddCallback(StateType::Editor, "Player_MoveUp", &State_Editor::Scroll, this);
	eventMgr->AddCallback(StateType::Editor, "Player_MoveDown", &State_Editor::Scroll, this);

	eventMgr->AddCallback(StateType::Editor, "Mouse_Left", &State_Editor::MouseClick, this);

	//set viewspace
	sf::Vector2u size = m_pStateMgr->GetContext()->m_pWindow->GetWindowSize();

	m_view.setSize(size.x, size.y);

	m_cameraPosition.x = size.x / 2;
	m_cameraPosition.y = size.y / 2;

	m_view.setCenter(m_cameraPosition);
	//m_view.zoom(0.6f);

	m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow().setView(m_view);

	//set map
	{
		bool mapAlreadyExists = false;
		
		for (auto &itr : m_pStateMgr->m_states)
		{
			if (itr.first == StateType::Game)
			{
				State_Game* pState_Game = (State_Game*)itr.second.get();
				if (pState_Game->m_pGameMap)
				{
					m_pGameMap = pState_Game->m_pGameMap;
					mapAlreadyExists = true;
					break; //map setup 
				}
			}
		}
		if (!mapAlreadyExists)
		{
			m_pGameMap = std::make_unique<Map>(m_pStateMgr->GetContext(), this);
			m_pGameMap->LoadMap("..//..//testbed//assets//maps//map1.map");
		}
	}
}

void State_Editor::OnDestroy()
{
	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;

	eventMgr->RemoveCallback(StateType::Editor, "Key_Escape");
	//eventMgr->RemoveCallback(StateType::Game, "Key_P");

	eventMgr->RemoveCallback(StateType::Editor, "Player_MoveLeft");
	eventMgr->RemoveCallback(StateType::Editor, "Player_MoveRight");
	eventMgr->RemoveCallback(StateType::Editor, "Player_MoveUp");
	eventMgr->RemoveCallback(StateType::Editor, "Player_MoveDown");
}

void State_Editor::Activate()
{
}

void State_Editor::Deactivate()
{
}

void State_Editor::Update(const sf::Time& time)
{
	m_pGameMap->Update(time.asSeconds());

	m_pStateMgr->GetContext()->m_pEntityManager->Update(time.asSeconds());

}

void State_Editor::Draw()
{
	m_pGameMap->Draw();
	m_pStateMgr->GetContext()->m_pEntityManager->Draw();

	//ui
	unsigned int tileSize = m_pGameMap->GetTileSize();
	sf::RenderWindow& renderWindow = m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow();

	//selected map tile
	{ 
		sf::RectangleShape selectedTileShape(sf::Vector2f(tileSize,	tileSize));
		selectedTileShape.setPosition(m_selectedTileMap.x * tileSize,
									  m_selectedTileMap.y * tileSize);
		selectedTileShape.setFillColor(sf::Color(128, 0, 0, 64));
		renderWindow.draw(selectedTileShape);
	}

	//game map bounds
	{ 
		float thickness = 20.0f;

		sf::RectangleShape mapBounds(sf::Vector2f(m_pGameMap->GetMapSize().x * tileSize + 2 * thickness,
											      m_pGameMap->GetMapSize().y * tileSize + 2 * thickness));
		mapBounds.setPosition(sf::Vector2f(-thickness, -thickness));
		mapBounds.setFillColor(sf::Color(128, 0, 0, 64));
		renderWindow.draw(mapBounds);
	}

}

void State_Editor::MainMenu(EventDetails* details)
{
	m_pStateMgr->SwitchTo(StateType::MainMenu);
}

void State_Editor::Scroll(EventDetails* details)
{
	sf::Vector2f delta(0.0f, 0.0f);

	//set direction
	if (details->m_name == "Player_MoveLeft")
	{
		delta.x -= m_scrollSpeed;
	}
	else
	if (details->m_name == "Player_MoveRight")
	{
		delta.x += m_scrollSpeed;
	}
	else
	if (details->m_name == "Player_MoveUp")
	{
		delta.y -= m_scrollSpeed;
	}
	else
	if (details->m_name == "Player_MoveDown")
	{
		delta.y += m_scrollSpeed;
	}

	//update position
	//set viewspace
	m_cameraPosition += delta;
	//std::cout << "scrolling to " << m_cameraPosition.x << " | " << m_cameraPosition.y << std::endl;

	m_view.setCenter(m_cameraPosition);
	
	m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow().setView(m_view);

}

void State_Editor::MouseClick(EventDetails* details)
{
	sf::Vector2i mousePosition = details->m_mouse;
	sf::Vector2i gridCoords;

	gridCoords.x = mousePosition.x / Sheet::Tile_Size;
	gridCoords.y = 0;

	sf::Vector2f viewTopLeftCorner(m_view.getCenter().x - 0.5f * m_view.getSize().x,
								   m_view.getCenter().y - 0.5f * m_view.getSize().y);



	std::cout << "Mouseclick in screen coords at: " << mousePosition.x << ":" << mousePosition.y << std::endl;
	std::cout << "Mouseclick in grid coords at: " << gridCoords.x << ":" << gridCoords.y << std::endl;
	//std::cout << "Viewspace position center coords: " << m_view.getCenter().x << ":" << m_view.getCenter().y << std::endl;
	std::cout << "Viewspace position top left coords: " << viewTopLeftCorner.x << ":" << viewTopLeftCorner.y << std::endl;

	sf::Vector2i gridCoordsUpdated;
	gridCoordsUpdated.x = (mousePosition.x + viewTopLeftCorner.x) / Sheet::Tile_Size;
	gridCoordsUpdated.y = (mousePosition.y + (int)viewTopLeftCorner.y) / Sheet::Tile_Size;

	std::cout << "Mouseclick in updated grid coords at: " << gridCoordsUpdated.x << ":" << gridCoordsUpdated.y << std::endl;

	m_selectedTileMap.x = gridCoordsUpdated.x;
	m_selectedTileMap.y = gridCoordsUpdated.y;

	//float halfX = m_buttonSize.x / 2.0f;
	//float halfY = m_buttonSize.y / 2.0f;

	//for (int i = 0; i < 3; ++i)
	//{
	//	if (mousePosition.x >= m_rects[i].getPosition().x - halfX &&
	//		mousePosition.x <= m_rects[i].getPosition().x + halfX &&
	//		mousePosition.y >= m_rects[i].getPosition().y - halfY &&
	//		mousePosition.y <= m_rects[i].getPosition().y + halfY)
	//	{
	//		if (i == 0)
	//		{
	//			//play/resume
	//			m_pStateMgr->SwitchTo(StateType::Game);
	//		}
	//		else
	//			if (i == 1)
	//			{
	//				//editor //
	//				m_pStateMgr->SwitchTo(StateType::Editor);
	//			}
	//			else
	//				if (i == 2)
	//				{
	//					//exit
	//					m_pStateMgr->GetContext()->m_pWindow->Close();
	//				}
	//	}
	//}

}