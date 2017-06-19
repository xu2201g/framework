#include "State_Editor.h"

#include "StateManager.h"
#include "EntityManager.h"
#include "State_Game.h"

State_Editor::State_Editor(StateManager* pStateManager)
	:
	BaseState(pStateManager),
	m_pGameMap(nullptr),
	m_scrollSpeed(10.0f),
	m_selectedTileMap(0,0),
	m_selectedSetMap(3)
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

	eventMgr->AddCallback(StateType::Editor, "Key_Space", &State_Editor::PlaceObject, this);

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

	eventMgr->RemoveCallback(StateType::Editor, "Mouse_Left");

	eventMgr->RemoveCallback(StateType::Editor, "Key_Space");
}

void State_Editor::Activate()
{
}

void State_Editor::Deactivate()
{
}

void State_Editor::Update(const sf::Time& time)
{	

	//fixed timestep at 60x per second
	float frametime = 1.0f /60.0f;
	m_elapsed += time;
	if (m_elapsed.asSeconds() >= frametime) //limit framerate for entitymovement
	{
	//do something 60x a second

		//update position
		//set viewspace
		m_cameraPosition += m_delta;
		//std::cout << "scrolling to " << m_cameraPosition.x << " | " << m_cameraPosition.y << std::endl;
		m_delta.x *= 0.7;
		m_delta.y *= 0.7;
		m_view.setCenter(m_cameraPosition);

		m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow().setView(m_view);

		m_elapsed -= sf::seconds(frametime);
	}

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

	//tools
	{
		sf::Vector2u windowSize(m_pStateMgr->GetContext()->m_pWindow->GetWindowSize());

		sf::RectangleShape toolBackground(sf::Vector2f(3 * Sheet::Tile_Size, windowSize.y));
		toolBackground.setPosition(windowSize.x - 3 * Sheet::Tile_Size + m_view.getCenter().x - m_view.getSize().x * 0.5f, 0.0f + m_view.getCenter().y - m_view.getSize().y * 0.5f);

		toolBackground.setFillColor(sf::Color(0, 0, 0, 128));
		renderWindow.draw(toolBackground);

		//set of tiles
		{
			sf::Vector2f offset(windowSize.x - Sheet::Tile_Size, 0.0f);

			for (auto &itr : m_pGameMap->GetTileSet())
			{
				sf::Sprite tile;
			
				tile = itr.second->m_sprite;
				tile.setPosition(offset.x + m_view.getCenter().x - m_view.getSize().x * 0.5f, offset.y + m_view.getCenter().y - m_view.getSize().y * 0.5f + Sheet::Tile_Size * itr.first);
				renderWindow.draw(tile);

			}

			if (m_pGameMap->GetTileSet().size() > 0)
			{
				sf::RectangleShape toolBackground(sf::Vector2f(Sheet::Tile_Size, Sheet::Tile_Size));
				toolBackground.setPosition(windowSize.x - Sheet::Tile_Size + m_view.getCenter().x - m_view.getSize().x * 0.5f,
					                       0.0f + m_view.getCenter().y - m_view.getSize().y * 0.5f + Sheet::Tile_Size * m_selectedSetMap);

				toolBackground.setFillColor(sf::Color(0, 64, 0, 128));
				renderWindow.draw(toolBackground);


				m_selectedSetMap;
			}
		}

	}
}

void State_Editor::MainMenu(EventDetails* details)
{
	m_pStateMgr->SwitchTo(StateType::MainMenu);
}

void State_Editor::Scroll(EventDetails* details)
{	
	//set direction
	if (details->m_name == "Player_MoveLeft")
	{
		m_delta.x = - m_scrollSpeed;
	}
	else
	if (details->m_name == "Player_MoveRight")
	{
		m_delta.x  = m_scrollSpeed;
	}
	else
	if (details->m_name == "Player_MoveUp")
	{
		m_delta.y = - m_scrollSpeed;
	}
	else
	if (details->m_name == "Player_MoveDown")
	{
		m_delta.y = m_scrollSpeed;
	}	
}

void State_Editor::MouseClick(EventDetails* details) //TODO cleanup
{
	sf::Vector2i mousePosition = details->m_mouse;
	sf::Vector2i gridCoords;
	sf::Vector2u windowSize(m_pStateMgr->GetContext()->m_pWindow->GetWindowSize());

	//editortools
	{
		if (mousePosition.x >= windowSize.x - Sheet::Tile_Size && mousePosition.x <= windowSize.x)
		{			

			sf::Vector2f offset(windowSize.x - Sheet::Tile_Size, 0.0f);

			int tile = mousePosition.y / Sheet::Tile_Size;
			
			if (tile >= 0 && tile <= m_pGameMap->GetTileSet().size())
			{
				std::cout << "tile: " << tile << std::endl;
				m_selectedSetMap = tile;
			}

			return;
 		}		
	}

	//gamemap selection

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

void State_Editor::PlaceObject(EventDetails* details)
{
	std::cout << "Placing object" << std::endl;

	m_selectedTileMap;
	m_selectedSetMap;

	m_pGameMap;

	//check validy of selected map tile and set tile //TODO

	//get the tile from the set
	auto itrS = m_pGameMap->GetTileSet().find(m_selectedSetMap);
	if (itrS == m_pGameMap->GetTileSet().end())
	{
		std::cout << "! Tile id(" << m_selectedSetMap << ") was not found in tileset." << std::endl;
	}
	
	

	//search for already emplaced tiles
	auto itrM = m_pGameMap->GetTIleMap().find(m_pGameMap->ConvertCoords(m_selectedTileMap.x, m_selectedTileMap.y));

	if (itrM == m_pGameMap->GetTIleMap().end())
	{
		Tile tile;

		//bind properties from the related tileset
		tile.m_properties = itrS->second.get();
		tile.m_warp = false;

		if (!m_pGameMap->GetTIleMap().emplace(m_pGameMap->ConvertCoords(m_selectedTileMap.x, m_selectedTileMap.y), std::make_unique<Tile>(tile)).second)
		{
			//duplicate
			std::cout << "! Duplicate tile coordinates: " << m_selectedTileMap.x << " " << m_selectedTileMap.y << std::endl;
		}
	}
	else
	{

	}

	//update tile

}
