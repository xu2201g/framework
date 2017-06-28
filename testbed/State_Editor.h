#pragma once
#include "BaseState.h"
#include "EventManager.h"
#include "Map.h"

class State_Editor : public BaseState
{

	friend class State_Game;

public:

	State_Editor(StateManager* pStateManager);
	~State_Editor();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	void MainMenu(EventDetails* details);
	void Scroll(EventDetails* details); //moves the viewspace to reach inaccessable parts of the map 
	void MouseClick(EventDetails* details); //select tiles, click buttons to save/load maps and place tiles/entities in the opened map
	void PlaceObject(EventDetails* details); //places selected object(tile or entities) at the selected cell in the gamemap
	void RemoveObject(EventDetails* details); //removes tile or entity
	
private:

	void NewMap(int dimX, int dimY);


	void SaveMap(std::string path);


	void LoadMap(std::string path);
	

	float m_scrollSpeed; 

	std::shared_ptr<Map> m_pGameMap;

	sf::Vector2f m_cameraPosition;

	sf::Vector2i m_selectedTileMap;
	unsigned int m_selectedSetMap;

	sf::Time m_elapsed;

	sf::Vector2f m_delta;

};