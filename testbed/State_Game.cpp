#include "State_Game.h"

#include "StateManager.h"
#include "EntityManager.h"

#include "Rocket.h"

//place to store magic numbers, not the best way handling it but still better than placing them between the code
//#define BLOCKSIZE 32

State_Game::State_Game(StateManager* pStateManager)
	:
	BaseState(pStateManager),
	m_pGameMap(nullptr)
{
}

State_Game::~State_Game()
{
}

void State_Game::OnCreate()
{
	//set callbacks
	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;
	eventMgr->AddCallback(StateType::Game, "Key_Escape", &State_Game::MainMenu, this);
	eventMgr->AddCallback(StateType::Game, "Key_P", &State_Game::Pause, this);

	//set viewspace
	sf::Vector2u size = m_pStateMgr->GetContext()->m_pWindow->GetWindowSize();

	m_view.setSize(size.x, size.y);
	m_view.setCenter(size.x / 2, size.y / 2);
	m_view.zoom(0.6f);

	m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow().setView(m_view);

	//set map
	m_pGameMap = std::make_unique<Map>(m_pStateMgr->GetContext(), this);
	m_pGameMap->LoadMap("..//..//testbed//assets//maps//map1.map");

	//m_pStateMgr->GetContext()->m_pEntityManager->Add(EntityType::Rocket, "ROCKET");

	EntityBase* pPlayer = m_pStateMgr->GetContext()->m_pEntityManager->Find("Player");
	m_view.setCenter(pPlayer->GetPosition());
	m_pStateMgr->GetContext()->m_pWindow->GetRenderWindow().setView(m_view);
}

void State_Game::OnDestroy()
{
	EventManager* eventMgr = m_pStateMgr->GetContext()->m_pEventManager;

	eventMgr->RemoveCallback(StateType::Game, "Key_Escape");
	eventMgr->RemoveCallback(StateType::Game, "Key_P");
}

void State_Game::Activate()
{
}

void State_Game::Deactivate()
{
}

void State_Game::Update(const sf::Time& time)
{	

	//m_elapsed += time;
	//unsigned int ticksPerSecond = 120;
	//float frametime = 1.0f / ticksPerSecond;
	

	//if (m_elapsed.asSeconds() >= frametime)
	//{				

		SharedContext* pSharedContext = m_pStateMgr->GetContext();

		EntityBase* pPlayer = pSharedContext->m_pEntityManager->Find("Player");

		if (!pPlayer)
		{
			//respawn player
			std::cout << "Respawning player..." << std::endl;
			pSharedContext->m_pEntityManager->Add(EntityType::Player, "Player");
			pPlayer = pSharedContext->m_pEntityManager->Find("Player");
			pPlayer->SetPosition(m_pGameMap->GetPlayerStart());

			m_view.setCenter(pPlayer->GetPosition());
			pSharedContext->m_pWindow->GetRenderWindow().setView(m_view);
		}
		else
		{
			//update view (scrolling)
			m_view.setCenter(m_view.getCenter().x, pPlayer->GetPosition().y);
			pSharedContext->m_pWindow->GetRenderWindow().setView(m_view);
		}

		//scroll horizontal
		float distance = 0.05f;
		m_view.move(distance, 0.0f);
		pSharedContext->m_pWindow->GetRenderWindow().setView(m_view);

		sf::FloatRect viewSpace = pSharedContext->m_pWindow->GetViewSpace();
		if (viewSpace.left <= 0)
		{
			m_view.setCenter(viewSpace.width / 2, m_view.getCenter().y);
			pSharedContext->m_pWindow->GetRenderWindow().setView(m_view);
		}
		else
		if (viewSpace.left + viewSpace.width > (m_pGameMap->GetMapSize().x + 1) * Sheet::Tile_Size)
		{
			m_view.setCenter((m_pGameMap->GetMapSize().x + 1) * Sheet::Tile_Size - (viewSpace.width / 2.0f), m_view.getCenter().y);
			pSharedContext->m_pWindow->GetRenderWindow().setView(m_view);
		}

		m_pGameMap->Update(time.asSeconds());

		m_pStateMgr->GetContext()->m_pEntityManager->Update(time.asSeconds());

	//	m_elapsed -= sf::seconds(frametime);
	//}
}

void State_Game::Draw()
{
	m_pGameMap->Draw();
	m_pStateMgr->GetContext()->m_pEntityManager->Draw();
}

void State_Game::MainMenu(EventDetails* details)
{
	m_pStateMgr->SwitchTo(StateType::MainMenu);
}

void State_Game::Pause(EventDetails* details)
{
	m_pStateMgr->SwitchTo(StateType::Paused);
}
