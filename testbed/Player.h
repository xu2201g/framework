#pragma once

#include "Character.h"

class Player : public Character
{

	friend class EntityManager;

public:

	Player(EntityManager* pEntityManager);
	~Player();

	void OnEntityCollision(EntityBase* pCollider, bool attack);
	void React(EventDetails* pDetails);
	void ToggleFireMode(EventDetails* pDetails);

	void IncreaseFireMode();
	void ResetFiremode();
};