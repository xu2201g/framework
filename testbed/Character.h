#pragma once

#include "EntityBase.h"

class Character : public EntityBase
{
	friend class EntityManager;

public:

private:

	sf::FloatRect m_attackAABB;
};