#pragma once

#include "EntityBase.h"

using EntityContainer = std::unordered_map<unsigned int, std::unique_ptr<EntityBase>>;

using EntityFactory = std::unordered_map<EntityType, std::function<std::unique_ptr<EntityBase> (void)>>;

using EnemyTypes = std::unordered_map<std::string, std::string>;


class EntityManager
{

public:

	EntityManager(SharedContext* pSharedContext, unsigned int maxEntities);
	~EntityManager();

	int Add(const EntityType& type, const std::string& name = "");

	EntityBase* Find(unsigned int id);
	EntityBase* Find(const std::string& name);

	void Remove(unsigned int id);

	void Update(float dT);
	void Draw();

	void Purge();

	SharedContext* GetContext();

private:

	template<class T>
	void RegisterEntity(const EntityType& type)
	{
		m_entityFactory[type] = [this]() -> std::unique_ptr<EntityBase>
		{
			return std::make_unique<T>(this);
		};
	}

	void ProcessRemovals();
	void LoadEnemyTypes(const std::string& path);
	void EntityCollisionCheck();

	EntityContainer m_entities;
	EnemyTypes m_enemyTypes;
	EntityFactory m_entityFactory;
	SharedContext* m_pSharedContext;

	unsigned int m_idCounter;
	unsigned int m_maxEntities;

	std::vector<unsigned int> m_entitiesToRemove;
};
