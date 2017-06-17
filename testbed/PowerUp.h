#pragma once

#include "EntityBase.h"
#include "SpriteSheet.h"

enum class PowerUpEffect
{
	ImproveFireSpread

};

class PowerUp : public EntityBase
{

	friend class EntityManager;

public:

	PowerUp(EntityManager* pEntityManager);
	~PowerUp();

	void Load(const std::string& path);

	void OnEntityCollision(EntityBase* collider, bool attack);


	void Update(float dT);

	void Draw(sf::RenderWindow* pWindow);

private:

	void Animate();

	SpriteSheet m_spriteSheet;



};