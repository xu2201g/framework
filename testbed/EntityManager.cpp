#include "EntityManager.h"
#include "Character.h"
#include "Enemy.h"
#include "Player.h"
#include "Rocket.h"
#include "Bolt.h"
#include "PowerUp.h"

EntityManager::EntityManager(SharedContext* pSharedContext, unsigned int maxEntities)
	:
	m_pSharedContext(pSharedContext),
	m_maxEntities(maxEntities),
	m_idCounter(0)
{
	LoadEnemyTypes("..//..//testbed//assets//characters//EnemyList.list"); //loads enemytype and related path to the .char file into m_enemyTypes

	RegisterEntity<Player>(EntityType::Player);
	RegisterEntity<Enemy>(EntityType::Enemy);
	RegisterEntity<Rocket>(EntityType::Rocket);
	RegisterEntity<Bolt>(EntityType::Bolt);
	RegisterEntity<PowerUp>(EntityType::PowerUp);
}

EntityManager::~EntityManager()
{
	Purge(); //removing allocated objects and registered callbacks etc.
}

int EntityManager::Add(const EntityType& type, const std::string& name)
{
	auto itr = m_entityFactory.find(type);
	if (itr == m_entityFactory.end())
	{
		//entity type not found
		return -1;
	}

	//create the new entity by the lambda function of our factory 
	std::unique_ptr<EntityBase> pEntity = itr->second();
	
	//set the id defined by how many entities already existed
	pEntity->m_id = m_idCounter;

	if (name != "")	
	{
		pEntity->m_name = name; //set the name - like rat for enemy types
	}

	//add the new entity to the map
	m_entities.emplace(pEntity->m_id, std::move(pEntity));

	//check wether the new entity is an enemy 
	if (type == EntityType::Enemy)
	{
		auto itr = m_enemyTypes.find(name);
		if (itr != m_enemyTypes.end())
		{
			//match found

			//get a rawpointer to the new added entity and cast it to Enemy 
			//necessary cause Enemy is a subclass with additional members like an attackAABB etc.
			Enemy* pEnemy = (Enemy*) m_entities.find(m_idCounter)->second.get(); 
			
			pEnemy->Load(itr->second); //load the enemy specific member values from a file defined identified by its name 
		}
	}

	//increment id counter to avoid id collisions 
	++m_idCounter;

	//return the id that was used for the new entity 
	return m_idCounter - 1;

	//return m_idCounter++; would do both at the same time, but its not that readable 
}

EntityBase* EntityManager::Find(unsigned int id)
{
	auto itr = m_entities.find(id);
	if (itr == m_entities.end())
	{
		return nullptr;
	}
	return itr->second.get();
}

EntityBase* EntityManager::Find(const std::string& name)
{
	//search for the entity by its name
	for (auto &itr : m_entities)
	{
		if (itr.second->m_name == name)
		{
			return itr.second.get();
		}
	}

	return nullptr;
}

void EntityManager::Remove(unsigned int id)
{
	//stores the id of the entity that has to be removed in a vector, so it can be removed at an appropriate time
	m_entitiesToRemove.emplace_back(id);
}

void EntityManager::Update(float dT)
{
	//update each entity 
	for (auto &itr : m_entities)
	{
		itr.second->Update(dT);
	}

	//check for possible collisions
	EntityCollisionCheck();

	//now its the time to remove enities stored by their id in a vector
	ProcessRemovals();
}

void EntityManager::Draw()
{
	//get a pointer to the window we ll draw the entities in
	sf::RenderWindow* pWindow = &m_pSharedContext->m_pWindow->GetRenderWindow();

	//get the viewspace
	sf::FloatRect viewSpace = m_pSharedContext->m_pWindow->GetViewSpace();

	//draw each entity if its intersecting with our viewspace, otherwise ignore it (culling)
	for (auto &itr : m_entities)
	{
		if (!viewSpace.intersects(itr.second->m_AABB))
		{
			continue;
		}

		itr.second->Draw(pWindow);
	}
}

void EntityManager::Purge()
{
	//no need to iterate through all entities and deallocate memory cause we re using smartpointers

	m_entities.clear();
	m_idCounter = 0;
}

SharedContext* EntityManager::GetContext()
{
	return m_pSharedContext;
}

void EntityManager::ProcessRemovals()
{
	//iterate through the vector
	while (m_entitiesToRemove.begin() != m_entitiesToRemove.end())
	{
		//get the id
		unsigned int id = m_entitiesToRemove.back();

		//get the entity
		auto itr = m_entities.find(id);
		if (itr != m_entities.end())
		{
			//std::cout << "Removing entity: " << itr->second->GetId() << " - " << itr->second->GetName() << std::endl;

			//check for the right type
			if (itr->second->GetType() == EntityType::Enemy)
			{
				////////////////////////////////
				//spawning powerup
				////////////////////////////////

				int id = Add(EntityType::PowerUp, "POWERUP");

				EntityBase* pEntity = Find(id);
				if (pEntity)
				{
					PowerUp* pPowerUp = (PowerUp*)pEntity;

					pPowerUp->SetPosition(itr->second->GetPosition());
				}

			}



			//remove the entity
			m_entities.erase(itr);
		}

		//remove id from the vector
		m_entitiesToRemove.pop_back();
	}

}

void EntityManager::LoadEnemyTypes(const std::string& path)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + path);

	if (!file.is_open())
	{
		std::cout << "! Failed loading enemy types: " << path << std::endl;
		std::cout << Utils::GetWorkingDirectory() + path << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|')
		{
			continue;
		}

		std::stringstream keystream(line);
		std::string name;
		std::string fileName;

		keystream >> name >> fileName;

		m_enemyTypes.emplace(name, fileName);
	}

	file.close();
}

void EntityManager::EntityCollisionCheck()
{
	//no collision without entities
	if (m_entities.empty())
	{
		return;
	}

	for (auto itrA = m_entities.begin(); std::next(itrA) != m_entities.end(); ++itrA) //iterate through all entities except the last one by iteratorA
	{		
		//iterate through all entities lying next the to iteratorA to avoid duplicate comparisons Notice that there are better ways to manage this 
		//"premature optimization is the root of all evil(or at least most of it) in programming" - Donald Knuth
		for (auto itrB = std::next(itrA); itrB != m_entities.end(); ++itrB) 
		{
			if (itrA->first == itrB->first)	//shouldnt be possible
			{
				std::cout << "duplicate comparison - EntityCollisionCheck" << std::endl;
				continue;
			}

			//regular AABB collision
			if (itrA->second->m_AABB.intersects(itrB->second->m_AABB)) 
			{
				//entity to entity collision so for each entity the oncollision functions getting invoked flaged as none attack
				itrA->second->OnEntityCollision(itrB->second.get(), false);  
				itrB->second->OnEntityCollision(itrA->second.get(), false);
			}

			////attack AABB collision
			//EntityType typeA = itrA->second->GetType();
			//EntityType typeB = itrB->second->GetType();

			////entity A
			//if (typeA == EntityType::Player || typeA == EntityType::Enemy)
			//{
			//	//cast entity as character to get access to the attackAABB
			//	Character* cA = (Character*)itrA->second.get();

			//	//if (cA->m_attackAABB.intersects(itrB->second->m_AABB))
			//	if (cA->m_AABB.intersects(itrB->second->m_AABB)) //direct collision hurts each other
			//	{
			//		//attackAABB of intersects with AABB of B 
			//		cA->OnEntityCollision(itrB->second.get(), true);
			//	}
			//}

			////entity B
			//if (typeB == EntityType::Player || typeB == EntityType::Enemy)
			//{
			//	//cast entity as character to get access to the attackAABB
			//	Character* cB = (Character*)itrB->second.get();

			//	//if (cB->m_attackAABB.intersects(itrA->second->m_AABB))
			//	if (cB->m_AABB.intersects(itrA->second->m_AABB))
			//	{
			//		//attackAABB of intersects with AABB of B 
			//		cB->OnEntityCollision(itrA->second.get(), true);
			//	}
			//}
		}
	}
}
