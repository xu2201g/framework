#pragma once

#include "Character.h"

class Player : public Character
{

public:

	Player(EntityManager* pEntityManager);
	~Player();

	void OnEntityCollision(EntityBase* pCollider, bool attack);
	void React(EventDetails* pDetails);
	void ToggleFireMode(EventDetails* pDetails);
};